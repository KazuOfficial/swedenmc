#include "Renderer.h"
#include "RenderGlobal.h"
#include "RenderSorter.h"
#include "Minecraft.h"
#include "Tessellator.h"
#include "RenderHelper.h"
#include "Block.h"
#include "MathHelper.h"
#include "EntityRenderer.h"
#include "BiomeGenBase.h"
#include "JavaSort.h"
#include "RenderManager.h"
#include "TileEntityRenderer.h"
#include "EffectRenderer.h"
#include "EntityBubbleFX.h"
#include "EntitySmokeFX.h"
#include "EntityNoteFX.h"
#include "EntityPortalFX.h"
#include "EntityExplodeFX.h"
#include "EntityFlameFX.h"
#include "EntityLavaFX.h"
#include "EntityFootStepFX.h"
#include "EntitySplashFX.h"
#include "EntityReddustFX.h"
#include "EntitySlimeFX.h"
#include "EntitySnowShovelFX.h"
#include "EntityHeartFX.h"
#include "Item.h"
#include "World.h"
#include "SoundManager.h"
#include "GuiIngame.h"
#include "StepSound.h"
#include "JavaMath.h"
#include "ImageBufferDownload.h"
#include "ChunkMesher.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#ifdef _WIN32
#include "platform.h"
#else
#include <unistd.h>
#endif

static IWorldAccessVtable RenderGlobal_iwaVtable;

static void WRList_init(WorldRendererList *l) {
    l->data = NULL;
    l->size = 0;
    l->capacity = 0;
}
static void WRList_free(WorldRendererList *l) {
    free(l->data);
    WRList_init(l);
}
static void WRList_add(WorldRendererList *l, WorldRenderer *wr) {
    if (l->size >= l->capacity) {
        int nc = l->capacity ? l->capacity * 2 : 32;
        l->data = (WorldRenderer **)realloc(l->data, nc * sizeof(WorldRenderer *));
        l->capacity = nc;
    }
    l->data[l->size++] = wr;
}
static void WRList_clear(WorldRendererList *l) { l->size = 0; }
static int WRList_contains(WorldRendererList *l, WorldRenderer *wr) {
    for (int i = 0; i < l->size; i++)
        if (l->data[i] == wr)
            return 1;
    return 0;
}

static void renderStars(void) {
    Tessellator *tess = &Tessellator_instance;
    JavaRandom rand;
    JavaRandom_init(&rand, 10842LL);
    Tessellator_startDrawingQuads(tess);
    for (int i = 0; i < 1500; i++) {
        double x = (double)(JavaRandom_nextFloat(&rand) * 2.0f - 1.0f);
        double y = (double)(JavaRandom_nextFloat(&rand) * 2.0f - 1.0f);
        double z = (double)(JavaRandom_nextFloat(&rand) * 2.0f - 1.0f);
        double sz = (double)(0.25f + JavaRandom_nextFloat(&rand) * 0.25f);
        double len = x * x + y * y + z * z;
        if (len < 1.0 && len > 0.01) {
            double inv = 1.0 / sqrt(len);
            x *= inv;
            y *= inv;
            z *= inv;
            double sx = x * 100.0, sy = y * 100.0, sz2 = z * 100.0;
            double ay = atan2(x, z);
            double sinAY = sin(ay), cosAY = cos(ay);
            double ax = atan2(sqrt(x * x + z * z), y);
            double sinAX = sin(ax), cosAX = cos(ax);
            double ar = JavaRandom_nextDouble(&rand) * M_PI * 2.0;
            double sinAR = sin(ar), cosAR = cos(ar);
            for (int v = 0; v < 4; v++) {
                double a = (double)((v & 2) - 1) * sz;
                double b = (double)((v + 1 & 2) - 1) * sz;
                double px = a * cosAR - b * sinAR;
                double py = b * cosAR + a * sinAR;
                double qx = px * sinAX + 0.0 * cosAX;
                double qy = 0.0 * sinAX - px * cosAX;
                double rx = qy * sinAY - py * cosAY;
                double ry = py * sinAY + qy * cosAY;
                Tessellator_addVertex(tess, sx + rx, sy + qx, sz2 + ry);
            }
        }
    }
    Tessellator_draw(tess);
}

RenderGlobal *RenderGlobal_create(struct Minecraft *mc, RenderEngine *re) {
    RenderGlobal *self = (RenderGlobal *)calloc(1, sizeof(RenderGlobal));
    self->iwa.vtable = &RenderGlobal_iwaVtable;
    self->mc = mc;
    self->renderEngine = re;
    self->renderDistance = -1;
    self->prevSortX = self->prevSortY = self->prevSortZ = -9999.0;
    self->renderEntitiesStartupCounter = 2;

    {
#ifdef _WIN32
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        long nproc = (long)si.dwNumberOfProcessors;
#else
        long nproc = sysconf(_SC_NPROCESSORS_ONLN);
#endif
        int workers = (int)(nproc > 1 ? nproc - 1 : 1);
        if (workers > 4)
            workers = 4;
        ChunkMesher_init(workers);
    }

    int maxChunks = 64;

    self->occlusionEnabled = R_occlusionQueriesSupported();
    if (self->occlusionEnabled) {
        self->glOcclusionQueryBase =
            (unsigned int *)calloc(maxChunks * maxChunks * maxChunks, sizeof(unsigned int));
        R_genOcclusionQueries(maxChunks * maxChunks * maxChunks, self->glOcclusionQueryBase);
    }

    Tessellator *tess = &Tessellator_instance;

    self->starVBO = R_bufferCreate();
    renderStars();

    {
        JavaRandom rand;
        JavaRandom_init(&rand, 10842LL);
        Tessellator_startDrawingQuads(tess);
        for (int i = 0; i < 1500; i++) {
            double x = (double)(JavaRandom_nextFloat(&rand) * 2.0f - 1.0f);
            double y = (double)(JavaRandom_nextFloat(&rand) * 2.0f - 1.0f);
            double z = (double)(JavaRandom_nextFloat(&rand) * 2.0f - 1.0f);
            double sz = (double)(0.25f + JavaRandom_nextFloat(&rand) * 0.25f);
            double len = x * x + y * y + z * z;
            if (len < 1.0 && len > 0.01) {
                double inv = 1.0 / sqrt(len);
                x *= inv;
                y *= inv;
                z *= inv;
                double sx = x * 100.0, sy2 = y * 100.0, sz2 = z * 100.0;
                double ay = atan2(x, z);
                double sinAY = sin(ay), cosAY = cos(ay);
                double ax = atan2(sqrt(x * x + z * z), y);
                double sinAX = sin(ax), cosAX = cos(ax);
                double ar = JavaRandom_nextDouble(&rand) * M_PI * 2.0;
                double sinAR = sin(ar), cosAR = cos(ar);
                for (int v = 0; v < 4; v++) {
                    double a = (double)((v & 2) - 1) * sz;
                    double b = (double)((v + 1 & 2) - 1) * sz;
                    double px = a * cosAR - b * sinAR;
                    double py = b * cosAR + a * sinAR;
                    double qx = px * sinAX + 0.0 * cosAX;
                    double qy = 0.0 * sinAX - px * cosAX;
                    double rx = qy * sinAY - py * cosAY;
                    double ry = py * sinAY + qy * cosAY;
                    Tessellator_addVertex(tess, sx + rx, sy2 + qx, sz2 + ry);
                }
            }
        }
        Tessellator_uploadToBuffer(tess, self->starVBO);
    }

    self->skyVBO = R_bufferCreate();
    {
        int step = 64;
        int tiles = 256 / step + 2;
        float y = 16.0f;
        Tessellator_startDrawingQuads(tess);
        for (int bx = -step * tiles; bx <= step * tiles; bx += step) {
            for (int bz = -step * tiles; bz <= step * tiles; bz += step) {
                Tessellator_addVertex(tess, (double)(bx), (double)y, (double)(bz));
                Tessellator_addVertex(tess, (double)(bx + step), (double)y, (double)(bz));
                Tessellator_addVertex(tess, (double)(bx + step), (double)y, (double)(bz + step));
                Tessellator_addVertex(tess, (double)(bx), (double)y, (double)(bz + step));
            }
        }
        Tessellator_uploadToBuffer(tess, self->skyVBO);
    }

    self->skyVBO2 = R_bufferCreate();
    {
        int step = 64;
        int tiles = 256 / step + 2;
        float y = -16.0f;
        Tessellator_startDrawingQuads(tess);
        for (int bx = -step * tiles; bx <= step * tiles; bx += step) {
            for (int bz = -step * tiles; bz <= step * tiles; bz += step) {
                Tessellator_addVertex(tess, (double)(bx + step), (double)y, (double)(bz));
                Tessellator_addVertex(tess, (double)(bx), (double)y, (double)(bz));
                Tessellator_addVertex(tess, (double)(bx), (double)y, (double)(bz + step));
                Tessellator_addVertex(tess, (double)(bx + step), (double)y, (double)(bz + step));
            }
        }
        Tessellator_uploadToBuffer(tess, self->skyVBO2);
    }

    TileEntityList_init(&self->tileEntities);
    WRList_init(&self->worldRenderersToUpdate);
    WRList_init(&self->glRenderLists);
    for (int i = 0; i < 4; i++)
        RenderList_init(&self->allRenderLists[i]);

    return self;
}

void RenderGlobal_func_28137_f(RenderGlobal *self) { (void)self; }

void RenderGlobal_destroy(RenderGlobal *self) {
    ChunkMesher_drain();
    ChunkMesher_shutdown();
    if (self->worldRenderers) {
        for (int i = 0; i < self->numRenderers; i++)
            WorldRenderer_destroy(self->worldRenderers[i]);
        free(self->worldRenderers);
    }
    free(self->sortedWorldRenderers);
    R_bufferDestroy(self->starVBO);
    R_bufferDestroy(self->skyVBO);
    R_bufferDestroy(self->skyVBO2);
    free(self->glOcclusionQueryBase);
    TileEntityList_free(&self->tileEntities);
    WRList_free(&self->worldRenderersToUpdate);
    WRList_free(&self->glRenderLists);
    for (int i = 0; i < 4; i++)
        RenderList_func_859_b(&self->allRenderLists[i]);
    free(self);
}

