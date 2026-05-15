#ifndef ENTITY_ITEM_H
#define ENTITY_ITEM_H

#include "Entity.h"
#include "ItemStack.h"

typedef struct EntityItem {
    Entity base;

    ItemStack item;

    int field_803_e;

    int age;

    int delayBeforeCanPickup;

    int health;

    float field_804_d;
} EntityItem;

void EntityItem_construct(EntityItem *self, World *world, double x, double y, double z,
                          ItemStack item);
EntityItem *EntityItem_create(World *world, double x, double y, double z, ItemStack item);
void EntityItem_constructDefault(EntityItem *self, World *world);
EntityItem *EntityItem_createDefault(World *world);

#endif
