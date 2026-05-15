#include "ContainerPlayer.h"
#include "InventoryCrafting.h"
#include "InventoryCraftResult.h"
#include "SlotCrafting.h"
#include "SlotArmor.h"
#include "CraftingManager.h"
#include "Slot.h"
#include "InventoryPlayer.h"
#include "EntityPlayer.h"
#include "Entity.h"
#include "ItemStack.h"
#include <stdlib.h>

static int vtable_isUsableByPlayer(Container *self, EntityPlayer *var1) {
    return ContainerPlayer_isUsableByPlayer((ContainerPlayer *)self, var1);
}
static ItemStack *vtable_getStackInSlot(Container *self, int var1) {
    return ContainerPlayer_getStackInSlot((ContainerPlayer *)self, var1);
}
static void vtable_onCraftMatrixChanged(Container *self, struct IInventory *var1) {
    ContainerPlayer_onCraftMatrixChanged((ContainerPlayer *)self, var1);
}
static void vtable_onCraftGuiClosed(Container *self, EntityPlayer *var1) {
    ContainerPlayer_onCraftGuiClosed((ContainerPlayer *)self, var1);
}

static const ContainerVtable ContainerPlayer_vtable = {vtable_isUsableByPlayer,
                                                       vtable_getStackInSlot,
                                                       Container_vtable_base_updateCraftingResults,
                                                       Container_vtable_base_func_20112_a,
                                                       vtable_onCraftMatrixChanged,
                                                       vtable_onCraftGuiClosed};

void ContainerPlayer_construct1(ContainerPlayer *self, InventoryPlayer *var1) {
    ContainerPlayer_construct(self, var1, 1);
}

void ContainerPlayer_construct(ContainerPlayer *self, InventoryPlayer *var1, int var2) {
    Container_construct(&self->base);
    self->base.vtable = &ContainerPlayer_vtable;
    self->craftMatrix = (InventoryCrafting *)calloc(1, sizeof(InventoryCrafting));
    InventoryCrafting_init(self->craftMatrix, &self->base, 2, 2);
    self->craftResult = (IInventory *)calloc(1, sizeof(InventoryCraftResult));
    InventoryCraftResult_init((InventoryCraftResult *)self->craftResult);
    self->isSinglePlayer = 0;
    self->isSinglePlayer = var2;
    Container_addSlot(&self->base, (Slot *)SlotCrafting_create((EntityPlayer *)var1->player,
                                                               (IInventory *)self->craftMatrix,
                                                               self->craftResult, 0, 144, 36));
    int var3, var4;
    for (var3 = 0; var3 < 2; ++var3) {
        for (var4 = 0; var4 < 2; ++var4) {
            Container_addSlot(&self->base,
                              Slot_create((IInventory *)self->craftMatrix, var4 + var3 * 2,
                                          88 + var4 * 18, 26 + var3 * 18));
        }
    }
    for (var3 = 0; var3 < 4; ++var3) {
        Container_addSlot(
            &self->base, (Slot *)SlotArmor_create(self, (IInventory *)var1,
                                                  InventoryPlayer_getSizeInventory(var1) - 1 - var3,
                                                  8, 8 + var3 * 18, var3));
    }
    for (var3 = 0; var3 < 3; ++var3) {
        for (var4 = 0; var4 < 9; ++var4) {
            Container_addSlot(&self->base, Slot_create((IInventory *)var1, var4 + (var3 + 1) * 9,
                                                       8 + var4 * 18, 84 + var3 * 18));
        }
    }
    for (var3 = 0; var3 < 9; ++var3) {
        Container_addSlot(&self->base, Slot_create((IInventory *)var1, var3, 8 + var3 * 18, 142));
    }
    Container_onCraftMatrixChanged_v(&self->base, (IInventory *)self->craftMatrix);
}

void ContainerPlayer_onCraftMatrixChanged(ContainerPlayer *self, struct IInventory *var1) {
    (void)var1;
    self->craftResult->vtable->setInventorySlotContents(
        self->craftResult, 0,
        CraftingManager_findMatchingRecipe(CraftingManager_getInstance(), self->craftMatrix));
}

void ContainerPlayer_onCraftGuiClosed(ContainerPlayer *self, EntityPlayer *var1) {
    Container_onCraftGuiClosed(&self->base, var1);
    int var2;
    for (var2 = 0; var2 < 4; ++var2) {
        ItemStack *var3 = InventoryCrafting_getStackInSlot(self->craftMatrix, var2);
        if (var3 != NULL) {
            EntityPlayer_dropPlayerItem(var1, var3);
            InventoryCrafting_setInventorySlotContents(self->craftMatrix, var2, NULL);
            ItemStack_free(var3);
        }
    }
}

int ContainerPlayer_isUsableByPlayer(ContainerPlayer *self, EntityPlayer *var1) {
    (void)self;
    (void)var1;
    return 1;
}

ItemStack *ContainerPlayer_getStackInSlot(ContainerPlayer *self, int var1) {
    ItemStack *var2 = NULL;
    struct Slot *var3 = self->base.slots.entries[var1];
    if (var3 != NULL && Slot_getHasStack(var3)) {
        ItemStack *var4 = Slot_getStack(var3);
        var2 = ItemStack_copy_ptr(var4);
        if (var1 == 0) {
            Container_func_28125_a(&self->base, var4, 9, 45, 1);
        } else if (var1 >= 9 && var1 < 36) {
            Container_func_28125_a(&self->base, var4, 36, 45, 0);
        } else if (var1 >= 36 && var1 < 45) {
            Container_func_28125_a(&self->base, var4, 9, 36, 0);
        } else {
            Container_func_28125_a(&self->base, var4, 9, 45, 0);
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
