#include "ContainerChest.h"
#include "IInventory.h"
#include "Slot.h"
#include "ItemStack.h"
#include "EntityPlayer.h"
#include <stdlib.h>

static int vtable_isUsableByPlayer(Container *self, EntityPlayer *var1) {
    return ContainerChest_isUsableByPlayer((ContainerChest *)self, var1);
}

static ItemStack *vtable_getStackInSlot(Container *self, int var1) {
    return ContainerChest_getStackInSlot((ContainerChest *)self, var1);
}

static const ContainerVtable ContainerChest_vtable = {vtable_isUsableByPlayer,
                                                      vtable_getStackInSlot,
                                                      Container_vtable_base_updateCraftingResults,
                                                      Container_vtable_base_func_20112_a,
                                                      Container_vtable_base_onCraftMatrixChanged,
                                                      Container_vtable_base_onCraftGuiClosed};

void ContainerChest_construct(ContainerChest *self, struct IInventory *var1,
                              struct IInventory *var2) {
    Container_construct(&self->base);
    self->base.vtable = &ContainerChest_vtable;
    self->field_20125_a = var2;
    self->field_27282_b = var2->vtable->getSizeInventory(var2) / 9;
    int var3 = (self->field_27282_b - 4) * 18;
    int var4, var5;
    for (var4 = 0; var4 < self->field_27282_b; ++var4) {
        for (var5 = 0; var5 < 9; ++var5) {
            Container_addSlot(&self->base,
                              Slot_create(var2, var5 + var4 * 9, 8 + var5 * 18, 18 + var4 * 18));
        }
    }
    for (var4 = 0; var4 < 3; ++var4) {
        for (var5 = 0; var5 < 9; ++var5) {
            Container_addSlot(&self->base, Slot_create(var1, var5 + var4 * 9 + 9, 8 + var5 * 18,
                                                       103 + var4 * 18 + var3));
        }
    }
    for (var4 = 0; var4 < 9; ++var4) {
        Container_addSlot(&self->base, Slot_create(var1, var4, 8 + var4 * 18, 161 + var3));
    }
}

int ContainerChest_isUsableByPlayer(ContainerChest *self, EntityPlayer *var1) {
    return self->field_20125_a->vtable->canInteractWith(self->field_20125_a, var1);
}

ItemStack *ContainerChest_getStackInSlot(ContainerChest *self, int var1) {
    ItemStack *var2 = NULL;
    struct Slot *var3 = self->base.slots.entries[var1];
    if (var3 != NULL && Slot_getHasStack(var3)) {
        ItemStack *var4 = Slot_getStack(var3);
        var2 = ItemStack_copy_ptr(var4);
        if (var1 < self->field_27282_b * 9) {
            Container_func_28125_a(&self->base, var4, self->field_27282_b * 9,
                                   self->base.slots.count, 1);
        } else {
            Container_func_28125_a(&self->base, var4, 0, self->field_27282_b * 9, 0);
        }
        if (var4->stackSize == 0) {
            Slot_putStack(var3, NULL);
        } else {
            Slot_onSlotChanged(var3);
        }
    }
    return var2;
}
