#include "ContainerDispenser.h"
#include "TileEntityDispenser.h"
#include "Slot.h"

static int vtable_isUsableByPlayer(Container *self, EntityPlayer *var1) {
    return ContainerDispenser_isUsableByPlayer((ContainerDispenser *)self, var1);
}

static const ContainerVtable ContainerDispenser_vtable = {
    vtable_isUsableByPlayer,
    Container_vtable_base_getStackInSlot,
    Container_vtable_base_updateCraftingResults,
    Container_vtable_base_func_20112_a,
    Container_vtable_base_onCraftMatrixChanged,
    Container_vtable_base_onCraftGuiClosed};

void ContainerDispenser_construct(ContainerDispenser *self, struct IInventory *var1,
                                  TileEntityDispenser *var2) {
    Container_construct(&self->base);
    self->base.vtable = &ContainerDispenser_vtable;
    self->field_21149_a = var2;
    int var3, var4;
    for (var3 = 0; var3 < 3; ++var3) {
        for (var4 = 0; var4 < 3; ++var4) {
            Container_addSlot(&self->base, Slot_create(&var2->iinv, var4 + var3 * 3, 62 + var4 * 18,
                                                       17 + var3 * 18));
        }
    }
    for (var3 = 0; var3 < 3; ++var3) {
        for (var4 = 0; var4 < 9; ++var4) {
            Container_addSlot(
                &self->base, Slot_create(var1, var4 + var3 * 9 + 9, 8 + var4 * 18, 84 + var3 * 18));
        }
    }
    for (var3 = 0; var3 < 9; ++var3) {
        Container_addSlot(&self->base, Slot_create(var1, var3, 8 + var3 * 18, 142));
    }
}

int ContainerDispenser_isUsableByPlayer(ContainerDispenser *self, EntityPlayer *var1) {
    return TileEntityDispenser_canInteractWith(self->field_21149_a, var1);
}
