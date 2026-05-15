#ifndef ENTITY_PAINTING_H
#define ENTITY_PAINTING_H

#include "Entity.h"
#include "EnumArt.h"

typedef struct EntityPainting {
    Entity base;

    int field_695_c;

    int direction;

    int xPosition;

    int yPosition;

    int zPosition;

    EnumArt *art;
} EntityPainting;

void EntityPainting_construct1(EntityPainting *self, World *var1);
void EntityPainting_construct2(EntityPainting *self, World *var1, int var2, int var3, int var4,
                               int var5);
void EntityPainting_construct3(EntityPainting *self, World *var1, int var2, int var3, int var4,
                               int var5, const char *var6);
EntityPainting *EntityPainting_create1(World *var1);
EntityPainting *EntityPainting_create2(World *var1, int var2, int var3, int var4, int var5);
EntityPainting *EntityPainting_create3(World *var1, int var2, int var3, int var4, int var5,
                                       const char *var6);

void EntityPainting_onUpdate(Entity *self);
int EntityPainting_canBeCollidedWith(Entity *self);
int EntityPainting_attackEntityFrom(Entity *self, Entity *var1, int var2);
void EntityPainting_writeEntityToNBT(Entity *self, NBTTagCompound *var1);
void EntityPainting_readEntityFromNBT(Entity *self, NBTTagCompound *var1);
void EntityPainting_moveEntity(Entity *self, double var1, double var3, double var5);
void EntityPainting_addVelocity(Entity *self, double var1, double var3, double var5);

void EntityPainting_func_412_b(EntityPainting *self, int var1);
int EntityPainting_func_410_i(EntityPainting *self);

#endif