void RenderGlobal_changeWorld(RenderGlobal *self, World *world) {
    if (self->worldObj != NULL) {
        World_removeWorldAccess(self->worldObj, &self->iwa);

        ChunkMesher_drain();
    }
    self->prevSortX = -9999.0;
    self->prevSortY = -9999.0;
    self->prevSortZ = -9999.0;
    RenderManager_func_852_a(RenderManager_instance, world);
    self->worldObj = world;
    RenderBlocks_init(&self->globalRenderBlocks, (IBlockAccess *)world);
    if (world != NULL) {
        World_addWorldAccess(world, &self->iwa);
        RenderGlobal_loadRenderers(self);
    }
}

static EntityLiving *_sortPlayer = NULL;

static int wrCompare(const void *a, const void *b) {
    WorldRenderer *wa = *(WorldRenderer **)a;
    WorldRenderer *wb = *(WorldRenderer **)b;
    float da = WorldRenderer_distanceToEntitySquared(wa, (Entity *)_sortPlayer);
    float db = WorldRenderer_distanceToEntitySquared(wb, (Entity *)_sortPlayer);
    if (da > db)
        return 1;
    if (da < db)
        return -1;
    return 0;
}

static void RenderGlobal_markRenderersForNewPosition(RenderGlobal *self, int var1, int var2,
                                                     int var3) {
    var1 -= 8;
    var2 -= 8;
    var3 -= 8;
    (void)var2;
    self->minBlockX = 0x7fffffff;
    self->minBlockY = 0x7fffffff;
    self->minBlockZ = 0x7fffffff;
    self->maxBlockX = (int)0x80000000;
    self->maxBlockY = (int)0x80000000;
    self->maxBlockZ = (int)0x80000000;
    int range = self->renderChunksWide * 16;
    int half = range / 2;
    for (int xi = 0; xi < self->renderChunksWide; xi++) {
        int wx = xi * 16;
        int off = wx + half - var1;
        if (off < 0)
            off -= range - 1;
        off /= range;
        wx -= off * range;
        if (wx < self->minBlockX)
            self->minBlockX = wx;
        if (wx > self->maxBlockX)
            self->maxBlockX = wx;
        for (int zi = 0; zi < self->renderChunksDeep; zi++) {
            int wz = zi * 16;
            int offz = wz + half - var3;
            if (offz < 0)
                offz -= range - 1;
            offz /= range;
            wz -= offz * range;
            if (wz < self->minBlockZ)
                self->minBlockZ = wz;
            if (wz > self->maxBlockZ)
                self->maxBlockZ = wz;
            for (int yi = 0; yi < self->renderChunksTall; yi++) {
                int wy = yi * 16;
                if (wy < self->minBlockY)
                    self->minBlockY = wy;
                if (wy > self->maxBlockY)
                    self->maxBlockY = wy;
                int idx = (zi * self->renderChunksTall + yi) * self->renderChunksWide + xi;
                WorldRenderer *wr = self->worldRenderers[idx];
                int wasNeeded = wr->needsUpdate;
                WorldRenderer_setPosition(wr, wx, wy, wz);
                if (!wasNeeded && wr->needsUpdate)
                    WRList_add(&self->worldRenderersToUpdate, wr);
            }
        }
    }
}

void RenderGlobal_loadRenderers(RenderGlobal *self) {
    if (self->worldObj == NULL)
        return;

    extern Block *Block_leaves;
    extern void BlockLeaves_setGraphicsLevel(Block * b, int level);
    BlockLeaves_setGraphicsLevel(Block_leaves, self->mc->gameSettings->fancyGraphics);
    self->renderDistance = self->mc->gameSettings->renderDistance;

    ChunkMesher_drain();

    if (self->worldRenderers) {
        for (int i = 0; i < self->numRenderers; i++)
            WorldRenderer_destroy(self->worldRenderers[i]);
        free(self->worldRenderers);
        free(self->sortedWorldRenderers);
    }

    WRList_clear(&self->worldRenderersToUpdate);
    TileEntityList_clear(&self->tileEntities);

    int range = 64 << (3 - self->mc->gameSettings->renderDistance);
    if (range > 400)
        range = 400;
    self->renderChunksWide = range / 16 + 1;
    self->renderChunksTall = 8;
    self->renderChunksDeep = range / 16 + 1;
    int total = self->renderChunksWide * self->renderChunksTall * self->renderChunksDeep;
    self->numRenderers = total;

    self->worldRenderers = (WorldRenderer **)calloc(total, sizeof(WorldRenderer *));
    self->sortedWorldRenderers = (WorldRenderer **)calloc(total, sizeof(WorldRenderer *));

    self->minBlockX = 0;
    self->minBlockY = 0;
    self->minBlockZ = 0;
    self->maxBlockX = self->renderChunksWide;
    self->maxBlockY = self->renderChunksTall;
    self->maxBlockZ = self->renderChunksDeep;

    int chunkIdx = 0;
    for (int xi = 0; xi < self->renderChunksWide; xi++) {
        for (int yi = 0; yi < self->renderChunksTall; yi++) {
            for (int zi = 0; zi < self->renderChunksDeep; zi++) {
                int idx = (zi * self->renderChunksTall + yi) * self->renderChunksWide + xi;
                WorldRenderer *wr = WorldRenderer_create(self->worldObj, &self->tileEntities,
                                                         xi * 16, yi * 16, zi * 16, 16, 0);

                if (self->occlusionEnabled) {
                    wr->glOcclusionQuery = (int)self->glOcclusionQueryBase[chunkIdx];
                }
                wr->isWaitingOnOcclusionQuery = 0;
                wr->isVisible = 1;
                wr->isInFrustum = 1;
                wr->chunkIndex = chunkIdx++;
                WorldRenderer_markDirty(wr);
                self->worldRenderers[idx] = wr;
                self->sortedWorldRenderers[idx] = wr;
                WRList_add(&self->worldRenderersToUpdate, wr);
            }
        }
    }

    if (self->worldObj != NULL) {
        EntityLiving *ve = self->mc->renderViewEntity;
        if (ve != NULL) {
            RenderGlobal_markRenderersForNewPosition(self,
                                                     MathHelper_floor_double(((Entity *)ve)->posX),
                                                     MathHelper_floor_double(((Entity *)ve)->posY),
                                                     MathHelper_floor_double(((Entity *)ve)->posZ));
            _sortPlayer = ve;
            Arrays_sort((void **)self->sortedWorldRenderers, total, wrCompare);
            _sortPlayer = NULL;
        }
    }

    self->renderEntitiesStartupCounter = 2;
}

int RenderGlobal_updateRenderers(RenderGlobal *self, EntityLiving *viewer, int forceFrustum) {

    ChunkMesher_flush();

    const int var4 = 2;
    RenderSorter var5;
    var5.baseEntity = viewer;
    WorldRenderer *var6[2] = {NULL, NULL};

    int var8 = self->worldRenderersToUpdate.size;
    int var9 = 0;

    WorldRenderer **var7 =
        var8 > 0 ? (WorldRenderer **)malloc(var8 * sizeof(WorldRenderer *)) : NULL;

    for (int var10 = 0; var10 < var8; ++var10) {
        WorldRenderer *var11 = self->worldRenderersToUpdate.data[var10];
        if (!forceFrustum) {
            if (WorldRenderer_distanceToEntitySquared(var11, (Entity *)viewer) > 256.0f) {

                int var12;
                for (var12 = 0;
                     var12 < var4 && (var6[var12] == NULL ||
                                      RenderSorter_doCompare(&var5, var6[var12], var11) <= 0);
                     ++var12) {
                }
                --var12;
                if (var12 <= 0)
                    continue;

                int var13 = var12;
                while (1) {
                    --var13;
                    if (var13 == 0) {
                        var6[var12] = var11;
                        break;
                    }
                    var6[var13 - 1] = var6[var13];
                }
                continue;
            }
        } else if (!var11->isInFrustum) {
            continue;
        }

        ++var9;
        var7[var9 - 1] = var11;
        self->worldRenderersToUpdate.data[var10] = NULL;
    }

    for (int i = 1; i < var9; i++) {
        WorldRenderer *key = var7[i];
        int j = i - 1;
        while (j >= 0 && RenderSorter_doCompare(&var5, var7[j], key) > 0) {
            var7[j + 1] = var7[j];
            j--;
        }
        var7[j + 1] = key;
    }

    for (int i = var9 - 1; i >= 0; --i) {
        WorldRenderer *wr = var7[i];
        if (wr->meshJobPending) {

            WRList_add(&self->worldRenderersToUpdate, wr);
        } else {
            ChunkMesher_submit(wr);
            wr->needsUpdate = 0;
        }
    }
    free(var7);

    int var10_updated = 0;
    for (int var21 = var4 - 1; var21 >= 0; --var21) {
        WorldRenderer *var22 = var6[var21];
        if (var22 != NULL) {
            if (!var22->isInFrustum && var21 != var4 - 1) {
                var6[var21] = NULL;
                var6[0] = NULL;
                break;
            }
            if (var22->meshJobPending) {
                WRList_add(&self->worldRenderersToUpdate, var22);
            } else {
                ChunkMesher_submit(var22);
                var22->needsUpdate = 0;
                ++var10_updated;
            }
        }
    }

    int compact_read = 0, compact_write = 0;
    int list_size = self->worldRenderersToUpdate.size;
    for (; compact_read != list_size; ++compact_read) {
        WorldRenderer *var14 = self->worldRenderersToUpdate.data[compact_read];
        if (var14 != NULL) {
            int in_var6 = 0;
            for (int var16 = 0; var16 < var4 && !in_var6; ++var16)
                if (var14 == var6[var16])
                    in_var6 = 1;
            if (!in_var6) {
                if (compact_write != compact_read)
                    self->worldRenderersToUpdate.data[compact_write] = var14;
                ++compact_write;
            }
        }
    }
    self->worldRenderersToUpdate.size = compact_write;

    return var8 == var9 + var10_updated;
}

