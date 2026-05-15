#ifndef ITEM_STACK_H
#define ITEM_STACK_H

#include "NBTTagCompound.h"
#include "GameForward.h"

typedef struct ItemStack {
    int stackSize;
    int animationsToGo;
    int itemID;
    int itemDamage;
} ItemStack;

ItemStack ItemStack_fromBlockID(int blockID, int count, int damage);
ItemStack ItemStack_fromItemID(int itemID, int count, int damage);
ItemStack ItemStack_fromNBT(NBTTagCompound *tag);

ItemStack ItemStack_splitStack(ItemStack *self, int amount);
int ItemStack_getIconIndex(ItemStack *self);
int ItemStack_getMaxStackSize(ItemStack *self);
int ItemStack_isStackable(ItemStack *self);
int ItemStack_isItemStackDamageable(ItemStack *self);
int ItemStack_isItemDamaged(ItemStack *self);
int ItemStack_getItemDamage(ItemStack *self);

int ItemStack_getItemDamageForDisplay(ItemStack *self);
void ItemStack_setItemDamage(ItemStack *self, int damage);

void ItemStack_func_1097_a(ItemStack *self, struct EntityPlayer *var1);

int ItemStack_isStackEqual(ItemStack *self, ItemStack *var1);
int ItemStack_getMaxDamage(ItemStack *self);
void ItemStack_damageItem(ItemStack *self, int amount, struct Entity *entity);
int ItemStack_getDamageVsEntity(ItemStack *self, struct Entity *entity);
int ItemStack_canHarvestBlock(ItemStack *self, struct Block *block);
float ItemStack_getStrVsBlock(ItemStack *self, struct Block *block);
int ItemStack_useItem(ItemStack *self, EntityPlayer *player, World *world, int x, int y, int z,
                      int side);
ItemStack *ItemStack_useItemRightClick(ItemStack *self, World *world, EntityPlayer *player);
void ItemStack_hitEntity(ItemStack *self, EntityLiving *target, EntityPlayer *attacker);

void ItemStack_useItemOnEntity(ItemStack *self, EntityLiving *var1);
void ItemStack_onDestroyBlock(ItemStack *self, int blockID, int x, int y, int z,
                              EntityPlayer *player);
void ItemStack_updateAnimation(ItemStack *self, World *world, struct Entity *entity, int slot,
                               int held);
void ItemStack_onCrafting(ItemStack *self, World *world, EntityPlayer *player);
ItemStack ItemStack_copy(ItemStack *self);

ItemStack *ItemStack_copy_ptr(ItemStack *self);

int ItemStack_areItemStacksEqual(ItemStack *var0, ItemStack *var1);

int ItemStack_getHasSubtypes(ItemStack *self);
int ItemStack_isItemEqual(ItemStack *self, ItemStack *other);
const char *ItemStack_getItemName(ItemStack *self);
NBTTagCompound *ItemStack_writeToNBT(ItemStack *self, NBTTagCompound *tag);
void ItemStack_readFromNBT(ItemStack *self, NBTTagCompound *tag);

ItemStack *ItemStack_new(int itemID, int count, int damage);
void ItemStack_free(ItemStack *self);

ItemStack *ItemStack_copyItemStack(ItemStack *var0);

#endif
