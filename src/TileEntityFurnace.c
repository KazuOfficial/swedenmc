#include "TileEntityFurnace.h"
#include "IInventory.h"
#include "Item.h"
#include "Block.h"
#include "Material.h"
#include "World.h"
#include "EntityPlayer.h"
#include "Entity.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "ItemStack.h"
#include "FurnaceRecipes.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

extern void BlockFurnace_updateFurnaceBlockState(int var0, World *var1, int var2, int var3,
                                                 int var4);

int TileEntityFurnace_isBurning(TileEntityFurnace *self) { return self->furnaceBurnTime > 0; }

int TileEntityFurnace_getCookProgressScaled(TileEntityFurnace *self, int var1) {
    return self->furnaceCookTime * var1 / 200;
}

int TileEntityFurnace_getBurnTimeRemainingScaled(TileEntityFurnace *self, int var1) {
    if (self->currentItemBurnTime == 0) {
        self->currentItemBurnTime = 200;
    }
    return self->furnaceBurnTime * var1 / self->currentItemBurnTime;
}

static int furnace_getItemBurnTime(ItemStack *var1) {
    if (var1 == NULL)
        return 0;
    int var2 = var1->itemID;
    if (var2 < 256 && Block_blocksList[var2] != NULL &&
        Block_blocksList[var2]->blockMaterial == Material_wood)
        return 300;
    if (var2 == Item_stick->shiftedIndex)
        return 100;
    if (var2 == Item_coal->shiftedIndex)
        return 1600;
    if (var2 == Item_bucketLava->shiftedIndex)
        return 20000;
    if (var2 == Block_sapling->blockID)
        return 100;
    return 0;
}

static int furnace_canSmelt(TileEntityFurnace *self) {
    if (!self->furnaceItemStacksHas[FURNACE_SLOT_INPUT])
        return 0;
    ItemStack *var1 = FurnaceRecipes_getSmeltingResult(
        FurnaceRecipes_smelting(), self->furnaceItemStacks[FURNACE_SLOT_INPUT].itemID);
    if (var1 == NULL)
        return 0;
    if (!self->furnaceItemStacksHas[FURNACE_SLOT_OUTPUT])
        return 1;
    if (!ItemStack_isItemEqual(&self->furnaceItemStacks[FURNACE_SLOT_OUTPUT], var1))
        return 0;
    int outSize = self->furnaceItemStacks[FURNACE_SLOT_OUTPUT].stackSize;
    if (outSize < 64 &&
        outSize < ItemStack_getMaxStackSize(&self->furnaceItemStacks[FURNACE_SLOT_OUTPUT]))
        return 1;
    return outSize < ItemStack_getMaxStackSize(var1);
}

static void furnace_smeltItem(TileEntityFurnace *self) {
    if (!furnace_canSmelt(self))
        return;
    ItemStack *var1 = FurnaceRecipes_getSmeltingResult(
        FurnaceRecipes_smelting(), self->furnaceItemStacks[FURNACE_SLOT_INPUT].itemID);
    if (!self->furnaceItemStacksHas[FURNACE_SLOT_OUTPUT]) {
        self->furnaceItemStacks[FURNACE_SLOT_OUTPUT] = ItemStack_copy(var1);
        self->furnaceItemStacksHas[FURNACE_SLOT_OUTPUT] = 1;
    } else if (self->furnaceItemStacks[FURNACE_SLOT_OUTPUT].itemID == var1->itemID) {
        ++self->furnaceItemStacks[FURNACE_SLOT_OUTPUT].stackSize;
    }
    --self->furnaceItemStacks[FURNACE_SLOT_INPUT].stackSize;
    if (self->furnaceItemStacks[FURNACE_SLOT_INPUT].stackSize <= 0) {
        self->furnaceItemStacksHas[FURNACE_SLOT_INPUT] = 0;
    }
}

