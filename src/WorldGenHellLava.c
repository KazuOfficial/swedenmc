#include "WorldGenHellLava.h"
#include "Block.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int WorldGenHellLava_generate(WorldGenerator *base, World *var1, JavaRandom *var2, int var3,
                                     int var4, int var5) {
    (void)var2;
    WorldGenHellLava *self = (WorldGenHellLava *)base;
    if (World_getBlockId(var1, var3, var4 + 1, var5) != Block_netherrack->blockID)
        return 0;
    if (World_getBlockId(var1, var3, var4, var5) != 0 &&
        World_getBlockId(var1, var3, var4, var5) != Block_netherrack->blockID)
        return 0;

    int var6 = 0;
    if (World_getBlockId(var1, var3 - 1, var4, var5) == Block_netherrack->blockID)
        ++var6;
    if (World_getBlockId(var1, var3 + 1, var4, var5) == Block_netherrack->blockID)
        ++var6;
    if (World_getBlockId(var1, var3, var4, var5 - 1) == Block_netherrack->blockID)
        ++var6;
    if (World_getBlockId(var1, var3, var4, var5 + 1) == Block_netherrack->blockID)
        ++var6;
    if (World_getBlockId(var1, var3, var4 - 1, var5) == Block_netherrack->blockID)
        ++var6;

    int var7 = 0;
    if (World_isAirBlock(var1, var3 - 1, var4, var5))
        ++var7;
    if (World_isAirBlock(var1, var3 + 1, var4, var5))
        ++var7;
    if (World_isAirBlock(var1, var3, var4, var5 - 1))
        ++var7;
    if (World_isAirBlock(var1, var3, var4, var5 + 1))
        ++var7;
    if (World_isAirBlock(var1, var3, var4 - 1, var5))
        ++var7;

    if (var6 == 4 && var7 == 1) {
        World_setBlockWithNotify(var1, var3, var4, var5, self->field_4158_a);
        var1->scheduledUpdatesAreImmediate = 1;
        Block_updateTick(Block_blocksList[self->field_4158_a], var1, var3, var4, var5, var2);
        var1->scheduledUpdatesAreImmediate = 0;
    }
    return 1;
}

static WorldGeneratorVtable WorldGenHellLava_vtable_inst = {
    WorldGenHellLava_generate,
    WorldGenerator_noop_func_517_a,
    WorldGenerator_simple_destroy,
};

WorldGenHellLava *WorldGenHellLava_create(int blockId) {
    WorldGenHellLava *self = (WorldGenHellLava *)calloc(1, sizeof(WorldGenHellLava));
    self->base.vtable = &WorldGenHellLava_vtable_inst;
    self->field_4158_a = blockId;
    return self;
}
