#ifndef CONTAINER_DISPENSER_H
#define CONTAINER_DISPENSER_H

#include "Container.h"
#include "TileEntityDispenser.h"

#include "IInventory.h"

typedef struct ContainerDispenser {
    Container base;

    TileEntityDispenser *field_21149_a;
} ContainerDispenser;

void ContainerDispenser_construct(ContainerDispenser *self, struct IInventory *var1,
                                  TileEntityDispenser *var2);

int ContainerDispenser_isUsableByPlayer(ContainerDispenser *self, EntityPlayer *var1);

#endif
