#include "ItemStack.h"
#include "Item.h"
#include "Block.h"
#include "NBTTagCompound.h"
#include "EntityPlayer.h"
#include "StatList.h"
#include <stdlib.h>
#include <string.h>

ItemStack ItemStack_fromBlockID(int blockID, int count, int damage) {
    ItemStack s;
    s.stackSize = count;
    s.animationsToGo = 0;
    s.itemID = blockID;
    s.itemDamage = damage;
    return s;
}
ItemStack ItemStack_fromItemID(int itemID, int count, int damage) {
    ItemStack s;
    s.stackSize = count;
    s.animationsToGo = 0;
    s.itemID = itemID;
    s.itemDamage = damage;
    return s;
}
ItemStack ItemStack_fromNBT(NBTTagCompound *tag) {
    ItemStack s;
    memset(&s, 0, sizeof(s));
    ItemStack_readFromNBT(&s, tag);
    return s;
}

ItemStack *ItemStack_new(int itemID, int count, int damage) {
    ItemStack *s = (ItemStack *)calloc(1, sizeof(ItemStack));
    s->stackSize = count;
    s->animationsToGo = 0;
    s->itemID = itemID;
    s->itemDamage = damage;
    return s;
}

void ItemStack_free(ItemStack *self) { free(self); }

ItemStack ItemStack_splitStack(ItemStack *self, int amount) {
    self->stackSize -= amount;
    return ItemStack_fromItemID(self->itemID, amount, self->itemDamage);
}

static Item *getItem(ItemStack *self) { return Item_itemsList[self->itemID]; }

int ItemStack_getIconIndex(ItemStack *self) {
    return getItem(self)->vtable->getIconFromDamage(getItem(self), self->itemDamage);
}
int ItemStack_getMaxStackSize(ItemStack *self) {
    return getItem(self)->vtable->getItemStackLimit(getItem(self));
}
int ItemStack_isItemStackDamageable(ItemStack *s) { return getItem(s)->maxDamage > 0; }
int ItemStack_isItemDamaged(ItemStack *self) {
    return ItemStack_isItemStackDamageable(self) && self->itemDamage > 0;
}
int ItemStack_isStackable(ItemStack *self) {
    return ItemStack_getMaxStackSize(self) > 1 &&
           (!ItemStack_isItemStackDamageable(self) || !ItemStack_isItemDamaged(self));
}
int ItemStack_getItemDamage(ItemStack *self) { return self->itemDamage; }

int ItemStack_getItemDamageForDisplay(ItemStack *self) { return self->itemDamage; }
void ItemStack_setItemDamage(ItemStack *self, int d) { self->itemDamage = d; }

void ItemStack_func_1097_a(ItemStack *self, struct EntityPlayer *var1) {
    (void)self;
    (void)var1;
}

int ItemStack_isStackEqual(ItemStack *self, ItemStack *var1) {
    return self->itemID == var1->itemID && self->stackSize == var1->stackSize &&
           self->itemDamage == var1->itemDamage;
}
int ItemStack_getMaxDamage(ItemStack *self) { return getItem(self)->maxDamage; }

void ItemStack_damageItem(ItemStack *self, int amount, struct Entity *entity) {
    if (!ItemStack_isItemStackDamageable(self))
        return;
    self->itemDamage += amount;
    if (self->itemDamage > ItemStack_getMaxDamage(self)) {
        if (entity && entity->isPlayer) {
            EntityPlayer_addStat((EntityPlayer *)entity, StatList_field_25170_B[self->itemID], 1);
        }
        --self->stackSize;
        if (self->stackSize < 0)
            self->stackSize = 0;
        self->itemDamage = 0;
    }
}

int ItemStack_getDamageVsEntity(ItemStack *self, struct Entity *e) {
    return getItem(self)->vtable->getDamageVsEntity(getItem(self), e);
}
int ItemStack_canHarvestBlock(ItemStack *self, struct Block *b) {
    return getItem(self)->vtable->canHarvestBlock(getItem(self), b);
}
float ItemStack_getStrVsBlock(ItemStack *self, struct Block *b) {
    return getItem(self)->vtable->getStrVsBlock(getItem(self), self, b);
}

