#include "BlockBed.h"
#include "World.h"
#include "EntityPlayer.h"
#include "Item.h"
#include "Block.h"
#include "ModelBed.h"
#include "EnumStatus.h"
#include <stdlib.h>

const int BlockBed_headBlockToFootBlockMap[4][2] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};

BlockVtable BlockBed_defaultVtable;

void BlockBed_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->blockActivated = BlockBed_blockActivated;
    vt->getBlockTextureFromSideAndMetadata = BlockBed_getBlockTextureFromSideAndMetadata;
    vt->getRenderType = BlockBed_getRenderType;
    vt->renderAsNormalBlock = BlockBed_renderAsNormalBlock;
    vt->isOpaqueCube = BlockBed_isOpaqueCube;
    vt->setBlockBoundsBasedOnState = BlockBed_setBlockBoundsBasedOnState;
    vt->onNeighborBlockChange = BlockBed_onNeighborBlockChange;
    vt->idDropped = BlockBed_idDropped;
    vt->dropBlockAsItemWithChance = BlockBed_dropBlockAsItemWithChance;
    vt->getMobilityFlag = BlockBed_getMobilityFlag;
}

static void setBounds(Block *self) {
    Block_setBlockBounds(self, 0.0F, 0.0F, 0.0F, 1.0F, 9.0F / 16.0F, 1.0F);
}

void BlockBed_construct(BlockBed *self, int id) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockBed_initVtable(&BlockBed_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockBed_defaultVtable;
    Block_constructWithTexture(&self->base, id, 134, Material_cloth);
    setBounds(&self->base);
}

BlockBed *BlockBed_create(int id) {
    BlockBed *self = (BlockBed *)calloc(1, sizeof(BlockBed));
    BlockBed_construct(self, id);
    return self;
}

int BlockBed_getDirectionFromMetadata(int meta) { return meta & 3; }

int BlockBed_isBlockFootOfBed(int meta) { return (meta & 8) != 0; }

int BlockBed_isBedOccupied(int meta) { return (meta & 4) != 0; }

void BlockBed_setBedOccupied(World *var0, int var1, int var2, int var3, int var4) {
    int var5 = World_getBlockMetadata(var0, var1, var2, var3);
    if (var4) {
        var5 |= 4;
    } else {
        var5 &= -5;
    }
    World_setBlockMetadataWithNotify(var0, var1, var2, var3, var5);
}

int BlockBed_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                            EntityPlayer *var5) {
    if (var1->isRemote) {
        return 1;
    } else {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        if (!BlockBed_isBlockFootOfBed(var6)) {
            int var7 = BlockBed_getDirectionFromMetadata(var6);
            var2 += BlockBed_headBlockToFootBlockMap[var7][0];
            var4 += BlockBed_headBlockToFootBlockMap[var7][1];
            if (World_getBlockId(var1, var2, var3, var4) != self->blockID) {
                return 1;
            }
            var6 = World_getBlockMetadata(var1, var2, var3, var4);
        }

        if (!WorldProvider_canRespawnHere(var1->worldProvider)) {
            double var16 = (double)var2 + 0.5;
            double var17 = (double)var3 + 0.5;
            double var11 = (double)var4 + 0.5;
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
            int var13 = BlockBed_getDirectionFromMetadata(var6);
            var2 += BlockBed_headBlockToFootBlockMap[var13][0];
            var4 += BlockBed_headBlockToFootBlockMap[var13][1];
            if (World_getBlockId(var1, var2, var3, var4) == self->blockID) {
                World_setBlockWithNotify(var1, var2, var3, var4, 0);
                var16 = (var16 + (double)var2 + 0.5) / 2.0;
                var17 = (var17 + (double)var3 + 0.5) / 2.0;
                var11 = (var11 + (double)var4 + 0.5) / 2.0;
            }
            World_newExplosion(var1, NULL, (double)((float)var2 + 0.5F),
                               (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F), 5.0F, 1);
            (void)var16;
            (void)var17;
            (void)var11;
            return 1;
        } else {
            if (BlockBed_isBedOccupied(var6)) {
                EntityPlayer *var14 = NULL;
                for (int var8 = 0; var8 < var1->numPlayerEntities; ++var8) {
                    EntityPlayer *var9 = var1->playerEntities[var8];
                    if (EntityPlayer_isPlayerSleeping(var9)) {
                        ChunkCoordinates *var10 = &var9->bedChunkCoordinates;
                        if (var10->x == var2 && var10->y == var3 && var10->z == var4) {
                            var14 = var9;
                        }
                    }
                }
                if (var14 != NULL) {
                    EntityPlayer_addChatMessage(var5, "tile.bed.occupied");
                    return 1;
                }
                BlockBed_setBedOccupied(var1, var2, var3, var4, 0);
            }

            EnumStatus var15 = EntityPlayer_sleepInBedAt(var5, var2, var3, var4);
            if (var15 == ENUMSTATUS_OK) {
                BlockBed_setBedOccupied(var1, var2, var3, var4, 1);
                return 1;
            } else {
                if (var15 == ENUMSTATUS_NOT_POSSIBLE_NOW) {
                    EntityPlayer_addChatMessage(var5, "tile.bed.noSleep");
                }
                return 1;
            }
        }
    }
}