void RenderGlobal_clipRenderersByFrustrum(RenderGlobal *self, ICamera *camera, float partialTick) {
    (void)partialTick;
    for (int i = 0; i < self->numRenderers; i++) {
        WorldRenderer *wr = self->worldRenderers[i];
        if (!WorldRenderer_skipAllRenderPasses(wr) &&
            (!wr->isInFrustum || (i + self->frustrumCheckOffset & 15) == 0)) {
            WorldRenderer_updateInFrustrum(wr, camera);
        }
    }
    ++self->frustrumCheckOffset;
}

static int RenderGlobal_renderSortedRenderers(RenderGlobal *self, int from, int to, int pass,
                                              double partialTick) {

    WRList_clear(&self->glRenderLists);
    int var6 = 0;

    for (int var7 = from; var7 < to; ++var7) {
        WorldRenderer *wr = self->sortedWorldRenderers[var7];
        if (pass == 0) {
            ++self->renderersLoaded;
            if (wr->skipRenderPass[pass])
                ++self->renderersSkippingRenderPass;
            else if (!wr->isInFrustum)
                ++self->renderersBeingClipped;
            else if (self->occlusionEnabled && !wr->isVisible)
                ++self->renderersBeingOccluded;
            else
                ++self->renderersBeingRendered;
        }
        if (!wr->skipRenderPass[pass] && wr->isInFrustum &&
            (!self->occlusionEnabled || wr->isVisible)) {
            RBuffer *var8 = WorldRenderer_getBufferForPass(wr, pass);
            if (var8 != NULL) {
                WRList_add(&self->glRenderLists, wr);
                ++var6;
            }
        }
    }

    Entity *var19 = (Entity *)self->mc->renderViewEntity;
    double var20 = var19->lastTickPosX + (var19->posX - var19->lastTickPosX) * partialTick;
    double var10 = var19->lastTickPosY + (var19->posY - var19->lastTickPosY) * partialTick;
    double var12 = var19->lastTickPosZ + (var19->posZ - var19->lastTickPosZ) * partialTick;
    int var14 = 0;

    for (int var15 = 0; var15 < 4; ++var15) {
        RenderList_func_859_b(&self->allRenderLists[var15]);
    }

    for (int var15 = 0; var15 < self->glRenderLists.size; ++var15) {
        WorldRenderer *var16 = self->glRenderLists.data[var15];
        int var17 = -1;

        for (int var18 = 0; var18 < var14; ++var18) {
            if (RenderList_func_862_a(&self->allRenderLists[var18], var16->posXMinus,
                                      var16->posYMinus, var16->posZMinus)) {
                var17 = var18;
            }
        }

        if (var17 < 0) {
            var17 = var14++;
            RenderList_func_861_a(&self->allRenderLists[var17], var16->posXMinus, var16->posYMinus,
                                  var16->posZMinus, var20, var10, var12);
        }

        RenderList_func_858_a(&self->allRenderLists[var17],
                              WorldRenderer_getBufferForPass(var16, pass), (float)var16->posXClip,
                              (float)var16->posYClip, (float)var16->posZClip);
    }

    RenderGlobal_renderAllRenderLists(self, pass, partialTick);
    return var6;
}

static void RenderGlobal_checkOcclusionQueryResult(RenderGlobal *self, int var1, int var2) {
    for (int var3 = var1; var3 < var2; ++var3) {
        WorldRenderer *wr = self->sortedWorldRenderers[var3];
        if (wr->isWaitingOnOcclusionQuery) {
            if (R_occlusionQueryAvailable((unsigned int)wr->glOcclusionQuery)) {
                wr->isWaitingOnOcclusionQuery = 0;
                unsigned int result = R_occlusionQueryResult((unsigned int)wr->glOcclusionQuery);
                wr->isVisible = (result != 0) ? 1 : 0;
            }
        }
    }
}

int RenderGlobal_sortAndRender(RenderGlobal *self, EntityLiving *viewer, int pass,
                               double partialTick) {

    for (int i = 0; i < 10; i++) {
        self->worldRenderersCheckIndex = (self->worldRenderersCheckIndex + 1) % self->numRenderers;
        WorldRenderer *wr = self->worldRenderers[self->worldRenderersCheckIndex];
        if (wr->needsUpdate && !WRList_contains(&self->worldRenderersToUpdate, wr))
            WRList_add(&self->worldRenderersToUpdate, wr);
    }

    if (self->mc->gameSettings->renderDistance != self->renderDistance)
        RenderGlobal_loadRenderers(self);

    if (pass == 0) {
        self->renderersLoaded = 0;
        self->renderersBeingClipped = 0;
        self->renderersBeingOccluded = 0;
        self->renderersBeingRendered = 0;
        self->renderersSkippingRenderPass = 0;
    }

    Entity *ve = (Entity *)viewer;
    double dx = ve->posX - self->prevSortX;
    double dy = ve->posY - self->prevSortY;
    double dz = ve->posZ - self->prevSortZ;
    if (dx * dx + dy * dy + dz * dz > 16.0) {
        self->prevSortX = ve->posX;
        self->prevSortY = ve->posY;
        self->prevSortZ = ve->posZ;
        RenderGlobal_markRenderersForNewPosition(self, MathHelper_floor_double(ve->posX),
                                                 MathHelper_floor_double(ve->posY),
                                                 MathHelper_floor_double(ve->posZ));
        _sortPlayer = viewer;
        Arrays_sort((void **)self->sortedWorldRenderers, self->numRenderers, wrCompare);
        _sortPlayer = NULL;
    }

    double var33 = ve->lastTickPosX + (ve->posX - ve->lastTickPosX) * partialTick;
    double var7 = ve->lastTickPosY + (ve->posY - ve->lastTickPosY) * partialTick;
    double var9 = ve->lastTickPosZ + (ve->posZ - ve->lastTickPosZ) * partialTick;

    RenderHelper_disableStandardItemLighting();
    int var17 = 0;
    int var34;

    if (self->occlusionEnabled && self->mc->gameSettings->advancedOpengl &&
        !self->mc->gameSettings->anaglyph && pass == 0) {
        int var18 = 0;
        int var19 = 16;
        RenderGlobal_checkOcclusionQueryResult(self, var18, var19);
        for (int var20 = var18; var20 < var19; ++var20) {
            self->sortedWorldRenderers[var20]->isVisible = 1;
        }
        var34 = var17 + RenderGlobal_renderSortedRenderers(self, var18, var19, pass, partialTick);

        do {
            int var35 = var19;
            var19 *= 2;
            if (var19 > self->numRenderers)
                var19 = self->numRenderers;

            R_disable(R_TEXTURE_2D);
            R_disable(R_LIGHTING);
            R_disable(R_ALPHA_TEST);
            R_disable(R_FOG);
            R_colorMask(R_FALSE, R_FALSE, R_FALSE, R_FALSE);
            R_depthMask(R_FALSE);
            RenderGlobal_checkOcclusionQueryResult(self, var35, var19);
            R_pushMatrix();
            float var36 = 0.0f;
            float var21 = 0.0f;
            float var22 = 0.0f;

            for (int var23 = var35; var23 < var19; ++var23) {
                WorldRenderer *wr23 = self->sortedWorldRenderers[var23];
                if (WorldRenderer_skipAllRenderPasses(wr23)) {
                    wr23->isInFrustum = 0;
                } else {
                    if (!wr23->isInFrustum) {
                        wr23->isVisible = 1;
                    }
                    if (wr23->isInFrustum && !wr23->isWaitingOnOcclusionQuery) {
                        float var24 = (float)sqrt(WorldRenderer_distanceToEntitySquared(wr23, ve));
                        int var25 = (int)(1.0f + var24 / 128.0f);
                        if (self->cloudOffsetX % var25 == var23 % var25) {
                            float var27 = (float)((double)wr23->posXMinus - var33);
                            float var28 = (float)((double)wr23->posYMinus - var7);
                            float var29 = (float)((double)wr23->posZMinus - var9);
                            float var30 = var27 - var36;
                            float var31 = var28 - var21;
                            float var32 = var29 - var22;
                            if (var30 != 0.0f || var31 != 0.0f || var32 != 0.0f) {
                                R_translatef(var30, var31, var32);
                                var36 += var30;
                                var21 += var31;
                                var22 += var32;
                            }
                            R_beginOcclusionQuery((unsigned int)wr23->glOcclusionQuery);
                            WorldRenderer_callOcclusionQueryList(wr23);
                            R_endOcclusionQuery();
                            wr23->isWaitingOnOcclusionQuery = 1;
                        }
                    }
                }
            }

            R_popMatrix();
            if (self->mc->gameSettings->anaglyph) {
                if (EntityRenderer_anaglyphField == 0) {
                    R_colorMask(R_FALSE, R_TRUE, R_TRUE, R_TRUE);
                } else {
                    R_colorMask(R_TRUE, R_FALSE, R_FALSE, R_TRUE);
                }
            } else {
                R_colorMask(R_TRUE, R_TRUE, R_TRUE, R_TRUE);
            }
            R_depthMask(R_TRUE);
            R_enable(R_TEXTURE_2D);
            R_enable(R_ALPHA_TEST);
            R_enable(R_FOG);
            var34 += RenderGlobal_renderSortedRenderers(self, var35, var19, pass, partialTick);
        } while (var19 < self->numRenderers);
    } else {
        var34 = var17 +
                RenderGlobal_renderSortedRenderers(self, 0, self->numRenderers, pass, partialTick);
    }

    return var34;
}

