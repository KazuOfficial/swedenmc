#include "ChunkMesher.h"
#include "Tessellator.h"
#include "ChunkCache.h"
#include "RenderBlocks.h"
#include "Block.h"
#include "TileEntityRenderer.h"
#include "Chunk.h"
#include "Renderer.h"
#include "Vec3D.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

#define JOB_QUEUED 0
#define JOB_RUNNING 1
#define JOB_DONE 2

typedef struct {
    int *rawBuffer;
    int rawBufferLen;
    int vertexCount;
    int mode;
    int flags;
    int hasContent;
} MeshPassResult;

typedef struct ChunkMeshJob {

    World *world;
    int posX, posY, posZ;
    int sizeWidth, sizeHeight, sizeDepth;

    WorldRenderer *wr;

    MeshPassResult results[2];
    TileEntityList tileEntityRenderers;
    int isChunkLit;

    atomic_int state;

    struct ChunkMeshJob *nextDone;
} ChunkMeshJob;

#define QUEUE_CAP 1024

static ChunkMeshJob **s_queue = NULL;
static int s_queueHead = 0;
static int s_queueTail = 0;
static int s_queueCount = 0;
static pthread_mutex_t s_queueMu;
static pthread_cond_t s_queueCv;
static pthread_cond_t s_drainCv;
static int s_activeWorkers = 0;
static int s_shutdown = 0;

static ChunkMeshJob *s_doneHead = NULL;
static ChunkMeshJob *s_doneTail = NULL;
static pthread_mutex_t s_doneMu;

static pthread_t *s_threads = NULL;
static int s_numThreads = 0;

static void capture_pass(Tessellator *tess, MeshPassResult *res, int hasContent) {
    if (tess->isDrawing && tess->vertexCount > 0) {
        int flags = 0;
        if (tess->hasTexture)
            flags |= R_HAS_TEXTURE;
        if (tess->hasColor)
            flags |= R_HAS_COLOR;
        if (tess->hasNormals)
            flags |= R_HAS_NORMALS;
        res->mode = (tess->drawMode == 7) ? R_TRIANGLES : tess->drawMode;
        res->flags = flags;
        res->vertexCount = tess->vertexCount;
        res->hasContent = hasContent;
        res->rawBufferLen = tess->rawBufferIndex;
        res->rawBuffer = (int *)malloc((size_t)tess->rawBufferIndex * sizeof(int));
        memcpy(res->rawBuffer, tess->rawBuffer, (size_t)tess->rawBufferIndex * sizeof(int));
    } else {
        res->rawBuffer = NULL;
        res->hasContent = 0;
    }

    tess->isDrawing = 0;
    tess->vertexCount = 0;
    tess->rawBufferIndex = 0;
    tess->addedVertices = 0;
    tess->xOffset = tess->yOffset = tess->zOffset = 0.0;
}

static void mesh_chunk(ChunkMeshJob *job) {
    Vec3D_initialize();
    int x1 = job->posX, y1 = job->posY, z1 = job->posZ;
    int x2 = x1 + job->sizeWidth, y2 = y1 + job->sizeHeight, z2 = z1 + job->sizeDepth;

    int pad = 1;
    ChunkCache cc;
    ChunkCache_init(&cc, job->world, x1 - pad, y1 - pad, z1 - pad, x2 + pad, y2 + pad, z2 + pad);

    RenderBlocks rb;
    RenderBlocks_init(&rb, (IBlockAccess *)&cc);

    Tessellator *tess = &Tessellator_instance;
    TileEntityList_init(&job->tileEntityRenderers);
    Chunk_isLit = 0;

    for (int pass = 0; pass < 2; pass++) {
        int anyBlock = 0;
        int hasContent = 0;
        int needPass2 = 0;

        for (int y = y1; y < y2; y++) {
            for (int z = z1; z < z2; z++) {
                for (int x = x1; x < x2; x++) {
                    int id = ChunkCache_getBlockId(&cc, x, y, z);
                    if (id <= 0)
                        continue;

                    if (!anyBlock) {
                        anyBlock = 1;
                        Tessellator_startDrawingQuads(tess);
                        Tessellator_setTranslationD(tess, (double)(-job->posX),
                                                    (double)(-job->posY), (double)(-job->posZ));
                    }

                    if (pass == 0 && Block_isBlockContainer[id]) {
                        TileEntity *te = ChunkCache_getBlockTileEntity(&cc, x, y, z);
                        if (TileEntityRenderer_hasSpecialRenderer(TileEntityRenderer_instance, te))
                            TileEntityList_add(&job->tileEntityRenderers, te);
                    }

                    Block *block = Block_blocksList[id];
                    if (!block)
                        continue;
                    int blockPass = block->vtable->getRenderBlockPass(block);
                    if (blockPass != pass) {
                        needPass2 = 1;
                    } else {
                        hasContent |= RenderBlocks_renderBlockByRenderType(&rb, block, x, y, z);
                    }
                }
            }
        }

        capture_pass(tess, &job->results[pass], hasContent);

        if (!needPass2)
            break;
    }

    job->isChunkLit = Chunk_isLit;
    ChunkCache_freeContents(&cc);
}

