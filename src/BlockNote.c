#include "Block.h"
#include "BlockContainer.h"
#include "Material.h"
#include "JavaRandom.h"
#include "World.h"
#include "TileEntityNote.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static void BlockNote_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    (void)self;
    World_setBlockTileEntity(world, x, y, z, (TileEntity *)TileEntityNote_create());
}

static int BlockNote_blockActivated(Block *self, World *world, int x, int y, int z,
                                    EntityPlayer *player) {
    (void)self;
    (void)player;
    if (world->isRemote)
        return 1;
    TileEntityNote *te = (TileEntityNote *)World_getBlockTileEntity(world, x, y, z);
    if (te) {
        TileEntityNote_changePitch(te);
        TileEntityNote_triggerNote(te, world, x, y, z);
    }
    return 1;
}

static void BlockNote_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                            int neighborID) {
    (void)self;
    if (neighborID > 0 && Block_blocksList[neighborID] != NULL &&
        Block_blocksList[neighborID]->vtable->canProvidePower(Block_blocksList[neighborID])) {
        int powered = World_isBlockGettingPowered(world, x, y, z);
        TileEntityNote *te = (TileEntityNote *)World_getBlockTileEntity(world, x, y, z);
        if (te && (int)te->previousRedstoneState != powered) {
            if (powered)
                TileEntityNote_triggerNote(te, world, x, y, z);
            te->previousRedstoneState = powered;
        }
    }
}

static void BlockNote_onBlockClicked(Block *self, World *world, int x, int y, int z,
                                     EntityPlayer *player) {
    (void)self;
    (void)player;
    if (!world->isRemote) {
        TileEntityNote *te = (TileEntityNote *)World_getBlockTileEntity(world, x, y, z);
        if (te)
            TileEntityNote_triggerNote(te, world, x, y, z);
    }
}

static void BlockNote_playBlock(Block *self, World *world, int x, int y, int z, int instrument,
                                int note) {
    (void)self;
    float pitch = (float)pow(2.0, (double)(note - 12) / 12.0);
    const char *name = "harp";
    if (instrument == 1)
        name = "bd";
    if (instrument == 2)
        name = "snare";
    if (instrument == 3)
        name = "hat";
    if (instrument == 4)
        name = "bassattack";
    char sound[32];
    snprintf(sound, sizeof(sound), "note.%s", name);
    World_playSoundEffect(world, (double)x + 0.5, (double)y + 0.5, (double)z + 0.5, sound, 3.0f,
                          pitch);
    World_spawnParticle(world, "note", (double)x + 0.5, (double)y + 1.2, (double)z + 0.5,
                        (double)note / 24.0, 0.0, 0.0);
}

Block *BlockNote_create(int id) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockContainer_initVtable(&vtable);
        vtable.onBlockAdded = BlockNote_onBlockAdded;
        vtable.blockActivated = BlockNote_blockActivated;
        vtable.onNeighborBlockChange = BlockNote_onNeighborBlockChange;
        vtable.onBlockClicked = BlockNote_onBlockClicked;
        vtable.playBlock = BlockNote_playBlock;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    BlockContainer_constructWithTexture(self, id, 74, Material_wood);
    return self;
}