void RenderGlobal_renderAllRenderLists(RenderGlobal *self, int pass, double partialTick) {
    (void)pass;
    (void)partialTick;
    for (int i = 0; i < 4; i++)
        RenderList_func_860_a(&self->allRenderLists[i]);
}

void RenderGlobal_renderSky(RenderGlobal *self, float var1) {
    if (self->worldObj->worldProvider->isNether)
        return;

    Tessellator *var17 = &Tessellator_instance;

    Vec3D *var2 = World_getSkyColor(self->worldObj, (Entity *)self->mc->renderViewEntity, var1);
    float var3 = (float)var2->xCoord;
    float var4 = (float)var2->yCoord;
    float var5 = (float)var2->zCoord;
    float var7;
    float var8;
    if (self->mc->gameSettings->anaglyph) {
        float var6 = (var3 * 30.0f + var4 * 59.0f + var5 * 11.0f) / 100.0f;
        var7 = (var3 * 30.0f + var4 * 70.0f) / 100.0f;
        var8 = (var3 * 30.0f + var5 * 70.0f) / 100.0f;
        var3 = var6;
        var4 = var7;
        var5 = var8;
    }

    R_disable(R_TEXTURE_2D);
    R_color3f(var3, var4, var5);
    R_depthMask(R_FALSE);
    R_enable(R_FOG);
    R_color3f(var3, var4, var5);
    R_bufferDraw(self->skyVBO, 0.0f, 0.0f, 0.0f);
    R_disable(R_FOG);
    R_disable(R_ALPHA_TEST);
    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    RenderHelper_disableStandardItemLighting();

    float *var18 = WorldProvider_calcSunriseSunsetColors(
        self->worldObj->worldProvider, World_getCelestialAngle(self->worldObj, var1), var1);
    float var9;
    float var10;
    float var11;
    float var12;
    if (var18 != NULL) {
        R_disable(R_TEXTURE_2D);
        R_shadeModel(R_SMOOTH);
        R_pushMatrix();
        R_rotatef(90.0f, 1.0f, 0.0f, 0.0f);
        var8 = World_getCelestialAngle(self->worldObj, var1);
        R_rotatef(var8 > 0.5f ? 180.0f : 0.0f, 0.0f, 0.0f, 1.0f);
        var9 = var18[0];
        var10 = var18[1];
        var11 = var18[2];
        float var14;
        if (self->mc->gameSettings->anaglyph) {
            var12 = (var9 * 30.0f + var10 * 59.0f + var11 * 11.0f) / 100.0f;
            float var13 = (var9 * 30.0f + var10 * 70.0f) / 100.0f;
            var14 = (var9 * 30.0f + var11 * 70.0f) / 100.0f;
            var9 = var12;
            var10 = var13;
            var11 = var14;
        }
        Tessellator_startDrawing(var17, R_TRIANGLE_FAN);
        Tessellator_setColorRGBA_F(var17, var9, var10, var11, var18[3]);
        Tessellator_addVertex(var17, 0.0, 100.0, 0.0);
        int var19 = 16;
        Tessellator_setColorRGBA_F(var17, var18[0], var18[1], var18[2], 0.0f);
        for (int var20 = 0; var20 <= var19; ++var20) {
            var14 = (float)var20 * (float)M_PI * 2.0f / (float)var19;
            float var15 = MathHelper_sin(var14);
            float var16 = MathHelper_cos(var14);
            Tessellator_addVertex(var17, (double)(var15 * 120.0f), (double)(var16 * 120.0f),
                                  (double)(-var16 * 40.0f * var18[3]));
        }
        Tessellator_draw(var17);
        R_popMatrix();
        R_shadeModel(R_FLAT);
    }

    R_enable(R_TEXTURE_2D);
    R_blendFunc(R_SRC_ALPHA, R_ONE);
    R_pushMatrix();

    var7 = 1.0f - World_func_27162_g(self->worldObj, var1);
    var8 = 0.0f;
    var9 = 0.0f;
    var10 = 0.0f;
    R_color4f(1.0f, 1.0f, 1.0f, var7);
    R_translatef(var8, var9, var10);
    R_rotatef(0.0f, 0.0f, 0.0f, 1.0f);
    R_rotatef(World_getCelestialAngle(self->worldObj, var1) * 360.0f, 1.0f, 0.0f, 0.0f);
    var11 = 30.0f;
    R_bindTexture(RenderEngine_getTexture(self->renderEngine, "/terrain/sun.png"));
    Tessellator_startDrawingQuads(var17);
    Tessellator_addVertexWithUV(var17, (double)(-var11), 100.0, (double)(-var11), 0.0, 0.0);
    Tessellator_addVertexWithUV(var17, (double)var11, 100.0, (double)(-var11), 1.0, 0.0);
    Tessellator_addVertexWithUV(var17, (double)var11, 100.0, (double)var11, 1.0, 1.0);
    Tessellator_addVertexWithUV(var17, (double)(-var11), 100.0, (double)var11, 0.0, 1.0);
    Tessellator_draw(var17);
    var11 = 20.0f;
    R_bindTexture(RenderEngine_getTexture(self->renderEngine, "/terrain/moon.png"));
    Tessellator_startDrawingQuads(var17);
    Tessellator_addVertexWithUV(var17, (double)(-var11), -100.0, (double)var11, 1.0, 1.0);
    Tessellator_addVertexWithUV(var17, (double)var11, -100.0, (double)var11, 0.0, 1.0);
    Tessellator_addVertexWithUV(var17, (double)var11, -100.0, (double)(-var11), 0.0, 0.0);
    Tessellator_addVertexWithUV(var17, (double)(-var11), -100.0, (double)(-var11), 1.0, 0.0);
    Tessellator_draw(var17);
    R_disable(R_TEXTURE_2D);
    var12 = World_getStarBrightness(self->worldObj, var1) * var7;
    if (var12 > 0.0f) {
        R_color4f(var12, var12, var12, var12);
        R_bufferDraw(self->starVBO, 0.0f, 0.0f, 0.0f);
    }
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    R_disable(R_BLEND);
    R_enable(R_ALPHA_TEST);
    R_enable(R_FOG);
    R_popMatrix();

    if (self->worldObj->worldProvider->vtable->func_28112_c(self->worldObj->worldProvider)) {
        R_color3f(var3 * 0.2f + 0.04f, var4 * 0.2f + 0.04f, var5 * 0.6f + 0.1f);
    } else {
        R_color3f(var3, var4, var5);
    }
    R_disable(R_TEXTURE_2D);
    R_bufferDraw(self->skyVBO2, 0.0f, 0.0f, 0.0f);
    R_enable(R_TEXTURE_2D);
    R_depthMask(R_TRUE);
}

