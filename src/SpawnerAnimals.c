#include "SpawnerAnimals.h"
#include "MathHelper.h"
#include "Material.h"
#include "EnumCreatureType.h"
#include "ChunkCoordIntPair.h"
#include "ChunkPosition.h"
#include "ChunkCoordinates.h"
#include "SpawnListEntry.h"
#include "EntityPlayer.h"
#include "EntityLiving.h"
#include "EntitySpider.h"
#include "EntitySkeleton.h"
#include "EntitySheep.h"
#include "EntityZombie.h"
#include "EntityCreeper.h"
#include "EntitySlime.h"
#include "EntityPig.h"
#include "EntityChicken.h"
#include "EntityCow.h"
#include "EntitySquid.h"
#include "EntityWolf.h"
#include "EntityGhast.h"
#include "EntityPigZombie.h"
#include "BiomeGenBase.h"
#include "WorldChunkManager.h"
#include "Pathfinder.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "BlockBed.h"
#include <stdlib.h>
#include <math.h>

const EntitySpawnType SpawnerAnimals_nightSpawnEntities[3] = {
    ENTITY_SPAWN_SPIDER, ENTITY_SPAWN_ZOMBIE, ENTITY_SPAWN_SKELETON};

#define ELIGIBLE_CHUNKS_MAX 1024
static ChunkCoordIntPair s_eligibleChunks[ELIGIBLE_CHUNKS_MAX];
static int s_eligibleCount = 0;

static void eligible_clear(void) { s_eligibleCount = 0; }

static void eligible_add(int x, int z) {
    int i;
    for (i = 0; i < s_eligibleCount; i++) {
        if (s_eligibleChunks[i].chunkXPos == x && s_eligibleChunks[i].chunkZPos == z)
            return;
    }
    if (s_eligibleCount < ELIGIBLE_CHUNKS_MAX) {
        s_eligibleChunks[s_eligibleCount].chunkXPos = x;
        s_eligibleChunks[s_eligibleCount].chunkZPos = z;
        s_eligibleCount++;
    }
}

static EntityLiving *spawnEntityByType(EntitySpawnType type, World *world) {
    switch (type) {
    case ENTITY_SPAWN_SPIDER:
        return (EntityLiving *)EntitySpider_create(world);
    case ENTITY_SPAWN_ZOMBIE:
        return (EntityLiving *)EntityZombie_create(world);
    case ENTITY_SPAWN_SKELETON:
        return (EntityLiving *)EntitySkeleton_create(world);
    case ENTITY_SPAWN_CREEPER:
        return (EntityLiving *)EntityCreeper_create(world);
    case ENTITY_SPAWN_SLIME:
        return (EntityLiving *)EntitySlime_create(world);
    case ENTITY_SPAWN_SHEEP:
        return (EntityLiving *)EntitySheep_create(world);
    case ENTITY_SPAWN_PIG:
        return (EntityLiving *)EntityPig_create(world);
    case ENTITY_SPAWN_CHICKEN:
        return (EntityLiving *)EntityChicken_create(world);
    case ENTITY_SPAWN_COW:
        return (EntityLiving *)EntityCow_create(world);
    case ENTITY_SPAWN_SQUID:
        return (EntityLiving *)EntitySquid_create(world);
    case ENTITY_SPAWN_WOLF:
        return (EntityLiving *)EntityWolf_create(world);
    case ENTITY_SPAWN_GHAST:
        return (EntityLiving *)EntityGhast_create(world);
    case ENTITY_SPAWN_PIG_ZOMBIE:
        return (EntityLiving *)EntityPigZombie_create(world);
    default:
        return NULL;
    }
}

ChunkPosition SpawnerAnimals_getRandomSpawningPointInChunk(World *var0, int var1, int var2) {
    int var3 = var1 + JavaRandom_nextInt(&var0->rand, 16);
    int var4 = JavaRandom_nextInt(&var0->rand, 128);
    int var5 = var2 + JavaRandom_nextInt(&var0->rand, 16);
    return ChunkPosition_create(var3, var4, var5);
}

