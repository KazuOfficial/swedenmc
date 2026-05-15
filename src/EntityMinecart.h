#ifndef ENTITY_MINECART_H
#define ENTITY_MINECART_H

#include "Entity.h"
#include "ItemStack.h"
#include "NBTTagCompound.h"
#include "Vec3D.h"

typedef struct EntityMinecart {
    Entity base;

    ItemStack *cargoItems[36];

    int minecartCurrentDamage;

    int minecartTimeSinceHit;

    int minecartRockDirection;

    int field_856_i;

    int minecartType;

    int fuel;

    double pushX;

    double pushZ;

    int field_9415_k;

    double field_9414_l;

    double field_9413_m;

    double field_9412_n;

    double field_9411_o;

    double field_9410_p;

    double field_9409_q;

    double field_9408_r;

    double field_9407_s;
} EntityMinecart;

void EntityMinecart_construct1(EntityMinecart *self, World *var1);
void EntityMinecart_construct2(EntityMinecart *self, World *var1, double var2, double var4,
                               double var6, int var8);
EntityMinecart *EntityMinecart_create(World *var1, double var2, double var4, double var6, int var8);

void EntityMinecart_setEntityDead(Entity *self);
int EntityMinecart_canTriggerWalking(Entity *self);
void EntityMinecart_entityInit(Entity *self);
AxisAlignedBB *EntityMinecart_getCollisionBox(Entity *self, Entity *var1);
AxisAlignedBB *EntityMinecart_getBoundingBox(Entity *self);
int EntityMinecart_canBePushed(Entity *self);
double EntityMinecart_getMountedYOffset(Entity *self);
int EntityMinecart_attackEntityFrom(Entity *self, Entity *var1, int var2);
void EntityMinecart_performHurtAnimation(Entity *self);
int EntityMinecart_canBeCollidedWith(Entity *self);
void EntityMinecart_onUpdate(Entity *self);
float EntityMinecart_getShadowSize(Entity *self);
void EntityMinecart_applyEntityCollision(Entity *self, Entity *var1);
int EntityMinecart_interact(Entity *self, EntityPlayer *var1);
void EntityMinecart_writeEntityToNBT(Entity *self, NBTTagCompound *var1);
void EntityMinecart_readEntityFromNBT(Entity *self, NBTTagCompound *var1);
void EntityMinecart_setPositionAndRotation2(Entity *self, double var1, double var3, double var5,
                                            float var7, float var8, int var9);
void EntityMinecart_setVelocity(Entity *self, double var1, double var3, double var5);

int EntityMinecart_getSizeInventory(EntityMinecart *self);
ItemStack *EntityMinecart_getStackInSlot(EntityMinecart *self, int var1);
ItemStack *EntityMinecart_decrStackSize(EntityMinecart *self, int var1, int var2);
void EntityMinecart_setInventorySlotContents(EntityMinecart *self, int var1, ItemStack *var2);
const char *EntityMinecart_getInvName(EntityMinecart *self);
int EntityMinecart_getInventoryStackLimit(EntityMinecart *self);
void EntityMinecart_onInventoryChanged(EntityMinecart *self);
int EntityMinecart_canInteractWith(EntityMinecart *self, EntityPlayer *var1);

Vec3D *EntityMinecart_func_515_a(EntityMinecart *self, double var1, double var3, double var5,
                                 double var7);
Vec3D *EntityMinecart_func_514_g(EntityMinecart *self, double var1, double var3, double var5);

#endif
