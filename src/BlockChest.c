#include "BlockChest.h"
#include "BlockContainer.h"
#include "World.h"
#include "TileEntityChest.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "JavaRandom.h"
#include <stdlib.h>

#include "InventoryLargeChest.h"
#include <string.h>

static void BlockChest_onBlockAdded(Block *self, World *world, int x, int y, int z);

BlockVtable BlockChest_defaultVtable;

void BlockChest_initVtable(BlockVtable *vt) {
    BlockContainer_initVtable(vt);
    vt->getBlockTexture = BlockChest_getBlockTexture;
    vt->getBlockTextureFromSide = BlockChest_getBlockTextureFromSide;
    vt->canPlaceBlockAt = BlockChest_canPlaceBlockAt;
    vt->onBlockAdded = BlockChest_onBlockAdded;
    vt->onBlockRemoval = BlockChest_onBlockRemoval;
    vt->blockActivated = BlockChest_blockActivated;
}

void BlockChest_construct(BlockChest *self, int id) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockChest_initVtable(&BlockChest_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockChest_defaultVtable;
    BlockContainer_construct(&self->base, id, Material_wood);
    self->base.blockIndexInTexture = 26;
    JavaRandom_init(&self->random, 0);
}

BlockChest *BlockChest_create(int id) {
    BlockChest *self = (BlockChest *)calloc(1, sizeof(BlockChest));
    BlockChest_construct(self, id);
    return self;
}

struct TileEntity *BlockChest_getBlockEntity(Block *self) {
    (void)self;
    return (struct TileEntity *)TileEntityChest_create();
}

static void BlockChest_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    World_setBlockTileEntity(world, x, y, z, BlockChest_getBlockEntity(self));
}

static int isThereANeighborChest(Block *self, World *var1, int var2, int var3, int var4) {
    if (World_getBlockId(var1, var2, var3, var4) != self->blockID) {
        return 0;
    }
    return World_getBlockId(var1, var2 - 1, var3, var4) == self->blockID   ? 1
           : World_getBlockId(var1, var2 + 1, var3, var4) == self->blockID ? 1
           : World_getBlockId(var1, var2, var3, var4 - 1) == self->blockID
               ? 1
               : World_getBlockId(var1, var2, var3, var4 + 1) == self->blockID;
}

