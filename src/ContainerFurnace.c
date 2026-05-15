#include "ContainerFurnace.h"
#include "TileEntityFurnace.h"
#include "Slot.h"
#include "SlotFurnace.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "ICrafting.h"
#include <stdlib.h>

static int vtable_isUsableByPlayer(Container *self, EntityPlayer *var1) {
    return ContainerFurnace_isUsableByPlayer((ContainerFurnace *)self, var1);
}

static ItemStack *vtable_getStackInSlot(Container *self, int var1) {
    return ContainerFurnace_getStackInSlot((ContainerFurnace *)self, var1);
}
static void vtable_updateCraftingResults(Container *self) {
    ContainerFurnace_updateCraftingResults((ContainerFurnace *)self);
}
static void vtable_func_20112_a(Container *self, int var1, int var2) {
    ContainerFurnace_func_20112_a((ContainerFurnace *)self, var1, var2);
}

static const ContainerVtable ContainerFurnace_vtable = {vtable_isUsableByPlayer,
                                                        vtable_getStackInSlot,
                                                        vtable_updateCraftingResults,
                                                        vtable_func_20112_a,
                                                        Container_vtable_base_onCraftMatrixChanged,
                                                        Container_vtable_base_onCraftGuiClosed};

void ContainerFurnace_construct(ContainerFurnace *self, InventoryPlayer *var1,
                                TileEntityFurnace *var2) {
    Container_construct(&self->base);
    self->base.vtable = &ContainerFurnace_vtable;
    self->furnace = var2;
    self->cookTime = 0;
    self->burnTime = 0;
    self->itemBurnTime = 0;
    Container_addSlot(&self->base, Slot_create(&var2->iinv, 0, 56, 17));
    Container_addSlot(&self->base, Slot_create(&var2->iinv, 1, 56, 53));
    Container_addSlot(&self->base, (Slot *)SlotFurnace_create((EntityPlayer *)var1->player,
                                                              &var2->iinv, 2, 116, 35));
    int var3, var4;
    for (var3 = 0; var3 < 3; ++var3) {
        for (var4 = 0; var4 < 9; ++var4) {
            Container_addSlot(&self->base, Slot_create((IInventory *)var1, var4 + var3 * 9 + 9,
                                                       8 + var4 * 18, 84 + var3 * 18));
        }
    }
    for (var3 = 0; var3 < 9; ++var3) {
        Container_addSlot(&self->base, Slot_create((IInventory *)var1, var3, 8 + var3 * 18, 142));
    }
    (void)var4;
}

void ContainerFurnace_updateCraftingResults(ContainerFurnace *self) {
    Container_updateCraftingResults(&self->base);
    int var1;
    for (var1 = 0; var1 < self->base.field_20121_g.count; ++var1) {
        struct ICrafting *var2 = self->base.field_20121_g.entries[var1];
        if (self->cookTime != self->furnace->furnaceCookTime) {
            var2->vtable->func_20158_a(var2, &self->base, 0, self->furnace->furnaceCookTime);
        }
        if (self->burnTime != self->furnace->furnaceBurnTime) {
            var2->vtable->func_20158_a(var2, &self->base, 1, self->furnace->furnaceBurnTime);
        }
        if (self->itemBurnTime != self->furnace->currentItemBurnTime) {
            var2->vtable->func_20158_a(var2, &self->base, 2, self->furnace->currentItemBurnTime);
        }
    }
    self->cookTime = self->furnace->furnaceCookTime;
    self->burnTime = self->furnace->furnaceBurnTime;
    self->itemBurnTime = self->furnace->currentItemBurnTime;
}

void ContainerFurnace_func_20112_a(ContainerFurnace *self, int var1, int var2) {
    if (var1 == 0) {
        self->furnace->furnaceCookTime = var2;
    }
    if (var1 == 1) {
        self->furnace->furnaceBurnTime = var2;
    }
    if (var1 == 2) {
        self->furnace->currentItemBurnTime = var2;
    }
}

int ContainerFurnace_isUsableByPlayer(ContainerFurnace *self, EntityPlayer *var1) {
    return TileEntityFurnace_canInteractWith(self->furnace, var1);
}

ItemStack *ContainerFurnace_getStackInSlot(ContainerFurnace *self, int var1) {
    ItemStack *var2 = NULL;
    struct Slot *var3 = self->base.slots.entries[var1];
    if (var3 != NULL && Slot_getHasStack(var3)) {
        ItemStack *var4 = Slot_getStack(var3);
        var2 = ItemStack_copy_ptr(var4);
        if (var1 == 2) {
            Container_func_28125_a(&self->base, var4, 3, 39, 1);
        } else if (var1 >= 3 && var1 < 30) {
            Container_func_28125_a(&self->base, var4, 30, 39, 0);
        } else if (var1 >= 30 && var1 < 39) {
            Container_func_28125_a(&self->base, var4, 3, 30, 0);
        } else {
            Container_func_28125_a(&self->base, var4, 3, 39, 0);
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
