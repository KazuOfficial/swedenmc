#ifndef ENTITY_CLIENT_PLAYER_MP_H
#define ENTITY_CLIENT_PLAYER_MP_H

#include "EntityPlayerSP.h"
#include "StatBase.h"

struct NetClientHandler;
struct EntityItem;

typedef struct EntityClientPlayerMP {
    EntityPlayerSP base;

    struct NetClientHandler *sendQueue;

    int field_9380_bx;

    int field_21093_bH;

    double oldPosX;

    double field_9378_bz;

    double oldPosY;

    double oldPosZ;

    float oldRotationYaw;

    float oldRotationPitch;

    int field_9382_bF;

    int wasSneaking;

    int field_12242_bI;
} EntityClientPlayerMP;

void EntityClientPlayerMP_construct(EntityClientPlayerMP *self, struct Minecraft *var1, World *var2,
                                    Session *var3, struct NetClientHandler *var4);

int EntityClientPlayerMP_attackEntityFrom(Entity *self, Entity *var1, int var2);

void EntityClientPlayerMP_heal(EntityLiving *self, int var1);

void EntityClientPlayerMP_onUpdate(Entity *self);

void EntityClientPlayerMP_func_4056_N(EntityClientPlayerMP *self);

void EntityClientPlayerMP_dropCurrentItem(EntityClientPlayerMP *self);

void EntityClientPlayerMP_joinEntityItemWithWorld(EntityClientPlayerMP *self,
                                                  struct EntityItem *var1);

void EntityClientPlayerMP_sendChatMessage(EntityClientPlayerMP *self, const char *var1);

void EntityClientPlayerMP_swingItem(EntityPlayer *self);

void EntityClientPlayerMP_respawnPlayer(EntityClientPlayerMP *self);

void EntityClientPlayerMP_damageEntity(EntityLiving *self, Entity *source, int var1);

void EntityClientPlayerMP_closeScreen(EntityPlayer *self);

void EntityClientPlayerMP_setHealth(EntityLiving *self, int var1);

void EntityClientPlayerMP_addStat(EntityClientPlayerMP *self, StatBase *var1, int var2);

void EntityClientPlayerMP_func_27027_b(EntityClientPlayerMP *self, StatBase *var1, int var2);

#endif
