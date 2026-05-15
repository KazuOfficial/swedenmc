#ifndef ENTITY_FX_H
#define ENTITY_FX_H

#include "Entity.h"
#include "Tessellator.h"
#include "NBTTagCompound.h"

typedef struct EntityFX EntityFX;

typedef struct EntityFXVtable {
    EntityVtable base;

    void (*renderParticle)(EntityFX *self, Tessellator *var1, float var2, float var3, float var4,
                           float var5, float var6, float var7);

    int (*getFXLayer)(EntityFX *self);
} EntityFXVtable;

typedef struct EntityFX {
    Entity base;

    int particleTextureIndex;

    float particleTextureJitterX;

    float particleTextureJitterY;

    int particleAge;

    int particleMaxAge;

    float particleScale;

    float particleGravity;

    float particleRed;

    float particleGreen;

    float particleBlue;
} EntityFX;

extern double EntityFX_interpPosX;
extern double EntityFX_interpPosY;
extern double EntityFX_interpPosZ;

void EntityFX_construct(EntityFX *self, World *var1, double var2, double var4, double var6,
                        double var8, double var10, double var12);

EntityFX *EntityFX_func_407_b(EntityFX *self, float var1);

EntityFX *EntityFX_func_405_d(EntityFX *self, float var1);

int EntityFX_canTriggerWalking(Entity *self_);

void EntityFX_entityInit(Entity *self_);

void EntityFX_onUpdate(Entity *self_);

void EntityFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3, float var4,
                             float var5, float var6, float var7);

int EntityFX_getFXLayer(EntityFX *self);

void EntityFX_writeEntityToNBT(Entity *self_, NBTTagCompound *var1);

void EntityFX_readEntityFromNBT(Entity *self_, NBTTagCompound *var1);

void EntityFX_initVtable(EntityFXVtable *vt);

#endif
