#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "EntityPlayer.h"
#include "Minecraft.h"
#include "ItemArmor.h"
#include "Item.h"
#include "Block.h"
#include "NBTTagList.h"
#include "NBTTagCompound.h"
#include "Entity.h"
#include <stdlib.h>
#include <string.h>

static int ii_getSizeInventory(IInventory *self) {
    return InventoryPlayer_getSizeInventory((InventoryPlayer *)self);
}
static ItemStack *ii_getStackInSlot(IInventory *self, int var1) {
    return InventoryPlayer_getStackInSlot((InventoryPlayer *)self, var1);
}
static ItemStack *ii_decrStackSize(IInventory *self, int var1, int var2) {
    return InventoryPlayer_decrStackSize((InventoryPlayer *)self, var1, var2);
}
static void ii_setInventorySlotContents(IInventory *self, int var1, ItemStack *var2) {
    InventoryPlayer_setInventorySlotContents((InventoryPlayer *)self, var1, var2);
}
static const char *ii_getInvName(IInventory *self) {
    return InventoryPlayer_getInvName((InventoryPlayer *)self);
}
static int ii_getInventoryStackLimit(IInventory *self) {
    return InventoryPlayer_getInventoryStackLimit((InventoryPlayer *)self);
}
static void ii_onInventoryChanged(IInventory *self) {
    InventoryPlayer_onInventoryChanged((InventoryPlayer *)self);
}
static int ii_canInteractWith(IInventory *self, EntityPlayer *var1) {
    return InventoryPlayer_canInteractWith((InventoryPlayer *)self, var1);
}

static const IInventoryVtable InventoryPlayer_vtable = {
    .getSizeInventory = ii_getSizeInventory,
    .getStackInSlot = ii_getStackInSlot,
    .decrStackSize = ii_decrStackSize,
    .setInventorySlotContents = ii_setInventorySlotContents,
    .getInvName = ii_getInvName,
    .getInventoryStackLimit = ii_getInventoryStackLimit,
    .onInventoryChanged = ii_onInventoryChanged,
    .canInteractWith = ii_canInteractWith,
};

void InventoryPlayer_init(InventoryPlayer *self, EntityPlayer *player) {
    memset(self, 0, sizeof(InventoryPlayer));
    self->base.vtable = &InventoryPlayer_vtable;
    self->player = player;
}

void InventoryPlayer_destroy(InventoryPlayer *self) {
    for (int i = 0; i < INVENTORY_MAIN_SIZE; i++)
        ItemStack_free(self->mainInventory[i]);
    for (int i = 0; i < INVENTORY_ARMOR_SIZE; i++)
        ItemStack_free(self->armorInventory[i]);
    ItemStack_free(self->itemStack);
}

static int getInventorySlotContainItem(InventoryPlayer *self, int var1) {
    for (int var2 = 0; var2 < INVENTORY_MAIN_SIZE; ++var2) {
        if (self->mainInventory[var2] != NULL && self->mainInventory[var2]->itemID == var1) {
            return var2;
        }
    }
    return -1;
}

static int storeItemStack(InventoryPlayer *self, ItemStack *var1) {
    for (int var2 = 0; var2 < INVENTORY_MAIN_SIZE; ++var2) {
        if (self->mainInventory[var2] != NULL &&
            self->mainInventory[var2]->itemID == var1->itemID &&
            ItemStack_isStackable(self->mainInventory[var2]) &&
            self->mainInventory[var2]->stackSize <
                ItemStack_getMaxStackSize(self->mainInventory[var2]) &&
            self->mainInventory[var2]->stackSize < InventoryPlayer_getInventoryStackLimit(self) &&
            (!Item_itemsList[self->mainInventory[var2]->itemID]->vtable->getHasSubtypes(
                 Item_itemsList[self->mainInventory[var2]->itemID]) ||
             ItemStack_getItemDamage(self->mainInventory[var2]) == ItemStack_getItemDamage(var1))) {
            return var2;
        }
    }
    return -1;
}

int InventoryPlayer_getFirstEmptyStack(InventoryPlayer *self) {
    for (int var1 = 0; var1 < INVENTORY_MAIN_SIZE; ++var1) {
        if (self->mainInventory[var1] == NULL) {
            return var1;
        }
    }
    return -1;
}

