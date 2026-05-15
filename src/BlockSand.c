#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include "World.h"
#include "EntityFallingSand.h"
#include <stdlib.h>

int BlockSand_fallInstantly = 0;

static int BlockSand_tickRate(Block *self) {
    (void)self;
    return 3;
}

static void BlockSand_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    world->vtable->scheduleBlockUpdate(world, x, y, z, self->blockID, self->vtable->tickRate(self));
}

static void BlockSand_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                            int neighborID) {
    (void)neighborID;
    world->vtable->scheduleBlockUpdate(world, x, y, z, self->blockID, self->vtable->tickRate(self));
}

static void BlockSand_tryToFall(Block *self, World *world, int x, int y, int z) {
    if (BlockSand_canFallBelow(world, x, y - 1, z) && y >= 0) {
        int var8 = 32;
        if (!BlockSand_fallInstantly && World_checkChunksExist(world, x - var8, y - var8, z - var8,
                                                               x + var8, y + var8, z + var8)) {
            EntityFallingSand *var9 = (EntityFallingSand *)calloc(1, sizeof(EntityFallingSand));
            EntityFallingSand_construct2(var9, world, (double)((float)x + 0.5F),
                                         (double)((float)y + 0.5F), (double)((float)z + 0.5F),
                                         self->blockID);
            World_entityJoinedWorld(world, (Entity *)var9);
        } else {
            World_setBlockWithNotify(world, x, y, z, 0);

            while (BlockSand_canFallBelow(world, x, y - 1, z) && y > 0) {
                --y;
            }

            if (y > 0) {
                World_setBlockWithNotify(world, x, y, z, self->blockID);
            }
        }
    }
}

static void BlockSand_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand) {
    (void)rand;
    BlockSand_tryToFall(self, world, x, y, z);
}

int BlockSand_canFallBelow(World *var0, int var1, int var2, int var3) {
    int var4 = World_getBlockId(var0, var1, var2, var3);
    if (var4 == 0) {
        return 1;
    }
    if (var4 == Block_fire->blockID) {
        return 1;
    }
    Material *var5 = Block_blocksList[var4]->blockMaterial;
    return var5 == Material_water || var5 == Material_lava;
}

BlockVtable BlockSand_vtable;
static int BlockSand_vtable_init = 0;

void BlockSand_initVtable(void) {
    if (BlockSand_vtable_init)
        return;
    BlockSand_vtable = Block_defaultVtable;
    BlockSand_vtable.tickRate = BlockSand_tickRate;
    BlockSand_vtable.onBlockAdded = BlockSand_onBlockAdded;
    BlockSand_vtable.onNeighborBlockChange = BlockSand_onNeighborBlockChange;
    BlockSand_vtable.updateTick = BlockSand_updateTick;
    BlockSand_vtable_init = 1;
}

Block *BlockSand_create(int id, int tex) {
    BlockSand_initVtable();
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &BlockSand_vtable;
    Block_constructWithTexture(self, id, tex, Material_sand);
    return self;
}
