#include "JavaMath.h"
#include "EntityFX.h"
#include "Entity.h"
#include "MathHelper.h"
#include "World.h"
#include <math.h>

double EntityFX_interpPosX = 0.0;
double EntityFX_interpPosY = 0.0;
double EntityFX_interpPosZ = 0.0;

static EntityFXVtable EntityFX_vtable;
static int EntityFX_vtable_init = 0;

static void ensure_vtable(void);

void EntityFX_construct(EntityFX *self, World *var1, double var2, double var4, double var6,
                        double var8, double var10, double var12) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = (const EntityVtable *)&EntityFX_vtable;

    Entity_setSize(&self->base, 0.2F, 0.2F);
    self->base.yOffset = self->base.height / 2.0F;
    self->base.vtable->setPosition(&self->base, var2, var4, var6);
    self->particleRed = self->particleGreen = self->particleBlue = 1.0F;
    self->base.motionX = var8 + (double)((float)(Math_random() * 2.0 - 1.0) * 0.4F);
    self->base.motionY = var10 + (double)((float)(Math_random() * 2.0 - 1.0) * 0.4F);
    self->base.motionZ = var12 + (double)((float)(Math_random() * 2.0 - 1.0) * 0.4F);
    float var14 = (float)(Math_random() + Math_random() + 1.0) * 0.15F;
    float var15 = MathHelper_sqrt_double(self->base.motionX * self->base.motionX +
                                         self->base.motionY * self->base.motionY +
                                         self->base.motionZ * self->base.motionZ);
    self->base.motionX = self->base.motionX / (double)var15 * (double)var14 * (double)0.4F;
    self->base.motionY =
        self->base.motionY / (double)var15 * (double)var14 * (double)0.4F + (double)0.1F;
    self->base.motionZ = self->base.motionZ / (double)var15 * (double)var14 * (double)0.4F;
    self->particleTextureJitterX = JavaRandom_nextFloat(&self->base.rand) * 3.0F;
    self->particleTextureJitterY = JavaRandom_nextFloat(&self->base.rand) * 3.0F;
    self->particleScale = (JavaRandom_nextFloat(&self->base.rand) * 0.5F + 0.5F) * 2.0F;
    self->particleMaxAge = (int)(4.0F / (JavaRandom_nextFloat(&self->base.rand) * 0.9F + 0.1F));
    self->particleAge = 0;
}

EntityFX *EntityFX_func_407_b(EntityFX *self, float var1) {
    self->base.motionX *= (double)var1;
    self->base.motionY = (self->base.motionY - (double)0.1F) * (double)var1 + (double)0.1F;
    self->base.motionZ *= (double)var1;
    return self;
}

EntityFX *EntityFX_func_405_d(EntityFX *self, float var1) {
    Entity_setSize(&self->base, 0.2F * var1, 0.2F * var1);
    self->particleScale *= var1;
    return self;
}

int EntityFX_canTriggerWalking(Entity *self_) {
    (void)self_;
    return 0;
}

void EntityFX_entityInit(Entity *self_) { (void)self_; }

void EntityFX_onUpdate(Entity *self_) {
    EntityFX *self = (EntityFX *)self_;
    self->base.prevPosX = self->base.posX;
    self->base.prevPosY = self->base.posY;
    self->base.prevPosZ = self->base.posZ;
    if (self->particleAge++ >= self->particleMaxAge) {
        Entity_setEntityDead(&self->base);
    }

    self->base.motionY -= 0.04 * (double)self->particleGravity;
    self->base.vtable->moveEntity(&self->base, self->base.motionX, self->base.motionY,
                                  self->base.motionZ);
    self->base.motionX *= (double)0.98F;
    self->base.motionY *= (double)0.98F;
    self->base.motionZ *= (double)0.98F;
    if (self->base.onGround) {
        self->base.motionX *= (double)0.7F;
        self->base.motionZ *= (double)0.7F;
    }
}

void EntityFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3, float var4,
                             float var5, float var6, float var7) {
    float var8 = (float)(self->particleTextureIndex % 16) / 16.0F;
    float var9 = var8 + 0.999F / 16.0F;
    float var10 = (float)(self->particleTextureIndex / 16) / 16.0F;
    float var11 = var10 + 0.999F / 16.0F;
    float var12 = 0.1F * self->particleScale;
    float var13 =
        (float)(self->base.prevPosX + (self->base.posX - self->base.prevPosX) * (double)var2 -
                EntityFX_interpPosX);
    float var14 =
        (float)(self->base.prevPosY + (self->base.posY - self->base.prevPosY) * (double)var2 -
                EntityFX_interpPosY);
    float var15 =
        (float)(self->base.prevPosZ + (self->base.posZ - self->base.prevPosZ) * (double)var2 -
                EntityFX_interpPosZ);
    float var16 = self->base.vtable->getEntityBrightness(&self->base, var2);
    Tessellator_setColorOpaque_F(var1, self->particleRed * var16, self->particleGreen * var16,
                                 self->particleBlue * var16);
    Tessellator_addVertexWithUV(
        var1, (double)(var13 - var3 * var12 - var6 * var12), (double)(var14 - var4 * var12),
        (double)(var15 - var5 * var12 - var7 * var12), (double)var9, (double)var11);
    Tessellator_addVertexWithUV(
        var1, (double)(var13 - var3 * var12 + var6 * var12), (double)(var14 + var4 * var12),
        (double)(var15 - var5 * var12 + var7 * var12), (double)var9, (double)var10);
    Tessellator_addVertexWithUV(
        var1, (double)(var13 + var3 * var12 + var6 * var12), (double)(var14 + var4 * var12),
        (double)(var15 + var5 * var12 + var7 * var12), (double)var8, (double)var10);
    Tessellator_addVertexWithUV(
        var1, (double)(var13 + var3 * var12 - var6 * var12), (double)(var14 - var4 * var12),
        (double)(var15 + var5 * var12 - var7 * var12), (double)var8, (double)var11);
}

int EntityFX_getFXLayer(EntityFX *self) {
    (void)self;
    return 0;
}

void EntityFX_writeEntityToNBT(Entity *self_, NBTTagCompound *var1) {
    (void)self_;
    (void)var1;
}

void EntityFX_readEntityFromNBT(Entity *self_, NBTTagCompound *var1) {
    (void)self_;
    (void)var1;
}

void EntityFX_initVtable(EntityFXVtable *vt) {
    Entity_initVtable(&vt->base);
    vt->base.canTriggerWalking = EntityFX_canTriggerWalking;
    vt->base.entityInit = EntityFX_entityInit;
    vt->base.onUpdate = EntityFX_onUpdate;
    vt->base.writeEntityToNBT = EntityFX_writeEntityToNBT;
    vt->base.readEntityFromNBT = EntityFX_readEntityFromNBT;
    vt->renderParticle = EntityFX_renderParticle;
    vt->getFXLayer = EntityFX_getFXLayer;
}

static void ensure_vtable(void) {
    if (!EntityFX_vtable_init) {
        EntityFX_initVtable(&EntityFX_vtable);
        EntityFX_vtable_init = 1;
    }
}