static void furnace_updateEntity(TileEntity *base) {
    TileEntityFurnace *self = (TileEntityFurnace *)base;
    int var1 = self->furnaceBurnTime > 0;
    int var2 = 0;
    if (self->furnaceBurnTime > 0) {
        --self->furnaceBurnTime;
    }
    if (!base->worldObj->multiplayerWorld) {
        if (self->furnaceBurnTime == 0 && furnace_canSmelt(self)) {
            self->currentItemBurnTime = self->furnaceBurnTime =
                furnace_getItemBurnTime(self->furnaceItemStacksHas[FURNACE_SLOT_FUEL]
                                            ? &self->furnaceItemStacks[FURNACE_SLOT_FUEL]
                                            : NULL);
            if (self->furnaceBurnTime > 0) {
                var2 = 1;
                if (self->furnaceItemStacksHas[FURNACE_SLOT_FUEL]) {
                    --self->furnaceItemStacks[FURNACE_SLOT_FUEL].stackSize;
                    if (self->furnaceItemStacks[FURNACE_SLOT_FUEL].stackSize == 0) {
                        self->furnaceItemStacksHas[FURNACE_SLOT_FUEL] = 0;
                    }
                }
            }
        }
        if (TileEntityFurnace_isBurning(self) && furnace_canSmelt(self)) {
            ++self->furnaceCookTime;
            if (self->furnaceCookTime == 200) {
                self->furnaceCookTime = 0;
                furnace_smeltItem(self);
                var2 = 1;
            }
        } else {
            self->furnaceCookTime = 0;
        }
        if (var1 != (self->furnaceBurnTime > 0)) {
            var2 = 1;
            BlockFurnace_updateFurnaceBlockState(self->furnaceBurnTime > 0, base->worldObj,
                                                 base->xCoord, base->yCoord, base->zCoord);
        }
    }
    if (var2) {
        TileEntity_onInventoryChanged(base);
    }
}

static int furnace_needsUpdate(TileEntity *self) {
    (void)self;
    return 1;
}

static void furnace_writeToNBT(TileEntity *base, NBTTagCompound *tag) {
    TileEntityFurnace *self = (TileEntityFurnace *)base;
    TileEntity_defaultVtable.writeToNBT(base, tag);
    NBTTagCompound_setShort(tag, "BurnTime", (int16_t)self->furnaceBurnTime);
    NBTTagCompound_setShort(tag, "CookTime", (int16_t)self->furnaceCookTime);
    NBTTagList *var2 = NBTTagList_create();
    for (int var3 = 0; var3 < FURNACE_SIZE; ++var3) {
        if (self->furnaceItemStacksHas[var3]) {
            NBTTagCompound *var4 = NBTTagCompound_create();
            NBTTagCompound_setByte(var4, "Slot", (int8_t)var3);
            ItemStack_writeToNBT(&self->furnaceItemStacks[var3], var4);
            NBTTagList_setTag(var2, (NBTBase *)var4);
        }
    }
    NBTTagCompound_setTag(tag, "Items", (NBTBase *)var2);
}

static void furnace_readFromNBT(TileEntity *base, NBTTagCompound *tag) {
    TileEntityFurnace *self = (TileEntityFurnace *)base;
    TileEntity_defaultVtable.readFromNBT(base, tag);
    NBTTagList *var2 = NBTTagCompound_getTagList(tag, "Items");
    memset(self->furnaceItemStacksHas, 0, sizeof(self->furnaceItemStacksHas));
    int count = var2 ? NBTTagList_tagCount(var2) : 0;
    for (int var3 = 0; var3 < count; ++var3) {
        NBTTagCompound *var4 = (NBTTagCompound *)NBTTagList_tagAt(var2, var3);
        int var5 = (uint8_t)NBTTagCompound_getByte(var4, "Slot");
        if (var5 < FURNACE_SIZE) {
            self->furnaceItemStacks[var5] = ItemStack_fromNBT(var4);
            self->furnaceItemStacksHas[var5] = 1;
        }
    }
    self->furnaceBurnTime = NBTTagCompound_getShort(tag, "BurnTime");
    self->furnaceCookTime = NBTTagCompound_getShort(tag, "CookTime");
    self->currentItemBurnTime =
        self->furnaceItemStacksHas[FURNACE_SLOT_FUEL]
            ? furnace_getItemBurnTime(&self->furnaceItemStacks[FURNACE_SLOT_FUEL])
            : 0;
}
static void furnace_destroy(TileEntity *base) { free(base); }

static TileEntityFurnace *furnace_from_iinv(IInventory *inv) {
    return (TileEntityFurnace *)((char *)inv - offsetof(TileEntityFurnace, iinv));
}
static int iinv_furnace_getSizeInventory(IInventory *inv) {
    (void)inv;
    return FURNACE_SIZE;
}

