#include "ItemTool.h"
#include "Block.h"
#include <stdlib.h>

static float ItemTool_getStrVsBlock(Item *self, ItemStack *stack, struct Block *block) {
    (void)stack;
    ItemTool *t = (ItemTool *)self;
    for (int i = 0; i < t->numEffectiveBlocks; i++) {
        if (t->blocksEffectiveAgainst[i] == block)
            return t->efficiencyOnProperMaterial;
    }
    return 1.0f;
}

static int ItemTool_hitEntity(Item *self, ItemStack *stack, EntityLiving *target,
                              EntityLiving *attacker) {
    (void)self;
    (void)target;
    ItemStack_damageItem(stack, 2, (struct Entity *)attacker);
    return 1;
}

static int ItemTool_onBlockDestroyed(Item *self, ItemStack *stack, int blockID, int x, int y, int z,
                                     EntityLiving *player) {
    (void)self;
    (void)blockID;
    (void)x;
    (void)y;
    (void)z;
    ItemStack_damageItem(stack, 1, (struct Entity *)player);
    return 1;
}

static int ItemTool_getDamageVsEntity(Item *self, struct Entity *entity) {
    (void)entity;
    return ((ItemTool *)self)->damageVsEntity;
}

static int ItemTool_isFull3D(Item *self) {
    (void)self;
    return 1;
}

void ItemTool_initVtable(ItemVtable *vtable) {
    *vtable = Item_defaultVtable;
    vtable->getStrVsBlock = ItemTool_getStrVsBlock;
    vtable->hitEntity = ItemTool_hitEntity;
    vtable->onBlockDestroyed = ItemTool_onBlockDestroyed;
    vtable->getDamageVsEntity = ItemTool_getDamageVsEntity;
    vtable->isFull3D = ItemTool_isFull3D;
}

Item *ItemTool_create(int id, int baseDamage, EnumToolMaterial mat, struct Block **blocks,
                      int numBlocks) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemTool_initVtable(&vtable);
        vi = 1;
    }

    ItemTool *self = (ItemTool *)calloc(1, sizeof(ItemTool));
    Item_construct(&self->base, id);
    self->base.vtable = &vtable;
    self->base.maxStackSize = 1;
    Item_setMaxDamage(&self->base, EnumToolMaterial_getMaxUses(mat));
    self->efficiencyOnProperMaterial = EnumToolMaterial_getEfficiencyOnProperMaterial(mat);
    self->damageVsEntity = baseDamage + EnumToolMaterial_getDamageVsEntity(mat);
    self->toolMaterial = mat;
    self->blocksEffectiveAgainst = blocks;
    self->numEffectiveBlocks = numBlocks;
    return &self->base;
}