static void RenderGlobal_renderCloudsFancy(RenderGlobal *self, float var1) {
    R_disable(R_CULL_FACE);
    Entity *ve = (Entity *)self->mc->renderViewEntity;
    float var2 = (float)(ve->lastTickPosY + (ve->posY - ve->lastTickPosY) * (double)var1);
    Tessellator *var3 = &Tessellator_instance;
    float var4 = 12.0f;
    float var5 = 4.0f;
    double var6 = (ve->prevPosX + (ve->posX - ve->prevPosX) * (double)var1 +
                   (double)(((float)self->cloudOffsetX + var1) * 0.03f)) /
                  (double)var4;
    double var8 =
        (ve->prevPosZ + (ve->posZ - ve->prevPosZ) * (double)var1) / (double)var4 + (double)0.33f;
    float var10 = WorldProvider_getCloudHeight(self->worldObj->worldProvider) - var2 + 0.33f;
    int var11 = MathHelper_floor_double(var6 / 2048.0);
    int var12 = MathHelper_floor_double(var8 / 2048.0);
    var6 -= (double)(var11 * 2048);
    var8 -= (double)(var12 * 2048);
    R_bindTexture(RenderEngine_getTexture(self->renderEngine, "/environment/clouds.png"));
    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    Vec3D *var13 = World_getCloudColor(self->worldObj, var1);
    float var14 = (float)var13->xCoord;
    float var15 = (float)var13->yCoord;
    float var16 = (float)var13->zCoord;
    float var17, var18, var19;
    if (self->mc->gameSettings->anaglyph) {
        var17 = (var14 * 30.0f + var15 * 59.0f + var16 * 11.0f) / 100.0f;
        var18 = (var14 * 30.0f + var15 * 70.0f) / 100.0f;
        var19 = (var14 * 30.0f + var16 * 70.0f) / 100.0f;
        var14 = var17;
        var15 = var18;
        var16 = var19;
    }

    var17 = (float)(var6 * 0.0);
    var18 = (float)(var8 * 0.0);
    var19 = 0.00390625f;
    var17 = (float)MathHelper_floor_double(var6) * var19;
    var18 = (float)MathHelper_floor_double(var8) * var19;
    float var20 = (float)(var6 - (double)MathHelper_floor_double(var6));
    float var21 = (float)(var8 - (double)MathHelper_floor_double(var8));
    int var22 = 8;
    int var23 = 3;
    float var24 = 1.0f / 1024.0f;
    R_scalef(var4, 1.0f, var4);

    for (int var25 = 0; var25 < 2; ++var25) {
        if (var25 == 0) {
            R_colorMask(R_FALSE, R_FALSE, R_FALSE, R_FALSE);
        } else if (self->mc->gameSettings->anaglyph) {
            if (EntityRenderer_anaglyphField == 0) {
                R_colorMask(R_FALSE, R_TRUE, R_TRUE, R_TRUE);
            } else {
                R_colorMask(R_TRUE, R_FALSE, R_FALSE, R_TRUE);
            }
        } else {
            R_colorMask(R_TRUE, R_TRUE, R_TRUE, R_TRUE);
        }

        for (int var26 = -var23 + 1; var26 <= var23; ++var26) {
            for (int var27 = -var23 + 1; var27 <= var23; ++var27) {
                Tessellator_startDrawingQuads(var3);
                float var28 = (float)(var26 * var22);
                float var29 = (float)(var27 * var22);
                float var30 = var28 - var20;
                float var31 = var29 - var21;
                int var32;
                if (var10 > -var5 - 1.0f) {
                    Tessellator_setColorRGBA_F(var3, var14 * 0.7f, var15 * 0.7f, var16 * 0.7f,
                                               0.8f);
                    Tessellator_setNormal(var3, 0.0f, -1.0f, 0.0f);
                    Tessellator_addVertexWithUV(
                        var3, (double)(var30 + 0.0f), (double)(var10 + 0.0f),
                        (double)(var31 + (float)var22), (double)((var28 + 0.0f) * var19 + var17),
                        (double)((var29 + (float)var22) * var19 + var18));
                    Tessellator_addVertexWithUV(var3, (double)(var30 + (float)var22),
                                                (double)(var10 + 0.0f),
                                                (double)(var31 + (float)var22),
                                                (double)((var28 + (float)var22) * var19 + var17),
                                                (double)((var29 + (float)var22) * var19 + var18));
                    Tessellator_addVertexWithUV(var3, (double)(var30 + (float)var22),
                                                (double)(var10 + 0.0f), (double)(var31 + 0.0f),
                                                (double)((var28 + (float)var22) * var19 + var17),
                                                (double)((var29 + 0.0f) * var19 + var18));
                    Tessellator_addVertexWithUV(var3, (double)(var30 + 0.0f),
                                                (double)(var10 + 0.0f), (double)(var31 + 0.0f),
                                                (double)((var28 + 0.0f) * var19 + var17),
                                                (double)((var29 + 0.0f) * var19 + var18));
                }
                if (var10 <= var5 + 1.0f) {
                    Tessellator_setColorRGBA_F(var3, var14, var15, var16, 0.8f);
                    Tessellator_setNormal(var3, 0.0f, 1.0f, 0.0f);
                    Tessellator_addVertexWithUV(
                        var3, (double)(var30 + 0.0f), (double)(var10 + var5 - var24),
                        (double)(var31 + (float)var22), (double)((var28 + 0.0f) * var19 + var17),
                        (double)((var29 + (float)var22) * var19 + var18));
                    Tessellator_addVertexWithUV(var3, (double)(var30 + (float)var22),
                                                (double)(var10 + var5 - var24),
                                                (double)(var31 + (float)var22),
                                                (double)((var28 + (float)var22) * var19 + var17),
                                                (double)((var29 + (float)var22) * var19 + var18));
                    Tessellator_addVertexWithUV(
                        var3, (double)(var30 + (float)var22), (double)(var10 + var5 - var24),
                        (double)(var31 + 0.0f), (double)((var28 + (float)var22) * var19 + var17),
                        (double)((var29 + 0.0f) * var19 + var18));
                    Tessellator_addVertexWithUV(
                        var3, (double)(var30 + 0.0f), (double)(var10 + var5 - var24),
                        (double)(var31 + 0.0f), (double)((var28 + 0.0f) * var19 + var17),
                        (double)((var29 + 0.0f) * var19 + var18));
                }
                Tessellator_setColorRGBA_F(var3, var14 * 0.9f, var15 * 0.9f, var16 * 0.9f, 0.8f);
                if (var26 > -1) {
                    Tessellator_setNormal(var3, -1.0f, 0.0f, 0.0f);
                    for (var32 = 0; var32 < var22; ++var32) {
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var32 + 0.0f), (double)(var10 + 0.0f),
                            (double)(var31 + (float)var22),
                            (double)((var28 + (float)var32 + 0.5f) * var19 + var17),
                            (double)((var29 + (float)var22) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var32 + 0.0f), (double)(var10 + var5),
                            (double)(var31 + (float)var22),
                            (double)((var28 + (float)var32 + 0.5f) * var19 + var17),
                            (double)((var29 + (float)var22) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var32 + 0.0f), (double)(var10 + var5),
                            (double)(var31 + 0.0f),
                            (double)((var28 + (float)var32 + 0.5f) * var19 + var17),
                            (double)((var29 + 0.0f) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var32 + 0.0f), (double)(var10 + 0.0f),
                            (double)(var31 + 0.0f),
                            (double)((var28 + (float)var32 + 0.5f) * var19 + var17),
                            (double)((var29 + 0.0f) * var19 + var18));
                    }
                }
                if (var26 <= 1) {
                    Tessellator_setNormal(var3, 1.0f, 0.0f, 0.0f);
                    for (var32 = 0; var32 < var22; ++var32) {
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var32 + 1.0f - var24),
                            (double)(var10 + 0.0f), (double)(var31 + (float)var22),
                            (double)((var28 + (float)var32 + 0.5f) * var19 + var17),
                            (double)((var29 + (float)var22) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var32 + 1.0f - var24),
                            (double)(var10 + var5), (double)(var31 + (float)var22),
                            (double)((var28 + (float)var32 + 0.5f) * var19 + var17),
                            (double)((var29 + (float)var22) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var32 + 1.0f - var24),
                            (double)(var10 + var5), (double)(var31 + 0.0f),
                            (double)((var28 + (float)var32 + 0.5f) * var19 + var17),
                            (double)((var29 + 0.0f) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var32 + 1.0f - var24),
                            (double)(var10 + 0.0f), (double)(var31 + 0.0f),
                            (double)((var28 + (float)var32 + 0.5f) * var19 + var17),
                            (double)((var29 + 0.0f) * var19 + var18));
                    }
                }
                Tessellator_setColorRGBA_F(var3, var14 * 0.8f, var15 * 0.8f, var16 * 0.8f, 0.8f);
                if (var27 > -1) {
                    Tessellator_setNormal(var3, 0.0f, 0.0f, -1.0f);
                    for (var32 = 0; var32 < var22; ++var32) {
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + 0.0f), (double)(var10 + var5),
                            (double)(var31 + (float)var32 + 0.0f),
                            (double)((var28 + 0.0f) * var19 + var17),
                            (double)((var29 + (float)var32 + 0.5f) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var22), (double)(var10 + var5),
                            (double)(var31 + (float)var32 + 0.0f),
                            (double)((var28 + (float)var22) * var19 + var17),
                            (double)((var29 + (float)var32 + 0.5f) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var22), (double)(var10 + 0.0f),
                            (double)(var31 + (float)var32 + 0.0f),
                            (double)((var28 + (float)var22) * var19 + var17),
                            (double)((var29 + (float)var32 + 0.5f) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + 0.0f), (double)(var10 + 0.0f),
                            (double)(var31 + (float)var32 + 0.0f),
                            (double)((var28 + 0.0f) * var19 + var17),
                            (double)((var29 + (float)var32 + 0.5f) * var19 + var18));
                    }
                }
                if (var27 <= 1) {
                    Tessellator_setNormal(var3, 0.0f, 0.0f, 1.0f);
                    for (var32 = 0; var32 < var22; ++var32) {
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + 0.0f), (double)(var10 + var5),
                            (double)(var31 + (float)var32 + 1.0f - var24),
                            (double)((var28 + 0.0f) * var19 + var17),
                            (double)((var29 + (float)var32 + 0.5f) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var22), (double)(var10 + var5),
                            (double)(var31 + (float)var32 + 1.0f - var24),
                            (double)((var28 + (float)var22) * var19 + var17),
                            (double)((var29 + (float)var32 + 0.5f) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + (float)var22), (double)(var10 + 0.0f),
                            (double)(var31 + (float)var32 + 1.0f - var24),
                            (double)((var28 + (float)var22) * var19 + var17),
                            (double)((var29 + (float)var32 + 0.5f) * var19 + var18));
                        Tessellator_addVertexWithUV(
                            var3, (double)(var30 + 0.0f), (double)(var10 + 0.0f),
                            (double)(var31 + (float)var32 + 1.0f - var24),
                            (double)((var28 + 0.0f) * var19 + var17),
                            (double)((var29 + (float)var32 + 0.5f) * var19 + var18));
                    }
                }
                Tessellator_draw(var3);
            }
        }
    }

    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    R_disable(R_BLEND);
    R_enable(R_CULL_FACE);
}

