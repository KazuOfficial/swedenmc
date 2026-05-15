#include "Block.h"
#include "IBlockAccess.h"
#include "Material.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "ColorizerGrass.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockGrass_getBlockTexture(Block *self, IBlockAccess *world, int x, int y, int z,
                                      int side) {
    (void)self;
    if (side == 1)
        return 0;
    if (side == 0)
        return 2;
    Material *var6 = world->vtable->getBlockMaterial(world, x, y + 1, z);
    return (var6 != Material_snow && var6 != Material_builtSnow) ? 3 : 68;
}

static int BlockGrass_colorMultiplier(Block *self, IBlockAccess *world, int x, int y, int z) {
    (void)self;
    (void)y;
    WorldChunkManager *wcm = world->vtable->getWorldChunkManager(world);
    double temperature, humidity;
    WorldChunkManager_getSingleBiomeData(wcm, x, z, &temperature, &humidity);
    return ColorizerGrass_getGrassColor(temperature, humidity);
}

static void BlockGrass_updateTick(Block *self, World *world, int x, int y, int z,
                                  JavaRandom *rand) {
    (void)self;
    if (world->multiplayerWorld)
        return;
    if (World_getBlockLightValue(world, x, y + 1, z) < 4 &&
        Block_lightOpacity[World_getBlockId(world, x, y + 1, z)] > 2) {
        if (JavaRandom_nextInt(rand, 4) != 0)
            return;
        World_setBlockWithNotify(world, x, y, z, Block_dirt->blockID);
    } else if (World_getBlockLightValue(world, x, y + 1, z) >= 9) {
        int var6 = x + JavaRandom_nextInt(rand, 3) - 1;
        int var7 = y + JavaRandom_nextInt(rand, 5) - 3;
        int var8 = z + JavaRandom_nextInt(rand, 3) - 1;
        int var9 = World_getBlockId(world, var6, var7 + 1, var8);
        if (World_getBlockId(world, var6, var7, var8) == Block_dirt->blockID &&
            World_getBlockLightValue(world, var6, var7 + 1, var8) >= 4 &&
            Block_lightOpacity[var9] <= 2) {
            World_setBlockWithNotify(world, var6, var7, var8, Block_grass->blockID);
        }
    }
}

static int BlockGrass_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    return Block_dirt->vtable->idDropped(Block_dirt, 0, rand);
}

Block *BlockGrass_create(int id) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getBlockTexture = BlockGrass_getBlockTexture;
        vtable.colorMultiplier = BlockGrass_colorMultiplier;
        vtable.updateTick = BlockGrass_updateTick;
        vtable.idDropped = BlockGrass_idDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_construct(self, id, Material_grassMaterial);
    self->blockIndexInTexture = 3;
    Block_setTickOnLoad(self, 1);
    return self;
}
