#ifndef RENDER_GLOBAL_H
#define RENDER_GLOBAL_H

#include "WorldRenderer.h"
#include "RenderList.h"
#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "ICamera.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "MovingObjectPosition.h"
#include "IWorldAccess.h"

struct Minecraft;

typedef struct WorldRendererList {
    WorldRenderer **data;
    int size;
    int capacity;
} WorldRendererList;

typedef struct RenderGlobal {

    IWorldAccess iwa;
    struct Minecraft *mc;
    World *worldObj;
    RenderEngine *renderEngine;
    TileEntityList tileEntities;

    WorldRenderer **worldRenderers;
    WorldRenderer **sortedWorldRenderers;
    int numRenderers;
    int renderChunksWide;
    int renderChunksTall;
    int renderChunksDeep;
    WorldRendererList worldRenderersToUpdate;

    RBuffer *starVBO;
    RBuffer *skyVBO;
    RBuffer *skyVBO2;

    unsigned int *glOcclusionQueryBase;

    int occlusionEnabled;

    unsigned int occlusionResult;

    int dummyBuf50k[50000];

    int dummyInt0;

    int renderDistance;

    int minBlockX, minBlockY, minBlockZ;
    int maxBlockX, maxBlockY, maxBlockZ;

    int renderEntitiesStartupCounter;
    int countEntitiesTotal;
    int countEntitiesRendered;
    int countEntitiesHidden;
    int renderersLoaded;
    int renderersBeingClipped;
    int renderersBeingOccluded;
    int renderersBeingRendered;
    int renderersSkippingRenderPass;
    int worldRenderersCheckIndex;

    double prevSortX, prevSortY, prevSortZ;
    float damagePartialTime;
    int frustrumCheckOffset;
    int cloudOffsetX;

    WorldRendererList glRenderLists;

    RenderList allRenderLists[4];

    RenderBlocks globalRenderBlocks;
} RenderGlobal;

RenderGlobal *RenderGlobal_create(struct Minecraft *mc, RenderEngine *re);
void RenderGlobal_destroy(RenderGlobal *self);

void RenderGlobal_changeWorld(RenderGlobal *self, World *world);
void RenderGlobal_loadRenderers(RenderGlobal *self);

int RenderGlobal_sortAndRender(RenderGlobal *self, EntityLiving *viewer, int pass,
                               double partialTick);
void RenderGlobal_renderAllRenderLists(RenderGlobal *self, int pass, double partialTick);
int RenderGlobal_updateRenderers(RenderGlobal *self, EntityLiving *viewer, int forceFrustum);
void RenderGlobal_clipRenderersByFrustrum(RenderGlobal *self, ICamera *camera, float partialTick);

void RenderGlobal_renderSky(RenderGlobal *self, float partialTick);
void RenderGlobal_renderClouds(RenderGlobal *self, float partialTick);
int RenderGlobal_func_27307_a(RenderGlobal *self, double x, double y, double z, float partialTick);

void RenderGlobal_renderEntities(RenderGlobal *self, Vec3D *pos, ICamera *camera,
                                 float partialTick);

void RenderGlobal_purgeStaleTileEntities(RenderGlobal *self);

void RenderGlobal_drawSelectionBox(RenderGlobal *self, EntityPlayer *player,
                                   MovingObjectPosition *mop, int mode, ItemStack *stack,
                                   float partialTick);
void RenderGlobal_drawBlockBreaking(RenderGlobal *self, EntityPlayer *player,
                                    MovingObjectPosition *mop, int mode, ItemStack *stack,
                                    float partialTick);

void RenderGlobal_markBlockAndNeighborsNeedsUpdate(RenderGlobal *self, int x, int y, int z);
void RenderGlobal_markBlockRangeNeedsUpdate(RenderGlobal *self, int x1, int y1, int z1, int x2,
                                            int y2, int z2);

void RenderGlobal_updateClouds(RenderGlobal *self);

const char *RenderGlobal_getDebugInfoRenders(RenderGlobal *self, char *buf, int bufLen);

const char *RenderGlobal_getDebugInfoEntities(RenderGlobal *self, char *buf, int bufLen);

void RenderGlobal_func_28137_f(RenderGlobal *self);

void RenderGlobal_spawnParticle(IWorldAccess *iwa, const char *name, double x, double y, double z,
                                double vx, double vy, double vz);

#endif
