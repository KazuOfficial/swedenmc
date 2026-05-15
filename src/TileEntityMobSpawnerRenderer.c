#include "Renderer.h"

#include "TileEntityMobSpawnerRenderer.h"
#include "EntityList.h"
#include "RenderManager.h"
#include "Entity.h"
#include "World.h"
#include <stdlib.h>
#include <string.h>

static void renderTileEntityMobSpawner(TileEntityMobSpawnerRenderer *self,
                                       TileEntityMobSpawner *var1, double var2, double var4,
                                       double var6, float var8) {
    R_pushMatrix();
    R_translatef((float)var2 + 0.5f, (float)var4, (float)var6 + 0.5f);

    const char *mobID = TileEntityMobSpawner_getMobID(var1);
    Entity *var9 = NULL;

    for (int i = 0; i < self->entityHashMapSize; ++i) {
        if (strcmp(self->entityHashMap[i].mobID, mobID) == 0) {
            var9 = self->entityHashMap[i].entity;
            break;
        }
    }
    if (var9 == NULL) {
        var9 = EntityList_createEntityInWorld(mobID, NULL);
        if (var9 != NULL && self->entityHashMapSize < TE_MOB_SPAWNER_RENDERER_MAP_SIZE) {
            int idx = self->entityHashMapSize++;
            strncpy(self->entityHashMap[idx].mobID, mobID,
                    sizeof(self->entityHashMap[idx].mobID) - 1);
            self->entityHashMap[idx].mobID[sizeof(self->entityHashMap[idx].mobID) - 1] = '\0';
            self->entityHashMap[idx].entity = var9;
        }
    }

    if (var9 != NULL) {

        var9->worldObj = var1->base.worldObj;
        float var10 = 7.0f / 16.0f;
        R_translatef(0.0f, 0.4f, 0.0f);

        R_rotatef((float)(var1->yaw2 + (var1->yaw - var1->yaw2) * (double)var8) * 10.0f, 0.0f, 1.0f,
                  0.0f);
        R_rotatef(-30.0f, 1.0f, 0.0f, 0.0f);
        R_translatef(0.0f, -0.4f, 0.0f);
        R_scalef(var10, var10, var10);

        var9->vtable->setLocationAndAngles(var9, var2, var4, var6, 0.0f, 0.0f);

        RenderManager_renderEntityWithPosYaw(RenderManager_instance, var9, 0.0, 0.0, 0.0, 0.0f,
                                             var8);
    }

    R_popMatrix();
}

static void vtable_renderTileEntityAt(TileEntitySpecialRenderer *base, TileEntity *var1,
                                      double var2, double var4, double var6, float var8) {
    TileEntityMobSpawnerRenderer *self = (TileEntityMobSpawnerRenderer *)base;
    renderTileEntityMobSpawner(self, (TileEntityMobSpawner *)var1, var2, var4, var6, var8);
}

static const TileEntitySpecialRendererVtable s_vtable = {
    .renderTileEntityAt = vtable_renderTileEntityAt,
    .func_31069_a = TileEntitySpecialRenderer_func_31069_a_default,
};

TileEntitySpecialRenderer *TileEntityMobSpawnerRenderer_create(void) {
    TileEntityMobSpawnerRenderer *self =
        (TileEntityMobSpawnerRenderer *)calloc(1, sizeof(TileEntityMobSpawnerRenderer));
    self->base.vtable = &s_vtable;
    self->base.tileEntityRenderer = NULL;
    self->entityHashMapSize = 0;
    return &self->base;
}