int ItemStack_useItem(ItemStack *self, EntityPlayer *player, World *world, int x, int y, int z,
                      int side) {
    int var7 = getItem(self)->vtable->onItemUse(getItem(self), self, player, world, x, y, z, side);
    if (var7)
        EntityPlayer_addStat(player, StatList_field_25172_A[self->itemID], 1);
    return var7;
}
ItemStack *ItemStack_useItemRightClick(ItemStack *self, World *world, EntityPlayer *player) {
    return getItem(self)->vtable->onItemRightClick(getItem(self), self, world, player);
}

void ItemStack_hitEntity(ItemStack *self, EntityLiving *target, EntityPlayer *attacker) {
    int var3 =
        getItem(self)->vtable->hitEntity(getItem(self), self, target, (EntityLiving *)attacker);
    if (var3) {
        EntityPlayer_addStat(attacker, StatList_field_25172_A[self->itemID], 1);
    }
}

void ItemStack_useItemOnEntity(ItemStack *self, EntityLiving *var1) {
    getItem(self)->vtable->saddleEntity(getItem(self), self, var1);
}
void ItemStack_onDestroyBlock(ItemStack *self, int blockID, int x, int y, int z,
                              EntityPlayer *player) {
    int var6 = getItem(self)->vtable->onBlockDestroyed(getItem(self), self, blockID, x, y, z,
                                                       (EntityLiving *)player);
    if (var6)
        EntityPlayer_addStat(player, StatList_field_25172_A[self->itemID], 1);
}
void ItemStack_updateAnimation(ItemStack *self, World *world, struct Entity *entity, int slot,
                               int held) {
    if (self->animationsToGo > 0)
        --self->animationsToGo;
    Item *item = getItem(self);
    item->vtable->onUpdate(item, self, world, entity, slot, held);
}
void ItemStack_onCrafting(ItemStack *self, World *world, EntityPlayer *player) {
    EntityPlayer_addStat(player, StatList_field_25158_z[self->itemID], self->stackSize);
    getItem(self)->vtable->onCreated(getItem(self), self, world, player);
}

ItemStack ItemStack_copy(ItemStack *self) {
    return ItemStack_fromItemID(self->itemID, self->stackSize, self->itemDamage);
}

ItemStack *ItemStack_copy_ptr(ItemStack *self) {
    return ItemStack_new(self->itemID, self->stackSize, self->itemDamage);
}
ItemStack *ItemStack_copyItemStack(ItemStack *var0) {
    return var0 == NULL ? NULL : ItemStack_copy_ptr(var0);
}

int ItemStack_areItemStacksEqual(ItemStack *var0, ItemStack *var1) {
    if (var0 == NULL && var1 == NULL)
        return 1;
    if (var0 != NULL && var1 != NULL)
        return var0->stackSize == var1->stackSize && var0->itemID == var1->itemID &&
               var0->itemDamage == var1->itemDamage;
    return 0;
}

int ItemStack_getHasSubtypes(ItemStack *self) {
    return Item_itemsList[self->itemID]->vtable->getHasSubtypes(Item_itemsList[self->itemID]);
}
int ItemStack_isItemEqual(ItemStack *self, ItemStack *other) {
    return self->itemID == other->itemID && self->itemDamage == other->itemDamage;
}
const char *ItemStack_getItemName(ItemStack *self) {
    return getItem(self)->vtable->getItemNameIS(getItem(self), self);
}

NBTTagCompound *ItemStack_writeToNBT(ItemStack *self, NBTTagCompound *tag) {
    NBTTagCompound_setShort(tag, "id", (int16_t)self->itemID);
    NBTTagCompound_setByte(tag, "Count", (int8_t)self->stackSize);
    NBTTagCompound_setShort(tag, "Damage", (int16_t)self->itemDamage);
    return tag;
}

void ItemStack_readFromNBT(ItemStack *self, NBTTagCompound *tag) {
    self->itemID = NBTTagCompound_getShort(tag, "id");
    self->stackSize = NBTTagCompound_getByte(tag, "Count");
    self->itemDamage = NBTTagCompound_getShort(tag, "Damage");
}
