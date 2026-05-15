#include "Container.h"
#include "ICrafting.h"
#include "Slot.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "EntityPlayer.h"
#include <stdlib.h>
#include <string.h>

static void SlotList_add(SlotList *list, struct Slot *entry) {
    if (list->count == list->capacity) {
        int newcap = list->capacity == 0 ? 8 : list->capacity * 2;
        list->entries = (struct Slot **)realloc(list->entries, newcap * sizeof(struct Slot *));
        list->capacity = newcap;
    }
    list->entries[list->count++] = entry;
}

static void ItemStackPtrList_add(ItemStackPtrList *list, ItemStack *entry) {
    if (list->count == list->capacity) {
        int newcap = list->capacity == 0 ? 8 : list->capacity * 2;
        list->entries = (ItemStack **)realloc(list->entries, newcap * sizeof(ItemStack *));
        list->capacity = newcap;
    }
    list->entries[list->count++] = entry;
}

ItemStack *Container_vtable_base_getStackInSlot(Container *self, int var1) {
    return Container_getStackInSlot(self, var1);
}
void Container_vtable_base_updateCraftingResults(Container *self) {
    Container_updateCraftingResults(self);
}
void Container_vtable_base_func_20112_a(Container *self, int var1, int var2) {
    Container_func_20112_a(self, var1, var2);
}
void Container_vtable_base_onCraftMatrixChanged(Container *self, struct IInventory *var1) {
    Container_onCraftMatrixChanged(self, var1);
}
void Container_vtable_base_onCraftGuiClosed(Container *self, EntityPlayer *var1) {
    Container_onCraftGuiClosed(self, var1);
}

void Container_construct(Container *self) {
    self->vtable = NULL;
    memset(&self->field_20123_d, 0, sizeof(ItemStackPtrList));
    memset(&self->slots, 0, sizeof(SlotList));
    self->windowId = 0;
    self->field_20917_a = 0;
    memset(&self->field_20121_g, 0, sizeof(ICraftingList));
    self->field_20918_b = NULL;
}

void Container_destroy(Container *self) {
    if (!self)
        return;
    for (int i = 0; i < self->slots.count; i++)
        free(self->slots.entries[i]);
    free(self->slots.entries);
    for (int i = 0; i < self->field_20123_d.count; i++)
        ItemStack_free(self->field_20123_d.entries[i]);
    free(self->field_20123_d.entries);
    free(self->field_20121_g.entries);
    free(self);
}

void Container_addSlot(Container *self, struct Slot *var1) {
    var1->slotNumber = self->slots.count;
    SlotList_add(&self->slots, var1);
    ItemStackPtrList_add(&self->field_20123_d, NULL);
}

void Container_updateCraftingResults(Container *self) {
    int var1;
    for (var1 = 0; var1 < self->slots.count; ++var1) {
        ItemStack *var2 = Slot_getStack(self->slots.entries[var1]);
        ItemStack *var3 = self->field_20123_d.entries[var1];
        if (!ItemStack_areItemStacksEqual(var3, var2)) {
            ItemStack_free(var3);
            var3 = (var2 == NULL) ? NULL : ItemStack_copy_ptr(var2);
            self->field_20123_d.entries[var1] = var3;

            for (int var4 = 0; var4 < self->field_20121_g.count; ++var4) {
                struct ICrafting *ic = self->field_20121_g.entries[var4];
                ic->vtable->func_20159_a(ic, self, var1, var3);
            }
        }
    }
}

struct Slot *Container_getSlot(Container *self, int var1) { return self->slots.entries[var1]; }

ItemStack *Container_getStackInSlot(Container *self, int var1) {
    struct Slot *var2 = (struct Slot *)self->slots.entries[var1];
    return var2 != NULL ? Slot_getStack(var2) : NULL;
}

