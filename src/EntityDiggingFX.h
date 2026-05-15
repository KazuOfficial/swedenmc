#ifndef ENTITY_DIGGING_FX_H
#define ENTITY_DIGGING_FX_H

#include "EntityFX.h"
#include "Block.h"
#include "Tessellator.h"

typedef struct EntityDiggingFX {
    EntityFX base;

    Block *field_4082_a;

    int field_32001_o;
} EntityDiggingFX;

void EntityDiggingFX_construct(EntityDiggingFX *self, World *var1, double var2, double var4,
                               double var6, double var8, double var10, double var12, Block *var14,
                               int var15, int var16);

EntityDiggingFX *EntityDiggingFX_func_4041_a(EntityDiggingFX *self, int var1, int var2, int var3);

int EntityDiggingFX_getFXLayer(EntityFX *self_);

void EntityDiggingFX_renderParticle(EntityFX *self_, Tessellator *var1, float var2, float var3,
                                    float var4, float var5, float var6, float var7);

#endif
