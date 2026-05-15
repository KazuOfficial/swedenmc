#ifndef ENTITY_OTHER_PLAYER_MP_H
#define ENTITY_OTHER_PLAYER_MP_H

#include "EntityPlayer.h"

typedef struct EntityOtherPlayerMP {
    EntityPlayer base;

    int field_785_bg;

    double field_784_bh;

    double field_783_bi;

    double field_782_bj;

    double field_780_bk;

    double field_786_bl;

    float field_20924_a;
} EntityOtherPlayerMP;

void EntityOtherPlayerMP_construct(EntityOtherPlayerMP *self, World *var1, const char *var2);
EntityOtherPlayerMP *EntityOtherPlayerMP_create(World *var1, const char *var2);

void EntityOtherPlayerMP_resetHeight(EntityPlayer *self);
int EntityOtherPlayerMP_attackEntityFrom(Entity *self, Entity *var1, int var2);
void EntityOtherPlayerMP_setPositionAndRotation2(Entity *self, double var1, double var3,
                                                 double var5, float var7, float var8, int var9);
void EntityOtherPlayerMP_onUpdate(Entity *self);
float EntityOtherPlayerMP_getShadowSize(Entity *self);
void EntityOtherPlayerMP_onLivingUpdate(EntityLiving *self);

void EntityOtherPlayerMP_outfitWithItem(EntityOtherPlayerMP *self, int var1, int var2, int var3);

void EntityOtherPlayerMP_func_6420_o(EntityOtherPlayerMP *self);

#endif
