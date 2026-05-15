#include "Renderer.h"

#include "EffectRenderer.h"
#include "EntityFX.h"
#include "DataWatcher.h"
#include "EntityDiggingFX.h"
#include "Block.h"
#include "MathHelper.h"
#include "Tessellator.h"
#include "RenderEngine.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static void EntityFX_free(EntityFX *fx) {
    DataWatcher_destroy(&fx->base.dataWatcher);
    free(fx);
}

static void FXLayer_init(FXLayer *l) {
    l->data = NULL;
    l->size = 0;
    l->cap = 0;
}

static void FXLayer_add(FXLayer *l, EntityFX *fx) {
    if (l->size >= l->cap) {
        int newcap = l->cap == 0 ? 64 : l->cap * 2;
        l->data = (EntityFX **)realloc(l->data, (size_t)newcap * sizeof(EntityFX *));
        l->cap = newcap;
    }
    l->data[l->size++] = fx;
}

static void FXLayer_removeAt(FXLayer *l, int i) {

    memmove(&l->data[i], &l->data[i + 1], (size_t)(l->size - i - 1) * sizeof(EntityFX *));
    l->size--;
}

static void FXLayer_clear(FXLayer *l) {
    for (int i = 0; i < l->size; i++)
        EntityFX_free(l->data[i]);
    l->size = 0;
}

static void FXLayer_free(FXLayer *l) {
    for (int i = 0; i < l->size; i++)
        EntityFX_free(l->data[i]);
    free(l->data);
    FXLayer_init(l);
}

EffectRenderer *EffectRenderer_create(World *world, RenderEngine *renderer) {
    EffectRenderer *self = (EffectRenderer *)calloc(1, sizeof(EffectRenderer));
    if (world != NULL) {
        self->worldObj = world;
    }
    self->renderer = renderer;
    for (int var3 = 0; var3 < 4; ++var3) {
        FXLayer_init(&self->fxLayers[var3]);
    }
    JavaRandom_new(&self->rand);
    return self;
}

void EffectRenderer_destroy(EffectRenderer *self) {
    if (!self)
        return;
    for (int i = 0; i < 4; i++)
        FXLayer_free(&self->fxLayers[i]);
    free(self);
}

void EffectRenderer_addEffect(EffectRenderer *self, EntityFX *var1) {
    EntityFXVtable *vt = (EntityFXVtable *)var1->base.vtable;
    int var2 = vt->getFXLayer(var1);
    if (self->fxLayers[var2].size >= 4000) {
        EntityFX_free(self->fxLayers[var2].data[0]);
        FXLayer_removeAt(&self->fxLayers[var2], 0);
    }
    FXLayer_add(&self->fxLayers[var2], var1);
}

void EffectRenderer_updateEffects(EffectRenderer *self) {
    for (int var1 = 0; var1 < 4; ++var1) {
        for (int var2 = 0; var2 < self->fxLayers[var1].size; ++var2) {
            EntityFX *var3 = self->fxLayers[var1].data[var2];
            var3->base.vtable->onUpdate((Entity *)var3);
            if (var3->base.isDead) {
                EntityFX_free(var3);
                FXLayer_removeAt(&self->fxLayers[var1], var2);
                --var2;
            }
        }
    }
}

void EffectRenderer_renderParticles(EffectRenderer *self, Entity *var1, float var2) {
    float var3 = MathHelper_cos(var1->rotationYaw * (float)M_PI / 180.0f);
    float var4 = MathHelper_sin(var1->rotationYaw * (float)M_PI / 180.0f);
    float var5 = -var4 * MathHelper_sin(var1->rotationPitch * (float)M_PI / 180.0f);
    float var6 = var3 * MathHelper_sin(var1->rotationPitch * (float)M_PI / 180.0f);
    float var7 = MathHelper_cos(var1->rotationPitch * (float)M_PI / 180.0f);
    EntityFX_interpPosX = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var2;
    EntityFX_interpPosY = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var2;
    EntityFX_interpPosZ = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var2;
    for (int var8 = 0; var8 < 3; ++var8) {
        if (self->fxLayers[var8].size != 0) {
            int var9 = 0;
            if (var8 == 0) {
                var9 = (int)RenderEngine_getTexture(self->renderer, "/particles.png");
            }
            if (var8 == 1) {
                var9 = (int)RenderEngine_getTexture(self->renderer, "/terrain.png");
            }
            if (var8 == 2) {
                var9 = (int)RenderEngine_getTexture(self->renderer, "/gui/items.png");
            }
            R_bindTexture((unsigned int)var9);
            Tessellator *var10 = &Tessellator_instance;
            Tessellator_startDrawingQuads(var10);
            for (int var11 = 0; var11 < self->fxLayers[var8].size; ++var11) {
                EntityFX *var12 = self->fxLayers[var8].data[var11];
                EntityFXVtable *vt = (EntityFXVtable *)var12->base.vtable;
                vt->renderParticle(var12, var10, var2, var3, var7, var4, var5, var6);
            }
            Tessellator_draw(var10);
        }
    }
}

