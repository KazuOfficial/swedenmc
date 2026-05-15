#include "EntityDiggingFX.h"
#include "EntityFX.h"
#include "Entity.h"
#include "World.h"
#include "IBlockAccess.h"

static EntityFXVtable EntityDiggingFX_vtable;
static int EntityDiggingFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (!EntityDiggingFX_vtable_init) {
        EntityFX_initVtable(&EntityDiggingFX_vtable);
        EntityDiggingFX_vtable.renderParticle = EntityDiggingFX_renderParticle;
        EntityDiggingFX_vtable.getFXLayer = EntityDiggingFX_getFXLayer;
        EntityDiggingFX_vtable_init = 1;
    }
}

void EntityDiggingFX_construct(EntityDiggingFX *self, World *var1, double var2, double var4,
                               double var6, double var8, double var10, double var12, Block *var14,
                               int var15, int var16) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, var8, var10, var12);
    self->base.base.vtable = (const EntityVtable *)&EntityDiggingFX_vtable;

    self->field_4082_a = var14;
    self->base.particleTextureIndex =
        var14->vtable->getBlockTextureFromSideAndMetadata(var14, 0, var16);
    self->base.particleGravity = var14->blockParticleGravity;
    self->base.particleRed = self->base.particleGreen = self->base.particleBlue = 0.6F;
    self->base.particleScale /= 2.0F;
    self->field_32001_o = var15;
}

EntityDiggingFX *EntityDiggingFX_func_4041_a(EntityDiggingFX *self, int var1, int var2, int var3) {
    if (self->field_4082_a == Block_grass) {
        return self;
    } else {
        int var4 = self->field_4082_a->vtable->colorMultiplier(
            self->field_4082_a, (IBlockAccess *)self->base.base.worldObj, var1, var2, var3);
        self->base.particleRed *= (float)(var4 >> 16 & 255) / 255.0F;
        self->base.particleGreen *= (float)(var4 >> 8 & 255) / 255.0F;
        self->base.particleBlue *= (float)(var4 & 255) / 255.0F;
        return self;
    }
}

int EntityDiggingFX_getFXLayer(EntityFX *self_) {
    (void)self_;
    return 1;
}

void EntityDiggingFX_renderParticle(EntityFX *self_, Tessellator *var1, float var2, float var3,
                                    float var4, float var5, float var6, float var7) {
    EntityDiggingFX *self = (EntityDiggingFX *)self_;
    float var8 =
        ((float)(self->base.particleTextureIndex % 16) + self->base.particleTextureJitterX / 4.0F) /
        16.0F;
    float var9 = var8 + 0.999F / 64.0F;
    float var10 =
        ((float)(self->base.particleTextureIndex / 16) + self->base.particleTextureJitterY / 4.0F) /
        16.0F;
    float var11 = var10 + 0.999F / 64.0F;
    float var12 = 0.1F * self->base.particleScale;
    float var13 = (float)(self->base.base.prevPosX +
                          (self->base.base.posX - self->base.base.prevPosX) * (double)var2 -
                          EntityFX_interpPosX);
    float var14 = (float)(self->base.base.prevPosY +
                          (self->base.base.posY - self->base.base.prevPosY) * (double)var2 -
                          EntityFX_interpPosY);
    float var15 = (float)(self->base.base.prevPosZ +
                          (self->base.base.posZ - self->base.base.prevPosZ) * (double)var2 -
                          EntityFX_interpPosZ);
    float var16 = self->base.base.vtable->getEntityBrightness(&self->base.base, var2);
    Tessellator_setColorOpaque_F(var1, var16 * self->base.particleRed,
                                 var16 * self->base.particleGreen, var16 * self->base.particleBlue);
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
