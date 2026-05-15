#include "NetworkManager.h"
#include "NetworkMasterThread.h"
#include "ThreadCloseConnection.h"
#include "Packet.h"
#include "DataStream.h"
#include "platform.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>

static pthread_mutex_t s_threadSyncMutex = PTHREAD_MUTEX_INITIALIZER;
int NetworkManager_numReadThreads = 0;
int NetworkManager_numWriteThreads = 0;

int NetworkManager_field_28145_d[256];
int NetworkManager_field_28144_e[256];

typedef struct {
    Packet **data;
    int size;
    int capacity;
    pthread_mutex_t lock;
} PacketList;

static void PacketList_init(PacketList *list) {
    list->capacity = 16;
    list->size = 0;
    list->data = (Packet **)malloc(list->capacity * sizeof(Packet *));
    pthread_mutex_init(&list->lock, NULL);
}

static void PacketList_add(PacketList *list, Packet *p) {
    pthread_mutex_lock(&list->lock);
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = (Packet **)realloc(list->data, list->capacity * sizeof(Packet *));
    }
    list->data[list->size++] = p;
    pthread_mutex_unlock(&list->lock);
}

static Packet *PacketList_removeFirst(PacketList *list) {
    if (list->size == 0)
        return NULL;
    Packet *p = list->data[0];
    memmove(list->data, list->data + 1, (list->size - 1) * sizeof(Packet *));
    list->size--;
    return p;
}

static int PacketList_isEmpty(PacketList *list) { return list->size == 0; }

struct NetworkManager {
    mc_socket_t socket_fd;
    FILE *socketInputStream;
    FILE *socketOutputStream;
    NetHandler *netHandler;

    int isRunning;

    int isServerTerminating;

    int isTerminating;

    char terminationReason[256];
    char terminationArg[512];

    PacketList readPackets;
    PacketList dataPackets;
    PacketList chunkDataPackets;

    pthread_mutex_t sendQueueLock;
    pthread_t readThread;
    pthread_t writeThread;

    pthread_mutex_t wakeMutex;
    pthread_cond_t wakeCond;

    int terminationArgSet;

    int timeSinceLastRead;

    int sendQueueByteLength;

    int chunkDataSendCounter;

    int field_20100_w;
};

static int NetworkManager_readPacket(NetworkManager *self) {
    int var1 = 0;
    DataStream in = DataStream_fromFile(self->socketInputStream);
    Packet *var2 =
        Packet_readPacket(&in, self->netHandler->vtable->isServerHandler(self->netHandler));
    if (var2 != NULL) {
        NetworkManager_field_28145_d[var2->packetId] += var2->vtable->getPacketSize(var2) + 1;
        PacketList_add(&self->readPackets, var2);
        var1 = 1;
    } else {
        NetworkManager_networkShutdown(self, "disconnect.endOfStream");
    }
    return var1;
}

