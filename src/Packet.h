#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include "DataStream.h"

typedef struct Packet Packet;
typedef struct NetHandler NetHandler;

typedef struct Packet_vtable {
    void (*readPacketData)(Packet *self, DataStream *var1);
    void (*writePacketData)(Packet *self, DataStream *var1);
    void (*processPacket)(Packet *self, NetHandler *var1);
    int (*getPacketSize)(Packet *self);
} Packet_vtable;

struct Packet {
    Packet_vtable *vtable;

    int64_t creationTimeMillis;

    int isChunkDataPacket;

    int packetId;
};

typedef Packet *(*PacketFactory)(void);
void Packet_addIdClassMapping(int id, int isClient, int isServer, PacketFactory factory);

Packet *Packet_getNewPacket(int id);

int Packet_getPacketId(Packet *self);

Packet *Packet_readPacket(DataStream *var0, int isServer);

void Packet_writePacket(Packet *var0, DataStream *var1);

void Packet_writeString(const char *var0, DataStream *var1);

void Packet_free(Packet *p);

char *Packet_readString(DataStream *var0, int maxLen);

void Packet_initRegistry(void);

#endif
