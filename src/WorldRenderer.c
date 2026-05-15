#include "WorldRenderer.h"
#include "ChunkMesher.h"
#include <stdio.h>
#include "Render.h"
#include "ChunkCache.h"
#include "RenderBlocks.h"
#include "Tessellator.h"
#include "Block.h"
#include "MathHelper.h"
#include "TileEntityRenderer.h"
#include "Render.h"
#include <stdlib.h>
#include <string.h>

int WorldRenderer_chunksUpdated = 0;

void TileEntityList_init(TileEntityList *list) {
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void TileEntityList_free(TileEntityList *list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void TileEntityList_add(TileEntityList *list, TileEntity *te) {
    if (list->size >= list->capacity) {
        int newCap = list->capacity ? list->capacity * 2 : 8;
        list->data = (TileEntity **)realloc(list->data, newCap * sizeof(TileEntity *));
        list->capacity = newCap;
    }
    list->data[list->size++] = te;
}

void TileEntityList_clear(TileEntityList *list) { list->size = 0; }

int TileEntityList_contains(TileEntityList *list, TileEntity *te) {
    for (int i = 0; i < list->size; i++)
        if (list->data[i] == te)
            return 1;
    return 0;
}

void TileEntityList_addAll(TileEntityList *dst, TileEntityList *src) {
    for (int i = 0; i < src->size; i++)
        if (!TileEntityList_contains(dst, src->data[i]))
            TileEntityList_add(dst, src->data[i]);
}

void TileEntityList_removeAll(TileEntityList *dst, TileEntityList *src) {
    int out = 0;
    for (int i = 0; i < dst->size; i++) {
        if (!TileEntityList_contains(src, dst->data[i]))
            dst->data[out++] = dst->data[i];
    }
    dst->size = out;
}

WorldRenderer *WorldRenderer_create(World *world, TileEntityList *tileEntities, int x, int y, int z,
                                    int chunkSize, int glRenderListBase) {
    WorldRenderer *self = (WorldRenderer *)calloc(1, sizeof(WorldRenderer));
    self->worldObj = world;
    self->tileEntities = tileEntities;
    self->sizeWidth = chunkSize;
    self->sizeHeight = chunkSize;
    self->sizeDepth = chunkSize;
    self->rendererRadius =
        MathHelper_sqrt_float(
            (float)(chunkSize * chunkSize + chunkSize * chunkSize + chunkSize * chunkSize)) /
        2.0f;
    (void)glRenderListBase;
    self->vbo[0] = R_bufferCreate();
    self->vbo[1] = R_bufferCreate();
    self->posX = -999;
    self->isVisible = 1;
    TileEntityList_init(&self->tileEntityRenderers);
    WorldRenderer_setPosition(self, x, y, z);
    self->needsUpdate = 0;
    return self;
}

void WorldRenderer_destroy(WorldRenderer *self) {
    ChunkMesher_cancel(self);
    R_bufferDestroy(self->vbo[0]);
    R_bufferDestroy(self->vbo[1]);
    TileEntityList_free(&self->tileEntityRenderers);
    free(self->rendererBoundingBox);
    free(self);
}

void WorldRenderer_setPosition(WorldRenderer *self, int x, int y, int z) {
    if (x == self->posX && y == self->posY && z == self->posZ)
        return;

    WorldRenderer_setDontDraw(self);
    self->posX = x;
    self->posY = y;
    self->posZ = z;
    self->posXPlus = x + self->sizeWidth / 2;
    self->posYPlus = y + self->sizeHeight / 2;
    self->posZPlus = z + self->sizeDepth / 2;
    self->posXClip = x & 1023;
    self->posYClip = y;
    self->posZClip = z & 1023;
    self->posXMinus = x - self->posXClip;
    self->posYMinus = y - self->posYClip;
    self->posZMinus = z - self->posZClip;

    float pad = 6.0f;
    free(self->rendererBoundingBox);
    self->rendererBoundingBox = AxisAlignedBB_getBoundingBox(
        (double)((float)x - pad), (double)((float)y - pad), (double)((float)z - pad),
        (double)((float)(x + self->sizeWidth) + pad), (double)((float)(y + self->sizeHeight) + pad),
        (double)((float)(z + self->sizeDepth) + pad));

    self->occlusionAABB.minX = (double)((float)self->posXClip - pad);
    self->occlusionAABB.minY = (double)((float)self->posYClip - pad);
    self->occlusionAABB.minZ = (double)((float)self->posZClip - pad);
    self->occlusionAABB.maxX = (double)((float)(self->posXClip + self->sizeWidth) + pad);
    self->occlusionAABB.maxY = (double)((float)(self->posYClip + self->sizeHeight) + pad);
    self->occlusionAABB.maxZ = (double)((float)(self->posZClip + self->sizeDepth) + pad);

    WorldRenderer_markDirty(self);
}

void WorldRenderer_updateRenderer(WorldRenderer *self) {
    if (!self->needsUpdate)
        return;

    ++WorldRenderer_chunksUpdated;

    int x1 = self->posX;
    int y1 = self->posY;
    int z1 = self->posZ;
    int x2 = self->posX + self->sizeWidth;
    int y2 = self->posY + self->sizeHeight;
    int z2 = self->posZ + self->sizeDepth;

    for (int p = 0; p < 2; p++)
        self->skipRenderPass[p] = 1;

    Chunk_isLit = 0;

    TileEntityList oldRenderers;
    TileEntityList_init(&oldRenderers);
    TileEntityList_addAll(&oldRenderers, &self->tileEntityRenderers);
    TileEntityList_clear(&self->tileEntityRenderers);

    int pad = 1;
    ChunkCache cc;
    ChunkCache_init(&cc, self->worldObj, x1 - pad, y1 - pad, z1 - pad, x2 + pad, y2 + pad,
                    z2 + pad);

    RenderBlocks rb;
    RenderBlocks_init(&rb, (IBlockAccess *)&cc);

    Tessellator *tess = &Tessellator_instance;

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
                        Tessellator_setTranslationD(tess, (double)(-self->posX),
                                                    (double)(-self->posY), (double)(-self->posZ));
                    }

                    if (pass == 0 && Block_isBlockContainer[id]) {
                        TileEntity *te = ChunkCache_getBlockTileEntity(&cc, x, y, z);
                        if (TileEntityRenderer_hasSpecialRenderer(TileEntityRenderer_instance,
                                                                  te)) {
                            TileEntityList_add(&self->tileEntityRenderers, te);
                        }
                    }

                    Block *block = Block_blocksList[id];
                    if (block == NULL)
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

        if (anyBlock) {
            Tessellator_uploadToBuffer(tess, self->vbo[pass]);
            Tessellator_setTranslationD(tess, 0.0, 0.0, 0.0);
        } else {
            R_bufferUpload(self->vbo[pass], NULL, 0, R_TRIANGLES, 0);
            hasContent = 0;
        }

        if (hasContent)
            self->skipRenderPass[pass] = 0;

        if (!needPass2)
            break;
    }

    TileEntityList newRenderers;
    TileEntityList_init(&newRenderers);
    TileEntityList_addAll(&newRenderers, &self->tileEntityRenderers);
    TileEntityList_removeAll(&newRenderers, &oldRenderers);
    TileEntityList_addAll(self->tileEntities, &newRenderers);

    TileEntityList_removeAll(&oldRenderers, &self->tileEntityRenderers);
    TileEntityList_removeAll(self->tileEntities, &oldRenderers);

    TileEntityList_free(&oldRenderers);
    TileEntityList_free(&newRenderers);

    ChunkCache_freeContents(&cc);

    self->isChunkLit = Chunk_isLit;
    self->isInitialized = 1;
}