int SpawnerAnimals_canCreatureTypeSpawnAtLocation(EnumCreatureType var0, World *var1, int var2,
                                                  int var3, int var4) {
    if (EnumCreatureType_getCreatureMaterial(var0) == Material_water) {
        return Material_getIsLiquid(World_getBlockMaterial(var1, var2, var3, var4)) &&
               !World_isBlockNormalCube(var1, var2, var3 + 1, var4);
    } else {
        return World_isBlockNormalCube(var1, var2, var3 - 1, var4) &&
               !World_isBlockNormalCube(var1, var2, var3, var4) &&
               !Material_getIsLiquid(World_getBlockMaterial(var1, var2, var3, var4)) &&
               !World_isBlockNormalCube(var1, var2, var3 + 1, var4);
    }
}

void SpawnerAnimals_creatureSpecificInit(EntityLiving *var0, World *var1, float var2, float var3,
                                         float var4) {

    if (var0->base.isSpider && JavaRandom_nextInt(&var1->rand, 100) == 0) {
        EntitySkeleton *var5 = EntitySkeleton_create(var1);
        var5->base.base.base.base.vtable->setLocationAndAngles(
            (Entity *)var5, (double)var2, (double)var3, (double)var4, var0->base.rotationYaw, 0.0f);
        World_entityJoinedWorld(var1, (Entity *)var5);
        Entity_mountEntity((Entity *)var5, (Entity *)var0);
    } else if (var0->base.isSheep) {
        EntitySheep_setFleeceColor((EntitySheep *)var0,
                                   EntitySheep_getRandomFleeceColor(&var1->rand));
    }
}