static void *worker_main(void *arg) {
    (void)arg;

    Tessellator_init(&Tessellator_instance);

    while (1) {
        pthread_mutex_lock(&s_queueMu);
        while (s_queueCount == 0 && !s_shutdown)
            pthread_cond_wait(&s_queueCv, &s_queueMu);

        if (s_shutdown && s_queueCount == 0) {
            pthread_mutex_unlock(&s_queueMu);
            break;
        }

        ChunkMeshJob *job = s_queue[s_queueHead];
        s_queueHead = (s_queueHead + 1) & (QUEUE_CAP - 1);
        s_queueCount--;
        s_activeWorkers++;
        pthread_mutex_unlock(&s_queueMu);

        atomic_store(&job->state, JOB_RUNNING);
        mesh_chunk(job);
        job->nextDone = NULL;
        atomic_store(&job->state, JOB_DONE);

        pthread_mutex_lock(&s_doneMu);
        if (s_doneTail)
            s_doneTail->nextDone = job;
        else
            s_doneHead = job;
        s_doneTail = job;
        pthread_mutex_unlock(&s_doneMu);

        pthread_mutex_lock(&s_queueMu);
        s_activeWorkers--;
        if (s_activeWorkers == 0 && s_queueCount == 0)
            pthread_cond_broadcast(&s_drainCv);
        pthread_mutex_unlock(&s_queueMu);
    }
    return NULL;
}

void ChunkMesher_init(int numThreads) {
    s_numThreads = numThreads;
    s_queue = (ChunkMeshJob **)calloc(QUEUE_CAP, sizeof(ChunkMeshJob *));
    pthread_mutex_init(&s_queueMu, NULL);
    pthread_cond_init(&s_queueCv, NULL);
    pthread_cond_init(&s_drainCv, NULL);
    pthread_mutex_init(&s_doneMu, NULL);
    s_threads = (pthread_t *)calloc(numThreads, sizeof(pthread_t));
    for (int i = 0; i < numThreads; i++)
        pthread_create(&s_threads[i], NULL, worker_main, NULL);
}

void ChunkMesher_drain(void) {
    pthread_mutex_lock(&s_queueMu);
    while (s_queueCount > 0 || s_activeWorkers > 0)
        pthread_cond_wait(&s_drainCv, &s_queueMu);
    pthread_mutex_unlock(&s_queueMu);

    ChunkMesher_flush();
}

void ChunkMesher_shutdown(void) {
    pthread_mutex_lock(&s_queueMu);
    s_shutdown = 1;
    pthread_cond_broadcast(&s_queueCv);
    pthread_mutex_unlock(&s_queueMu);
    for (int i = 0; i < s_numThreads; i++)
        pthread_join(s_threads[i], NULL);
    free(s_threads);
    free(s_queue);
    pthread_mutex_destroy(&s_queueMu);
    pthread_cond_destroy(&s_queueCv);
    pthread_cond_destroy(&s_drainCv);
    pthread_mutex_destroy(&s_doneMu);
    s_threads = NULL;
    s_queue = NULL;
}