int BlockChest_getBlockTexture(Block *self, IBlockAccess *var1, int var2, int var3, int var4,
                               int var5) {
    if (var5 == 1 || var5 == 0) {
        return self->blockIndexInTexture - 1;
    }

    int var6 = var1->vtable->getBlockId(var1, var2, var3, var4 - 1);
    int var7 = var1->vtable->getBlockId(var1, var2, var3, var4 + 1);
    int var8 = var1->vtable->getBlockId(var1, var2 - 1, var3, var4);
    int var9 = var1->vtable->getBlockId(var1, var2 + 1, var3, var4);
    int var10, var11, var12, var13;

    if (var6 != self->blockID && var7 != self->blockID) {
        if (var8 != self->blockID && var9 != self->blockID) {
            int var14 = 3;
            if (Block_opaqueCubeLookup[var6] && !Block_opaqueCubeLookup[var7]) {
                var14 = 3;
            }
            if (Block_opaqueCubeLookup[var7] && !Block_opaqueCubeLookup[var6]) {
                var14 = 2;
            }
            if (Block_opaqueCubeLookup[var8] && !Block_opaqueCubeLookup[var9]) {
                var14 = 5;
            }
            if (Block_opaqueCubeLookup[var9] && !Block_opaqueCubeLookup[var8]) {
                var14 = 4;
            }
            return var5 == var14 ? self->blockIndexInTexture + 1 : self->blockIndexInTexture;
        } else if (var5 != 4 && var5 != 5) {
            var10 = 0;
            if (var8 == self->blockID) {
                var10 = -1;
            }
            var11 = var1->vtable->getBlockId(var1, var8 == self->blockID ? var2 - 1 : var2 + 1,
                                             var3, var4 - 1);
            var12 = var1->vtable->getBlockId(var1, var8 == self->blockID ? var2 - 1 : var2 + 1,
                                             var3, var4 + 1);
            if (var5 == 3) {
                var10 = -1 - var10;
            }
            var13 = 3;
            if ((Block_opaqueCubeLookup[var6] || Block_opaqueCubeLookup[var11]) &&
                !Block_opaqueCubeLookup[var7] && !Block_opaqueCubeLookup[var12]) {
                var13 = 3;
            }
            if ((Block_opaqueCubeLookup[var7] || Block_opaqueCubeLookup[var12]) &&
                !Block_opaqueCubeLookup[var6] && !Block_opaqueCubeLookup[var11]) {
                var13 = 2;
            }
            return (var5 == var13 ? self->blockIndexInTexture + 16
                                  : self->blockIndexInTexture + 32) +
                   var10;
        } else {
            return self->blockIndexInTexture;
        }
    } else if (var5 != 2 && var5 != 3) {
        var10 = 0;
        if (var6 == self->blockID) {
            var10 = -1;
        }
        var11 = var1->vtable->getBlockId(var1, var2 - 1, var3,
                                         var6 == self->blockID ? var4 - 1 : var4 + 1);
        var12 = var1->vtable->getBlockId(var1, var2 + 1, var3,
                                         var6 == self->blockID ? var4 - 1 : var4 + 1);
        if (var5 == 4) {
            var10 = -1 - var10;
        }
        var13 = 5;
        if ((Block_opaqueCubeLookup[var8] || Block_opaqueCubeLookup[var11]) &&
            !Block_opaqueCubeLookup[var9] && !Block_opaqueCubeLookup[var12]) {
            var13 = 5;
        }
        if ((Block_opaqueCubeLookup[var9] || Block_opaqueCubeLookup[var12]) &&
            !Block_opaqueCubeLookup[var8] && !Block_opaqueCubeLookup[var11]) {
            var13 = 4;
        }
        return (var5 == var13 ? self->blockIndexInTexture + 16 : self->blockIndexInTexture + 32) +
               var10;
    } else {
        return self->blockIndexInTexture;
    }
}

int BlockChest_getBlockTextureFromSide(Block *self, int var1) {
    return var1 == 1   ? self->blockIndexInTexture - 1
           : var1 == 0 ? self->blockIndexInTexture - 1
           : var1 == 3 ? self->blockIndexInTexture + 1
                       : self->blockIndexInTexture;
}

int BlockChest_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    int var5 = 0;
    if (World_getBlockId(var1, var2 - 1, var3, var4) == self->blockID) {
        ++var5;
    }
    if (World_getBlockId(var1, var2 + 1, var3, var4) == self->blockID) {
        ++var5;
    }
    if (World_getBlockId(var1, var2, var3, var4 - 1) == self->blockID) {
        ++var5;
    }
    if (World_getBlockId(var1, var2, var3, var4 + 1) == self->blockID) {
        ++var5;
    }
    return var5 > 1                                                  ? 0
           : isThereANeighborChest(self, var1, var2 - 1, var3, var4) ? 0
           : isThereANeighborChest(self, var1, var2 + 1, var3, var4) ? 0
           : isThereANeighborChest(self, var1, var2, var3, var4 - 1)
               ? 0
               : !isThereANeighborChest(self, var1, var2, var3, var4 + 1);
}

