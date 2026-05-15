#include "ItemBlock.h"
#include "ItemStack.h"
#include "Block.h"
#include "World.h"
#include "StepSound.h"
#include "Material.h"
#include "EntityLiving.h"
#include <stdlib.h>
#include <stdio.h>

static int ItemBlock_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3,
                               int var4, int var5, int var6, int var7) {
    ItemBlock *b = (ItemBlock *)self;

    if (World_getBlockId(var3, var4, var5, var6) == Block_snow->blockID) {
        var7 = 0;
    } else {
        if (var7 == 0) {
            --var5;
        }
        if (var7 == 1) {
            ++var5;
        }
        if (var7 == 2) {
            --var6;
        }
        if (var7 == 3) {
            ++var6;
        }
        if (var7 == 4) {
            --var4;
        }
        if (var7 == 5) {
            ++var4;
        }
    }
    if (var1->stackSize == 0) {
        return 0;
    } else if (var5 == 127 && Block_blocksList[b->blockID] != NULL &&
               Block_blocksList[b->blockID]->blockMaterial != NULL &&
               Material_isSolid(Block_blocksList[b->blockID]->blockMaterial)) {
        return 0;
    } else if (World_canBlockBePlacedAt(var3, b->blockID, var4, var5, var6, 0, var7)) {
        Block *var8 = Block_blocksList[b->blockID];
        if (World_setBlockAndMetadataWithNotify(
                var3, var4, var5, var6, b->blockID,
                self->vtable->getPlacedBlockMetadata(self, var1->itemDamage))) {
            Block_blocksList[b->blockID]->vtable->onBlockPlaced(Block_blocksList[b->blockID], var3,
                                                                var4, var5, var6, var7);
            Block_blocksList[b->blockID]->vtable->onBlockPlacedBy(
                Block_blocksList[b->blockID], var3, var4, var5, var6, (EntityLiving *)var2);
            World_playSoundEffect(var3, (double)((float)var4 + 0.5F), (double)((float)var5 + 0.5F),
                                  (double)((float)var6 + 0.5F),
                                  StepSound_func_1145_d(var8->stepSound),
                                  (StepSound_getVolume(var8->stepSound) + 1.0F) / 2.0F,
                                  StepSound_getPitch(var8->stepSound) * 0.8F);
            --var1->stackSize;
        }
        return 1;
    } else {
        return 0;
    }
}

static const char *ItemBlock_getItemNameIS(Item *self, ItemStack *stack) {
    (void)stack;
    ItemBlock *b = (ItemBlock *)self;
    Block *blk = Block_blocksList[b->blockID];
    if (blk)
        return blk->vtable->getBlockName(blk);
    return self->itemName;
}

const char *ItemBlock_getItemName(Item *self) {
    ItemBlock *b = (ItemBlock *)self;
    Block *blk = Block_blocksList[b->blockID];
    if (blk)
        return blk->vtable->getBlockName(blk);
    return self->itemName;
}

void ItemBlock_initVtable(ItemVtable *vtable) {
    *vtable = Item_defaultVtable;
    vtable->onItemUse = ItemBlock_onItemUse;
    vtable->getItemNameIS = ItemBlock_getItemNameIS;
    vtable->getItemName = ItemBlock_getItemName;
}

static ItemBlock *ItemBlock_alloc(int blockID) {
    ItemBlock *self = (ItemBlock *)calloc(1, sizeof(ItemBlock));
    self->base.maxStackSize = 64;
    self->base.maxDamage = 0;
    self->base.iconIndex = 0;
    self->base.bFull3D = 0;
    self->base.hasSubtypes = 0;
    self->base.containerItem = NULL;
    self->base.itemName[0] = '\0';
    self->base.shiftedIndex = blockID;
    self->blockID = blockID;
    if (blockID < ITEM_LIST_SIZE)
        Item_itemsList[blockID] = &self->base;
    if (Block_blocksList[blockID])
        self->base.iconIndex = Block_blocksList[blockID]->vtable->getBlockTextureFromSide(
            Block_blocksList[blockID], 2);
    return self;
}

Item *ItemBlock_create(int blockID) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemBlock_initVtable(&vtable);
        vi = 1;
    }
    ItemBlock *self = ItemBlock_alloc(blockID);
    self->base.vtable = &vtable;
    return &self->base;
}

extern const char *ItemDye_dyeColors[16];
extern int BlockCloth_func_21034_c(int meta);

static int ItemCloth_getIconFromDamage(Item *self, int damage) {
    (void)self;
    return Block_cloth->vtable->getBlockTextureFromSideAndMetadata(Block_cloth, 2,
                                                                   BlockCloth_func_21034_c(damage));
}
static int ItemCloth_getPlacedBlockMetadata(Item *self, int damage) {
    (void)self;
    return damage;
}
static const char *ItemCloth_getItemNameIS(Item *self, ItemStack *stack) {
    static char buf[64];

    const char *base = ItemBlock_getItemName(self);
    int dyeIdx = BlockCloth_func_21034_c(stack->itemDamage);
    snprintf(buf, sizeof(buf), "%s.%s", base, ItemDye_dyeColors[dyeIdx]);
    return buf;
}

Item *ItemCloth_create(int blockID) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemBlock_initVtable(&vtable);
        vtable.getIconFromDamage = ItemCloth_getIconFromDamage;
        vtable.getPlacedBlockMetadata = ItemCloth_getPlacedBlockMetadata;
        vtable.getItemNameIS = ItemCloth_getItemNameIS;
        vi = 1;
    }
    ItemBlock *self = ItemBlock_alloc(blockID);
    self->base.vtable = &vtable;
    Item_setMaxDamage(&self->base, 0);
    Item_setHasSubtypes(&self->base, 1);
    return &self->base;
}