void EffectRenderer_func_1187_b(EffectRenderer *self, Entity *var1, float var2) {
    (void)var1;
    int8_t var3 = 3;
    if (self->fxLayers[var3].size != 0) {
        Tessellator *var4 = &Tessellator_instance;
        for (int var5 = 0; var5 < self->fxLayers[var3].size; ++var5) {
            EntityFX *var6 = self->fxLayers[var3].data[var5];
            EntityFXVtable *vt = (EntityFXVtable *)var6->base.vtable;
            vt->renderParticle(var6, var4, var2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        }
    }
}

void EffectRenderer_clearEffects(EffectRenderer *self, World *var1) {
    self->worldObj = var1;
    for (int var2 = 0; var2 < 4; ++var2) {
        FXLayer_clear(&self->fxLayers[var2]);
    }
}

void EffectRenderer_addBlockDestroyEffects(EffectRenderer *self, int var1, int var2, int var3,
                                           int var4, int var5) {
    if (var4 != 0 && Block_blocksList[var4]) {
        Block *var6 = Block_blocksList[var4];
        int8_t var7 = 4;
        for (int var8 = 0; var8 < var7; ++var8) {
            for (int var9 = 0; var9 < var7; ++var9) {
                for (int var10 = 0; var10 < var7; ++var10) {
                    double var11 = (double)var1 + ((double)var8 + 0.5) / (double)var7;
                    double var13 = (double)var2 + ((double)var9 + 0.5) / (double)var7;
                    double var15 = (double)var3 + ((double)var10 + 0.5) / (double)var7;
                    int var17 = JavaRandom_nextInt(&self->rand, 6);
                    EntityDiggingFX *fx = (EntityDiggingFX *)calloc(1, sizeof(EntityDiggingFX));
                    EntityDiggingFX_construct(
                        fx, self->worldObj, var11, var13, var15, var11 - (double)var1 - 0.5,
                        var13 - (double)var2 - 0.5, var15 - (double)var3 - 0.5, var6, var17, var5);
                    EntityDiggingFX_func_4041_a(fx, var1, var2, var3);
                    EffectRenderer_addEffect(self, (EntityFX *)fx);
                }
            }
        }
    }
}

void EffectRenderer_addBlockHitEffects(EffectRenderer *self, int var1, int var2, int var3,
                                       int var4) {
    int var5 = World_getBlockId(self->worldObj, var1, var2, var3);
    if (var5 != 0 && Block_blocksList[var5]) {
        Block *var6 = Block_blocksList[var5];
        float var7 = 0.1f;
        double var8 =
            (double)var1 +
            JavaRandom_nextDouble(&self->rand) * (var6->maxX - var6->minX - (double)(var7 * 2.0f)) +
            (double)var7 + var6->minX;
        double var10 =
            (double)var2 +
            JavaRandom_nextDouble(&self->rand) * (var6->maxY - var6->minY - (double)(var7 * 2.0f)) +
            (double)var7 + var6->minY;
        double var12 =
            (double)var3 +
            JavaRandom_nextDouble(&self->rand) * (var6->maxZ - var6->minZ - (double)(var7 * 2.0f)) +
            (double)var7 + var6->minZ;
        if (var4 == 0) {
            var10 = (double)var2 + var6->minY - (double)var7;
        }
        if (var4 == 1) {
            var10 = (double)var2 + var6->maxY + (double)var7;
        }
        if (var4 == 2) {
            var12 = (double)var3 + var6->minZ - (double)var7;
        }
        if (var4 == 3) {
            var12 = (double)var3 + var6->maxZ + (double)var7;
        }
        if (var4 == 4) {
            var8 = (double)var1 + var6->minX - (double)var7;
        }
        if (var4 == 5) {
            var8 = (double)var1 + var6->maxX + (double)var7;
        }
        EntityDiggingFX *fx = (EntityDiggingFX *)calloc(1, sizeof(EntityDiggingFX));
        EntityDiggingFX_construct(fx, self->worldObj, var8, var10, var12, 0.0, 0.0, 0.0, var6, var4,
                                  World_getBlockMetadata(self->worldObj, var1, var2, var3));
        EntityDiggingFX_func_4041_a(fx, var1, var2, var3);
        EntityFX_func_407_b((EntityFX *)fx, 0.2f);
        EntityFX_func_405_d((EntityFX *)fx, 0.6f);
        EffectRenderer_addEffect(self, (EntityFX *)fx);
    }
}

const char *EffectRenderer_getStatistics(EffectRenderer *self, char *buf, int bufLen) {
    snprintf(buf, (size_t)bufLen, "%d",
             self->fxLayers[0].size + self->fxLayers[1].size + self->fxLayers[2].size);
    return buf;
}
