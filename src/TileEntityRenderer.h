#ifndef TILE_ENTITY_RENDERER_H
#define TILE_ENTITY_RENDERER_H

#include "TileEntity.h"
#include "TileEntitySpecialRenderer.h"
#include "RenderEngine.h"
#include "FontRenderer.h"
#include "EntityLiving.h"

struct World;

typedef struct TileEntityRenderer {

    TileEntitySpecialRenderer *specialRendererMap[4];

    FontRenderer *fontRenderer;

    RenderEngine *renderEngine;

    World *worldObj;

    EntityLiving *entityLivingPlayer;

    float playerYaw;

    float playerPitch;

    double playerX, playerY, playerZ;
} TileEntityRenderer;

extern TileEntityRenderer *TileEntityRenderer_instance;

extern double TileEntityRenderer_staticPlayerX;
extern double TileEntityRenderer_staticPlayerY;
extern double TileEntityRenderer_staticPlayerZ;

TileEntityRenderer *TileEntityRenderer_create(void);

TileEntitySpecialRenderer *TileEntityRenderer_getSpecialRendererForClass(TileEntityRenderer *self,
                                                                         int typeId);

int TileEntityRenderer_hasSpecialRenderer(TileEntityRenderer *self, TileEntity *var1);

TileEntitySpecialRenderer *TileEntityRenderer_getSpecialRendererForEntity(TileEntityRenderer *self,
                                                                          TileEntity *var1);

void TileEntityRenderer_cacheActiveRenderInfo(TileEntityRenderer *self, World *var1,
                                              RenderEngine *var2, FontRenderer *var3,
                                              EntityLiving *var4, float var5);

void TileEntityRenderer_renderTileEntity(TileEntityRenderer *self, TileEntity *var1, float var2);

void TileEntityRenderer_renderTileEntityAt(TileEntityRenderer *self, TileEntity *var1, double var2,
                                           double var4, double var6, float var8);

void TileEntityRenderer_func_31072_a(TileEntityRenderer *self, World *var1);

FontRenderer *TileEntityRenderer_getFontRenderer(TileEntityRenderer *self);

#endif
