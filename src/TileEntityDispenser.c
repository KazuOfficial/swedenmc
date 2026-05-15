#include "TileEntityDispenser.h"
#include "IInventory.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NBTBase.h"
#include "World.h"
#include "EntityPlayer.h"
#include "Entity.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

static void disp_writeToNBT(TileEntity *base, NBTTagCompound *var1) {
    TileEntityDispenser *self = (TileEntityDispenser *)base;
    TileEntity_defaultVtable.writeToNBT(base, var1);
    NBTTagList *var2 = NBTTagList_create();
    for (int var3 = 0; var3 < DISPENSER_SIZE; ++var3) {
        if (self->dispenserItemStacksHas[var3]) {
            NBTTagCompound *var4 = NBTTagCompound_create();
            NBTTagCompound_setByte(var4, "Slot", (int8_t)var3);
            ItemStack_writeToNBT(&self->dispenserItemStacks[var3], var4);
            NBTTagList_setTag(var2, (NBTBase *)var4);
        }
    }
    NBTTagCompound_setTag(var1, "Items", (NBTBase *)var2);
}

static void disp_readFromNBT(TileEntity *base, NBTTagCompound *var1) {
    TileEntityDispenser *self = (TileEntityDispenser *)base;
    TileEntity_defaultVtable.readFromNBT(base, var1);
    NBTTagList *var2 = NBTTagCompound_getTagList(var1, "Items");
    memset(self->dispenserItemStacksHas, 0, sizeof(self->dispenserItemStacksHas));
    int tagCount = var2 ? NBTTagList_tagCount(var2) : 0;
    for (int var3 = 0; var3 < tagCount; ++var3) {
        NBTTagCompound *var4 = (NBTTagCompound *)NBTTagList_tagAt(var2, var3);
        int var5 = (uint8_t)NBTTagCompound_getByte(var4, "Slot");
        if (var5 >= 0 && var5 < DISPENSER_SIZE) {
            self->dispenserItemStacks[var5] = ItemStack_fromNBT(var4);
            self->dispenserItemStacksHas[var5] = 1;
        }
    }
}

static void disp_destroy(TileEntity *base) { free(base); }

static TileEntityDispenser *dispenser_from_iinv(IInventory *inv) {
    return (TileEntityDispenser *)((char *)inv - offsetof(TileEntityDispenser, iinv));
}

static int iinv_disp_getSizeInventory(IInventory *inv) {
    (void)inv;
    return 9;
}

static ItemStack *iinv_disp_getStackInSlot(IInventory *inv, int var1) {
    TileEntityDispenser *self = dispenser_from_iinv(inv);
    return self->dispenserItemStacksHas[var1] ? &self->dispenserItemStacks[var1] : NULL;
}

static ItemStack *iinv_disp_decrStackSize(IInventory *inv, int var1, int var2) {
    return TileEntityDispenser_decrStackSize(dispenser_from_iinv(inv), var1, var2);
}

static void iinv_disp_setInventorySlotContents(IInventory *inv, int var1, ItemStack *var2) {
    TileEntityDispenser *self = dispenser_from_iinv(inv);
    if (var2) {
        self->dispenserItemStacks[var1] = *var2;
        self->dispenserItemStacksHas[var1] = 1;
        if (self->dispenserItemStacks[var1].stackSize > 64) {
            self->dispenserItemStacks[var1].stackSize = 64;
        }
    } else {
        self->dispenserItemStacksHas[var1] = 0;
    }
    TileEntity_onInventoryChanged(&self->base);
}

static const char *iinv_disp_getInvName(IInventory *inv) {
    (void)inv;
    return "Trap";
}

static int iinv_disp_getInventoryStackLimit(IInventory *inv) {
    (void)inv;
    return 64;
}

static void iinv_disp_onInventoryChanged(IInventory *inv) {
    TileEntity_onInventoryChanged(&dispenser_from_iinv(inv)->base);
}

static int iinv_disp_canInteractWith(IInventory *inv, EntityPlayer *var1) {
    return TileEntityDispenser_canInteractWith(dispenser_from_iinv(inv), var1);
}

static const IInventoryVtable TileEntityDispenser_iinv_vtable = {
    iinv_disp_getSizeInventory,         iinv_disp_getStackInSlot,  iinv_disp_decrStackSize,
    iinv_disp_setInventorySlotContents, iinv_disp_getInvName,      iinv_disp_getInventoryStackLimit,
    iinv_disp_onInventoryChanged,       iinv_disp_canInteractWith,
};

TileEntityDispenser *TileEntityDispenser_create(void) {
    static TileEntityVtable vt;
    static int once = 0;
    if (!once) {
        TileEntity_initVtable(&vt);
        vt.typeName = "Trap";
        vt.writeToNBT = disp_writeToNBT;
        vt.readFromNBT = disp_readFromNBT;
        vt.destroy = disp_destroy;
        once = 1;
    }
    TileEntityDispenser *self = (TileEntityDispenser *)calloc(1, sizeof(TileEntityDispenser));
    TileEntity_init(&self->base);
    self->base.vtable = &vt;
    self->iinv.vtable = &TileEntityDispenser_iinv_vtable;

    JavaRandom_init(&self->dispenserRandom, (int64_t)time(NULL));
    return self;
}

ItemStack *TileEntityDispenser_getStackInSlot(TileEntityDispenser *self, int slot) {
    if (slot < 0 || slot >= DISPENSER_SIZE || !self->dispenserItemStacksHas[slot])
        return NULL;
    return &self->dispenserItemStacks[slot];
}

ItemStack *TileEntityDispenser_decrStackSize(TileEntityDispenser *self, int var1, int var2) {
    if (!self->dispenserItemStacksHas[var1]) {
        return NULL;
    }
    ItemStack *result = (ItemStack *)calloc(1, sizeof(ItemStack));
    if (self->dispenserItemStacks[var1].stackSize <= var2) {
        *result = self->dispenserItemStacks[var1];
        self->dispenserItemStacksHas[var1] = 0;
        TileEntity_onInventoryChanged(&self->base);
        return result;
    } else {
        *result = ItemStack_splitStack(&self->dispenserItemStacks[var1], var2);
        if (self->dispenserItemStacks[var1].stackSize == 0) {
            self->dispenserItemStacksHas[var1] = 0;
        }
        TileEntity_onInventoryChanged(&self->base);
        return result;
    }
}

ItemStack *TileEntityDispenser_getRandomStackFromInventory(TileEntityDispenser *self) {
    int var1 = -1;
    int var2 = 1;
    for (int var3 = 0; var3 < DISPENSER_SIZE; ++var3) {
        if (self->dispenserItemStacksHas[var3] &&
            JavaRandom_nextInt(&self->dispenserRandom, var2++) == 0) {
            var1 = var3;
        }
    }
    if (var1 >= 0) {
        return TileEntityDispenser_decrStackSize(self, var1, 1);
    } else {
        return NULL;
    }
}

int TileEntityDispenser_canInteractWith(TileEntityDispenser *self, struct EntityPlayer *var1) {
    TileEntity *base = &self->base;
    if (World_getBlockTileEntity(base->worldObj, base->xCoord, base->yCoord, base->zCoord) !=
        (TileEntity *)self) {
        return 0;
    }
    return ((Entity *)var1)
               ->vtable->getDistanceSq((Entity *)var1, (double)base->xCoord + 0.5,
                                       (double)base->yCoord + 0.5,
                                       (double)base->zCoord + 0.5) <= 64.0;
}