static int storePartialItemStack(InventoryPlayer *self, ItemStack *var1) {
    int var2 = var1->itemID;
    int var3 = var1->stackSize;
    int var4 = storeItemStack(self, var1);
    if (var4 < 0) {
        var4 = InventoryPlayer_getFirstEmptyStack(self);
    }
    if (var4 < 0) {
        return var3;
    } else {
        if (self->mainInventory[var4] == NULL) {

            self->mainInventory[var4] = ItemStack_new(var2, 0, ItemStack_getItemDamage(var1));
        }
        int var5 = var3;
        if (var3 > ItemStack_getMaxStackSize(self->mainInventory[var4]) -
                       self->mainInventory[var4]->stackSize) {
            var5 = ItemStack_getMaxStackSize(self->mainInventory[var4]) -
                   self->mainInventory[var4]->stackSize;
        }
        if (var5 >
            InventoryPlayer_getInventoryStackLimit(self) - self->mainInventory[var4]->stackSize) {
            var5 =
                InventoryPlayer_getInventoryStackLimit(self) - self->mainInventory[var4]->stackSize;
        }
        if (var5 == 0) {
            return var3;
        } else {
            var3 -= var5;
            self->mainInventory[var4]->stackSize += var5;
            self->mainInventory[var4]->animationsToGo = 5;
            return var3;
        }
    }
}

ItemStack *InventoryPlayer_getCurrentItem(InventoryPlayer *self) {
    return self->currentItem < 9 && self->currentItem >= 0 ? self->mainInventory[self->currentItem]
                                                           : NULL;
}

void InventoryPlayer_setCurrentItem(InventoryPlayer *self, int var1, int var2) {
    (void)var2;
    int var3 = getInventorySlotContainItem(self, var1);
    if (var3 >= 0 && var3 < 9) {
        self->currentItem = var3;
    }
}

void InventoryPlayer_changeCurrentItem(InventoryPlayer *self, int var1) {
    if (var1 > 0) {
        var1 = 1;
    }
    if (var1 < 0) {
        var1 = -1;
    }
    for (self->currentItem -= var1; self->currentItem < 0; self->currentItem += 9) {
    }
    while (self->currentItem >= 9) {
        self->currentItem -= 9;
    }
}

void InventoryPlayer_decrementAnimations(InventoryPlayer *self) {
    for (int var1 = 0; var1 < INVENTORY_MAIN_SIZE; ++var1) {
        if (self->mainInventory[var1] != NULL) {
            ItemStack_updateAnimation(self->mainInventory[var1], self->player->base.base.worldObj,
                                      (struct Entity *)self->player, var1,
                                      self->currentItem == var1);
        }
    }
}

int InventoryPlayer_consumeInventoryItem(InventoryPlayer *self, int var1) {
    int var2 = getInventorySlotContainItem(self, var1);
    if (var2 < 0) {
        return 0;
    } else {
        if (--self->mainInventory[var2]->stackSize <= 0) {
            Minecraft_invalidateItemToRender(self->mainInventory[var2]);
            ItemStack_free(self->mainInventory[var2]);
            self->mainInventory[var2] = NULL;
        }
        return 1;
    }
}

int InventoryPlayer_addItemStackToInventory(InventoryPlayer *self, ItemStack *var1) {
    int var2;
    if (ItemStack_isItemDamaged(var1)) {
        var2 = InventoryPlayer_getFirstEmptyStack(self);
        if (var2 >= 0) {

            ItemStack copy = ItemStack_copy(var1);
            self->mainInventory[var2] = (ItemStack *)calloc(1, sizeof(ItemStack));
            *self->mainInventory[var2] = copy;
            self->mainInventory[var2]->animationsToGo = 5;
            var1->stackSize = 0;
            return 1;
        } else {
            return 0;
        }
    } else {
        do {
            var2 = var1->stackSize;
            var1->stackSize = storePartialItemStack(self, var1);
        } while (var1->stackSize > 0 && var1->stackSize < var2);
        return var1->stackSize < var2;
    }
}

ItemStack *InventoryPlayer_decrStackSize(InventoryPlayer *self, int var1, int var2) {
    ItemStack **var3 = self->mainInventory;
    if (var1 >= INVENTORY_MAIN_SIZE) {
        var3 = self->armorInventory;
        var1 -= INVENTORY_MAIN_SIZE;
    }
    if (var3[var1] != NULL) {
        ItemStack *var4;
        if (var3[var1]->stackSize <= var2) {
            var4 = var3[var1];
            var3[var1] = NULL;
            return var4;
        } else {
            ItemStack split = ItemStack_splitStack(var3[var1], var2);
            var4 = (ItemStack *)calloc(1, sizeof(ItemStack));
            *var4 = split;
            if (var3[var1]->stackSize == 0) {
                var3[var1] = NULL;
            }
            return var4;
        }
    } else {
        return NULL;
    }
}

