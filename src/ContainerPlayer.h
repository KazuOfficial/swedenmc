#ifndef CONTAINER_PLAYER_H
#define CONTAINER_PLAYER_H

#include "Container.h"
#include "InventoryPlayer.h"

struct InventoryCrafting;
struct IInventory;

typedef struct ContainerPlayer {
    Container base;

    struct InventoryCrafting *craftMatrix;

    struct IInventory *craftResult;

    int isSinglePlayer;
} ContainerPlayer;

void ContainerPlayer_construct1(ContainerPlayer *self, InventoryPlayer *var1);

void ContainerPlayer_construct(ContainerPlayer *self, InventoryPlayer *var1, int var2);

void ContainerPlayer_onCraftMatrixChanged(ContainerPlayer *self, struct IInventory *var1);

void ContainerPlayer_onCraftGuiClosed(ContainerPlayer *self, EntityPlayer *var1);

int ContainerPlayer_isUsableByPlayer(ContainerPlayer *self, EntityPlayer *var1);

ItemStack *ContainerPlayer_getStackInSlot(ContainerPlayer *self, int var1);

#endif