static ItemStack *iinv_furnace_getStackInSlot(IInventory *inv, int var1) {
    TileEntityFurnace *self = furnace_from_iinv(inv);
    return self->furnaceItemStacksHas[var1] ? &self->furnaceItemStacks[var1] : NULL;
}

static ItemStack *iinv_furnace_decrStackSize(IInventory *inv, int var1, int var2) {
    TileEntityFurnace *self = furnace_from_iinv(inv);
    if (!self->furnaceItemStacksHas[var1])
        return NULL;
    ItemStack *var3;
    if (self->furnaceItemStacks[var1].stackSize <= var2) {
        var3 = ItemStack_copy_ptr(&self->furnaceItemStacks[var1]);
        self->furnaceItemStacksHas[var1] = 0;
        return var3;
    } else {
        ItemStack split = ItemStack_splitStack(&self->furnaceItemStacks[var1], var2);
        var3 = ItemStack_copy_ptr(&split);
        if (self->furnaceItemStacks[var1].stackSize == 0) {
            self->furnaceItemStacksHas[var1] = 0;
        }
        return var3;
    }
}

static void iinv_furnace_setInventorySlotContents(IInventory *inv, int var1, ItemStack *var2) {
    TileEntityFurnace *self = furnace_from_iinv(inv);
    if (var2) {
        self->furnaceItemStacks[var1] = *var2;
        self->furnaceItemStacksHas[var1] = 1;
        if (self->furnaceItemStacks[var1].stackSize > 64) {
            self->furnaceItemStacks[var1].stackSize = 64;
        }
    } else {
        self->furnaceItemStacksHas[var1] = 0;
    }
}

static const char *iinv_furnace_getInvName(IInventory *inv) {
    (void)inv;
    return "Furnace";
}

static int iinv_furnace_getInventoryStackLimit(IInventory *inv) {
    (void)inv;
    return 64;
}

static void iinv_furnace_onInventoryChanged(IInventory *inv) {
    TileEntity_onInventoryChanged(&furnace_from_iinv(inv)->base);
}

static int iinv_furnace_canInteractWith(IInventory *inv, EntityPlayer *var1) {
    return TileEntityFurnace_canInteractWith(furnace_from_iinv(inv), var1);
}

static const IInventoryVtable TileEntityFurnace_iinv_vtable = {
    iinv_furnace_getSizeInventory,   iinv_furnace_getStackInSlot,
    iinv_furnace_decrStackSize,      iinv_furnace_setInventorySlotContents,
    iinv_furnace_getInvName,         iinv_furnace_getInventoryStackLimit,
    iinv_furnace_onInventoryChanged, iinv_furnace_canInteractWith,
};

TileEntityFurnace *TileEntityFurnace_create(void) {
    static TileEntityVtable vt;
    static int once = 0;
    if (!once) {
        TileEntity_initVtable(&vt);
        vt.typeName = "Furnace";
        vt.updateEntity = furnace_updateEntity;
        vt.needsUpdate = furnace_needsUpdate;
        vt.writeToNBT = furnace_writeToNBT;
        vt.readFromNBT = furnace_readFromNBT;
        vt.destroy = furnace_destroy;
        once = 1;
    }
    TileEntityFurnace *self = (TileEntityFurnace *)calloc(1, sizeof(TileEntityFurnace));
    TileEntity_init(&self->base);
    self->base.vtable = &vt;
    self->iinv.vtable = &TileEntityFurnace_iinv_vtable;
    return self;
}

ItemStack *TileEntityFurnace_getStackInSlot(TileEntityFurnace *self, int slot) {
    return self->furnaceItemStacksHas[slot] ? &self->furnaceItemStacks[slot] : NULL;
}

void TileEntityFurnace_setSlotContents(TileEntityFurnace *self, int slot, ItemStack *stack) {
    if (slot < 0 || slot >= FURNACE_SIZE)
        return;
    if (stack) {
        self->furnaceItemStacks[slot] = *stack;
        self->furnaceItemStacksHas[slot] = 1;
    } else {
        self->furnaceItemStacksHas[slot] = 0;
    }
}

int TileEntityFurnace_canInteractWith(TileEntityFurnace *self, struct EntityPlayer *var1) {
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