void RenderGlobal_renderClouds(RenderGlobal *self, float var1) {
    if (self->worldObj->worldProvider->isNether)
        return;

    if (self->mc->gameSettings->fancyGraphics) {
        RenderGlobal_renderCloudsFancy(self, var1);
    } else {
        R_disable(R_CULL_FACE);
        Entity *ve = (Entity *)self->mc->renderViewEntity;
        float var2 = (float)(ve->lastTickPosY + (ve->posY - ve->lastTickPosY) * (double)var1);
        int var3 = 32;
        int var4 = 256 / var3;
        Tessellator *var5 = &Tessellator_instance;
        R_bindTexture(RenderEngine_getTexture(self->renderEngine, "/environment/clouds.png"));
        R_enable(R_BLEND);
        R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
        Vec3D *var6 = World_getCloudColor(self->worldObj, var1);
        float var7 = (float)var6->xCoord;
        float var8 = (float)var6->yCoord;
        float var9 = (float)var6->zCoord;
        float var10;
        if (self->mc->gameSettings->anaglyph) {
            var10 = (var7 * 30.0f + var8 * 59.0f + var9 * 11.0f) / 100.0f;
            float var11 = (var7 * 30.0f + var8 * 70.0f) / 100.0f;
            float var12 = (var7 * 30.0f + var9 * 70.0f) / 100.0f;
            var7 = var10;
            var8 = var11;
            var9 = var12;
        }
        var10 = 0.5f / 1024.0f;
        double var22 = ve->prevPosX + (ve->posX - ve->prevPosX) * (double)var1 +
                       (double)(((float)self->cloudOffsetX + var1) * 0.03f);
        double var13 = ve->prevPosZ + (ve->posZ - ve->prevPosZ) * (double)var1;
        int var15 = MathHelper_floor_double(var22 / 2048.0);
        int var16 = MathHelper_floor_double(var13 / 2048.0);
        var22 -= (double)(var15 * 2048);
        var13 -= (double)(var16 * 2048);
        float var17 = WorldProvider_getCloudHeight(self->worldObj->worldProvider) - var2 + 0.33f;
        float var18 = (float)(var22 * (double)var10);
        float var19 = (float)(var13 * (double)var10);
        Tessellator_startDrawingQuads(var5);
        Tessellator_setColorRGBA_F(var5, var7, var8, var9, 0.8f);
        for (int var20 = -var3 * var4; var20 < var3 * var4; var20 += var3) {
            for (int var21 = -var3 * var4; var21 < var3 * var4; var21 += var3) {
                Tessellator_addVertexWithUV(var5, (double)(var20 + 0), (double)var17,
                                            (double)(var21 + var3),
                                            (double)((float)(var20 + 0) * var10 + var18),
                                            (double)((float)(var21 + var3) * var10 + var19));
                Tessellator_addVertexWithUV(var5, (double)(var20 + var3), (double)var17,
                                            (double)(var21 + var3),
                                            (double)((float)(var20 + var3) * var10 + var18),
                                            (double)((float)(var21 + var3) * var10 + var19));
                Tessellator_addVertexWithUV(var5, (double)(var20 + var3), (double)var17,
                                            (double)(var21 + 0),
                                            (double)((float)(var20 + var3) * var10 + var18),
                                            (double)((float)(var21 + 0) * var10 + var19));
                Tessellator_addVertexWithUV(var5, (double)(var20 + 0), (double)var17,
                                            (double)(var21 + 0),
                                            (double)((float)(var20 + 0) * var10 + var18),
                                            (double)((float)(var21 + 0) * var10 + var19));
            }
        }
        Tessellator_draw(var5);
        R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
        R_disable(R_BLEND);
        R_enable(R_CULL_FACE);
    }
}

void RenderGlobal_purgeStaleTileEntities(RenderGlobal *self) {
    int i = 0;
    while (i < self->tileEntities.size) {
        if (TileEntity_is_queued_for_free(self->tileEntities.data[i])) {
            self->tileEntities.data[i] = self->tileEntities.data[--self->tileEntities.size];
        } else {
            i++;
        }
    }
}

void RenderGlobal_renderEntities(RenderGlobal *self, Vec3D *pos, ICamera *camera,
                                 float partialTick) {
    if (self->renderEntitiesStartupCounter > 0) {
        --self->renderEntitiesStartupCounter;
        return;
    }

    TileEntityRenderer_cacheActiveRenderInfo(TileEntityRenderer_instance, self->worldObj,
                                             self->renderEngine, self->mc->fontRenderer,
                                             self->mc->renderViewEntity, partialTick);

    RenderManager_cacheActiveRenderInfo(RenderManager_instance, self->worldObj, self->renderEngine,
                                        self->mc->fontRenderer, self->mc->renderViewEntity,
                                        self->mc->gameSettings, partialTick);

    self->countEntitiesTotal = 0;
    self->countEntitiesRendered = 0;
    self->countEntitiesHidden = 0;

    EntityLiving *var4 = self->mc->renderViewEntity;
    RenderManager_renderPosX =
        var4->base.lastTickPosX + (var4->base.posX - var4->base.lastTickPosX) * (double)partialTick;
    RenderManager_renderPosY =
        var4->base.lastTickPosY + (var4->base.posY - var4->base.lastTickPosY) * (double)partialTick;
    RenderManager_renderPosZ =
        var4->base.lastTickPosZ + (var4->base.posZ - var4->base.lastTickPosZ) * (double)partialTick;
    TileEntityRenderer_staticPlayerX =
        var4->base.lastTickPosX + (var4->base.posX - var4->base.lastTickPosX) * (double)partialTick;
    TileEntityRenderer_staticPlayerY =
        var4->base.lastTickPosY + (var4->base.posY - var4->base.lastTickPosY) * (double)partialTick;
    TileEntityRenderer_staticPlayerZ =
        var4->base.lastTickPosZ + (var4->base.posZ - var4->base.lastTickPosZ) * (double)partialTick;

    int var5count = self->worldObj->numEntities;
    Entity **var5 = self->worldObj->loadedEntityList;
    self->countEntitiesTotal = var5count;

    for (int var6 = 0; var6 < self->worldObj->numWeatherEffects; ++var6) {
        Entity *var7 = self->worldObj->weatherEffects[var6];
        ++self->countEntitiesRendered;
        if (var7->vtable->isInRangeToRenderVec3D(var7, pos)) {
            RenderManager_renderEntity(RenderManager_instance, var7, partialTick);
        }
    }

    for (int var6 = 0; var6 < var5count; ++var6) {
        Entity *var7 = var5[var6];
        if (var7->vtable->isInRangeToRenderVec3D(var7, pos) &&
            (var7->ignoreFrustumCheck ||
             camera->isBoundingBoxInFrustum(camera, &var7->boundingBox)) &&
            ((Entity *)var7 != (Entity *)self->mc->renderViewEntity ||
             self->mc->gameSettings->thirdPersonView ||
             Entity_isPlayerSleeping((Entity *)self->mc->renderViewEntity))) {
            int var8 = MathHelper_floor_double(var7->posY);
            if (var8 < 0)
                var8 = 0;
            if (var8 >= 128)
                var8 = 127;
            if (World_blockExists(self->worldObj, MathHelper_floor_double(var7->posX), var8,
                                  MathHelper_floor_double(var7->posZ))) {
                ++self->countEntitiesRendered;
                RenderManager_renderEntity(RenderManager_instance, var7, partialTick);
            }
        }
    }

    for (int var6 = 0; var6 < self->tileEntities.size; ++var6) {
        TileEntityRenderer_renderTileEntity(TileEntityRenderer_instance,
                                            self->tileEntities.data[var6], partialTick);
    }
}

int RenderGlobal_func_27307_a(RenderGlobal *self, double x, double y, double z, float partialTick) {
    (void)self;
    (void)x;
    (void)y;
    (void)z;
    (void)partialTick;
    return 0;
}

static void drawOutlinedBoundingBox(AxisAlignedBB *bb) {
    Tessellator *tess = &Tessellator_instance;

    Tessellator_startDrawing(tess, R_LINE_LOOP);
    Tessellator_addVertex(tess, bb->minX, bb->minY, bb->minZ);
    Tessellator_addVertex(tess, bb->maxX, bb->minY, bb->minZ);
    Tessellator_addVertex(tess, bb->maxX, bb->minY, bb->maxZ);
    Tessellator_addVertex(tess, bb->minX, bb->minY, bb->maxZ);
    Tessellator_draw(tess);

    Tessellator_startDrawing(tess, R_LINE_LOOP);
    Tessellator_addVertex(tess, bb->minX, bb->maxY, bb->minZ);
    Tessellator_addVertex(tess, bb->maxX, bb->maxY, bb->minZ);
    Tessellator_addVertex(tess, bb->maxX, bb->maxY, bb->maxZ);
    Tessellator_addVertex(tess, bb->minX, bb->maxY, bb->maxZ);
    Tessellator_draw(tess);

    Tessellator_startDrawing(tess, R_LINES);
    Tessellator_addVertex(tess, bb->minX, bb->minY, bb->minZ);
    Tessellator_addVertex(tess, bb->minX, bb->maxY, bb->minZ);
    Tessellator_addVertex(tess, bb->maxX, bb->minY, bb->minZ);
    Tessellator_addVertex(tess, bb->maxX, bb->maxY, bb->minZ);
    Tessellator_addVertex(tess, bb->maxX, bb->minY, bb->maxZ);
    Tessellator_addVertex(tess, bb->maxX, bb->maxY, bb->maxZ);
    Tessellator_addVertex(tess, bb->minX, bb->minY, bb->maxZ);
    Tessellator_addVertex(tess, bb->minX, bb->maxY, bb->maxZ);
    Tessellator_draw(tess);
}

void RenderGlobal_drawSelectionBox(RenderGlobal *self, EntityPlayer *player,
                                   MovingObjectPosition *mop, int mode, ItemStack *stack,
                                   float partialTick) {
    (void)stack;
    if (mode != 0 || mop->typeOfHit != EnumMovingObjectType_TILE)
        return;

    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    R_color4f(0.0f, 0.0f, 0.0f, 0.4f);
    R_lineWidth(2.0f);
    R_disable(R_TEXTURE_2D);
    R_depthMask(R_FALSE);

    int id = World_getBlockId(self->worldObj, mop->blockX, mop->blockY, mop->blockZ);
    if (id > 0 && Block_blocksList[id]) {
        Block *b = Block_blocksList[id];
        b->vtable->setBlockBoundsBasedOnState(b, (IBlockAccess *)self->worldObj, mop->blockX,
                                              mop->blockY, mop->blockZ);
        Entity *pe = (Entity *)player;
        double px = pe->lastTickPosX + (pe->posX - pe->lastTickPosX) * (double)partialTick;
        double py = pe->lastTickPosY + (pe->posY - pe->lastTickPosY) * (double)partialTick;
        double pz = pe->lastTickPosZ + (pe->posZ - pe->lastTickPosZ) * (double)partialTick;
        float eps = 0.002f;
        AxisAlignedBB *sel = b->vtable->getSelectedBoundingBoxFromPool(
            b, self->worldObj, mop->blockX, mop->blockY, mop->blockZ);
        AxisAlignedBB *exp = AxisAlignedBB_expand(sel, (double)eps, (double)eps, (double)eps);
        AxisAlignedBB *off = AxisAlignedBB_getOffsetBoundingBox(exp, -px, -py, -pz);
        drawOutlinedBoundingBox(off);
    }

    R_depthMask(R_TRUE);
    R_enable(R_TEXTURE_2D);
    R_disable(R_BLEND);
}