ItemStack *Container_func_27280_a(Container *self, int var1, int var2, int var3,
                                  EntityPlayer *var4) {
    ItemStack *var5 = NULL;
    if (var2 == 0 || var2 == 1) {
        InventoryPlayer *var6 = &var4->inventory;
        if (var1 == -999) {
            if (InventoryPlayer_getItemStack(var6) != NULL && var1 == -999) {
                if (var2 == 0) {
                    EntityPlayer_dropPlayerItem(var4, InventoryPlayer_getItemStack(var6));
                    ItemStack_free(InventoryPlayer_getItemStack(var6));
                    InventoryPlayer_setItemStack(var6, NULL);
                }
                if (var2 == 1) {
                    ItemStack split = ItemStack_splitStack(InventoryPlayer_getItemStack(var6), 1);
                    EntityPlayer_dropPlayerItem(var4, &split);
                    if (InventoryPlayer_getItemStack(var6)->stackSize == 0) {
                        ItemStack_free(InventoryPlayer_getItemStack(var6));
                        InventoryPlayer_setItemStack(var6, NULL);
                    }
                }
            }
        } else {
            int var10;
            if (var3) {
                ItemStack *var7 = Container_getStackInSlot(self, var1);
                if (var7 != NULL) {
                    int var8 = var7->stackSize;
                    var5 = ItemStack_copy_ptr(var7);
                    struct Slot *var9 = self->slots.entries[var1];
                    if (var9 != NULL && Slot_getStack(var9) != NULL) {
                        var10 = Slot_getStack(var9)->stackSize;
                        if (var10 < var8) {
                            ItemStack_free(Container_func_27280_a(self, var1, var2, var3, var4));
                        }
                    }
                    (void)var10;
                }
            } else {
                struct Slot *var12 = self->slots.entries[var1];
                if (var12 != NULL) {
                    Slot_onSlotChanged(var12);
                    ItemStack *var13 = Slot_getStack(var12);
                    ItemStack *var14 = InventoryPlayer_getItemStack(var6);
                    if (var13 != NULL) {
                        var5 = ItemStack_copy_ptr(var13);
                    }
                    if (var13 == NULL) {
                        if (var14 != NULL && Slot_isItemValid(var12, var14)) {
                            var10 = var2 == 0 ? var14->stackSize : 1;
                            if (var10 > Slot_getSlotStackLimit(var12)) {
                                var10 = Slot_getSlotStackLimit(var12);
                            }
                            ItemStack tmp = ItemStack_splitStack(var14, var10);
                            ItemStack *tmp_ptr = ItemStack_copy_ptr(&tmp);
                            Slot_putStack(var12, tmp_ptr);
                            if (Slot_getStack(var12) != tmp_ptr)
                                ItemStack_free(tmp_ptr);
                            if (var14->stackSize == 0) {
                                ItemStack_free(var14);
                                InventoryPlayer_setItemStack(var6, NULL);
                            }
                        }
                    } else if (var14 == NULL) {
                        var10 = var2 == 0 ? var13->stackSize : (var13->stackSize + 1) / 2;
                        ItemStack *var11 = Slot_decrStackSize(var12, var10);
                        InventoryPlayer_setItemStack(var6, var11);
                        if (var13->stackSize == 0) {
                            Slot_putStack(var12, NULL);
                        }
                        Slot_onPickupFromSlot(var12, InventoryPlayer_getItemStack(var6));
                    } else if (Slot_isItemValid(var12, var14)) {
                        if (var13->itemID != var14->itemID ||
                            (ItemStack_getHasSubtypes(var13) &&
                             var13->itemDamage != var14->itemDamage)) {
                            if (var14->stackSize <= Slot_getSlotStackLimit(var12)) {

                                ItemStack *old_slot = ItemStack_copy_ptr(var13);
                                Slot_putStack(var12, var14);
                                if (Slot_getStack(var12) != var14)
                                    ItemStack_free(var14);
                                InventoryPlayer_setItemStack(var6, old_slot);
                            }
                        } else {
                            var10 = var2 == 0 ? var14->stackSize : 1;
                            if (var10 > Slot_getSlotStackLimit(var12) - var13->stackSize) {
                                var10 = Slot_getSlotStackLimit(var12) - var13->stackSize;
                            }
                            if (var10 > ItemStack_getMaxStackSize(var14) - var13->stackSize) {
                                var10 = ItemStack_getMaxStackSize(var14) - var13->stackSize;
                            }
                            ItemStack_splitStack(var14, var10);
                            if (var14->stackSize == 0) {
                                ItemStack_free(var14);
                                InventoryPlayer_setItemStack(var6, NULL);
                            }
                            var13->stackSize += var10;
                        }
                    } else if (var13->itemID == var14->itemID &&
                               ItemStack_getMaxStackSize(var14) > 1 &&
                               (!ItemStack_getHasSubtypes(var13) ||
                                var13->itemDamage == var14->itemDamage)) {
                        var10 = var13->stackSize;
                        if (var10 > 0 &&
                            var10 + var14->stackSize <= ItemStack_getMaxStackSize(var14)) {
                            var14->stackSize += var10;
                            ItemStack_splitStack(var13, var10);
                            if (var13->stackSize == 0) {
                                Slot_putStack(var12, NULL);
                            }
                            Slot_onPickupFromSlot(var12, InventoryPlayer_getItemStack(var6));
                        }
                    }
                }
            }
        }
    }
    return var5;
}