int SpawnerAnimals_performSpawning(World *var0, int var1, int var2) {
    if (!var1 && !var2)
        return 0;

    eligible_clear();

    int var3;
    int var6;
    for (var3 = 0; var3 < var0->numPlayerEntities; var3++) {
        EntityPlayer *var4 = var0->playerEntities[var3];
        int var5 = MathHelper_floor_double(var4->base.base.posX / 16.0);
        var6 = MathHelper_floor_double(var4->base.base.posZ / 16.0);
        int var7 = 8;
        for (int var8 = -var7; var8 <= var7; var8++) {
            for (int var9 = -var7; var9 <= var7; var9++) {
                eligible_add(var8 + var5, var9 + var6);
            }
        }
    }

    var3 = 0;
    ChunkCoordinates var35 = World_getSpawnPoint(var0);
    int var36_len = ENUM_CREATURE_TYPE_COUNT;

    for (int var37 = 0; var37 < var36_len; var37++) {
        EnumCreatureType var38 = (EnumCreatureType)var37;
        if ((!EnumCreatureType_getPeacefulCreature(var38) || var2) &&
            (EnumCreatureType_getPeacefulCreature(var38) || var1) &&
            World_countEntities(var0, EnumCreatureType_getCreatureClass(var38)) <=
                EnumCreatureType_getMaxNumberOfCreature(var38) * s_eligibleCount / 256) {

            int var39 = 0;

            while (1) {
                SpawnListEntry *var15 = NULL;
                int var18, var19, var42;

                int found_chunk = 0;
                while (var39 < s_eligibleCount) {
                    ChunkCoordIntPair *var10 = &s_eligibleChunks[var39];
                    var39++;
                    WorldChunkManager *wcm = World_getWorldChunkManager(var0);
                    BiomeGenBase *var11 = WorldChunkManager_getBiomeGenAtChunkCoord(
                        wcm, var10->chunkXPos, var10->chunkZPos);
                    SpawnList *var12 = BiomeGenBase_getSpawnableList(var11, var38);
                    if (var12 == NULL || var12->count == 0)
                        continue;

                    int var13 = 0;
                    for (int vi = 0; vi < var12->count; vi++) {
                        var13 += var12->entries[vi].spawnRarityRate;
                    }

                    int var40 = JavaRandom_nextInt(&var0->rand, var13);
                    var15 = &var12->entries[0];
                    for (int vi = 0; vi < var12->count; vi++) {
                        var40 -= var12->entries[vi].spawnRarityRate;
                        if (var40 < 0) {
                            var15 = &var12->entries[vi];
                            break;
                        }
                    }

                    ChunkPosition var41 = SpawnerAnimals_getRandomSpawningPointInChunk(
                        var0, var10->chunkXPos * 16, var10->chunkZPos * 16);
                    var42 = var41.x;
                    var18 = var41.y;
                    var19 = var41.z;

                    if (World_isBlockNormalCube(var0, var42, var18, var19))
                        continue;
                    if (World_getBlockMaterial(var0, var42, var18, var19) !=
                        EnumCreatureType_getCreatureMaterial(var38))
                        continue;

                    found_chunk = 1;
                    break;
                }

                if (!found_chunk)
                    goto label133_continue;

                int var20 = 0;
                for (int var21 = 0; var21 < 3; var21++) {
                    int var22 = var42;
                    int var23 = var18;
                    int var24 = var19;
                    int var25 = 6;
                    for (int var26 = 0; var26 < 4; var26++) {
                        var22 += JavaRandom_nextInt(&var0->rand, var25) -
                                 JavaRandom_nextInt(&var0->rand, var25);
                        var23 +=
                            JavaRandom_nextInt(&var0->rand, 1) - JavaRandom_nextInt(&var0->rand, 1);
                        var24 += JavaRandom_nextInt(&var0->rand, var25) -
                                 JavaRandom_nextInt(&var0->rand, var25);
                        if (SpawnerAnimals_canCreatureTypeSpawnAtLocation(var38, var0, var22, var23,
                                                                          var24)) {
                            float var27 = (float)var22 + 0.5f;
                            float var28 = (float)var23;
                            float var29 = (float)var24 + 0.5f;
                            if (World_getClosestPlayer(var0, (double)var27, (double)var28,
                                                       (double)var29, 24.0) == NULL) {
                                float var30 = var27 - (float)var35.x;
                                float var31 = var28 - (float)var35.y;
                                float var32 = var29 - (float)var35.z;
                                float var33 = var30 * var30 + var31 * var31 + var32 * var32;
                                if (var33 >= 576.0f) {
                                    EntityLiving *var43 =
                                        spawnEntityByType(var15->entityType, var0);
                                    if (!var43) {
                                        return var3;
                                    }
                                    var43->base.vtable->setLocationAndAngles(
                                        (Entity *)var43, (double)var27, (double)var28,
                                        (double)var29, JavaRandom_nextFloat(&var0->rand) * 360.0f,
                                        0.0f);
                                    if (((EntityLivingVtable *)var43->base.vtable)
                                            ->getCanSpawnHere(var43)) {
                                        var20++;
                                        World_entityJoinedWorld(var0, (Entity *)var43);
                                        SpawnerAnimals_creatureSpecificInit(var43, var0, var27,
                                                                            var28, var29);
                                        if (var20 >= ((EntityLivingVtable *)var43->base.vtable)
                                                         ->getMaxSpawnedInChunk(var43)) {
                                            goto label130_continue;
                                        }
                                    } else {
                                        Entity_destroy((Entity *)var43);
                                    }
                                    var3 += var20;
                                }
                            }
                        }
                    }
                }
            label130_continue:;
            }
        }
    label133_continue:;
    }

    return var3;
}