void RenderGlobal_drawBlockBreaking(RenderGlobal *self, EntityPlayer *player,
                                    MovingObjectPosition *mop, int mode, ItemStack *stack,
                                    float partialTick) {
    Tessellator *var6 = &Tessellator_instance;
    R_enable(R_BLEND);
    R_enable(R_ALPHA_TEST);
    R_blendFunc(R_SRC_ALPHA, R_ONE);
    struct timespec _ts;
    clock_gettime(CLOCK_MONOTONIC, &_ts);
    float _timeMs = (float)(_ts.tv_sec * 1000LL + _ts.tv_nsec / 1000000LL);
    R_color4f(1.0f, 1.0f, 1.0f, (MathHelper_sin(_timeMs / 100.0f) * 0.2f + 0.4f) * 0.5f);
    int var8;
    if (mode == 0) {
        if (self->damagePartialTime > 0.0f) {
            R_blendFunc(R_DST_COLOR, R_SRC_COLOR);
            int var7 = RenderEngine_getTexture(self->renderEngine, "/terrain.png");
            R_bindTexture(var7);
            R_color4f(1.0f, 1.0f, 1.0f, 0.5f);
            R_pushMatrix();
            var8 = World_getBlockId(self->worldObj, mop->blockX, mop->blockY, mop->blockZ);
            Block *var9 = (var8 > 0) ? Block_blocksList[var8] : NULL;
            R_disable(R_ALPHA_TEST);
            R_polygonOffset(-3.0f, -3.0f);
            R_enable(R_POLYGON_OFFSET_FILL);
            double var10 =
                ((Entity *)player)->lastTickPosX +
                (((Entity *)player)->posX - ((Entity *)player)->lastTickPosX) * (double)partialTick;
            double var12 =
                ((Entity *)player)->lastTickPosY +
                (((Entity *)player)->posY - ((Entity *)player)->lastTickPosY) * (double)partialTick;
            double var14 =
                ((Entity *)player)->lastTickPosZ +
                (((Entity *)player)->posZ - ((Entity *)player)->lastTickPosZ) * (double)partialTick;
            if (var9 == NULL) {
                var9 = Block_stone;
            }
            R_enable(R_ALPHA_TEST);
            Tessellator_startDrawingQuads(var6);
            Tessellator_setTranslationD(var6, -var10, -var12, -var14);
            Tessellator_disableColor(var6);
            RenderBlocks_renderBlockUsingTexture(&self->globalRenderBlocks, var9, mop->blockX,
                                                 mop->blockY, mop->blockZ,
                                                 240 + (int)(self->damagePartialTime * 10.0f));
            Tessellator_draw(var6);
            Tessellator_setTranslationD(var6, 0.0, 0.0, 0.0);
            R_disable(R_ALPHA_TEST);
            R_polygonOffset(0.0f, 0.0f);
            R_disable(R_POLYGON_OFFSET_FILL);
            R_enable(R_ALPHA_TEST);
            R_depthMask(R_TRUE);
            R_popMatrix();
        }
    } else if (stack != NULL) {
        R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
        float var16 = MathHelper_sin(_timeMs / 100.0f) * 0.2f + 0.8f;
        R_color4f(var16, var16, var16, MathHelper_sin(_timeMs / 200.0f) * 0.2f + 0.5f);
        var8 = RenderEngine_getTexture(self->renderEngine, "/terrain.png");
        R_bindTexture(var8);
        int var17 = mop->blockX;
        int var18 = mop->blockY;
        int var11 = mop->blockZ;
        if (mop->sideHit == 0) {
            --var18;
        }
        if (mop->sideHit == 1) {
            ++var18;
        }
        if (mop->sideHit == 2) {
            --var11;
        }
        if (mop->sideHit == 3) {
            ++var11;
        }
        if (mop->sideHit == 4) {
            --var17;
        }
        if (mop->sideHit == 5) {
            ++var17;
        }
        (void)var17;
        (void)var18;
        (void)var11;
    }

    R_disable(R_BLEND);
    R_disable(R_ALPHA_TEST);
}

static void markRange(RenderGlobal *self, int x1, int y1, int z1, int x2, int y2, int z2) {
    if (self->worldRenderers == NULL)
        return;
    int cx1 = MathHelper_bucketInt(x1, 16);
    int cy1 = MathHelper_bucketInt(y1, 16);
    int cz1 = MathHelper_bucketInt(z1, 16);
    int cx2 = MathHelper_bucketInt(x2, 16);
    int cy2 = MathHelper_bucketInt(y2, 16);
    int cz2 = MathHelper_bucketInt(z2, 16);
    for (int xi = cx1; xi <= cx2; xi++) {
        int xm = xi % self->renderChunksWide;
        if (xm < 0)
            xm += self->renderChunksWide;
        for (int yi = cy1; yi <= cy2; yi++) {
            int ym = yi % self->renderChunksTall;
            if (ym < 0)
                ym += self->renderChunksTall;
            for (int zi = cz1; zi <= cz2; zi++) {
                int zm = zi % self->renderChunksDeep;
                if (zm < 0)
                    zm += self->renderChunksDeep;
                int idx = (zm * self->renderChunksTall + ym) * self->renderChunksWide + xm;
                WorldRenderer *wr = self->worldRenderers[idx];
                if (!wr->needsUpdate) {
                    WRList_add(&self->worldRenderersToUpdate, wr);
                    WorldRenderer_markDirty(wr);
                }
            }
        }
    }
}

void RenderGlobal_markBlockAndNeighborsNeedsUpdate(RenderGlobal *self, int x, int y, int z) {
    markRange(self, x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

void RenderGlobal_markBlockRangeNeedsUpdate(RenderGlobal *self, int x1, int y1, int z1, int x2,
                                            int y2, int z2) {
    markRange(self, x1 - 1, y1 - 1, z1 - 1, x2 + 1, y2 + 1, z2 + 1);
}

void RenderGlobal_updateClouds(RenderGlobal *self) { ++self->cloudOffsetX; }

const char *RenderGlobal_getDebugInfoRenders(RenderGlobal *self, char *buf, int bufLen) {
    snprintf(buf, (size_t)bufLen, "C: %d/%d. F: %d, O: %d, E: %d", self->renderersBeingRendered,
             self->renderersLoaded, self->renderersBeingClipped, self->renderersBeingOccluded,
             self->renderersSkippingRenderPass);
    return buf;
}

const char *RenderGlobal_getDebugInfoEntities(RenderGlobal *self, char *buf, int bufLen) {
    snprintf(buf, (size_t)bufLen, "E: %d/%d. B: %d, I: %d", self->countEntitiesRendered,
             self->countEntitiesTotal, self->countEntitiesHidden,
             self->countEntitiesTotal - self->countEntitiesHidden - self->countEntitiesRendered);
    return buf;
}

static RenderGlobal *iwa_to_rg(IWorldAccess *iwa) { return (RenderGlobal *)iwa; }

static void iwa_markBlockAndNeighborsNeedsUpdate(IWorldAccess *iwa, int x, int y, int z) {
    RenderGlobal_markBlockAndNeighborsNeedsUpdate(iwa_to_rg(iwa), x, y, z);
}

static void iwa_markBlockRangeNeedsUpdate(IWorldAccess *iwa, int x1, int y1, int z1, int x2, int y2,
                                          int z2) {
    RenderGlobal_markBlockRangeNeedsUpdate(iwa_to_rg(iwa), x1, y1, z1, x2, y2, z2);
}

static void iwa_playSound(IWorldAccess *iwa, const char *var1, double var2, double var4,
                          double var6, float var8, float var9) {
    RenderGlobal *self = iwa_to_rg(iwa);
    if (self->mc == NULL || self->mc->renderViewEntity == NULL)
        return;
    float var10 = 16.0f;
    if (var8 > 1.0f) {
        var10 *= var8;
    }
    Entity *rve = (Entity *)self->mc->renderViewEntity;
    if (rve->vtable->getDistanceSq(rve, var2, var4, var6) < (double)(var10 * var10)) {
        SoundManager_playSound(self->mc->sndManager, var1, (float)var2, (float)var4, (float)var6,
                               var8, var9);
    }
}

void RenderGlobal_spawnParticle(IWorldAccess *iwa, const char *var1, double var2, double var4,
                                double var6, double var8, double var10, double var12) {
    RenderGlobal *self = iwa_to_rg(iwa);
    if (self->mc == NULL || self->mc->renderViewEntity == NULL || self->mc->effectRenderer == NULL)
        return;
    double var14 = ((Entity *)self->mc->renderViewEntity)->posX - var2;
    double var16 = ((Entity *)self->mc->renderViewEntity)->posY - var4;
    double var18 = ((Entity *)self->mc->renderViewEntity)->posZ - var6;
    double var20 = 16.0;
    if (var14 * var14 + var16 * var16 + var18 * var18 > var20 * var20)
        return;

    EffectRenderer *er = self->mc->effectRenderer;
    World *w = self->worldObj;
    if (strcmp(var1, "bubble") == 0) {
        EntityBubbleFX *fx = (EntityBubbleFX *)calloc(1, sizeof(EntityBubbleFX));
        EntityBubbleFX_construct(fx, w, var2, var4, var6, var8, var10, var12);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "smoke") == 0) {
        EntitySmokeFX *fx = (EntitySmokeFX *)calloc(1, sizeof(EntitySmokeFX));
        EntitySmokeFX_construct1(fx, w, var2, var4, var6, var8, var10, var12);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "note") == 0) {
        EntityNoteFX *fx = (EntityNoteFX *)calloc(1, sizeof(EntityNoteFX));
        EntityNoteFX_construct1(fx, w, var2, var4, var6, var8, var10, var12);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "portal") == 0) {
        EntityPortalFX *fx = (EntityPortalFX *)calloc(1, sizeof(EntityPortalFX));
        EntityPortalFX_construct(fx, w, var2, var4, var6, var8, var10, var12);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "explode") == 0) {
        EntityExplodeFX *fx = (EntityExplodeFX *)calloc(1, sizeof(EntityExplodeFX));
        EntityExplodeFX_construct(fx, w, var2, var4, var6, var8, var10, var12);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "flame") == 0) {
        EntityFlameFX *fx = EntityFlameFX_create(w, var2, var4, var6, var8, var10, var12);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "lava") == 0) {
        EntityLavaFX *fx = EntityLavaFX_create(w, var2, var4, var6);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "footstep") == 0) {
        EntityFootStepFX *fx = EntityFootStepFX_create(self->renderEngine, w, var2, var4, var6);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "splash") == 0) {
        EntitySplashFX *fx = EntitySplashFX_create(w, var2, var4, var6, var8, var10, var12);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "largesmoke") == 0) {
        EntitySmokeFX *fx = (EntitySmokeFX *)calloc(1, sizeof(EntitySmokeFX));
        EntitySmokeFX_construct2(fx, w, var2, var4, var6, var8, var10, var12, 2.5f);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "reddust") == 0) {
        EntityReddustFX *fx =
            EntityReddustFX_create1(w, var2, var4, var6, (float)var8, (float)var10, (float)var12);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "snowballpoof") == 0) {
        EntitySlimeFX *fx = EntitySlimeFX_create(w, var2, var4, var6, Item_snowball);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "snowshovel") == 0) {
        EntitySnowShovelFX *fx =
            EntitySnowShovelFX_create1(w, var2, var4, var6, var8, var10, var12);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "slime") == 0) {
        EntitySlimeFX *fx = EntitySlimeFX_create(w, var2, var4, var6, Item_slimeBall);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    } else if (strcmp(var1, "heart") == 0) {
        EntityHeartFX *fx = EntityHeartFX_create(w, var2, var4, var6, var8, var10, var12);
        EffectRenderer_addEffect(er, (EntityFX *)fx);
    }
}

