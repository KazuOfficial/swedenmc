#include "BlockLeavesBase.h"
#include "Block.h"
#include "World.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "Item.h"
#include "JavaRandom.h"
#include "ColorizerFoliage.h"
#include "WorldChunkManager.h"
#include "StatList.h"
#include <stdlib.h>
#include <string.h>

typedef struct BlockLeaves {
    BlockLeavesBase base;
    int baseIndexInPNG;
    int *adjacentTreeBlocks;
} BlockLeaves;

static int BlockLeaves_getRenderColor(Block *self, int meta) {
    (void)self;
    if ((meta & 1) == 1)
        return ColorizerFoliage_getFoliageColorPine();
    if ((meta & 2) == 2)
        return ColorizerFoliage_getFoliageColorBirch();
    return ColorizerFoliage_func_31073_c();
}

static int BlockLeaves_colorMultiplier(Block *self, IBlockAccess *world, int x, int y, int z) {
    (void)self;
    int var5 = world->vtable->getBlockMetadata(world, x, y, z);
    if ((var5 & 1) == 1)
        return ColorizerFoliage_getFoliageColorPine();
    if ((var5 & 2) == 2)
        return ColorizerFoliage_getFoliageColorBirch();
    WorldChunkManager *wcm = world->vtable->getWorldChunkManager(world);
    double var6, var8;
    WorldChunkManager_getSingleBiomeData(wcm, x, z, &var6, &var8);
    return ColorizerFoliage_getFoliageColor(var6, var8);
}

static void BlockLeaves_onBlockRemoval(Block *self, World *world, int x, int y, int z) {
    (void)self;
    int var5 = 1;
    int var6 = var5 + 1;
    extern Block *Block_leaves;
    if (World_checkChunksExist(world, x - var6, y - var6, z - var6, x + var6, y + var6, z + var6)) {
        for (int var7 = -var5; var7 <= var5; ++var7) {
            for (int var8 = -var5; var8 <= var5; ++var8) {
                for (int var9 = -var5; var9 <= var5; ++var9) {
                    int var10 = World_getBlockId(world, x + var7, y + var8, z + var9);
                    if (var10 == Block_leaves->blockID) {
                        int var11 = World_getBlockMetadata(world, x + var7, y + var8, z + var9);
                        World_setBlockMetadata(world, x + var7, y + var8, z + var9, var11 | 8);
                    }
                }
            }
        }
    }
}

static void BlockLeaves_removeLeaves(Block *self, World *world, int x, int y, int z) {
    int meta = World_getBlockMetadata(world, x, y, z);
    self->vtable->dropBlockAsItemWithChance(self, world, x, y, z, meta, 1.0f);
    World_setBlockWithNotify(world, x, y, z, 0);
}

static void BlockLeaves_updateTick(Block *self, World *world, int x, int y, int z,
                                   JavaRandom *rand) {
    (void)rand;
    if (!world->multiplayerWorld) {
        BlockLeaves *b = (BlockLeaves *)self;
        int var6 = World_getBlockMetadata(world, x, y, z);
        if ((var6 & 8) != 0) {
            int var7 = 4;
            int var8 = var7 + 1;
            int var9 = 32;
            int var10 = var9 * var9;
            int var11 = var9 / 2;
            if (b->adjacentTreeBlocks == NULL) {
                b->adjacentTreeBlocks = (int *)calloc((size_t)(var9 * var9 * var9), sizeof(int));
            }
            extern Block *Block_wood;
            extern Block *Block_leaves;
            int var12;
            if (World_checkChunksExist(world, x - var8, y - var8, z - var8, x + var8, y + var8,
                                       z + var8)) {

                for (var12 = -var7; var12 <= var7; ++var12) {
                    int var13;
                    int var14;
                    int var15;
                    for (var13 = -var7; var13 <= var7; ++var13) {
                        for (var14 = -var7; var14 <= var7; ++var14) {
                            var15 = World_getBlockId(world, x + var12, y + var13, z + var14);
                            if (var15 == Block_wood->blockID) {
                                b->adjacentTreeBlocks[(var12 + var11) * var10 +
                                                      (var13 + var11) * var9 + var14 + var11] = 0;
                            } else if (var15 == Block_leaves->blockID) {
                                b->adjacentTreeBlocks[(var12 + var11) * var10 +
                                                      (var13 + var11) * var9 + var14 + var11] = -2;
                            } else {
                                b->adjacentTreeBlocks[(var12 + var11) * var10 +
                                                      (var13 + var11) * var9 + var14 + var11] = -1;
                            }
                        }
                    }
                }

                for (var12 = 1; var12 <= 4; ++var12) {
                    int var13;
                    int var14;
                    int var15;
                    for (var13 = -var7; var13 <= var7; ++var13) {
                        for (var14 = -var7; var14 <= var7; ++var14) {
                            for (var15 = -var7; var15 <= var7; ++var15) {
                                if (b->adjacentTreeBlocks[(var13 + var11) * var10 +
                                                          (var14 + var11) * var9 + var15 + var11] ==
                                    var12 - 1) {
                                    if (b->adjacentTreeBlocks[(var13 + var11 - 1) * var10 +
                                                              (var14 + var11) * var9 + var15 +
                                                              var11] == -2)
                                        b->adjacentTreeBlocks[(var13 + var11 - 1) * var10 +
                                                              (var14 + var11) * var9 + var15 +
                                                              var11] = var12;
                                    if (b->adjacentTreeBlocks[(var13 + var11 + 1) * var10 +
                                                              (var14 + var11) * var9 + var15 +
                                                              var11] == -2)
                                        b->adjacentTreeBlocks[(var13 + var11 + 1) * var10 +
                                                              (var14 + var11) * var9 + var15 +
                                                              var11] = var12;
                                    if (b->adjacentTreeBlocks[(var13 + var11) * var10 +
                                                              (var14 + var11 - 1) * var9 + var15 +
                                                              var11] == -2)
                                        b->adjacentTreeBlocks[(var13 + var11) * var10 +
                                                              (var14 + var11 - 1) * var9 + var15 +
                                                              var11] = var12;
                                    if (b->adjacentTreeBlocks[(var13 + var11) * var10 +
                                                              (var14 + var11 + 1) * var9 + var15 +
                                                              var11] == -2)
                                        b->adjacentTreeBlocks[(var13 + var11) * var10 +
                                                              (var14 + var11 + 1) * var9 + var15 +
                                                              var11] = var12;
                                    if (b->adjacentTreeBlocks[(var13 + var11) * var10 +
                                                              (var14 + var11) * var9 +
                                                              (var15 + var11 - 1)] == -2)
                                        b->adjacentTreeBlocks[(var13 + var11) * var10 +
                                                              (var14 + var11) * var9 +
                                                              (var15 + var11 - 1)] = var12;
                                    if (b->adjacentTreeBlocks[(var13 + var11) * var10 +
                                                              (var14 + var11) * var9 + var15 +
                                                              var11 + 1] == -2)
                                        b->adjacentTreeBlocks[(var13 + var11) * var10 +
                                                              (var14 + var11) * var9 + var15 +
                                                              var11 + 1] = var12;
                                }
                            }
                        }
                    }
                }
            }
            var12 = b->adjacentTreeBlocks[var11 * var10 + var11 * var9 + var11];
            if (var12 >= 0) {
                World_setBlockMetadata(world, x, y, z, var6 & -9);
            } else {
                BlockLeaves_removeLeaves(self, world, x, y, z);
            }
        }
    }
}

