#include "Block.h"
#include "Material.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockLog_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta) {
    (void)self;
    if (side == 1 || side == 0)
        return 21;
    if (meta == 1)
        return 116;
    if (meta == 2)
        return 117;
    return 20;
}

static int BlockLog_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return Block_wood->blockID;
}

static int BlockLog_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    (void)rand;
    return 1;
}

static int BlockLog_damageDropped(Block *self, int meta) {
    (void)self;
    return meta;
}

static void BlockLog_onBlockRemoval(Block *self, World *world, int x, int y, int z) {
    (void)self;
    int var5 = 4;
    int var6 = var5 + 1;
    if (World_checkChunksExist(world, x - var6, y - var6, z - var6, x + var6, y + var6, z + var6)) {
        for (int var7 = -var5; var7 <= var5; ++var7) {
            for (int var8 = -var5; var8 <= var5; ++var8) {
                for (int var9 = -var5; var9 <= var5; ++var9) {
                    int var10 = World_getBlockId(world, x + var7, y + var8, z + var9);
                    if (var10 == Block_leaves->blockID) {
                        int var11 = World_getBlockMetadata(world, x + var7, y + var8, z + var9);
                        if ((var11 & 8) == 0) {
                            World_setBlockMetadata(world, x + var7, y + var8, z + var9, var11 | 8);
                        }
                    }
                }
            }
        }
    }
}

Block *BlockLog_create(int id) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getBlockTextureFromSideAndMetadata = BlockLog_getBlockTextureFromSideAndMetadata;
        vtable.idDropped = BlockLog_idDropped;
        vtable.quantityDropped = BlockLog_quantityDropped;
        vtable.damageDropped = BlockLog_damageDropped;
        vtable.onBlockRemoval = BlockLog_onBlockRemoval;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_construct(self, id, Material_wood);
    self->blockIndexInTexture = 20;
    return self;
}
