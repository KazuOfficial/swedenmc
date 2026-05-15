#include "EntitySlimeFX.h"
#include "Block.h"
#include "Entity.h"
#include <stdlib.h>

static EntityFXVtable EntitySlimeFX_vtable;
static int EntitySlimeFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntitySlimeFX_vtable_init)
        return;
    EntityFX_initVtable(&EntitySlimeFX_vtable);
    EntitySlimeFX_vtable.getFXLayer = EntitySlimeFX_getFXLayer;
    EntitySlimeFX_vtable.renderParticle = EntitySlimeFX_renderParticle;
    EntitySlimeFX_vtable_init = 1;
}

void EntitySlimeFX_construct(EntitySlimeFX *self, World *var1, double var2, double var4,
                             double var6, Item *var8) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, 0.0, 0.0, 0.0);
    self->base.base.vtable = (const EntityVtable *)&EntitySlimeFX_vtable;

    self->base.particleTextureIndex = var8->vtable->getIconFromDamage(var8, 0);
    self->base.particleRed = self->base.particleGreen = self->base.particleBlue = 1.0F;

    self->base.particleGravity = Block_blockSnow->blockParticleGravity;
    self->base.particleScale /= 2.0F;
}

EntitySlimeFX *EntitySlimeFX_create(World *var1, double var2, double var4, double var6,
                                    Item *var8) {
    EntitySlimeFX *self = (EntitySlimeFX *)calloc(1, sizeof(EntitySlimeFX));
    EntitySlimeFX_construct(self, var1, var2, var4, var6, var8);
    return self;
}

int EntitySlimeFX_getFXLayer(EntityFX *self) {
    (void)self;
    return 2;
}

void EntitySlimeFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                  float var4, float var5, float var6, float var7) {
    Entity *selfE = &selfFX->base;
    float var8 =
        ((float)(selfFX->particleTextureIndex % 16) + selfFX->particleTextureJitterX / 4.0F) /
        16.0F;
    float var9 = var8 + 0.999F / 64.0F;
    float var10 =
        ((float)(selfFX->particleTextureIndex / 16) + selfFX->particleTextureJitterY / 4.0F) /
        16.0F;
    float var11 = var10 + 0.999F / 64.0F;
    float var12 = 0.1F * selfFX->particleScale;
    float var13 = (float)(selfE->prevPosX + (selfE->posX - selfE->prevPosX) * (double)var2 -
                          EntityFX_interpPosX);
    float var14 = (float)(selfE->prevPosY + (selfE->posY - selfE->prevPosY) * (double)var2 -
                          EntityFX_interpPosY);
    float var15 = (float)(selfE->prevPosZ + (selfE->posZ - selfE->prevPosZ) * (double)var2 -
                          EntityFX_interpPosZ);

    float var16 = selfE->vtable->getEntityBrightness(selfE, var2);
    Tessellator_setColorOpaque_F(var1, var16 * selfFX->particleRed, var16 * selfFX->particleGreen,
                                 var16 * selfFX->particleBlue);
    Tessellator_addVertexWithUV(
        var1, (double)(var13 - var3 * var12 - var6 * var12), (double)(var14 - var4 * var12),
        (double)(var15 - var5 * var12 - var7 * var12), (double)var8, (double)var11);
    Tessellator_addVertexWithUV(
        var1, (double)(var13 - var3 * var12 + var6 * var12), (double)(var14 + var4 * var12),
        (double)(var15 - var5 * var12 + var7 * var12), (double)var8, (double)var10);
    Tessellator_addVertexWithUV(
        var1, (double)(var13 + var3 * var12 + var6 * var12), (double)(var14 + var4 * var12),
        (double)(var15 + var5 * var12 + var7 * var12), (double)var9, (double)var10);
    Tessellator_addVertexWithUV(
        var1, (double)(var13 + var3 * var12 - var6 * var12), (double)(var14 - var4 * var12),
        (double)(var15 + var5 * var12 - var7 * var12), (double)var9, (double)var11);
}
