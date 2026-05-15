#include "Block.h"
#include "IBlockAccess.h"
#include "Material.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockLockedChest_getBlockTexture(Block *self, IBlockAccess *var1, int var2, int var3,
                                            int var4, int var5) {
    if (var5 == 1)
        return self->blockIndexInTexture - 1;
    if (var5 == 0)
        return self->blockIndexInTexture - 1;
    int var6 = var1->vtable->getBlockId(var1, var2, var3, var4 - 1);
    int var7 = var1->vtable->getBlockId(var1, var2, var3, var4 + 1);
    int var8 = var1->vtable->getBlockId(var1, var2 - 1, var3, var4);
    int var9 = var1->vtable->getBlockId(var1, var2 + 1, var3, var4);
    int var10 = 3;
    if (Block_opaqueCubeLookup[var6] && !Block_opaqueCubeLookup[var7])
        var10 = 3;
    if (Block_opaqueCubeLookup[var7] && !Block_opaqueCubeLookup[var6])
        var10 = 2;
    if (Block_opaqueCubeLookup[var8] && !Block_opaqueCubeLookup[var9])
        var10 = 5;
    if (Block_opaqueCubeLookup[var9] && !Block_opaqueCubeLookup[var8])
        var10 = 4;
    return var5 == var10 ? self->blockIndexInTexture + 1 : self->blockIndexInTexture;
}

static int BlockLockedChest_getBlockTextureFromSide(Block *self, int side) {
    if (side == 1)
        return self->blockIndexInTexture - 1;
    if (side == 0)
        return self->blockIndexInTexture - 1;
    if (side == 3)
        return self->blockIndexInTexture + 1;
    return self->blockIndexInTexture;
}

static int BlockLockedChest_canPlaceBlockAt(Block *self, World *var1, int var2, int var3,
                                            int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return 1;
}

static void BlockLockedChest_updateTick(Block *self, World *var1, int var2, int var3, int var4,
                                        JavaRandom *var5) {
    (void)self;
    (void)var5;
    World_setBlockWithNotify(var1, var2, var3, var4, 0);
}

Block *BlockLockedChest_create(int id) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getBlockTexture = BlockLockedChest_getBlockTexture;
        vtable.getBlockTextureFromSide = BlockLockedChest_getBlockTextureFromSide;
        vtable.canPlaceBlockAt = BlockLockedChest_canPlaceBlockAt;
        vtable.updateTick = BlockLockedChest_updateTick;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_construct(self, id, Material_wood);
    self->blockIndexInTexture = 26;
    return self;
}