void Container_onCraftGuiClosed(Container *self, EntityPlayer *var1) {
    (void)self;
    InventoryPlayer *var2 = &var1->inventory;
    if (InventoryPlayer_getItemStack(var2) != NULL) {
        EntityPlayer_dropPlayerItem(var1, InventoryPlayer_getItemStack(var2));
        ItemStack_free(InventoryPlayer_getItemStack(var2));
        InventoryPlayer_setItemStack(var2, NULL);
    }
}

void Container_onCraftMatrixChanged(Container *self, struct IInventory *var1) {
    (void)var1;
    Container_updateCraftingResults(self);
}

void Container_putStackInSlot(Container *self, int var1, ItemStack *var2) {
    Slot_putStack(Container_getSlot(self, var1), var2);
}

void Container_putStacksInSlots(Container *self, ItemStack **var1, int length) {
    int var2;
    for (var2 = 0; var2 < length; ++var2) {
        Slot_putStack(Container_getSlot(self, var2), var1[var2]);
    }
}

void Container_func_20112_a(Container *self, int var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

short Container_func_20111_a(Container *self, InventoryPlayer *var1) {
    (void)var1;
    ++self->field_20917_a;
    return self->field_20917_a;
}

void Container_func_20113_a(Container *self, short var1) {
    (void)self;
    (void)var1;
}

void Container_func_20110_b(Container *self, short var1) {
    (void)self;
    (void)var1;
}

void Container_func_28125_a(Container *self, ItemStack *var1, int var2, int var3, int var4) {
    int var5 = var2;
    if (var4) {
        var5 = var3 - 1;
    }

    struct Slot *var6;
    ItemStack *var7;

    if (ItemStack_isStackable(var1)) {
        while (var1->stackSize > 0 && ((!var4 && var5 < var3) || (var4 && var5 >= var2))) {
            var6 = self->slots.entries[var5];
            var7 = Slot_getStack(var6);
            if (var7 != NULL && var7->itemID == var1->itemID &&
                (!ItemStack_getHasSubtypes(var1) || var1->itemDamage == var7->itemDamage)) {
                int var8 = var7->stackSize + var1->stackSize;
                if (var8 <= ItemStack_getMaxStackSize(var1)) {
                    var1->stackSize = 0;
                    var7->stackSize = var8;
                    Slot_onSlotChanged(var6);
                } else if (var7->stackSize < ItemStack_getMaxStackSize(var1)) {
                    var1->stackSize -= ItemStack_getMaxStackSize(var1) - var7->stackSize;
                    var7->stackSize = ItemStack_getMaxStackSize(var1);
                    Slot_onSlotChanged(var6);
                }
            }
            if (var4) {
                --var5;
            } else {
                ++var5;
            }
        }
    }

    if (var1->stackSize > 0) {
        if (var4) {
            var5 = var3 - 1;
        } else {
            var5 = var2;
        }
        while ((!var4 && var5 < var3) || (var4 && var5 >= var2)) {
            var6 = self->slots.entries[var5];
            var7 = Slot_getStack(var6);
            if (var7 == NULL) {
                ItemStack *copy = ItemStack_copy_ptr(var1);
                Slot_putStack(var6, copy);
                if (Slot_getStack(var6) != copy)
                    ItemStack_free(copy);
                Slot_onSlotChanged(var6);
                var1->stackSize = 0;
                break;
            }
            if (var4) {
                --var5;
            } else {
                ++var5;
            }
        }
    }
}