void ChunkMesher_submit(WorldRenderer *wr) {
    ChunkMeshJob *job = (ChunkMeshJob *)calloc(1, sizeof(ChunkMeshJob));
    job->wr = wr;
    job->world = wr->worldObj;
    job->posX = wr->posX;
    job->posY = wr->posY;
    job->posZ = wr->posZ;
    job->sizeWidth = wr->sizeWidth;
    job->sizeHeight = wr->sizeHeight;
    job->sizeDepth = wr->sizeDepth;
    atomic_store(&job->state, JOB_QUEUED);

    wr->meshJob = job;
    wr->meshJobPending = 1;

    pthread_mutex_lock(&s_queueMu);
    if (s_queueCount >= QUEUE_CAP) {

        pthread_mutex_unlock(&s_queueMu);
        free(job);
        wr->meshJob = NULL;
        wr->meshJobPending = 0;
        WorldRenderer_updateRenderer(wr);
        wr->needsUpdate = 0;
        return;
    }
    s_queue[s_queueTail] = job;
    s_queueTail = (s_queueTail + 1) & (QUEUE_CAP - 1);
    s_queueCount++;
    pthread_cond_signal(&s_queueCv);
    pthread_mutex_unlock(&s_queueMu);
}

void ChunkMesher_cancel(WorldRenderer *wr) {
    if (!wr->meshJobPending)
        return;
    ChunkMeshJob *job = (ChunkMeshJob *)wr->meshJob;
    if (!job)
        return;

    pthread_mutex_lock(&s_queueMu);
    int found = 0;
    for (int i = 0; i < s_queueCount; i++) {
        int idx = (s_queueHead + i) & (QUEUE_CAP - 1);
        if (s_queue[idx] == job) {

            for (int j = i; j < s_queueCount - 1; j++) {
                int a = (s_queueHead + j) & (QUEUE_CAP - 1);
                int b = (s_queueHead + j + 1) & (QUEUE_CAP - 1);
                s_queue[a] = s_queue[b];
            }
            s_queueCount--;
            found = 1;
            break;
        }
    }
    pthread_mutex_unlock(&s_queueMu);

    if (found) {

        TileEntityList_free(&job->tileEntityRenderers);
        free(job);
    } else {

        job->wr = NULL;
    }

    wr->meshJob = NULL;
    wr->meshJobPending = 0;
}

int ChunkMesher_flush(void) {

    pthread_mutex_lock(&s_doneMu);
    ChunkMeshJob *list = s_doneHead;
    s_doneHead = NULL;
    s_doneTail = NULL;
    pthread_mutex_unlock(&s_doneMu);

    int uploaded = 0;
    for (ChunkMeshJob *job = list; job;) {
        ChunkMeshJob *next = job->nextDone;
        WorldRenderer *wr = job->wr;

        if (wr) {

            for (int pass = 0; pass < 2; pass++) {
                MeshPassResult *res = &job->results[pass];
                if (res->rawBuffer) {
                    R_bufferUpload(wr->vbo[pass], res->rawBuffer, res->vertexCount, res->mode,
                                   res->flags);
                    wr->skipRenderPass[pass] = res->hasContent ? 0 : 1;
                    free(res->rawBuffer);
                    res->rawBuffer = NULL;
                } else {
                    R_bufferUpload(wr->vbo[pass], NULL, 0, R_TRIANGLES, 0);
                    wr->skipRenderPass[pass] = 1;
                }
            }

            TileEntityList oldRenderers;
            TileEntityList_init(&oldRenderers);
            TileEntityList_addAll(&oldRenderers, &wr->tileEntityRenderers);
            TileEntityList_clear(&wr->tileEntityRenderers);
            TileEntityList_addAll(&wr->tileEntityRenderers, &job->tileEntityRenderers);

            TileEntityList added;
            TileEntityList_init(&added);
            TileEntityList_addAll(&added, &wr->tileEntityRenderers);
            TileEntityList_removeAll(&added, &oldRenderers);
            TileEntityList_addAll(wr->tileEntities, &added);
            TileEntityList_free(&added);

            TileEntityList_removeAll(&oldRenderers, &wr->tileEntityRenderers);
            TileEntityList_removeAll(wr->tileEntities, &oldRenderers);
            TileEntityList_free(&oldRenderers);

            wr->isChunkLit = job->isChunkLit;
            wr->isInitialized = 1;
            wr->meshJobPending = 0;
            wr->meshJob = NULL;
            ++uploaded;
        } else {

            for (int pass = 0; pass < 2; pass++)
                free(job->results[pass].rawBuffer);
        }

        TileEntityList_free(&job->tileEntityRenderers);
        free(job);
        job = next;
    }
    return uploaded;
}