static int BlockLeaves_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    return JavaRandom_nextInt(rand, 20) == 0 ? 1 : 0;
}

static int BlockLeaves_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    extern Block *Block_sapling;
    return Block_sapling->blockID;
}

static void BlockLeaves_harvestBlock(Block *self, World *world, EntityPlayer *player, int x, int y,
                                     int z, int meta) {
    ItemStack *equipped = EntityPlayer_getCurrentEquippedItem(player);
    extern Item *Item_shears;
    if (!world->multiplayerWorld && equipped != NULL &&
        equipped->itemID == Item_shears->shiftedIndex) {
        EntityPlayer_addStat(player, StatList_mineBlockStatArray[self->blockID], 1);
        extern Block *Block_leaves;
        ItemStack stack = ItemStack_fromBlockID(Block_leaves->blockID, 1, meta & 3);
        self->vtable->dropBlockAsItem_do(self, world, x, y, z, &stack);
    } else {
        Block_defaultVtable.harvestBlock(self, world, player, x, y, z, meta);
    }
}

static int BlockLeaves_damageDropped(Block *self, int meta) {
    (void)self;
    return meta & 3;
}

static int BlockLeaves_isOpaqueCube(Block *self) {
    BlockLeaves *b = (BlockLeaves *)self;
    return !b->base.graphicsLevel;
}

static int BlockLeaves_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta) {
    (void)side;
    return (meta & 3) == 1 ? self->blockIndexInTexture + 80 : self->blockIndexInTexture;
}

void BlockLeaves_setGraphicsLevel(Block *blockSelf, int var1) {
    BlockLeaves *self = (BlockLeaves *)blockSelf;
    self->base.graphicsLevel = var1;
    self->base.base.blockIndexInTexture = self->baseIndexInPNG + (var1 ? 0 : 1);
}

static void BlockLeaves_onEntityWalking(Block *self, World *world, int x, int y, int z,
                                        Entity *entity) {
    Block_defaultVtable.onEntityWalking(self, world, x, y, z, entity);
}

Block *BlockLeaves_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;

        vtable.shouldSideBeRendered = BlockLeavesBase_shouldSideBeRendered;
        vtable.isOpaqueCube = BlockLeaves_isOpaqueCube;
        vtable.getRenderColor = BlockLeaves_getRenderColor;
        vtable.colorMultiplier = BlockLeaves_colorMultiplier;
        vtable.onBlockRemoval = BlockLeaves_onBlockRemoval;
        vtable.updateTick = BlockLeaves_updateTick;
        vtable.quantityDropped = BlockLeaves_quantityDropped;
        vtable.idDropped = BlockLeaves_idDropped;
        vtable.harvestBlock = BlockLeaves_harvestBlock;
        vtable.damageDropped = BlockLeaves_damageDropped;
        vtable.getBlockTextureFromSideAndMetadata = BlockLeaves_getBlockTextureFromSideAndMetadata;
        vtable.onEntityWalking = BlockLeaves_onEntityWalking;
        vtable_init = 1;
    }
    BlockLeaves *self = (BlockLeaves *)calloc(1, sizeof(BlockLeaves));
    self->base.base.vtable = &vtable;
    BlockLeavesBase_construct(&self->base, id, tex, Material_leaves, 0);
    self->baseIndexInPNG = tex;
    Block_setTickOnLoad(&self->base.base, 1);
    return &self->base.base;
}