void InventoryPlayer_setInventorySlotContents(InventoryPlayer *self, int var1, ItemStack *var2) {
    ItemStack **var3 = self->mainInventory;
    if (var1 >= INVENTORY_MAIN_SIZE) {
        var1 -= INVENTORY_MAIN_SIZE;
        var3 = self->armorInventory;
    }
    if (var3[var1] != NULL && var3[var1] != var2) {
        Minecraft_invalidateItemToRender(var3[var1]);
        ItemStack_free(var3[var1]);
    }
    var3[var1] = var2;
}

float InventoryPlayer_getStrVsBlock(InventoryPlayer *self, struct Block *var1) {
    float var2 = 1.0F;
    if (self->mainInventory[self->currentItem] != NULL) {
        var2 *= ItemStack_getStrVsBlock(self->mainInventory[self->currentItem], var1);
    }
    return var2;
}

NBTTagList *InventoryPlayer_writeToNBT(InventoryPlayer *self, NBTTagList *var1) {
    int var2;
    NBTTagCompound *var3;
    for (var2 = 0; var2 < INVENTORY_MAIN_SIZE; ++var2) {
        if (self->mainInventory[var2] != NULL) {
            var3 = NBTTagCompound_create();
            NBTTagCompound_setByte(var3, "Slot", (int8_t)var2);
            ItemStack_writeToNBT(self->mainInventory[var2], var3);
            NBTTagList_setTag(var1, (NBTBase *)var3);
        }
    }
    for (var2 = 0; var2 < INVENTORY_ARMOR_SIZE; ++var2) {
        if (self->armorInventory[var2] != NULL) {
            var3 = NBTTagCompound_create();
            NBTTagCompound_setByte(var3, "Slot", (int8_t)(var2 + 100));
            ItemStack_writeToNBT(self->armorInventory[var2], var3);
            NBTTagList_setTag(var1, (NBTBase *)var3);
        }
    }
    return var1;
}

void InventoryPlayer_readFromNBT(InventoryPlayer *self, NBTTagList *var1) {
    for (int i = 0; i < INVENTORY_MAIN_SIZE; i++)
        ItemStack_free(self->mainInventory[i]);
    for (int i = 0; i < INVENTORY_ARMOR_SIZE; i++)
        ItemStack_free(self->armorInventory[i]);
    memset(self->mainInventory, 0, sizeof(self->mainInventory));
    memset(self->armorInventory, 0, sizeof(self->armorInventory));
    for (int var2 = 0; var2 < NBTTagList_tagCount(var1); ++var2) {
        NBTTagCompound *var3 = (NBTTagCompound *)NBTTagList_tagAt(var1, var2);
        int var4 = NBTTagCompound_getByte(var3, "Slot") & 255;

        ItemStack tmp = ItemStack_fromNBT(var3);
        ItemStack *var5 = (ItemStack *)calloc(1, sizeof(ItemStack));
        *var5 = tmp;

        if (var5->itemID > 0) {
            if (var4 >= 0 && var4 < INVENTORY_MAIN_SIZE) {
                self->mainInventory[var4] = var5;
            } else if (var4 >= 100 && var4 < INVENTORY_ARMOR_SIZE + 100) {
                self->armorInventory[var4 - 100] = var5;
            } else {
                free(var5);
            }
        } else {
            free(var5);
        }
    }
}

int InventoryPlayer_getSizeInventory(InventoryPlayer *self) {
    (void)self;

    return INVENTORY_MAIN_SIZE + 4;
}

ItemStack *InventoryPlayer_getStackInSlot(InventoryPlayer *self, int var1) {
    ItemStack **var2 = self->mainInventory;
    if (var1 >= INVENTORY_MAIN_SIZE) {
        var1 -= INVENTORY_MAIN_SIZE;
        var2 = self->armorInventory;
    }
    return var2[var1];
}

const char *InventoryPlayer_getInvName(InventoryPlayer *self) {
    (void)self;
    return "Inventory";
}

int InventoryPlayer_getInventoryStackLimit(InventoryPlayer *self) {
    (void)self;
    return 64;
}

int InventoryPlayer_getDamageVsEntity(InventoryPlayer *self, struct Entity *var1) {
    ItemStack *var2 = InventoryPlayer_getStackInSlot(self, self->currentItem);
    return var2 != NULL ? ItemStack_getDamageVsEntity(var2, var1) : 1;
}

