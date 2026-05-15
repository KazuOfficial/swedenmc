#include "ContainerWorkbench.h"
#include "InventoryCrafting.h"
#include "InventoryCraftResult.h"
#include "SlotCrafting.h"
#include "CraftingManager.h"
#include "Slot.h"
#include "InventoryPlayer.h"
#include "EntityPlayer.h"
#include "Entity.h"
#include "World.h"
#include "Block.h"
#include "ItemStack.h"
#include <stdlib.h>

static int vtable_isUsableByPlayer(Container *self, EntityPlayer *var1) {
    return ContainerWorkbench_isUsableByPlayer((ContainerWorkbench *)self, var1);
}
static ItemStack *vtable_getStackInSlot(Container *self, int var1) {
    return ContainerWorkbench_getStackInSlot((ContainerWorkbench *)self, var1);
}
static void vtable_onCraftMatrixChanged(Container *self, struct IInventory *var1) {
    ContainerWorkbench_onCraftMatrixChanged((ContainerWorkbench *)self, var1);
}
static void vtable_onCraftGuiClosed(Container *self, EntityPlayer *var1) {
    ContainerWorkbench_onCraftGuiClosed((ContainerWorkbench *)self, var1);
}

static const ContainerVtable ContainerWorkbench_vtable = {
    vtable_isUsableByPlayer,
    vtable_getStackInSlot,
    Container_vtable_base_updateCraftingResults,
    Container_vtable_base_func_20112_a,
    vtable_onCraftMatrixChanged,
    vtable_onCraftGuiClosed};

void ContainerWorkbench_construct(ContainerWorkbench *self, InventoryPlayer *var1,
                                  struct World *var2, int var3, int var4, int var5) {
    Container_construct(&self->base);
    self->base.vtable = &ContainerWorkbench_vtable;

    self->craftMatrix = (InventoryCrafting *)calloc(1, sizeof(InventoryCrafting));
    InventoryCrafting_init(self->craftMatrix, &self->base, 3, 3);
    self->craftResult = (IInventory *)calloc(1, sizeof(InventoryCraftResult));
    InventoryCraftResult_init((InventoryCraftResult *)self->craftResult);
    self->field_20133_c = var2;
    self->field_20132_h = var3;
    self->field_20131_i = var4;
    self->field_20130_j = var5;
    Container_addSlot(&self->base, (Slot *)SlotCrafting_create((EntityPlayer *)var1->player,
                                                               (IInventory *)self->craftMatrix,
                                                               self->craftResult, 0, 124, 35));
    int var6, var7;
    for (var6 = 0; var6 < 3; ++var6) {
        for (var7 = 0; var7 < 3; ++var7) {
            Container_addSlot(&self->base,
                              Slot_create((IInventory *)self->craftMatrix, var7 + var6 * 3,
                                          30 + var7 * 18, 17 + var6 * 18));
        }
    }
    for (var6 = 0; var6 < 3; ++var6) {
        for (var7 = 0; var7 < 9; ++var7) {
            Container_addSlot(&self->base, Slot_create((IInventory *)var1, var7 + var6 * 9 + 9,
                                                       8 + var7 * 18, 84 + var6 * 18));
        }
    }
    for (var6 = 0; var6 < 9; ++var6) {
        Container_addSlot(&self->base, Slot_create((IInventory *)var1, var6, 8 + var6 * 18, 142));
    }
    Container_onCraftMatrixChanged_v(&self->base, (IInventory *)self->craftMatrix);
}

void ContainerWorkbench_onCraftMatrixChanged(ContainerWorkbench *self, struct IInventory *var1) {
    (void)var1;
    self->craftResult->vtable->setInventorySlotContents(
        self->craftResult, 0,
        CraftingManager_findMatchingRecipe(CraftingManager_getInstance(), self->craftMatrix));
}

void ContainerWorkbench_onCraftGuiClosed(ContainerWorkbench *self, EntityPlayer *var1) {
    Container_onCraftGuiClosed(&self->base, var1);
    if (!self->field_20133_c->multiplayerWorld) {
        int var2;
        for (var2 = 0; var2 < 9; ++var2) {
            ItemStack *var3 = InventoryCrafting_getStackInSlot(self->craftMatrix, var2);
            if (var3 != NULL) {
                EntityPlayer_dropPlayerItem(var1, var3);
            }
        }
    }
}

int ContainerWorkbench_isUsableByPlayer(ContainerWorkbench *self, EntityPlayer *var1) {
    return World_getBlockId(self->field_20133_c, self->field_20132_h, self->field_20131_i,
                            self->field_20130_j) != Block_workbench->blockID
               ? 0
               : ((Entity *)var1)
                         ->vtable->getDistanceSq((Entity *)var1, (double)self->field_20132_h + 0.5,
                                                 (double)self->field_20131_i + 0.5,
                                                 (double)self->field_20130_j + 0.5) <= 64.0;
}

ItemStack *ContainerWorkbench_getStackInSlot(ContainerWorkbench *self, int var1) {
    ItemStack *var2 = NULL;
    struct Slot *var3 = self->base.slots.entries[var1];
    if (var3 != NULL && Slot_getHasStack(var3)) {
        ItemStack *var4 = Slot_getStack(var3);
        var2 = ItemStack_copy_ptr(var4);
        if (var1 == 0) {
            Container_func_28125_a(&self->base, var4, 10, 46, 1);
        } else if (var1 >= 10 && var1 < 37) {
            Container_func_28125_a(&self->base, var4, 37, 46, 0);
        } else if (var1 >= 37 && var1 < 46) {
            Container_func_28125_a(&self->base, var4, 10, 37, 0);
        } else {
            Container_func_28125_a(&self->base, var4, 10, 46, 0);
        }
        if (var4->stackSize == 0) {
            Slot_putStack(var3, NULL);
        } else {
            Slot_onSlotChanged(var3);
        }
        if (var4->stackSize == var2->stackSize) {
            return NULL;
        }
        Slot_onPickupFromSlot(var3, var4);
    }
    return var2;
}
