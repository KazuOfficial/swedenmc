#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include "Renderer.h"
#include "World.h"
#include "TileEntity.h"
#include "AxisAlignedBB.h"
#include "ICamera.h"
#include "Entity.h"

typedef struct {
    TileEntity **data;
    int size;
    int capacity;
} TileEntityList;

typedef struct WorldRenderer {
    World *worldObj;
    RBuffer *vbo[2];
    AxisAlignedBB occlusionAABB;
    int posX, posY, posZ;
    int sizeWidth, sizeHeight, sizeDepth;
    int posXMinus, posYMinus, posZMinus;
    int posXClip, posYClip, posZClip;
    int isInFrustum;
    int skipRenderPass[2];
    int posXPlus, posYPlus, posZPlus;
    float rendererRadius;
    int needsUpdate;
    AxisAlignedBB *rendererBoundingBox;
    int chunkIndex;
    int isVisible;
    int isWaitingOnOcclusionQuery;
    int glOcclusionQuery;
    int isChunkLit;
    TileEntityList tileEntityRenderers;
    TileEntityList *tileEntities;
    int isInitialized;
    int meshJobPending;
    void *meshJob;
} WorldRenderer;

extern int WorldRenderer_chunksUpdated;

WorldRenderer *WorldRenderer_create(World *world, TileEntityList *tileEntities, int x, int y, int z,
                                    int chunkSize, int glRenderListBase);
void WorldRenderer_destroy(WorldRenderer *self);

void WorldRenderer_setPosition(WorldRenderer *self, int x, int y, int z);
void WorldRenderer_updateRenderer(WorldRenderer *self);
void WorldRenderer_setDontDraw(WorldRenderer *self);
void WorldRenderer_func_1204_c(WorldRenderer *self);
void WorldRenderer_markDirty(WorldRenderer *self);
void WorldRenderer_updateInFrustrum(WorldRenderer *self, ICamera *camera);
void WorldRenderer_callOcclusionQueryList(WorldRenderer *self);

RBuffer *WorldRenderer_getBufferForPass(WorldRenderer *self, int pass);
int WorldRenderer_skipAllRenderPasses(WorldRenderer *self);
float WorldRenderer_distanceToEntitySquared(WorldRenderer *self, Entity *entity);

void TileEntityList_init(TileEntityList *list);
void TileEntityList_free(TileEntityList *list);
void TileEntityList_add(TileEntityList *list, TileEntity *te);
void TileEntityList_clear(TileEntityList *list);
int TileEntityList_contains(TileEntityList *list, TileEntity *te);
void TileEntityList_addAll(TileEntityList *dst, TileEntityList *src);
void TileEntityList_removeAll(TileEntityList *dst, TileEntityList *src);

#endif
