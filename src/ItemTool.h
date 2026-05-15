#ifndef ITEM_TOOL_H
#define ITEM_TOOL_H

#include "Item.h"
#include "EnumToolMaterial.h"

typedef struct {
    Item base;
    struct Block **blocksEffectiveAgainst;
    int numEffectiveBlocks;
    float efficiencyOnProperMaterial;
    int damageVsEntity;
    EnumToolMaterial toolMaterial;
} ItemTool;

void ItemTool_initVtable(ItemVtable *vtable);
Item *ItemTool_create(int id, int baseDamage, EnumToolMaterial mat, struct Block **effectiveBlocks,
                      int numBlocks);

#endif
