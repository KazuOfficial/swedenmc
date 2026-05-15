#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "EnumToolMaterial.h"
#include <stdlib.h>

typedef struct {
    Item base;
    int weaponDamage;
} ItemSword;

static float ItemSword_getStrVsBlock(Item *self, ItemStack *stack, struct Block *block) {
    (void)self;
    (void)stack;
    return block->blockID == Block_web->blockID ? 15.0f : 1.5f;
}

static int ItemSword_hitEntity(Item *self, ItemStack *stack, EntityLiving *target,
                               EntityLiving *attacker) {
    (void)self;
    (void)target;
    ItemStack_damageItem(stack, 1, (struct Entity *)attacker);
    return 1;
}

static int ItemSword_onBlockDestroyed(Item *self, ItemStack *stack, int blockID, int x, int y,
                                      int z, EntityLiving *player) {
    (void)self;
    (void)blockID;
    (void)x;
    (void)y;
    (void)z;
    ItemStack_damageItem(stack, 2, (struct Entity *)player);
    return 1;
}

static int ItemSword_getDamageVsEntity(Item *self, struct Entity *entity) {
    (void)entity;
    return ((ItemSword *)self)->weaponDamage;
}

static int ItemSword_isFull3D(Item *self) {
    (void)self;
    return 1;
}
static int ItemSword_canHarvestBlock(Item *self, struct Block *block) {
    (void)self;
    return block->blockID == Block_web->blockID;
}

Item *ItemSword_create(int id, EnumToolMaterial mat) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.getStrVsBlock = ItemSword_getStrVsBlock;
        vtable.hitEntity = ItemSword_hitEntity;
        vtable.onBlockDestroyed = ItemSword_onBlockDestroyed;
        vtable.getDamageVsEntity = ItemSword_getDamageVsEntity;
        vtable.isFull3D = ItemSword_isFull3D;
        vtable.canHarvestBlock = ItemSword_canHarvestBlock;
        vi = 1;
    }
    ItemSword *self = (ItemSword *)calloc(1, sizeof(ItemSword));
    Item_construct(&self->base, id);
    self->base.vtable = &vtable;
    self->base.maxStackSize = 1;
    Item_setMaxDamage(&self->base, EnumToolMaterial_getMaxUses(mat));
    self->weaponDamage = 4 + EnumToolMaterial_getDamageVsEntity(mat) * 2;
    return &self->base;
}
