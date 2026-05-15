#include "Block.h"
#include "BlockContainer.h"
#include "Material.h"
#include "MathHelper.h"
#include "World.h"
#include "TileEntityFurnace.h"
#include "TileEntity.h"
#include "EntityPlayer.h"
#include "EntityItem.h"
#include "ItemStack.h"
#include "JavaRandom.h"
#include <stdlib.h>
#include <time.h>

static int BlockFurnace_keepFurnaceInventory = 0;

typedef struct {
    Block base;
    int isActive;
    JavaRandom furnaceRand;
} BlockFurnace;

static int BlockFurnace_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return Block_stoneOvenIdle->blockID;
}

static void BlockFurnace_setDefaultDirection(Block *self, World *var1, int var2, int var3,
                                             int var4) {
    (void)self;
    if (!var1->multiplayerWorld) {
        int var5 = World_getBlockId(var1, var2, var3, var4 - 1);
        int var6 = World_getBlockId(var1, var2, var3, var4 + 1);
        int var7 = World_getBlockId(var1, var2 - 1, var3, var4);
        int var8 = World_getBlockId(var1, var2 + 1, var3, var4);
        int var9 = 3;
        if (Block_opaqueCubeLookup[var5] && !Block_opaqueCubeLookup[var6]) {
            var9 = 3;
        }
        if (Block_opaqueCubeLookup[var6] && !Block_opaqueCubeLookup[var5]) {
            var9 = 2;
        }
        if (Block_opaqueCubeLookup[var7] && !Block_opaqueCubeLookup[var8]) {
            var9 = 5;
        }
        if (Block_opaqueCubeLookup[var8] && !Block_opaqueCubeLookup[var7]) {
            var9 = 4;
        }
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, var9);
    }
}

static void BlockFurnace_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4) {

    World_setBlockTileEntity(var1, var2, var3, var4, (TileEntity *)TileEntityFurnace_create());
    BlockFurnace_setDefaultDirection(self, var1, var2, var3, var4);
}

static int BlockFurnace_getBlockTexture(Block *self, IBlockAccess *var1, int var2, int var3,
                                        int var4, int var5) {
    BlockFurnace *b = (BlockFurnace *)self;
    if (var5 == 1) {
        return self->blockIndexInTexture + 17;
    } else if (var5 == 0) {
        return self->blockIndexInTexture + 17;
    } else {
        int var6 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
        return var5 != var6
                   ? self->blockIndexInTexture
                   : (b->isActive ? self->blockIndexInTexture + 16 : self->blockIndexInTexture - 1);
    }
}

static void BlockFurnace_randomDisplayTick(Block *self, World *var1, int var2, int var3, int var4,
                                           JavaRandom *var5) {
    BlockFurnace *b = (BlockFurnace *)self;
    if (b->isActive) {
        int var6 = World_getBlockMetadata(var1, var2, var3, var4);
        float var7 = (float)var2 + 0.5f;
        float var8 = (float)var3 + 0.0f + JavaRandom_nextFloat(var5) * 6.0f / 16.0f;
        float var9 = (float)var4 + 0.5f;
        float var10 = 0.52f;
        float var11 = JavaRandom_nextFloat(var5) * 0.6f - 0.3f;
        if (var6 == 4) {
            World_spawnParticle(var1, "smoke", (double)(var7 - var10), (double)var8,
                                (double)(var9 + var11), 0.0, 0.0, 0.0);
            World_spawnParticle(var1, "flame", (double)(var7 - var10), (double)var8,
                                (double)(var9 + var11), 0.0, 0.0, 0.0);
        } else if (var6 == 5) {
            World_spawnParticle(var1, "smoke", (double)(var7 + var10), (double)var8,
                                (double)(var9 + var11), 0.0, 0.0, 0.0);
            World_spawnParticle(var1, "flame", (double)(var7 + var10), (double)var8,
                                (double)(var9 + var11), 0.0, 0.0, 0.0);
        } else if (var6 == 2) {
            World_spawnParticle(var1, "smoke", (double)(var7 + var11), (double)var8,
                                (double)(var9 - var10), 0.0, 0.0, 0.0);
            World_spawnParticle(var1, "flame", (double)(var7 + var11), (double)var8,
                                (double)(var9 - var10), 0.0, 0.0, 0.0);
        } else if (var6 == 3) {
            World_spawnParticle(var1, "smoke", (double)(var7 + var11), (double)var8,
                                (double)(var9 + var10), 0.0, 0.0, 0.0);
            World_spawnParticle(var1, "flame", (double)(var7 + var11), (double)var8,
                                (double)(var9 + var10), 0.0, 0.0, 0.0);
        }
    }
}

static int BlockFurnace_getBlockTextureFromSide(Block *self, int var1) {
    return var1 == 1
               ? self->blockIndexInTexture + 17
               : (var1 == 0
                      ? self->blockIndexInTexture + 17
                      : (var1 == 3 ? self->blockIndexInTexture - 1 : self->blockIndexInTexture));
}