float WorldRenderer_distanceToEntitySquared(WorldRenderer *self, Entity *entity) {
    float dx = (float)(entity->posX - (double)self->posXPlus);
    float dy = (float)(entity->posY - (double)self->posYPlus);
    float dz = (float)(entity->posZ - (double)self->posZPlus);
    return dx * dx + dy * dy + dz * dz;
}

void WorldRenderer_setDontDraw(WorldRenderer *self) {
    self->skipRenderPass[0] = 1;
    self->skipRenderPass[1] = 1;
    self->isInFrustum = 0;
    self->isInitialized = 0;
}

void WorldRenderer_func_1204_c(WorldRenderer *self) {
    WorldRenderer_setDontDraw(self);
    self->worldObj = NULL;
}

RBuffer *WorldRenderer_getBufferForPass(WorldRenderer *self, int pass) {
    if (!self->isInFrustum)
        return NULL;
    if (self->skipRenderPass[pass])
        return NULL;
    return self->vbo[pass];
}

void WorldRenderer_updateInFrustrum(WorldRenderer *self, ICamera *camera) {
    self->isInFrustum = camera->isBoundingBoxInFrustum(camera, self->rendererBoundingBox);
}

void WorldRenderer_callOcclusionQueryList(WorldRenderer *self) {
    Render_renderAABB(&self->occlusionAABB);
}

int WorldRenderer_skipAllRenderPasses(WorldRenderer *self) {
    if (!self->isInitialized)
        return 0;
    return self->skipRenderPass[0] && self->skipRenderPass[1];
}

void WorldRenderer_markDirty(WorldRenderer *self) { self->needsUpdate = 1; }