static void iwa_obtainEntitySkin(IWorldAccess *iwa, Entity *var1) {
    RenderGlobal *self = iwa_to_rg(iwa);
    if (var1->isPlayer) {
        EntityPlayer_updateCloak((EntityPlayer *)var1);
    }
    if (var1->skinUrl[0] != '\0') {
        ImageBufferDownload *ibd = (ImageBufferDownload *)calloc(1, sizeof(ImageBufferDownload));
        ImageBufferDownload_init(ibd);
        RenderEngine_obtainImageData(self->renderEngine, var1->skinUrl, (ImageBuffer *)ibd);
    }
    if (var1->cloakUrl[0] != '\0') {
        ImageBufferDownload *ibd = (ImageBufferDownload *)calloc(1, sizeof(ImageBufferDownload));
        ImageBufferDownload_init(ibd);
        RenderEngine_obtainImageData(self->renderEngine, var1->cloakUrl, (ImageBuffer *)ibd);
    }
}

static void iwa_releaseEntitySkin(IWorldAccess *iwa, Entity *var1) {
    RenderGlobal *self = iwa_to_rg(iwa);
    if (var1->skinUrl[0] != '\0') {
        RenderEngine_releaseImageData(self->renderEngine, var1->skinUrl);
    }
    if (var1->cloakUrl[0] != '\0') {
        RenderEngine_releaseImageData(self->renderEngine, var1->cloakUrl);
    }
}

static void iwa_updateAllRenderers(IWorldAccess *iwa) {
    RenderGlobal *self = iwa_to_rg(iwa);
    for (int var1 = 0; var1 < self->numRenderers; ++var1) {
        WorldRenderer *wr = self->worldRenderers[var1];
        if (wr->isChunkLit && !wr->needsUpdate) {
            WRList_add(&self->worldRenderersToUpdate, wr);
            WorldRenderer_markDirty(wr);
        }
    }
}

static void iwa_playRecord(IWorldAccess *iwa, const char *var1, int var2, int var3, int var4) {
    RenderGlobal *self = iwa_to_rg(iwa);
    if (var1 != NULL && self->mc->ingameGUI != NULL) {
        char msg[128];
        snprintf(msg, sizeof(msg), "C418 - %s", var1);
        GuiIngame_setRecordPlayingMessage(self->mc->ingameGUI, msg);
    }
    SoundManager_playStreaming(self->mc->sndManager, var1, (float)var2, (float)var3, (float)var4,
                               1.0f, 1.0f);
}

static void iwa_doNothingWithTileEntity(IWorldAccess *iwa, int var1, int var2, int var3,
                                        TileEntity *var4) {
    (void)iwa;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
}

static void iwa_func_28136_a(IWorldAccess *iwa, EntityPlayer *var1, int var2, int var3, int var4,
                             int var5, int var6) {
    (void)var1;
    RenderGlobal *self = iwa_to_rg(iwa);
    JavaRandom *var7 = &self->worldObj->rand;
    int var16;
    switch (var2) {
    case 1000:
        World_playSoundEffect(self->worldObj, (double)var3, (double)var4, (double)var5,
                              "random.click", 1.0f, 1.0f);
        break;
    case 1001:
        World_playSoundEffect(self->worldObj, (double)var3, (double)var4, (double)var5,
                              "random.click", 1.0f, 1.2f);
        break;
    case 1002:
        World_playSoundEffect(self->worldObj, (double)var3, (double)var4, (double)var5,
                              "random.bow", 1.0f, 1.2f);
        break;
    case 1003:
        if (Math_random() < 0.5) {
            World_playSoundEffect(self->worldObj, (double)var3 + 0.5, (double)var4 + 0.5,
                                  (double)var5 + 0.5, "random.door_open", 1.0f,
                                  JavaRandom_nextFloat(var7) * 0.1f + 0.9f);
        } else {
            World_playSoundEffect(self->worldObj, (double)var3 + 0.5, (double)var4 + 0.5,
                                  (double)var5 + 0.5, "random.door_close", 1.0f,
                                  JavaRandom_nextFloat(var7) * 0.1f + 0.9f);
        }
        break;
    case 1004:
        World_playSoundEffect(
            self->worldObj, (double)((float)var3 + 0.5f), (double)((float)var4 + 0.5f),
            (double)((float)var5 + 0.5f), "random.fizz", 0.5f,
            2.6f + (JavaRandom_nextFloat(var7) - JavaRandom_nextFloat(var7)) * 0.8f);
        break;
    case 1005:
        if (var6 > 0 && var6 < ITEM_LIST_SIZE && Item_itemsList[var6] != NULL &&
            Item_itemsList[var6]->isRecord) {
            World_playRecord(self->worldObj, Item_itemsList[var6]->recordName, var3, var4, var5);
        } else {
            World_playRecord(self->worldObj, NULL, var3, var4, var5);
        }
        break;
    case 2000: {
        int var8 = var6 % 3 - 1;
        int var9 = var6 / 3 % 3 - 1;
        double var10 = (double)var3 + (double)var8 * 0.6 + 0.5;
        double var12 = (double)var4 + 0.5;
        double var14 = (double)var5 + (double)var9 * 0.6 + 0.5;
        for (var16 = 0; var16 < 10; ++var16) {
            double var31 = JavaRandom_nextDouble(var7) * 0.2 + 0.01;
            double var19 = var10 + (double)var8 * 0.01 +
                           (JavaRandom_nextDouble(var7) - 0.5) * (double)var9 * 0.5;
            double var21 = var12 + (JavaRandom_nextDouble(var7) - 0.5) * 0.5;
            double var23 = var14 + (double)var9 * 0.01 +
                           (JavaRandom_nextDouble(var7) - 0.5) * (double)var8 * 0.5;
            double var25 = (double)var8 * var31 + JavaRandom_nextGaussian(var7) * 0.01;
            double var27 = -0.03 + JavaRandom_nextGaussian(var7) * 0.01;
            double var29 = (double)var9 * var31 + JavaRandom_nextGaussian(var7) * 0.01;
            RenderGlobal_spawnParticle(&self->iwa, "smoke", var19, var21, var23, var25, var27,
                                       var29);
        }
        return;
    }
    case 2001:
        var16 = var6 & 255;
        if (var16 > 0 && Block_blocksList[var16]) {
            Block *var17 = Block_blocksList[var16];
            SoundManager_playSound(self->mc->sndManager, StepSound_stepSoundDir(var17->stepSound),
                                   (float)var3 + 0.5f, (float)var4 + 0.5f, (float)var5 + 0.5f,
                                   (StepSound_getVolume(var17->stepSound) + 1.0f) / 2.0f,
                                   StepSound_getPitch(var17->stepSound) * 0.8f);
        }
        EffectRenderer_addBlockDestroyEffects(self->mc->effectRenderer, var3, var4, var5,
                                              var6 & 255, (var6 >> 8) & 255);
        break;
    }
}

static IWorldAccessVtable RenderGlobal_iwaVtable = {
    .markBlockAndNeighborsNeedsUpdate = iwa_markBlockAndNeighborsNeedsUpdate,
    .markBlockRangeNeedsUpdate = iwa_markBlockRangeNeedsUpdate,
    .playSound = iwa_playSound,
    .spawnParticle = RenderGlobal_spawnParticle,
    .obtainEntitySkin = iwa_obtainEntitySkin,
    .releaseEntitySkin = iwa_releaseEntitySkin,
    .updateAllRenderers = iwa_updateAllRenderers,
    .playRecord = iwa_playRecord,
    .doNothingWithTileEntity = iwa_doNothingWithTileEntity,
    .func_28136_a = iwa_func_28136_a,
};
