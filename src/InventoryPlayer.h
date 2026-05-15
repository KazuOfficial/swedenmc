#ifndef INVENTORY_PLAYER_H
#define INVENTORY_PLAYER_H

#include "IInventory.h"
#include "ItemStack.h"
#include "GameForward.h"
#include "NBTTagList.h"

struct Entity;
struct Block;

#define INVENTORY_MAIN_SIZE 36
#define INVENTORY_ARMOR_SIZE 4

typedef struct InventoryPlayer {
    IInventory base;

    ItemStack *mainInventory[INVENTORY_MAIN_SIZE];

    ItemStack *armorInventory[INVENTORY_ARMOR_SIZE];

    int currentItem;

    EntityPlayer *player;

    ItemStack *itemStack;

    int inventoryChanged;
} InventoryPlayer;

void InventoryPlayer_init(InventoryPlayer *self, EntityPlayer *player);
void InventoryPlayer_destroy(InventoryPlayer *self);

ItemStack *InventoryPlayer_getCurrentItem(InventoryPlayer *self);

void InventoryPlayer_setCurrentItem(InventoryPlayer *self, int var1, int var2);

void InventoryPlayer_changeCurrentItem(InventoryPlayer *self, int var1);

void InventoryPlayer_decrementAnimations(InventoryPlayer *self);

int InventoryPlayer_consumeInventoryItem(InventoryPlayer *self, int var1);

int InventoryPlayer_addItemStackToInventory(InventoryPlayer *self, ItemStack *var1);

float InventoryPlayer_getStrVsBlock(InventoryPlayer *self, struct Block *var1);

NBTTagList *InventoryPlayer_writeToNBT(InventoryPlayer *self, NBTTagList *var1);

void InventoryPlayer_readFromNBT(InventoryPlayer *self, NBTTagList *var1);

int InventoryPlayer_getDamageVsEntity(InventoryPlayer *self, struct Entity *var1);

int InventoryPlayer_canHarvestBlock(InventoryPlayer *self, struct Block *var1);

ItemStack *InventoryPlayer_armorItemInSlot(InventoryPlayer *self, int var1);

int InventoryPlayer_getTotalArmorValue(InventoryPlayer *self);

void InventoryPlayer_damageArmor(InventoryPlayer *self, int var1);

void InventoryPlayer_dropAllItems(InventoryPlayer *self);

void InventoryPlayer_setItemStack(InventoryPlayer *self, ItemStack *var1);

ItemStack *InventoryPlayer_getItemStack(InventoryPlayer *self);

int InventoryPlayer_func_28018_c(InventoryPlayer *self, ItemStack *var1);

int InventoryPlayer_getSizeInventory(InventoryPlayer *self);
ItemStack *InventoryPlayer_getStackInSlot(InventoryPlayer *self, int var1);
ItemStack *InventoryPlayer_decrStackSize(InventoryPlayer *self, int var1, int var2);
void InventoryPlayer_setInventorySlotContents(InventoryPlayer *self, int var1, ItemStack *var2);
const char *InventoryPlayer_getInvName(InventoryPlayer *self);
int InventoryPlayer_getInventoryStackLimit(InventoryPlayer *self);
void InventoryPlayer_onInventoryChanged(InventoryPlayer *self);
int InventoryPlayer_canInteractWith(InventoryPlayer *self, EntityPlayer *var1);

int InventoryPlayer_getFirstEmptyStack(InventoryPlayer *self);

#endif