int BlockBed_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    if (var1 == 0) {
        return Block_planks->blockIndexInTexture;
    } else {
        int var3 = BlockBed_getDirectionFromMetadata(var2);
        int var4 = ModelBed_bedDirection[var3][var1];
        return BlockBed_isBlockFootOfBed(var2)
                   ? (var4 == 2 ? self->blockIndexInTexture + 2 + 16
                                : (var4 != 5 && var4 != 4 ? self->blockIndexInTexture + 1
                                                          : self->blockIndexInTexture + 1 + 16))
                   : (var4 == 3 ? self->blockIndexInTexture - 1 + 16
                                : (var4 != 5 && var4 != 4 ? self->blockIndexInTexture
                                                          : self->blockIndexInTexture + 16));
    }
}

int BlockBed_getRenderType(Block *self) {
    (void)self;
    return 14;
}

int BlockBed_renderAsNormalBlock(Block *self) {
    (void)self;
    return 0;
}

int BlockBed_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

void BlockBed_setBlockBoundsBasedOnState(Block *self, IBlockAccess *var1, int var2, int var3,
                                         int var4) {
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    setBounds(self);
}

void BlockBed_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                    int var5) {
    (void)var5;
    int var6 = World_getBlockMetadata(var1, var2, var3, var4);
    int var7 = BlockBed_getDirectionFromMetadata(var6);
    if (BlockBed_isBlockFootOfBed(var6)) {
        if (World_getBlockId(var1, var2 - BlockBed_headBlockToFootBlockMap[var7][0], var3,
                             var4 - BlockBed_headBlockToFootBlockMap[var7][1]) != self->blockID) {
            World_setBlockWithNotify(var1, var2, var3, var4, 0);
        }
    } else if (World_getBlockId(var1, var2 + BlockBed_headBlockToFootBlockMap[var7][0], var3,
                                var4 + BlockBed_headBlockToFootBlockMap[var7][1]) !=
               self->blockID) {
        World_setBlockWithNotify(var1, var2, var3, var4, 0);
        if (!var1->isRemote) {
            Block_dropBlockAsItem(self, var1, var2, var3, var4, var6);
        }
    }
}

int BlockBed_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var2;
    return BlockBed_isBlockFootOfBed(var1) ? 0 : Item_bed->shiftedIndex;
}

void BlockBed_dropBlockAsItemWithChance(Block *self, World *var1, int var2, int var3, int var4,
                                        int var5, float var6) {
    if (!BlockBed_isBlockFootOfBed(var5)) {
        Block_defaultVtable.dropBlockAsItemWithChance(self, var1, var2, var3, var4, var5, var6);
    }
}

int BlockBed_getMobilityFlag(Block *self) {
    (void)self;
    return 1;
}

ChunkCoordinates *BlockBed_getNearestEmptyChunkCoordinates(World *var0, int var1, int var2,
                                                           int var3, int var4) {
    int var5 = World_getBlockMetadata(var0, var1, var2, var3);
    int var6 = BlockBed_getDirectionFromMetadata(var5);

    for (int var7 = 0; var7 <= 1; ++var7) {
        int var8 = var1 - BlockBed_headBlockToFootBlockMap[var6][0] * var7 - 1;
        int var9 = var3 - BlockBed_headBlockToFootBlockMap[var6][1] * var7 - 1;
        int var10 = var8 + 2;
        int var11 = var9 + 2;

        for (int var12 = var8; var12 <= var10; ++var12) {
            for (int var13 = var9; var13 <= var11; ++var13) {
                if (World_isBlockNormalCube(var0, var12, var2 - 1, var13) &&
                    World_isAirBlock(var0, var12, var2, var13) &&
                    World_isAirBlock(var0, var12, var2 + 1, var13)) {
                    if (var4 <= 0) {
                        ChunkCoordinates *result =
                            (ChunkCoordinates *)calloc(1, sizeof(ChunkCoordinates));
                        result->x = var12;
                        result->y = var2;
                        result->z = var13;
                        return result;
                    }
                    --var4;
                }
            }
        }
    }

    return NULL;
}
