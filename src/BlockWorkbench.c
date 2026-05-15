#include "Block.h"
#include "Material.h"
#include "World.h"
#include "EntityPlayer.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockWorkbench_getBlockTextureFromSide(Block *self, int side) {
    if (side == 1)
        return self->blockIndexInTexture - 16;
    if (side == 0)
        return Block_planks->vtable->getBlockTextureFromSide(Block_planks, 0);
    if (side == 2 || side == 4)
        return self->blockIndexInTexture + 1;
    return self->blockIndexInTexture;
}

static int BlockWorkbench_blockActivated(Block *self, World *world, int x, int y, int z,
                                         EntityPlayer *player) {
    (void)self;
    if (world->multiplayerWorld) {
        return 1;
    } else {
        EntityPlayer_displayWorkbenchGUI(player, x, y, z);
        return 1;
    }
}

Block *BlockWorkbench_create(int id) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getBlockTextureFromSide = BlockWorkbench_getBlockTextureFromSide;
        vtable.blockActivated = BlockWorkbench_blockActivated;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;

    Block_constructWithTexture(self, id, 59, Material_wood);
    return self;
}