int InventoryPlayer_canHarvestBlock(InventoryPlayer *self, struct Block *var1) {
    if (var1->blockMaterial != NULL && Material_getIsHarvestable(var1->blockMaterial)) {
        return 1;
    }
    ItemStack *var2 = InventoryPlayer_getStackInSlot(self, self->currentItem);
    return var2 != NULL ? ItemStack_canHarvestBlock(var2, var1) : 0;
}

ItemStack *InventoryPlayer_armorItemInSlot(InventoryPlayer *self, int var1) {
    return self->armorInventory[var1];
}

int InventoryPlayer_getTotalArmorValue(InventoryPlayer *self) {
    int var1 = 0;
    int var2 = 0;
    int var3 = 0;
    for (int var4 = 0; var4 < INVENTORY_ARMOR_SIZE; ++var4) {

        if (self->armorInventory[var4] != NULL &&
            ItemArmor_isInstance(Item_itemsList[self->armorInventory[var4]->itemID])) {
            int var5 = ItemStack_getMaxDamage(self->armorInventory[var4]);
            int var6 = ItemStack_getItemDamageForDisplay(self->armorInventory[var4]);
            int var7 = var5 - var6;
            var2 += var7;
            var3 += var5;
            int var8 = ((ItemArmor *)Item_itemsList[self->armorInventory[var4]->itemID])
                           ->damageReduceAmount;
            var1 += var8;
        }
    }
    if (var3 == 0) {
        return 0;
    } else {
        return (var1 - 1) * var2 / var3 + 1;
    }
}

void InventoryPlayer_damageArmor(InventoryPlayer *self, int var1) {
    for (int var2 = 0; var2 < INVENTORY_ARMOR_SIZE; ++var2) {

        if (self->armorInventory[var2] != NULL &&
            ItemArmor_isInstance(Item_itemsList[self->armorInventory[var2]->itemID])) {
            ItemStack_damageItem(self->armorInventory[var2], var1, (struct Entity *)self->player);
            if (self->armorInventory[var2]->stackSize == 0) {
                ItemStack_func_1097_a(self->armorInventory[var2], self->player);
                self->armorInventory[var2] = NULL;
            }
        }
    }
}

void InventoryPlayer_dropAllItems(InventoryPlayer *self) {
    int var1;
    for (var1 = 0; var1 < INVENTORY_MAIN_SIZE; ++var1) {
        if (self->mainInventory[var1] != NULL) {
            EntityPlayer_dropPlayerItemWithRandomChoice(self->player, self->mainInventory[var1], 1);
            ItemStack_free(self->mainInventory[var1]);
            self->mainInventory[var1] = NULL;
        }
    }
    for (var1 = 0; var1 < INVENTORY_ARMOR_SIZE; ++var1) {
        if (self->armorInventory[var1] != NULL) {
            EntityPlayer_dropPlayerItemWithRandomChoice(self->player, self->armorInventory[var1],
                                                        1);
            ItemStack_free(self->armorInventory[var1]);
            self->armorInventory[var1] = NULL;
        }
    }
}

void InventoryPlayer_onInventoryChanged(InventoryPlayer *self) { self->inventoryChanged = 1; }

void InventoryPlayer_setItemStack(InventoryPlayer *self, ItemStack *var1) {
    self->itemStack = var1;
    EntityPlayer_onItemStackChanged(self->player, var1);
}

ItemStack *InventoryPlayer_getItemStack(InventoryPlayer *self) { return self->itemStack; }

int InventoryPlayer_canInteractWith(InventoryPlayer *self, EntityPlayer *var1) {

    if (self->player->base.base.isDead) {
        return 0;
    }
    return Entity_getDistanceSqToEntity((Entity *)var1, (Entity *)self->player) <= 64.0;
}

int InventoryPlayer_func_28018_c(InventoryPlayer *self, ItemStack *var1) {
    int var2;
    for (var2 = 0; var2 < INVENTORY_ARMOR_SIZE; ++var2) {
        if (self->armorInventory[var2] != NULL &&
            ItemStack_isStackEqual(self->armorInventory[var2], var1)) {
            return 1;
        }
    }
    for (var2 = 0; var2 < INVENTORY_MAIN_SIZE; ++var2) {
        if (self->mainInventory[var2] != NULL &&
            ItemStack_isStackEqual(self->mainInventory[var2], var1)) {
            return 1;
        }
    }
    return 0;
}
