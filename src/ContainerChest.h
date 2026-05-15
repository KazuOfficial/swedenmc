#ifndef CONTAINER_CHEST_H
#define CONTAINER_CHEST_H

#include "Container.h"

struct IInventory;

typedef struct ContainerChest {
    Container base;

    struct IInventory *field_20125_a;

    int field_27282_b;
} ContainerChest;

void ContainerChest_construct(ContainerChest *self, struct IInventory *var1,
                              struct IInventory *var2);

int ContainerChest_isUsableByPlayer(ContainerChest *self, EntityPlayer *var1);

ItemStack *ContainerChest_getStackInSlot(ContainerChest *self, int var1);

#endif