static int NetworkManager_sendPacket(NetworkManager *self) {
    int var1 = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t nowMs = (int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;

    pthread_mutex_lock(&self->sendQueueLock);

    if (!PacketList_isEmpty(&self->dataPackets)) {
        Packet *firstData = self->dataPackets.data[0];
        if (self->chunkDataSendCounter == 0 ||
            nowMs - firstData->creationTimeMillis >= (int64_t)self->chunkDataSendCounter) {
            Packet *var2 = PacketList_removeFirst(&self->dataPackets);
            self->sendQueueByteLength -= var2->vtable->getPacketSize(var2) + 1;
            pthread_mutex_unlock(&self->sendQueueLock);

            DataStream out = DataStream_fromFile(self->socketOutputStream);
            Packet_writePacket(var2, &out);
            fflush(self->socketOutputStream);
            NetworkManager_field_28144_e[var2->packetId] += var2->vtable->getPacketSize(var2) + 1;
            Packet_free(var2);
            var1 = 1;
            pthread_mutex_lock(&self->sendQueueLock);
        }
    }

    if (self->field_20100_w-- <= 0 && !PacketList_isEmpty(&self->chunkDataPackets)) {
        Packet *firstChunk = self->chunkDataPackets.data[0];
        if (self->chunkDataSendCounter == 0 ||
            nowMs - firstChunk->creationTimeMillis >= (int64_t)self->chunkDataSendCounter) {
            Packet *var2 = PacketList_removeFirst(&self->chunkDataPackets);
            self->sendQueueByteLength -= var2->vtable->getPacketSize(var2) + 1;
            pthread_mutex_unlock(&self->sendQueueLock);

            DataStream out = DataStream_fromFile(self->socketOutputStream);
            Packet_writePacket(var2, &out);
            NetworkManager_field_28144_e[var2->packetId] += var2->vtable->getPacketSize(var2) + 1;
            free(var2);
            self->field_20100_w = 0;
            var1 = 1;
            pthread_mutex_lock(&self->sendQueueLock);
        }
    }

    pthread_mutex_unlock(&self->sendQueueLock);
    return var1;
}

static void *readerThreadFunc(void *arg) {
    NetworkManager *nm = (NetworkManager *)arg;
    pthread_mutex_lock(&s_threadSyncMutex);
    ++NetworkManager_numReadThreads;
    pthread_mutex_unlock(&s_threadSyncMutex);

    while (1) {
        if (!nm->isRunning)
            break;
        if (nm->isServerTerminating)
            break;
        while (NetworkManager_readPacket(nm)) {
        }

        struct timespec abs;
        clock_gettime(CLOCK_REALTIME, &abs);
        abs.tv_nsec += 100000000L;
        if (abs.tv_nsec >= 1000000000L) {
            abs.tv_sec++;
            abs.tv_nsec -= 1000000000L;
        }
        pthread_mutex_lock(&nm->wakeMutex);
        pthread_cond_timedwait(&nm->wakeCond, &nm->wakeMutex, &abs);
        pthread_mutex_unlock(&nm->wakeMutex);
    }

    pthread_mutex_lock(&s_threadSyncMutex);
    --NetworkManager_numReadThreads;
    pthread_mutex_unlock(&s_threadSyncMutex);
    return NULL;
}

static void *writerThreadFunc(void *arg) {
    NetworkManager *nm = (NetworkManager *)arg;
    pthread_mutex_lock(&s_threadSyncMutex);
    ++NetworkManager_numWriteThreads;
    pthread_mutex_unlock(&s_threadSyncMutex);

    while (1) {
        if (!nm->isRunning)
            break;
        while (NetworkManager_sendPacket(nm)) {
        }

        struct timespec abs;
        clock_gettime(CLOCK_REALTIME, &abs);
        abs.tv_nsec += 100000000L;
        if (abs.tv_nsec >= 1000000000L) {
            abs.tv_sec++;
            abs.tv_nsec -= 1000000000L;
        }
        pthread_mutex_lock(&nm->wakeMutex);
        pthread_cond_timedwait(&nm->wakeCond, &nm->wakeMutex, &abs);
        pthread_mutex_unlock(&nm->wakeMutex);
        if (nm->socketOutputStream) {
            if (fflush(nm->socketOutputStream) != 0) {
                if (!nm->isTerminating) {
                    NetworkManager_networkShutdownWithArg(nm, "disconnect.genericReason",
                                                          "Internal exception: flush error");
                }
                perror("flush error");
            }
        }
    }

    pthread_mutex_lock(&s_threadSyncMutex);
    --NetworkManager_numWriteThreads;
    pthread_mutex_unlock(&s_threadSyncMutex);
    return NULL;
}

NetworkManager *NetworkManager_create(const char *host, int port, NetHandler *netHandler) {
    NetworkManager *self = (NetworkManager *)calloc(1, sizeof(NetworkManager));
    self->netHandler = netHandler;
    self->isRunning = 1;
    self->field_20100_w = 50;
    self->terminationReason[0] = '\0';

    self->socket_fd = MC_INVALID_SOCKET;

    PacketList_init(&self->readPackets);
    PacketList_init(&self->dataPackets);
    PacketList_init(&self->chunkDataPackets);
    pthread_mutex_init(&self->sendQueueLock, NULL);
    pthread_mutex_init(&self->wakeMutex, NULL);
    pthread_cond_init(&self->wakeCond, NULL);

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    char portStr[8];
    snprintf(portStr, sizeof(portStr), "%d", port);
    if (getaddrinfo(host, portStr, &hints, &res) != 0) {
        fprintf(stderr, "NetworkManager: getaddrinfo failed for %s:%d\n", host, port);
        return self;
    }
    self->socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (self->socket_fd == MC_INVALID_SOCKET) {
        freeaddrinfo(res);
        return self;
    }
    connect(self->socket_fd, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

#ifdef _WIN32
    DWORD timeout_ms = 30000;
    setsockopt(self->socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout_ms,
               sizeof(timeout_ms));
#else
    struct timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    setsockopt(self->socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    int tos = 24;
    setsockopt(self->socket_fd, IPPROTO_IP, IP_TOS, &tos, sizeof(tos));
#endif

#ifdef _WIN32
    HANDLE proc = GetCurrentProcess();
    HANDLE rh = INVALID_HANDLE_VALUE, wh = INVALID_HANDLE_VALUE;
    DuplicateHandle(proc, (HANDLE)self->socket_fd, proc, &rh, 0, FALSE, DUPLICATE_SAME_ACCESS);
    DuplicateHandle(proc, (HANDLE)self->socket_fd, proc, &wh, 0, FALSE, DUPLICATE_SAME_ACCESS);
    int rfd = _open_osfhandle((intptr_t)rh, _O_RDONLY | _O_BINARY);
    int wfd = _open_osfhandle((intptr_t)wh, _O_WRONLY | _O_BINARY);
    self->socketInputStream = _fdopen(rfd, "rb");
    self->socketOutputStream = _fdopen(wfd, "wb");
#else
    int rfd = dup(self->socket_fd);
    int wfd = dup(self->socket_fd);
    self->socketInputStream = fdopen(rfd, "r");
    self->socketOutputStream = fdopen(wfd, "w");
#endif

    pthread_create(&self->readThread, NULL, readerThreadFunc, self);
    pthread_create(&self->writeThread, NULL, writerThreadFunc, self);

    return self;
}

void NetworkManager_networkShutdownWithArg(NetworkManager *self, const char *reason,
                                           const char *arg) {
    NetworkManager_networkShutdown(self, reason);
    if (arg)
        strncpy(self->terminationArg, arg, sizeof(self->terminationArg) - 1);
    self->terminationArgSet = 1;
}

void NetworkManager_addToSendQueue(NetworkManager *self, Packet *packet) {
    if (self->isServerTerminating)
        return;
    pthread_mutex_lock(&self->sendQueueLock);
    self->sendQueueByteLength += packet->vtable->getPacketSize(packet) + 1;
    if (packet->isChunkDataPacket) {
        PacketList_add(&self->chunkDataPackets, packet);
    } else {
        PacketList_add(&self->dataPackets, packet);
    }
    pthread_mutex_unlock(&self->sendQueueLock);
}

void NetworkManager_wakeThreads(NetworkManager *self) {
    pthread_mutex_lock(&self->wakeMutex);
    pthread_cond_broadcast(&self->wakeCond);
    pthread_mutex_unlock(&self->wakeMutex);
}

void NetworkManager_networkShutdown(NetworkManager *self, const char *reason) {
    if (!self->isRunning)
        return;
    self->isTerminating = 1;
    strncpy(self->terminationReason, reason ? reason : "", sizeof(self->terminationReason) - 1);
    self->terminationArg[0] = '\0';
    self->terminationArgSet = 0;

    NetworkMasterThread_start(self);
    self->isRunning = 0;
    if (self->socketInputStream) {
        fclose(self->socketInputStream);
        self->socketInputStream = NULL;
    }
    if (self->socketOutputStream) {
        fclose(self->socketOutputStream);
        self->socketOutputStream = NULL;
    }
    if (self->socket_fd != MC_INVALID_SOCKET) {
        mc_closesocket(self->socket_fd);
        self->socket_fd = MC_INVALID_SOCKET;
    }
}

void NetworkManager_processReadPackets(NetworkManager *self) {
    if (self->sendQueueByteLength > 1048576) {
        NetworkManager_networkShutdown(self, "disconnect.overflow");
    }
    if (PacketList_isEmpty(&self->readPackets)) {
        if (self->timeSinceLastRead++ == 1200) {
            NetworkManager_networkShutdown(self, "disconnect.timeout");
        }
    } else {
        self->timeSinceLastRead = 0;
    }
    int var1 = 100;
    while (!PacketList_isEmpty(&self->readPackets) && var1-- >= 0) {
        pthread_mutex_lock(&self->readPackets.lock);
        Packet *var2 = PacketList_removeFirst(&self->readPackets);
        pthread_mutex_unlock(&self->readPackets.lock);
        var2->vtable->processPacket(var2, self->netHandler);
        Packet_free(var2);
    }
    NetworkManager_wakeThreads(self);
    if (self->isTerminating && PacketList_isEmpty(&self->readPackets)) {
        self->netHandler->vtable->handleErrorMessage(self->netHandler, self->terminationReason,
                                                     self->terminationArgSet ? self->terminationArg
                                                                             : NULL);
    }
}

void NetworkManager_func_28142_c(NetworkManager *self) {
    NetworkManager_wakeThreads(self);
    self->isServerTerminating = 1;

    NetworkManager_wakeThreads(self);

    ThreadCloseConnection_create(self);
}

int NetworkManager_isRunning(NetworkManager *var0) { return var0->isRunning; }

pthread_t *NetworkManager_getReadThread(NetworkManager *var0) { return &var0->readThread; }

pthread_t *NetworkManager_getWriteThread(NetworkManager *var0) { return &var0->writeThread; }

void NetworkManager_destroy(NetworkManager *self) {

    if (self->isRunning) {
        self->isRunning = 0;
        if (self->socketInputStream) {
            fclose(self->socketInputStream);
            self->socketInputStream = NULL;
        }
        if (self->socketOutputStream) {
            fclose(self->socketOutputStream);
            self->socketOutputStream = NULL;
        }
        if (self->socket_fd != MC_INVALID_SOCKET) {
            mc_closesocket(self->socket_fd);
            self->socket_fd = MC_INVALID_SOCKET;
        }
    }
    NetworkManager_wakeThreads(self);
    pthread_join(self->readThread, NULL);
    pthread_join(self->writeThread, NULL);

    while (!PacketList_isEmpty(&self->readPackets)) {
        Packet *p = PacketList_removeFirst(&self->readPackets);
        Packet_free(p);
    }

    while (!PacketList_isEmpty(&self->dataPackets)) {
        Packet *p = PacketList_removeFirst(&self->dataPackets);
        Packet_free(p);
    }
    while (!PacketList_isEmpty(&self->chunkDataPackets)) {
        Packet *p = PacketList_removeFirst(&self->chunkDataPackets);
        Packet_free(p);
    }
    free(self->readPackets.data);
    free(self->dataPackets.data);
    free(self->chunkDataPackets.data);
    free(self);
}
