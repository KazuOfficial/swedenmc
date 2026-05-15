#include "Block.h"
#include "Material.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

typedef struct {
    Block base;
    int glowing;
} BlockRedstoneOre;

static int BlockRedstoneOre_tickRate(Block *self) {
    (void)self;
    return 30;
}

static void BlockRedstoneOre_func_319_i(World *var1, int var2, int var3, int var4) {
    JavaRandom *var5 = &var1->rand;
    double var6 = 1.0 / 16.0;
    int var8;
    for (var8 = 0; var8 < 6; ++var8) {
        double var9 = (double)((float)var2 + JavaRandom_nextFloat(var5));
        double var11 = (double)((float)var3 + JavaRandom_nextFloat(var5));
        double var13 = (double)((float)var4 + JavaRandom_nextFloat(var5));
        if (var8 == 0 && !World_isBlockOpaqueCube(var1, var2, var3 + 1, var4)) {
            var11 = (double)(var3 + 1) + var6;
        }
        if (var8 == 1 && !World_isBlockOpaqueCube(var1, var2, var3 - 1, var4)) {
            var11 = (double)(var3 + 0) - var6;
        }
        if (var8 == 2 && !World_isBlockOpaqueCube(var1, var2, var3, var4 + 1)) {
            var13 = (double)(var4 + 1) + var6;
        }
        if (var8 == 3 && !World_isBlockOpaqueCube(var1, var2, var3, var4 - 1)) {
            var13 = (double)(var4 + 0) - var6;
        }
        if (var8 == 4 && !World_isBlockOpaqueCube(var1, var2 + 1, var3, var4)) {
            var9 = (double)(var2 + 1) + var6;
        }
        if (var8 == 5 && !World_isBlockOpaqueCube(var1, var2 - 1, var3, var4)) {
            var9 = (double)(var2 + 0) - var6;
        }
        if (var9 < (double)var2 || var9 > (double)(var2 + 1) || var11 < 0.0 ||
            var11 > (double)(var3 + 1) || var13 < (double)var4 || var13 > (double)(var4 + 1)) {
            World_spawnParticle(var1, "reddust", var9, var11, var13, 0.0, 0.0, 0.0);
        }
    }
}

static void BlockRedstoneOre_func_320_h(Block *self, World *var1, int var2, int var3, int var4) {
    BlockRedstoneOre_func_319_i(var1, var2, var3, var4);
    if (self->blockID == Block_oreRedstone->blockID) {
        World_setBlockWithNotify(var1, var2, var3, var4, Block_oreRedstoneGlowing->blockID);
    }
}

static void BlockRedstoneOre_onBlockClicked(Block *self, World *world, int x, int y, int z,
                                            EntityPlayer *player) {
    BlockRedstoneOre_func_320_h(self, world, x, y, z);
    Block_defaultVtable.onBlockClicked(self, world, x, y, z, player);
}

static void BlockRedstoneOre_onEntityWalking(Block *self, World *world, int x, int y, int z,
                                             Entity *entity) {
    BlockRedstoneOre_func_320_h(self, world, x, y, z);
    Block_defaultVtable.onEntityWalking(self, world, x, y, z, entity);
}

static int BlockRedstoneOre_blockActivated(Block *self, World *world, int x, int y, int z,
                                           EntityPlayer *player) {
    BlockRedstoneOre_func_320_h(self, world, x, y, z);
    return Block_defaultVtable.blockActivated(self, world, x, y, z, player);
}

static void BlockRedstoneOre_updateTick(Block *self, World *world, int x, int y, int z,
                                        JavaRandom *rand) {
    (void)rand;
    if (self->blockID == Block_oreRedstoneGlowing->blockID) {
        World_setBlockWithNotify(world, x, y, z, Block_oreRedstone->blockID);
    }
}

static int BlockRedstoneOre_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return 331;
}

static int BlockRedstoneOre_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    return 4 + JavaRandom_nextInt(rand, 2);
}

static void BlockRedstoneOre_randomDisplayTick(Block *self, World *world, int x, int y, int z,
                                               JavaRandom *rand) {
    BlockRedstoneOre *b = (BlockRedstoneOre *)self;
    (void)rand;
    if (b->glowing) {
        BlockRedstoneOre_func_319_i(world, x, y, z);
    }
}

Block *BlockRedstoneOre_create(int id, int tex, int glowing) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.tickRate = BlockRedstoneOre_tickRate;
        vtable.idDropped = BlockRedstoneOre_idDropped;
        vtable.quantityDropped = BlockRedstoneOre_quantityDropped;
        vtable.onBlockClicked = BlockRedstoneOre_onBlockClicked;
        vtable.onEntityWalking = BlockRedstoneOre_onEntityWalking;
        vtable.blockActivated = BlockRedstoneOre_blockActivated;
        vtable.updateTick = BlockRedstoneOre_updateTick;
        vtable.randomDisplayTick = BlockRedstoneOre_randomDisplayTick;
        vtable_init = 1;
    }
    BlockRedstoneOre *self = (BlockRedstoneOre *)calloc(1, sizeof(BlockRedstoneOre));
    self->glowing = glowing;
    self->base.vtable = &vtable;
    Block_constructWithTexture(&self->base, id, tex, Material_rock);
    if (glowing) {
        Block_setTickOnLoad(&self->base, 1);
    }
    return &self->base;
}
