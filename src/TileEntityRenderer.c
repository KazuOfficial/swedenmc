#include "Renderer.h"

#include "TileEntityRenderer.h"
#include "TileEntitySignRenderer.h"
#include "TileEntityMobSpawnerRenderer.h"
#include "TileEntityRendererPiston.h"
#include "World.h"
#include <stdlib.h>
#include <string.h>

TileEntityRenderer *TileEntityRenderer_instance = NULL;

double TileEntityRenderer_staticPlayerX = 0.0;
double TileEntityRenderer_staticPlayerY = 0.0;
double TileEntityRenderer_staticPlayerZ = 0.0;

TileEntityRenderer *TileEntityRenderer_create(void) {
    TileEntityRenderer *self = (TileEntityRenderer *)calloc(1, sizeof(TileEntityRenderer));
    TileEntityRenderer_instance = self;

    self->specialRendererMap[TE_TYPE_SIGN] = TileEntitySignRenderer_create();

    self->specialRendererMap[TE_TYPE_MOB_SPAWNER] = TileEntityMobSpawnerRenderer_create();

    self->specialRendererMap[TE_TYPE_PISTON] = TileEntityRendererPiston_create();

    for (int i = 0; i < 4; ++i) {
        if (self->specialRendererMap[i]) {
            TileEntitySpecialRenderer_setTileEntityRenderer(self->specialRendererMap[i], self);
        }
    }
    return self;
}

TileEntitySpecialRenderer *TileEntityRenderer_getSpecialRendererForClass(TileEntityRenderer *self,
                                                                         int typeId) {
    if (typeId < 0 || typeId >= 4)
        return NULL;
    return self->specialRendererMap[typeId];
}

int TileEntityRenderer_hasSpecialRenderer(TileEntityRenderer *self, TileEntity *var1) {
    return TileEntityRenderer_getSpecialRendererForEntity(self, var1) != NULL;
}

TileEntitySpecialRenderer *TileEntityRenderer_getSpecialRendererForEntity(TileEntityRenderer *self,
                                                                          TileEntity *var1) {
    if (var1 == NULL)
        return NULL;
    return TileEntityRenderer_getSpecialRendererForClass(self, var1->tileEntityTypeId);
}

void TileEntityRenderer_cacheActiveRenderInfo(TileEntityRenderer *self, World *var1,
                                              RenderEngine *var2, FontRenderer *var3,
                                              EntityLiving *var4, float var5) {
    if (self->worldObj != var1) {
        TileEntityRenderer_func_31072_a(self, var1);
    }
    self->renderEngine = var2;
    self->entityLivingPlayer = var4;
    self->fontRenderer = var3;
    self->playerYaw =
        var4->base.prevRotationYaw + (var4->base.rotationYaw - var4->base.prevRotationYaw) * var5;
    self->playerPitch = var4->base.prevRotationPitch +
                        (var4->base.rotationPitch - var4->base.prevRotationPitch) * var5;
    self->playerX =
        var4->base.lastTickPosX + (var4->base.posX - var4->base.lastTickPosX) * (double)var5;
    self->playerY =
        var4->base.lastTickPosY + (var4->base.posY - var4->base.lastTickPosY) * (double)var5;
    self->playerZ =
        var4->base.lastTickPosZ + (var4->base.posZ - var4->base.lastTickPosZ) * (double)var5;
}

void TileEntityRenderer_renderTileEntity(TileEntityRenderer *self, TileEntity *var1, float var2) {
    if (TileEntity_getDistanceFrom(var1, self->playerX, self->playerY, self->playerZ) < 4096.0) {
        float var3 = (float)self->worldObj->vtable->getLightBrightness(
            (World *)self->worldObj, var1->xCoord, var1->yCoord, var1->zCoord);
        R_color3f(var3, var3, var3);
        TileEntityRenderer_renderTileEntityAt(
            self, var1, (double)var1->xCoord - TileEntityRenderer_staticPlayerX,
            (double)var1->yCoord - TileEntityRenderer_staticPlayerY,
            (double)var1->zCoord - TileEntityRenderer_staticPlayerZ, var2);
    }
}

void TileEntityRenderer_renderTileEntityAt(TileEntityRenderer *self, TileEntity *var1, double var2,
                                           double var4, double var6, float var8) {
    TileEntitySpecialRenderer *var9 = TileEntityRenderer_getSpecialRendererForEntity(self, var1);
    if (var9 != NULL) {
        var9->vtable->renderTileEntityAt(var9, var1, var2, var4, var6, var8);
    }
}

void TileEntityRenderer_func_31072_a(TileEntityRenderer *self, World *var1) {
    self->worldObj = var1;
    for (int i = 0; i < 4; ++i) {
        TileEntitySpecialRenderer *var3 = self->specialRendererMap[i];
        if (var3 != NULL) {
            var3->vtable->func_31069_a(var3, var1);
        }
    }
}

FontRenderer *TileEntityRenderer_getFontRenderer(TileEntityRenderer *self) {
    return self->fontRenderer;
}
