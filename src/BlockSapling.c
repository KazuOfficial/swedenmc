#include "Block.h"
#include "BlockFlower.h"
#include "Material.h"
#include "JavaRandom.h"
#include "World.h"
#include "WorldGenTrees.h"
#include "WorldGenBigTree.h"
#include "WorldGenForest.h"
#include "WorldGenTaiga2.h"
#include "WorldGenerator.h"
#include <stdlib.h>

static int BlockSapling_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta) {
    (void)self;
    (void)side;
    int m = meta & 3;
    if (m == 1)
        return 63;
    if (m == 2)
        return 79;
    return Block_defaultVtable.getBlockTextureFromSideAndMetadata(self, side, meta);
}

void BlockSapling_growTree(Block *self, World *var1, int var2, int var3, int var4,
                           JavaRandom *var5) {
    int var6 = World_getBlockMetadata(var1, var2, var3, var4) & 3;
    World_setBlock(var1, var2, var3, var4, 0);
    WorldGenerator *var7 = NULL;
    if (var6 == 1) {
        var7 = (WorldGenerator *)WorldGenTaiga2_create();
    } else if (var6 == 2) {
        var7 = (WorldGenerator *)WorldGenForest_create();
    } else {
        var7 = (WorldGenerator *)WorldGenTrees_create();
        if (JavaRandom_nextInt(var5, 10) == 0) {
            var7 = (WorldGenerator *)WorldGenBigTree_create();
        }
    }
    if (!WorldGenerator_generate(var7, var1, var5, var2, var3, var4)) {
        World_setBlockAndMetadata(var1, var2, var3, var4, self->blockID, var6);
    }
}

static void BlockSapling_updateTick(Block *self, World *world, int x, int y, int z,
                                    JavaRandom *rand) {
    if (!world->multiplayerWorld) {
        BlockFlower_updateTick(self, world, x, y, z, rand);
        if (World_getBlockLightValue(world, x, y + 1, z) >= 9 &&
            JavaRandom_nextInt(rand, 30) == 0) {
            int var6 = World_getBlockMetadata(world, x, y, z);
            if ((var6 & 8) == 0) {
                World_setBlockMetadataWithNotify(world, x, y, z, var6 | 8);
            } else {
                BlockSapling_growTree(self, world, x, y, z, rand);
            }
        }
    }
}

static int BlockSapling_damageDropped(Block *self, int meta) {
    (void)self;
    return meta & 3;
}

Block *BlockSapling_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockFlower_initVtable(&vtable);
        vtable.getBlockTextureFromSideAndMetadata = BlockSapling_getBlockTextureFromSideAndMetadata;
        vtable.updateTick = BlockSapling_updateTick;
        vtable.damageDropped = BlockSapling_damageDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_construct(self, id, Material_plants);
    self->blockIndexInTexture = tex;
    Block_setTickOnLoad(self, 1);
    float var3 = 0.4f;
    Block_setBlockBounds(self, 0.5f - var3, 0.0f, 0.5f - var3, 0.5f + var3, var3 * 2.0f,
                         0.5f + var3);
    return self;
}