static int BlockFurnace_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                                       EntityPlayer *var5) {
    (void)self;
    if (var1->multiplayerWorld) {
        return 1;
    } else {
        TileEntityFurnace *var6 =
            (TileEntityFurnace *)World_getBlockTileEntity(var1, var2, var3, var4);
        EntityPlayer_displayGUIFurnace(var5, var6);
        return 1;
    }
}

void BlockFurnace_updateFurnaceBlockState(int var0, World *var1, int var2, int var3, int var4) {
    int var5 = World_getBlockMetadata(var1, var2, var3, var4);
    TileEntity *var6 = World_getBlockTileEntity(var1, var2, var3, var4);
    BlockFurnace_keepFurnaceInventory = 1;
    if (var0) {
        World_setBlockWithNotify(var1, var2, var3, var4, Block_stoneOvenActive->blockID);
    } else {
        World_setBlockWithNotify(var1, var2, var3, var4, Block_stoneOvenIdle->blockID);
    }
    BlockFurnace_keepFurnaceInventory = 0;
    World_setBlockMetadataWithNotify(var1, var2, var3, var4, var5);
    TileEntity_validate(var6);
    World_setBlockTileEntity(var1, var2, var3, var4, var6);
}

static void BlockFurnace_onBlockPlacedBy(Block *self, World *var1, int var2, int var3, int var4,
                                         EntityLiving *var5) {
    (void)self;
    int var6 =
        MathHelper_floor_double((double)(((Entity *)var5)->rotationYaw * 4.0f / 360.0f) + 0.5) & 3;
    if (var6 == 0) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 2);
    }
    if (var6 == 1) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 5);
    }
    if (var6 == 2) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 3);
    }
    if (var6 == 3) {
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 4);
    }
}

static void BlockFurnace_onBlockRemoval(Block *self, World *var1, int var2, int var3, int var4) {
    BlockFurnace *b = (BlockFurnace *)self;
    if (!BlockFurnace_keepFurnaceInventory) {
        TileEntityFurnace *var5 =
            (TileEntityFurnace *)World_getBlockTileEntity(var1, var2, var3, var4);
        if (var5) {
            for (int var6 = 0; var6 < FURNACE_SIZE; ++var6) {
                ItemStack *var7 = TileEntityFurnace_getStackInSlot(var5, var6);
                if (var7 != NULL) {
                    float var8 = JavaRandom_nextFloat(&b->furnaceRand) * 0.8f + 0.1f;
                    float var9 = JavaRandom_nextFloat(&b->furnaceRand) * 0.8f + 0.1f;
                    float var10 = JavaRandom_nextFloat(&b->furnaceRand) * 0.8f + 0.1f;
                    while (var7->stackSize > 0) {
                        int var11 = JavaRandom_nextInt(&b->furnaceRand, 21) + 10;
                        if (var11 > var7->stackSize) {
                            var11 = var7->stackSize;
                        }
                        var7->stackSize -= var11;
                        ItemStack newStack = ItemStack_fromItemID(var7->itemID, var11,
                                                                  ItemStack_getItemDamage(var7));
                        EntityItem *var12 = EntityItem_create(
                            var1, (double)((float)var2 + var8), (double)((float)var3 + var9),
                            (double)((float)var4 + var10), newStack);
                        float var13 = 0.05f;
                        var12->base.motionX =
                            (double)((float)JavaRandom_nextGaussian(&b->furnaceRand) * var13);
                        var12->base.motionY =
                            (double)((float)JavaRandom_nextGaussian(&b->furnaceRand) * var13 +
                                     0.2f);
                        var12->base.motionZ =
                            (double)((float)JavaRandom_nextGaussian(&b->furnaceRand) * var13);
                        World_entityJoinedWorld(var1, (Entity *)var12);
                    }
                }
            }
        }
    }

    BlockContainer_onBlockRemoval(self, var1, var2, var3, var4);
}

Block *BlockFurnace_create(int id, int active) {
    static BlockVtable vtable_idle;
    static BlockVtable vtable_active;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockContainer_initVtable(&vtable_idle);
        vtable_idle.idDropped = BlockFurnace_idDropped;
        vtable_idle.onBlockAdded = BlockFurnace_onBlockAdded;
        vtable_idle.getBlockTexture = BlockFurnace_getBlockTexture;
        vtable_idle.randomDisplayTick = BlockFurnace_randomDisplayTick;
        vtable_idle.getBlockTextureFromSide = BlockFurnace_getBlockTextureFromSide;
        vtable_idle.blockActivated = BlockFurnace_blockActivated;
        vtable_idle.onBlockPlacedBy = BlockFurnace_onBlockPlacedBy;
        vtable_idle.onBlockRemoval = BlockFurnace_onBlockRemoval;

        vtable_active = vtable_idle;
        vtable_init = 1;
    }
    BlockFurnace *self = (BlockFurnace *)calloc(1, sizeof(BlockFurnace));
    self->isActive = active;
    self->base.vtable = active ? &vtable_active : &vtable_idle;

    BlockContainer_constructWithTexture(&self->base, id, 45, Material_rock);
    JavaRandom_init(&self->furnaceRand, (int64_t)time(NULL));
    return &self->base;
}
