#include "TileEntityChest.h"
#include "Entity.h"
#include "World.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NBTBase.h"
#include "ItemStack.h"
#include "EntityPlayer.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

static TileEntityChest *inv_to_chest(IInventory *inv) {
    return (TileEntityChest *)((char *)inv - offsetof(TileEntityChest, inventory));
}

static int inv_getSizeInventory(IInventory *self) {
    return TileEntityChest_getSizeInventory(inv_to_chest(self));
}
static ItemStack *inv_getStackInSlot(IInventory *self, int slot) {
    return TileEntityChest_getStackInSlot(inv_to_chest(self), slot);
}
static ItemStack *inv_decrStackSize(IInventory *self, int slot, int count) {
    return TileEntityChest_decrStackSize(inv_to_chest(self), slot, count);
}
static void inv_setInventorySlotContents(IInventory *self, int slot, ItemStack *stack) {
    TileEntityChest_setSlotContents(inv_to_chest(self), slot, stack);
}
static const char *inv_getInvName(IInventory *self) {
    (void)self;
    return "Chest";
}
static int inv_getInventoryStackLimit(IInventory *self) {
    (void)self;
    return 64;
}
static void inv_onInventoryChanged(IInventory *self) { (void)self; }
static int inv_canInteractWith(IInventory *self, EntityPlayer *var1) {
    TileEntityChest *chest = inv_to_chest(self);
    TileEntity *te = &chest->base;
    if (World_getBlockTileEntity(te->worldObj, te->xCoord, te->yCoord, te->zCoord) != te)
        return 0;
    return ((Entity *)var1)
               ->vtable->getDistanceSq((Entity *)var1, (double)te->xCoord + 0.5,
                                       (double)te->yCoord + 0.5, (double)te->zCoord + 0.5) <= 64.0;
}

static const IInventoryVtable chest_inv_vtable = {
    .getSizeInventory = inv_getSizeInventory,
    .getStackInSlot = inv_getStackInSlot,
    .decrStackSize = inv_decrStackSize,
    .setInventorySlotContents = inv_setInventorySlotContents,
    .getInvName = inv_getInvName,
    .getInventoryStackLimit = inv_getInventoryStackLimit,
    .onInventoryChanged = inv_onInventoryChanged,
    .canInteractWith = inv_canInteractWith,
};

static void chest_writeToNBT(TileEntity *base, NBTTagCompound *var1) {
    TileEntityChest *self = (TileEntityChest *)base;
    TileEntity_defaultVtable.writeToNBT(base, var1);
    NBTTagList *var2 = NBTTagList_create();
    for (int var3 = 0; var3 < CHEST_SIZE; ++var3) {
        if (self->chestContentsHas[var3]) {
            NBTTagCompound *var4 = NBTTagCompound_create();
            NBTTagCompound_setByte(var4, "Slot", (int8_t)var3);
            ItemStack_writeToNBT(&self->chestContents[var3], var4);
            NBTTagList_setTag(var2, (NBTBase *)var4);
        }
    }
    NBTTagCompound_setTag(var1, "Items", (NBTBase *)var2);
}

static void chest_readFromNBT(TileEntity *base, NBTTagCompound *var1) {
    TileEntityChest *self = (TileEntityChest *)base;
    TileEntity_defaultVtable.readFromNBT(base, var1);
    NBTTagList *var2 = NBTTagCompound_getTagList(var1, "Items");
    memset(self->chestContentsHas, 0, sizeof(self->chestContentsHas));
    int tagCount = var2 ? NBTTagList_tagCount(var2) : 0;
    for (int var3 = 0; var3 < tagCount; ++var3) {
        NBTTagCompound *var4 = (NBTTagCompound *)NBTTagList_tagAt(var2, var3);
        int var5 = (uint8_t)NBTTagCompound_getByte(var4, "Slot");
        if (var5 >= 0 && var5 < CHEST_SIZE) {
            self->chestContents[var5] = ItemStack_fromNBT(var4);
            self->chestContentsHas[var5] = 1;
        }
    }
}

static void chest_destroy(TileEntity *base) { free(base); }

TileEntityChest *TileEntityChest_create(void) {
    static TileEntityVtable vt;
    static int once = 0;
    if (!once) {
        TileEntity_initVtable(&vt);
        vt.typeName = "Chest";
        vt.writeToNBT = chest_writeToNBT;
        vt.readFromNBT = chest_readFromNBT;
        vt.destroy = chest_destroy;
        once = 1;
    }
    TileEntityChest *self = (TileEntityChest *)calloc(1, sizeof(TileEntityChest));
    TileEntity_init(&self->base);
    self->base.vtable = &vt;
    self->inventory.vtable = &chest_inv_vtable;
    return self;
}

int TileEntityChest_getSizeInventory(TileEntityChest *self) {
    (void)self;
    return CHEST_SIZE;
}

ItemStack *TileEntityChest_decrStackSize(TileEntityChest *self, int slot, int count) {
    if (slot < 0 || slot >= CHEST_CONTENTS_SIZE || !self->chestContentsHas[slot])
        return NULL;
    ItemStack *contents = &self->chestContents[slot];
    ItemStack *result;
    if (contents->stackSize <= count) {
        result = ItemStack_copy_ptr(contents);
        self->chestContentsHas[slot] = 0;
        inv_onInventoryChanged(&self->inventory);
        return result;
    } else {
        ItemStack split = ItemStack_splitStack(contents, count);
        result = ItemStack_copy_ptr(&split);
        if (contents->stackSize == 0) {
            self->chestContentsHas[slot] = 0;
        }
        inv_onInventoryChanged(&self->inventory);
        return result;
    }
}

ItemStack *TileEntityChest_getStackInSlot(TileEntityChest *self, int slot) {
    if (slot < 0 || slot >= CHEST_CONTENTS_SIZE || !self->chestContentsHas[slot])
        return NULL;
    return &self->chestContents[slot];
}

void TileEntityChest_setSlotContents(TileEntityChest *self, int slot, ItemStack *stack) {
    if (slot < 0 || slot >= CHEST_CONTENTS_SIZE)
        return;
    if (stack) {
        self->chestContents[slot] = *stack;
        self->chestContentsHas[slot] = 1;
        if (stack->stackSize > inv_getInventoryStackLimit(&self->inventory)) {
            stack->stackSize = inv_getInventoryStackLimit(&self->inventory);
            self->chestContents[slot].stackSize = stack->stackSize;
        }
    } else {
        self->chestContentsHas[slot] = 0;
    }
    inv_onInventoryChanged(&self->inventory);
}
