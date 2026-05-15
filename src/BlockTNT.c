#include "Block.h"
#include "Material.h"
#include "World.h"
#include "EntityPlayer.h"
#include "EntityTNTPrimed.h"
#include "ItemStack.h"
#include "Item.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockTNT_getBlockTextureFromSide(Block *self, int var1) {
    return var1 == 0   ? self->blockIndexInTexture + 2
           : var1 == 1 ? self->blockIndexInTexture + 1
                       : self->blockIndexInTexture;
}

static void BlockTNT_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4) {
    Block_defaultVtable.onBlockAdded(self, var1, var2, var3, var4);
    if (World_isBlockIndirectlyGettingPowered(var1, var2, var3, var4)) {
        self->vtable->onBlockDestroyedByPlayer(self, var1, var2, var3, var4, 1);
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
}

static void BlockTNT_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                           int var5) {
    if (var5 > 0 && Block_blocksList[var5] &&
        Block_blocksList[var5]->vtable->canProvidePower(Block_blocksList[var5]) &&
        World_isBlockIndirectlyGettingPowered(var1, var2, var3, var4)) {
        self->vtable->onBlockDestroyedByPlayer(self, var1, var2, var3, var4, 1);
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
    }
}

static int BlockTNT_quantityDropped(Block *self, JavaRandom *var1) {
    (void)self;
    (void)var1;
    return 0;
}

static void BlockTNT_onBlockDestroyedByExplosion(Block *self, World *var1, int var2, int var3,
                                                 int var4) {
    (void)self;
    EntityTNTPrimed *var5 =
        EntityTNTPrimed_create2(var1, (double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F),
                                (double)((float)var4 + 0.5F));
    var5->fuse = JavaRandom_nextInt(&var1->rand, var5->fuse / 4) + var5->fuse / 8;
    World_entityJoinedWorld(var1, (Entity *)var5);
}

static void BlockTNT_onBlockDestroyedByPlayer(Block *self, World *var1, int var2, int var3,
                                              int var4, int var5) {
    if (!var1->multiplayerWorld) {
        if ((var5 & 1) == 0) {
            ItemStack stack = ItemStack_fromBlockID(self->blockID, 1, 0);
            self->vtable->dropBlockAsItem_do(self, var1, var2, var3, var4, &stack);
        } else {
            EntityTNTPrimed *var6 =
                EntityTNTPrimed_create2(var1, (double)((float)var2 + 0.5F),
                                        (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F));
            World_entityJoinedWorld(var1, (Entity *)var6);
            World_playSoundAtEntity(var1, (Entity *)var6, "random.fuse", 1.0F, 1.0F);
        }
    }
}

static void BlockTNT_onBlockClicked(Block *self, World *var1, int var2, int var3, int var4,
                                    EntityPlayer *var5) {
    ItemStack *held = EntityPlayer_getCurrentEquippedItem(var5);
    if (held != NULL && held->itemID == Item_flintAndSteel->shiftedIndex) {
        World_setBlockMetadata(var1, var2, var3, var4, 1);
    }
    Block_defaultVtable.onBlockClicked(self, var1, var2, var3, var4, var5);
}

static int BlockTNT_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                                   EntityPlayer *var5) {
    return Block_defaultVtable.blockActivated(self, var1, var2, var3, var4, var5);
}

Block *BlockTNT_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getBlockTextureFromSide = BlockTNT_getBlockTextureFromSide;
        vtable.onBlockAdded = BlockTNT_onBlockAdded;
        vtable.onNeighborBlockChange = BlockTNT_onNeighborBlockChange;
        vtable.quantityDropped = BlockTNT_quantityDropped;
        vtable.onBlockDestroyedByExplosion = BlockTNT_onBlockDestroyedByExplosion;
        vtable.onBlockDestroyedByPlayer = BlockTNT_onBlockDestroyedByPlayer;
        vtable.onBlockClicked = BlockTNT_onBlockClicked;
        vtable.blockActivated = BlockTNT_blockActivated;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_tnt);
    return self;
}
