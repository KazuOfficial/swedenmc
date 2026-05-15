#include "Block.h"
#include "Material.h"
#include "World.h"
#include "EntityLiving.h"
#include "MathHelper.h"
#include <stdlib.h>

typedef struct {
    Block base;
    int blockType;
} BlockPumpkin;

static int BlockPumpkin_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    BlockPumpkin *b = (BlockPumpkin *)self;
    if (var1 == 1)
        return self->blockIndexInTexture;
    if (var1 == 0)
        return self->blockIndexInTexture;
    int var3 = self->blockIndexInTexture + 1 + 16;
    if (b->blockType)
        ++var3;
    if (var2 == 2 && var1 == 2)
        return var3;
    if (var2 == 3 && var1 == 5)
        return var3;
    if (var2 == 0 && var1 == 3)
        return var3;
    if (var2 == 1 && var1 == 4)
        return var3;
    return self->blockIndexInTexture + 16;
}

static int BlockPumpkin_getBlockTextureFromSide(Block *self, int var1) {
    if (var1 == 1)
        return self->blockIndexInTexture;
    if (var1 == 0)
        return self->blockIndexInTexture;
    if (var1 == 3)
        return self->blockIndexInTexture + 1 + 16;
    return self->blockIndexInTexture + 16;
}

static int BlockPumpkin_canPlaceBlockAt(Block *self, World *var1, int var2, int var3, int var4) {
    (void)self;
    int var5 = World_getBlockId(var1, var2, var3, var4);
    if (!(var5 == 0 || (Block_blocksList[var5] &&
                        Material_getIsGroundCover(Block_blocksList[var5]->blockMaterial))))
        return 0;
    return World_isBlockNormalCube(var1, var2, var3 - 1, var4);
}

static void BlockPumpkin_onBlockPlacedBy(Block *self, World *var1, int var2, int var3, int var4,
                                         EntityLiving *var5) {
    (void)self;
    int var6 = MathHelper_floor_double((double)(var5->base.rotationYaw * 4.0f / 360.0f) + 2.5) & 3;
    World_setBlockMetadataWithNotify(var1, var2, var3, var4, var6);
}

Block *BlockPumpkin_create(int id, int tex, int blockType) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getBlockTextureFromSideAndMetadata = BlockPumpkin_getBlockTextureFromSideAndMetadata;
        vtable.getBlockTextureFromSide = BlockPumpkin_getBlockTextureFromSide;
        vtable.canPlaceBlockAt = BlockPumpkin_canPlaceBlockAt;
        vtable.onBlockPlacedBy = BlockPumpkin_onBlockPlacedBy;
        vtable_init = 1;
    }
    BlockPumpkin *self = (BlockPumpkin *)calloc(1, sizeof(BlockPumpkin));
    self->blockType = blockType;
    self->base.vtable = &vtable;
    Block_constructWithTexture(&self->base, id, tex, Material_pumpkin);
    Block_setTickOnLoad(&self->base, 1);
    return &self->base;
}
