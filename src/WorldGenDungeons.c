#include "WorldGenDungeons.h"
#include "TileEntityMobSpawner.h"
#include "TileEntityChest.h"
#include <stdlib.h>
#include <string.h>

#define BLOCK_AIR 0
#define BLOCK_COBBLESTONE 4
#define BLOCK_MOSSY_COBBLE 48
#define BLOCK_CHEST 54
#define BLOCK_MOB_SPAWNER 52

static const char *pick_mob(JavaRandom *rand) {
    switch (JavaRandom_nextInt(rand, 4)) {
    case 0:
        return "Skeleton";
    case 1:
    case 2:
        return "Zombie";
    default:
        return "Spider";
    }
}

static void fill_chest(TileEntityChest *chest, JavaRandom *rand) {
    for (int i = 0; i < 8; i++) {
        int roll = JavaRandom_nextInt(rand, 11);
        int itemId = 0, count = 1, meta = 0;
        switch (roll) {
        case 0:
            itemId = 329;
            break;
        case 1:
            itemId = 265;
            count = JavaRandom_nextInt(rand, 4) + 1;
            break;
        case 2:
            itemId = 297;
            break;
        case 3:
            itemId = 296;
            count = JavaRandom_nextInt(rand, 4) + 1;
            break;
        case 4:
            itemId = 289;
            count = JavaRandom_nextInt(rand, 4) + 1;
            break;
        case 5:
            itemId = 287;
            count = JavaRandom_nextInt(rand, 4) + 1;
            break;
        case 6:
            itemId = 325;
            break;
        case 7:
            if (JavaRandom_nextInt(rand, 100) == 0)
                itemId = 322;
            break;
        case 8:
            if (JavaRandom_nextInt(rand, 2) == 0) {
                itemId = 331;
                count = JavaRandom_nextInt(rand, 4) + 1;
            }
            break;
        case 9:
            if (JavaRandom_nextInt(rand, 10) == 0) {
                itemId = 2256 + JavaRandom_nextInt(rand, 2);
            }
            break;
        case 10:
            itemId = 351;
            count = 1;
            meta = 3;
            break;
        }
        if (itemId == 0)
            continue;
        int slot = JavaRandom_nextInt(rand, CHEST_SIZE);
        ItemStack s = {.itemID = itemId, .stackSize = count, .itemDamage = meta};
        chest->chestContents[slot] = s;
        chest->chestContentsHas[slot] = 1;
    }
}

static int dungeons_generate(WorldGenerator *base, World *world, JavaRandom *rand, int x, int y,
                             int z) {
    (void)base;
    int height = 3;
    int rw = JavaRandom_nextInt(rand, 2) + 2;
    int rd = JavaRandom_nextInt(rand, 2) + 2;
    int openings = 0;

    for (int bx = x - rw - 1; bx <= x + rw + 1; bx++)
        for (int by = y - 1; by <= y + height + 1; by++)
            for (int bz = z - rd - 1; bz <= z + rd + 1; bz++) {
                if (by == y - 1 && !World_isBlockSolid(world, bx, by, bz))
                    return 0;
                if (by == y + height + 1 && !World_isBlockSolid(world, bx, by, bz))
                    return 0;
                if ((bx == x - rw - 1 || bx == x + rw + 1 || bz == z - rd - 1 ||
                     bz == z + rd + 1) &&
                    by == y && World_getBlockId(world, bx, by, bz) == BLOCK_AIR &&
                    World_getBlockId(world, bx, by + 1, bz) == BLOCK_AIR)
                    ++openings;
            }

    if (openings < 1 || openings > 5)
        return 0;

    for (int bx = x - rw - 1; bx <= x + rw + 1; bx++)
        for (int by = y + height; by >= y - 1; by--)
            for (int bz = z - rd - 1; bz <= z + rd + 1; bz++) {
                int interior = (bx != x - rw - 1 && by != y - 1 && bz != z - rd - 1 &&
                                bx != x + rw + 1 && by != y + height + 1 && bz != z + rd + 1);
                if (interior) {
                    World_setBlockWithNotify(world, bx, by, bz, BLOCK_AIR);
                } else if (by >= 0 && World_isBlockSolid(world, bx, by, bz)) {
                    int id = (by == y - 1 && JavaRandom_nextInt(rand, 4) != 0) ? BLOCK_MOSSY_COBBLE
                                                                               : BLOCK_COBBLESTONE;
                    World_setBlockWithNotify(world, bx, by, bz, id);
                }
            }

    for (int ci = 0; ci < 2; ci++) {
        for (int attempt = 0; attempt < 3; attempt++) {
            int cx = x + JavaRandom_nextInt(rand, rw * 2 + 1) - rw;
            int cz = z + JavaRandom_nextInt(rand, rd * 2 + 1) - rd;
            if (World_getBlockId(world, cx, y, cz) != BLOCK_AIR)
                continue;
            int adj = 0;
            if (World_isBlockSolid(world, cx - 1, y, cz))
                adj++;
            if (World_isBlockSolid(world, cx + 1, y, cz))
                adj++;
            if (World_isBlockSolid(world, cx, y, cz - 1))
                adj++;
            if (World_isBlockSolid(world, cx, y, cz + 1))
                adj++;
            if (adj == 1) {
                World_setBlockWithNotify(world, cx, y, cz, BLOCK_CHEST);
                TileEntityChest *chest =
                    (TileEntityChest *)World_getBlockTileEntity(world, cx, y, cz);
                if (chest)
                    fill_chest(chest, rand);
                break;
            }
        }
    }

    World_setBlockWithNotify(world, x, y, z, BLOCK_MOB_SPAWNER);
    TileEntityMobSpawner *spawner =
        (TileEntityMobSpawner *)World_getBlockTileEntity(world, x, y, z);
    if (spawner) {
        strncpy(spawner->mobID, pick_mob(rand), sizeof(spawner->mobID) - 1);
    }
    return 1;
}

static WorldGeneratorVtable dungeons_vtable = {dungeons_generate, WorldGenerator_noop_func_517_a,
                                               WorldGenerator_simple_destroy};

WorldGenDungeons *WorldGenDungeons_create(void) {
    WorldGenDungeons *self = (WorldGenDungeons *)calloc(1, sizeof(WorldGenDungeons));
    self->base.vtable = &dungeons_vtable;
    self->base.scaleX = self->base.scaleY = self->base.scaleZ = 1.0;
    return self;
}

void WorldGenDungeons_free(WorldGenDungeons *self) { free(self); }
