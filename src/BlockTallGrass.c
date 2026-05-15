#include "Block.h"
#include "BlockFlower.h"
#include "Material.h"
#include "Item.h"
#include "IBlockAccess.h"
#include "WorldChunkManager.h"
#include "ColorizerGrass.h"
#include "JavaRandom.h"
#include <stdint.h>
#include <stdlib.h>

static int BlockTallGrass_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    (void)var1;
    return var2 == 1
               ? self->blockIndexInTexture
               : (var2 == 2
                      ? self->blockIndexInTexture + 16 + 1
                      : (var2 == 0 ? self->blockIndexInTexture + 16 : self->blockIndexInTexture));
}

static int BlockTallGrass_colorMultiplier(Block *self, IBlockAccess *var1, int var2, int var3,
                                          int var4) {
    (void)self;
    int var5 = var1->vtable->getBlockMetadata(var1, var2, var3, var4);
    if (var5 == 0) {
        return 16777215;
    } else {
        long var6 = (long)(int)((unsigned int)var2 * 3129871U + (unsigned int)var4 * 6129781U +
                                (unsigned int)var3);
        var6 = (long)(((uint64_t)var6 * (uint64_t)var6 * 42317861ULL) + ((uint64_t)var6 * 11ULL));
        var2 = (int)((long)var2 + (var6 >> 14 & 31L));
        var3 = (int)((long)var3 + (var6 >> 19 & 31L));
        var4 = (int)((long)var4 + (var6 >> 24 & 31L));
        WorldChunkManager *wcm = var1->vtable->getWorldChunkManager(var1);
        double var8, var10;
        WorldChunkManager_getSingleBiomeData(wcm, var2, var4, &var8, &var10);
        return ColorizerGrass_getGrassColor(var8, var10);
    }
}

static int BlockTallGrass_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    return JavaRandom_nextInt(var2, 8) == 0 ? Item_seeds->shiftedIndex : -1;
}

Block *BlockTallGrass_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockFlower_initVtable(&vtable);
        vtable.getBlockTextureFromSideAndMetadata =
            BlockTallGrass_getBlockTextureFromSideAndMetadata;
        vtable.colorMultiplier = BlockTallGrass_colorMultiplier;
        vtable.idDropped = BlockTallGrass_idDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_plants);

    float var3 = 0.4f;
    Block_setBlockBounds(self, 0.5f - var3, 0.0f, 0.5f - var3, 0.5f + var3, 0.8f, 0.5f + var3);
    return self;
}
