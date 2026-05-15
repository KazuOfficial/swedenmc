#include "BlockJukeBox.h"
#include "BlockContainer.h"
#include "Material.h"
#include "JavaRandom.h"
#include "World.h"
#include "ItemStack.h"
#include "EntityItem.h"
#include "Entity.h"
#include "TileEntity.h"
#include "TileEntityRecordPlayer.h"
#include <stdlib.h>

typedef struct {
    Block base;
} BlockJukeBox;

static int BlockJukeBox_getBlockTextureFromSide(Block *self, int side) {
    return self->blockIndexInTexture + (side == 1 ? 1 : 0);
}

static void BlockJukeBox_func_28038_b_(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    if (!var1->multiplayerWorld) {
        TileEntityRecordPlayer *var5 =
            (TileEntityRecordPlayer *)World_getBlockTileEntity(var1, var2, var3, var4);
        int var6 = var5->record;
        if (var6 != 0) {
            World_func_28106_e(var1, 1005, var2, var3, var4, 0);
            World_playRecord(var1, NULL, var2, var3, var4);
            var5->record = 0;
            TileEntity_onInventoryChanged((TileEntity *)var5);
            World_setBlockMetadataWithNotify(var1, var2, var3, var4, 0);
            float var8 = 0.7f;
            double var9 =
                (double)(JavaRandom_nextFloat(&var1->rand) * var8) + (double)(1.0f - var8) * 0.5;
            double var11 = (double)(JavaRandom_nextFloat(&var1->rand) * var8) +
                           (double)(1.0f - var8) * 0.2 + 0.6;
            double var13 =
                (double)(JavaRandom_nextFloat(&var1->rand) * var8) + (double)(1.0f - var8) * 0.5;
            ItemStack is = ItemStack_fromItemID(var6, 1, 0);
            EntityItem *var15 = EntityItem_create(var1, (double)var2 + var9, (double)var3 + var11,
                                                  (double)var4 + var13, is);
            var15->delayBeforeCanPickup = 10;
            World_entityJoinedWorld(var1, (Entity *)var15);
        }
    }
}

static int BlockJukeBox_blockActivated(Block *self, World *var1, int var2, int var3, int var4,
                                       EntityPlayer *var5) {
    (void)var5;
    if (World_getBlockMetadata(var1, var2, var3, var4) == 0) {
        return 0;
    } else {
        BlockJukeBox_func_28038_b_(self, var1, var2, var3, var4);
        return 1;
    }
}

void BlockJukeBox_ejectRecord(Block *self, World *var1, int var2, int var3, int var4, int var5) {
    (void)self;
    if (!var1->multiplayerWorld) {
        TileEntityRecordPlayer *var6 =
            (TileEntityRecordPlayer *)World_getBlockTileEntity(var1, var2, var3, var4);
        var6->record = var5;
        TileEntity_onInventoryChanged((TileEntity *)var6);
        World_setBlockMetadataWithNotify(var1, var2, var3, var4, 1);
    }
}

static void BlockJukeBox_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    (void)self;
    World_setBlockTileEntity(world, x, y, z, (TileEntity *)TileEntityRecordPlayer_create());
}

static void BlockJukeBox_onBlockRemoval(Block *self, World *var1, int var2, int var3, int var4) {
    BlockJukeBox_func_28038_b_(self, var1, var2, var3, var4);
    BlockContainer_onBlockRemoval(self, var1, var2, var3, var4);
}

static void BlockJukeBox_dropBlockAsItemWithChance(Block *self, World *var1, int var2, int var3,
                                                   int var4, int var5, float var6) {
    if (!var1->multiplayerWorld) {
        Block_defaultVtable.dropBlockAsItemWithChance(self, var1, var2, var3, var4, var5, var6);
    }
}

Block *BlockJukeBox_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockContainer_initVtable(&vtable);
        vtable.getBlockTextureFromSide = BlockJukeBox_getBlockTextureFromSide;
        vtable.onBlockAdded = BlockJukeBox_onBlockAdded;
        vtable.blockActivated = BlockJukeBox_blockActivated;
        vtable.onBlockRemoval = BlockJukeBox_onBlockRemoval;
        vtable.dropBlockAsItemWithChance = BlockJukeBox_dropBlockAsItemWithChance;
        vtable_init = 1;
    }
    BlockJukeBox *self = (BlockJukeBox *)calloc(1, sizeof(BlockJukeBox));
    self->base.vtable = &vtable;
    BlockContainer_constructWithTexture(&self->base, id, tex, Material_wood);
    return &self->base;
}