int SpawnerAnimals_performSleepSpawning(World *var0, struct EntityPlayer **var1, int var1Count) {
    int var2 = 0;
    Pathfinder *var3 = Pathfinder_create((IBlockAccess *)var0);
    int var4 = 0;

    while (1) {
        EntityPlayer *var5;

        if (var4 >= var1Count) {
            free(var3);
            return var2;
        }
        var5 = var1[var4++];

        int var7 = 0;
        for (int var8 = 0; var8 < 20 && !var7; var8++) {
            int var9 = MathHelper_floor_double(var5->base.base.posX) +
                       JavaRandom_nextInt(&var0->rand, 32) - JavaRandom_nextInt(&var0->rand, 32);
            int var10 = MathHelper_floor_double(var5->base.base.posZ) +
                        JavaRandom_nextInt(&var0->rand, 32) - JavaRandom_nextInt(&var0->rand, 32);
            int var11 = MathHelper_floor_double(var5->base.base.posY) +
                        JavaRandom_nextInt(&var0->rand, 16) - JavaRandom_nextInt(&var0->rand, 16);
            if (var11 < 1)
                var11 = 1;
            else if (var11 > 128)
                var11 = 128;

            int var12 = JavaRandom_nextInt(&var0->rand, 3);

            int var13;
            for (var13 = var11; var13 > 2 && !World_isBlockNormalCube(var0, var9, var13 - 1, var10);
                 var13--) {
            }

            while (!SpawnerAnimals_canCreatureTypeSpawnAtLocation(EnumCreatureType_monster, var0,
                                                                  var9, var13, var10) &&
                   var13 < var11 + 16 && var13 < 128) {
                var13++;
            }

            if (var13 < var11 + 16 && var13 < 128) {
                float var14 = (float)var9 + 0.5f;
                float var15 = (float)var13;
                float var16 = (float)var10 + 0.5f;

                EntityLiving *var17 =
                    spawnEntityByType(SpawnerAnimals_nightSpawnEntities[var12], var0);
                if (!var17) {
                    free(var3);
                    return var2;
                }

                var17->base.vtable->setLocationAndAngles(
                    (Entity *)var17, (double)var14, (double)var15, (double)var16,
                    JavaRandom_nextFloat(&var0->rand) * 360.0f, 0.0f);

                int var17_spawned = 0;
                if (((EntityLivingVtable *)var17->base.vtable)->getCanSpawnHere(var17)) {
                    PathEntity *var18 = Pathfinder_createEntityPathToEntity(var3, (Entity *)var17,
                                                                            (Entity *)var5, 32.0f);
                    if (var18 != NULL && var18->pathLength > 1) {
                        PathPoint *var19 = PathEntity_func_22328_c(var18);
                        if (fabs((double)var19->xCoord - var5->base.base.posX) < 1.5 &&
                            fabs((double)var19->zCoord - var5->base.base.posZ) < 1.5 &&
                            fabs((double)var19->yCoord - var5->base.base.posY) < 1.5) {
                            ChunkCoordinates *var20 = BlockBed_getNearestEmptyChunkCoordinates(
                                var0, MathHelper_floor_double(var5->base.base.posX),
                                MathHelper_floor_double(var5->base.base.posY),
                                MathHelper_floor_double(var5->base.base.posZ), 1);
                            ChunkCoordinates fallback;
                            if (var20 == NULL) {
                                fallback.x = var9;
                                fallback.y = var13 + 1;
                                fallback.z = var10;
                                var20 = &fallback;
                            }
                            var17->base.vtable->setLocationAndAngles(
                                (Entity *)var17, (double)((float)var20->x + 0.5f), (double)var20->y,
                                (double)((float)var20->z + 0.5f), 0.0f, 0.0f);
                            World_entityJoinedWorld(var0, (Entity *)var17);
                            SpawnerAnimals_creatureSpecificInit(var17, var0, (float)var20->x + 0.5f,
                                                                (float)var20->y,
                                                                (float)var20->z + 0.5f);
                            EntityPlayer_wakeUpPlayer(var5, 1, 0, 0);
                            EntityLiving_playLivingSound(var17);
                            var2 = 1;
                            var7 = 1;
                            var17_spawned = 1;
                        }
                    }
                }
                if (!var17_spawned)
                    Entity_destroy((Entity *)var17);
            }
        }
    }
}
