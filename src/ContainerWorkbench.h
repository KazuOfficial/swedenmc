#ifndef CONTAINER_WORKBENCH_H
#define CONTAINER_WORKBENCH_H

#include "Container.h"
#include "InventoryPlayer.h"

struct InventoryCrafting;
struct IInventory;
struct World;

typedef struct ContainerWorkbench {
    Container base;

    struct InventoryCrafting *craftMatrix;

    struct IInventory *craftResult;

    struct World *field_20133_c;

    int field_20132_h;

    int field_20131_i;

    int field_20130_j;
} ContainerWorkbench;

void ContainerWorkbench_construct(ContainerWorkbench *self, InventoryPlayer *var1,
                                  struct World *var2, int var3, int var4, int var5);

void ContainerWorkbench_onCraftMatrixChanged(ContainerWorkbench *self, struct IInventory *var1);

void ContainerWorkbench_onCraftGuiClosed(ContainerWorkbench *self, EntityPlayer *var1);

int ContainerWorkbench_isUsableByPlayer(ContainerWorkbench *self, EntityPlayer *var1);

ItemStack *ContainerWorkbench_getStackInSlot(ContainerWorkbench *self, int var1);

#endif