void BlockChest_onBlockRemoval(Block *self, World *var1, int var2, int var3, int var4) {
    BlockChest *chest_self = (BlockChest *)self;
    TileEntityChest *var5 = (TileEntityChest *)World_getBlockTileEntity(var1, var2, var3, var4);
    if (var5 == NULL)
        return;

    for (int var6 = 0; var6 < TileEntityChest_getSizeInventory(var5); ++var6) {
        ItemStack *var7 = TileEntityChest_getStackInSlot(var5, var6);
        if (var7 != NULL) {
            float var8 = JavaRandom_nextFloat(&chest_self->random) * 0.8F + 0.1F;
            float var9 = JavaRandom_nextFloat(&chest_self->random) * 0.8F + 0.1F;
            float var10 = JavaRandom_nextFloat(&chest_self->random) * 0.8F + 0.1F;

            while (var7->stackSize > 0) {
                int var11 = JavaRandom_nextInt(&chest_self->random, 21) + 10;
                if (var11 > var7->stackSize) {
                    var11 = var7->stackSize;
                }
                var7->stackSize -= var11;
                ItemStack stack =
                    ItemStack_fromItemID(var7->itemID, var11, ItemStack_getItemDamage(var7));
                EntityItem *var12 = EntityItem_create(var1, (double)((float)var2 + var8),
                                                      (double)((float)var3 + var9),
                                                      (double)((float)var4 + var10), stack);
                float var13 = 0.05F;
                var12->base.motionX =
                    (double)((float)JavaRandom_nextGaussian(&chest_self->random) * var13);
                var12->base.motionY =
                    (double)((float)JavaRandom_nextGaussian(&chest_self->random) * var13 + 0.2F);
                var12->base.motionZ =
                    (double)((float)JavaRandom_nextGaussian(&chest_self->random) * var13);
                World_entityJoinedWorld(var1, (Entity *)var12);
            }
        }
    }

    World_removeBlockTileEntity(var1, var2, var3, var4);
}

#define CHEST_INV(te) (&((TileEntityChest *)(te))->inventory)

int BlockChest_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                              EntityPlayer *var5) {
    IInventory *var6 = CHEST_INV(World_getBlockTileEntity(var1, var2, var3, var4));
    if (World_isBlockNormalCube(var1, var2, var3 + 1, var4)) {
        return 1;
    }
    if (World_getBlockId(var1, var2 - 1, var3, var4) == self->blockID &&
        World_isBlockNormalCube(var1, var2 - 1, var3 + 1, var4)) {
        return 1;
    }
    if (World_getBlockId(var1, var2 + 1, var3, var4) == self->blockID &&
        World_isBlockNormalCube(var1, var2 + 1, var3 + 1, var4)) {
        return 1;
    }
    if (World_getBlockId(var1, var2, var3, var4 - 1) == self->blockID &&
        World_isBlockNormalCube(var1, var2, var3 + 1, var4 - 1)) {
        return 1;
    }
    if (World_getBlockId(var1, var2, var3, var4 + 1) == self->blockID &&
        World_isBlockNormalCube(var1, var2, var3 + 1, var4 + 1)) {
        return 1;
    }
    if (World_getBlockId(var1, var2 - 1, var3, var4) == self->blockID) {
        InventoryLargeChest *lc = (InventoryLargeChest *)calloc(1, sizeof(InventoryLargeChest));
        InventoryLargeChest_init(lc, "Large chest",
                                 CHEST_INV(World_getBlockTileEntity(var1, var2 - 1, var3, var4)),
                                 var6);
        var6 = (IInventory *)lc;
    }
    if (World_getBlockId(var1, var2 + 1, var3, var4) == self->blockID) {
        InventoryLargeChest *lc = (InventoryLargeChest *)calloc(1, sizeof(InventoryLargeChest));
        InventoryLargeChest_init(lc, "Large chest", var6,
                                 CHEST_INV(World_getBlockTileEntity(var1, var2 + 1, var3, var4)));
        var6 = (IInventory *)lc;
    }
    if (World_getBlockId(var1, var2, var3, var4 - 1) == self->blockID) {
        InventoryLargeChest *lc = (InventoryLargeChest *)calloc(1, sizeof(InventoryLargeChest));
        InventoryLargeChest_init(lc, "Large chest",
                                 CHEST_INV(World_getBlockTileEntity(var1, var2, var3, var4 - 1)),
                                 var6);
        var6 = (IInventory *)lc;
    }
    if (World_getBlockId(var1, var2, var3, var4 + 1) == self->blockID) {
        InventoryLargeChest *lc = (InventoryLargeChest *)calloc(1, sizeof(InventoryLargeChest));
        InventoryLargeChest_init(lc, "Large chest", var6,
                                 CHEST_INV(World_getBlockTileEntity(var1, var2, var3, var4 + 1)));
        var6 = (IInventory *)lc;
    }
    if (var1->multiplayerWorld) {
        return 1;
    }
    EntityPlayer_displayGUIChest(var5, var6);
    return 1;
}
