#include "World.h"
#include "Block.h"
#include "Chunk.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "EntityLightningBolt.h"
#include "TileEntity.h"
#include "ISaveHandler.h"
#include "WorldInfo.h"
#include "WorldProvider.h"
#include "MathHelper.h"
#include "NBTTagCompound.h"
#include "ChunkProviderLoadOrGenerate.h"
#include "ChunkProvider.h"
#include "ChunkProviderClient.h"
#include "WorldClient.h"
#include "NetClientHandler.h"
#include "IChunkLoader.h"
#include "IWorldAccess.h"
#include "AxisAlignedBB.h"
#include "Explosion.h"
#include "Vec3D.h"
#include "MovingObjectPosition.h"
#include "BlockFluid.h"
#include "Pathfinder.h"
#include "MapStorage.h"
#include "SpawnerAnimals.h"
#include "BiomeGenBase.h"
#include "WorldChunkManager.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

static int World_lightingUpdatesScheduled = 0;

int World_isBlockNormalCube(World *self, int x, int y, int z);
int World_isBlockIndirectlyProvidingPowerTo(World *self, int x, int y, int z, int side);
void World_func_28107_a(World *self, struct EntityPlayer *player, int eventID, int x, int y, int z,
                        int data);
static float World_func_27166_f(World *self, float var1);

static int chunk_hash(int cx, int cz) {
    int h = (int)((unsigned int)cx * 1664525U + (unsigned int)cz * 1013904223U);
    return ((h >> 16) ^ h) & (WORLD_CHUNK_TABLE_SIZE - 1);
}

Chunk *World_getOrCreateChunk(World *self, int cx, int cz) {
    int idx = chunk_hash(cx, cz);

    for (int i = 0; i < WORLD_CHUNK_TABLE_SIZE; i++) {
        int slot = (idx + i) & (WORLD_CHUNK_TABLE_SIZE - 1);
        Chunk *c = self->chunkTable[slot];
        if (!c) {
            c = Chunk_create(self, cx, cz);
            self->chunkTable[slot] = c;
            return c;
        }
        if (c->xPosition == cx && c->zPosition == cz)
            return c;
    }
    return NULL;
}

static Chunk *world_getChunk(World *self, int cx, int cz) {
    int idx = chunk_hash(cx, cz);
    for (int i = 0; i < WORLD_CHUNK_TABLE_SIZE; i++) {
        int slot = (idx + i) & (WORLD_CHUNK_TABLE_SIZE - 1);
        Chunk *c = self->chunkTable[slot];
        if (!c)
            return NULL;
        if (c->xPosition == cx && c->zPosition == cz)
            return c;
    }
    return NULL;
}

static Chunk *def_getChunkFromBlockCoords(World *self, int x, int z) {
    return self->vtable->getChunkFromChunkCoords(self, x >> 4, z >> 4);
}

static Chunk *def_getChunkFromChunkCoords(World *self, int cx, int cz) {
    if (self->chunkProvider)
        return (Chunk *)self->chunkProvider->vtable->provideChunk(self->chunkProvider, cx, cz);
    return world_getChunk(self, cx, cz);
}

static int def_getBlockId(World *self, int x, int y, int z) {
    if (y < 0 || y >= self->worldHeight)
        return 0;
    Chunk *c = def_getChunkFromChunkCoords(self, x >> 4, z >> 4);
    if (!c)
        return 0;
    int id = Chunk_getBlockID(c, x & 15, y, z & 15);
    if (id < 0 || id >= 256 || !Block_blocksList[id])
        return 0;
    return id;
}
static int def_getBlockMetadata(World *self, int x, int y, int z) {
    if (y < 0 || y >= self->worldHeight)
        return 0;
    Chunk *c = def_getChunkFromChunkCoords(self, x >> 4, z >> 4);
    if (!c)
        return 0;
    return Chunk_getBlockMetadata(c, x & 15, y, z & 15);
}

static TileEntity *def_getBlockTileEntity(World *self, int x, int y, int z) {
    Chunk *var4 = self->vtable->getChunkFromChunkCoords(self, x >> 4, z >> 4);
    return var4 ? Chunk_getChunkBlockTileEntity(var4, x & 15, y, z & 15) : NULL;
}
static float def_getLightBrightness(World *self, int x, int y, int z) {

    int idx = self->vtable->getBlockLightValue(self, x, y, z);
    if (idx < 0)
        idx = 0;
    if (idx > 15)
        idx = 15;
    return self->lightBrightnessTable[idx];
}
static int def_isBlockOpaqueCube(World *self, int x, int y, int z) {
    int id = def_getBlockId(self, x, y, z);
    if (id == 0 || !Block_blocksList[id])
        return 0;
    return Block_blocksList[id]->vtable->isOpaqueCube(Block_blocksList[id]);
}

static int def_isBlockNormalCube(World *self, int x, int y, int z) {
    int id = def_getBlockId(self, x, y, z);
    if (id == 0 || !Block_blocksList[id])
        return 0;
    Block *b = Block_blocksList[id];
    return Material_getIsTranslucent(b->blockMaterial) && b->vtable->renderAsNormalBlock(b);
}
static int def_isAirBlock(World *self, int x, int y, int z) {
    return def_getBlockId(self, x, y, z) == 0;
}
static int def_blockExists(World *self, int x, int y, int z) {
    if (y < 0 || y >= 128)
        return 0;
    return World_chunkExists(self, x >> 4, z >> 4);
}
static int def_doesBlockHaveSolidTopSurface(World *self, int x, int y, int z) {
    int id = def_getBlockId(self, x, y, z);
    if (id == 0 || !Block_blocksList[id])
        return 0;
    Block *b = Block_blocksList[id];
    return b->vtable->isOpaqueCube(b) && b->vtable->renderAsNormalBlock(b);
}
static int def_setBlock(World *self, int x, int y, int z, int id) {
    if (x < -32000000 || z < -32000000 || x >= 32000000 || z > 32000000)
        return 0;
    if (y < 0 || y >= self->worldHeight)
        return 0;
    Chunk *c = self->vtable->getChunkFromChunkCoords(self, x >> 4, z >> 4);
    if (!c)
        return 0;
    return Chunk_setBlockID(c, x & 15, y, z & 15, id);
}
static int def_setBlockWithNotify(World *self, int x, int y, int z, int id) {
    if (!def_setBlock(self, x, y, z, id))
        return 0;
    World_notifyBlockChange(self, x, y, z, id);
    return 1;
}
static int def_setBlockAndMetadata(World *self, int x, int y, int z, int id, int meta) {
    if (!def_setBlock(self, x, y, z, id))
        return 0;
    Chunk *c = self->vtable->getChunkFromChunkCoords(self, x >> 4, z >> 4);
    if (c)
        Chunk_setBlockMetadata(c, x & 15, y, z & 15, meta);
    return 1;
}
static int def_setBlockMetadataWithNotify(World *self, int x, int y, int z, int meta) {
    if (y < 0 || y >= self->worldHeight)
        return 0;
    Chunk *c = self->vtable->getChunkFromChunkCoords(self, x >> 4, z >> 4);
    if (!c)
        return 0;
    if (!Chunk_setBlockMetadata(c, x & 15, y, z & 15, meta))
        return 0;
    int id = Chunk_getBlockID(c, x & 15, y, z & 15);
    if (Block_field_28032_t[id & 255]) {
        World_notifyBlockChange(self, x, y, z, id);
    } else {
        World_notifyBlocksOfNeighborChange(self, x, y, z, id);
    }
    return 1;
}
static int def_setBlockToAir(World *self, int x, int y, int z) {
    return def_setBlockWithNotify(self, x, y, z, 0);
}

static void def_setBlockTileEntity(World *self, int x, int y, int z, TileEntity *te) {
    if (!te || TileEntity_isInvalid(te))
        return;
    if (self->field_31055_L) {
        te->xCoord = x;
        te->yCoord = y;
        te->zCoord = z;
        if (self->field_30900_E_count >= self->field_30900_E_cap) {
            int nc = self->field_30900_E_cap ? self->field_30900_E_cap * 2 : 16;
            self->field_30900_E = realloc(self->field_30900_E, nc * sizeof(TileEntity *));
            self->field_30900_E_cap = nc;
        }
        self->field_30900_E[self->field_30900_E_count++] = te;
    } else {
        if (self->numLoadedTileEntities >= self->loadedTileEntityCap) {
            int nc = self->loadedTileEntityCap ? self->loadedTileEntityCap * 2 : 16;
            self->loadedTileEntityList =
                realloc(self->loadedTileEntityList, nc * sizeof(TileEntity *));
            self->loadedTileEntityCap = nc;
        }
        self->loadedTileEntityList[self->numLoadedTileEntities++] = te;
        Chunk *var5 = self->vtable->getChunkFromChunkCoords(self, x >> 4, z >> 4);
        if (var5)
            Chunk_setChunkBlockTileEntity(var5, x & 15, y, z & 15, te);
    }
}

static void def_removeBlockTileEntity(World *self, int x, int y, int z) {
    TileEntity *var4 = def_getBlockTileEntity(self, x, y, z);
    if (var4 != NULL && self->field_31055_L) {
        var4->tileEntityInvalid = 1;
    } else {
        if (var4 != NULL) {
            for (int i = 0; i < self->numLoadedTileEntities; i++) {
                if (self->loadedTileEntityList[i] == var4) {
                    self->loadedTileEntityList[i] =
                        self->loadedTileEntityList[--self->numLoadedTileEntities];
                    break;
                }
            }
        }
        Chunk *var5 = self->vtable->getChunkFromChunkCoords(self, x >> 4, z >> 4);
        if (var5)
            Chunk_removeChunkBlockTileEntity(var5, x & 15, y, z & 15);
        if (var4 != NULL)
            TileEntity_destroy(var4);
    }
}

static void def_scheduleBlockUpdate(World *self, int x, int y, int z, int blockId, int delay) {
    int8_t var7 = 8;
    if (self->scheduledUpdatesAreImmediate) {
        if (World_checkChunksExist(self, x - var7, y - var7, z - var7, x + var7, y + var7,
                                   z + var7)) {
            int var8 = World_getBlockId(self, x, y, z);
            if (var8 == blockId && var8 > 0) {
                Block_blocksList[var8]->vtable->updateTick(Block_blocksList[var8], self, x, y, z,
                                                           &self->rand);
            }
        }
    } else {
        if (World_checkChunksExist(self, x - var7, y - var7, z - var7, x + var7, y + var7,
                                   z + var7)) {

            for (int i = 0; i < self->numPendingTicks; i++) {
                NextTickListEntry *e = &self->pendingTickList[i];
                if (e->xCoord == x && e->yCoord == y && e->zCoord == z && e->blockID == blockId)
                    return;
            }
            if (self->numPendingTicks >= self->pendingTickCap) {
                int newCap = self->pendingTickCap ? self->pendingTickCap * 2 : 64;
                self->pendingTickList =
                    realloc(self->pendingTickList, newCap * sizeof(NextTickListEntry));
                self->pendingTickCap = newCap;
            }
            NextTickListEntry newEntry;
            newEntry.xCoord = x;
            newEntry.yCoord = y;
            newEntry.zCoord = z;
            newEntry.blockID = blockId;
            newEntry.scheduledTime =
                (blockId > 0) ? (int64_t)delay + WorldInfo_getWorldTime(self->worldInfo) : 0;
            newEntry.tickEntryID = NextTickListEntry_nextTickEntryID++;
            int pos = self->numPendingTicks;
            for (int i = 0; i < self->numPendingTicks; i++) {
                if (newEntry.scheduledTime < self->pendingTickList[i].scheduledTime) {
                    pos = i;
                    break;
                }
            }
            if (pos < self->numPendingTicks)
                memmove(&self->pendingTickList[pos + 1], &self->pendingTickList[pos],
                        (size_t)(self->numPendingTicks - pos) * sizeof(NextTickListEntry));
            self->pendingTickList[pos] = newEntry;
            self->numPendingTicks++;
        }
    }
}
static void def_notifyBlocksOfNeighborChange(World *self, int x, int y, int z, int blockId) {
    World_notifyBlockOfNeighborChange(self, x - 1, y, z, blockId);
    World_notifyBlockOfNeighborChange(self, x + 1, y, z, blockId);
    World_notifyBlockOfNeighborChange(self, x, y - 1, z, blockId);
    World_notifyBlockOfNeighborChange(self, x, y + 1, z, blockId);
    World_notifyBlockOfNeighborChange(self, x, y, z - 1, blockId);
    World_notifyBlockOfNeighborChange(self, x, y, z + 1, blockId);
}
static void def_notifyBlockOfNeighborChange(World *self, int x, int y, int z, int neighborId) {
    if (self->editingBlocks || self->multiplayerWorld)
        return;
    int id = def_getBlockId(self, x, y, z);
    if (id == 0 || !Block_blocksList[id])
        return;
    Block_blocksList[id]->vtable->onNeighborBlockChange(Block_blocksList[id], self, x, y, z,
                                                        neighborId);
}
static int def_getHeightValue(World *self, int x, int z) {
    if (x >= -32000000 && z >= -32000000 && x < 32000000 && z <= 32000000) {
        if (!World_chunkExists(self, x >> 4, z >> 4))
            return 0;
        Chunk *c = self->vtable->getChunkFromChunkCoords(self, x >> 4, z >> 4);
        return Chunk_getHeightValue(c, x & 15, z & 15);
    }
    return 0;
}

static int def_getBlockLightValue_do(World *self, int x, int y, int z, int var4) {

    if (x >= -32000000 && z >= -32000000 && x < 32000000 && z <= 32000000) {
        if (var4) {
            int var5 = self->vtable->getBlockId(self, x, y, z);
            if (var5 == Block_stairSingle->blockID || var5 == Block_tilledField->blockID ||
                var5 == Block_stairCompactCobblestone->blockID ||
                var5 == Block_stairCompactPlanks->blockID) {
                int var6 = def_getBlockLightValue_do(self, x, y + 1, z, 0);
                int var7 = def_getBlockLightValue_do(self, x + 1, y, z, 0);
                int var8 = def_getBlockLightValue_do(self, x - 1, y, z, 0);
                int var9 = def_getBlockLightValue_do(self, x, y, z + 1, 0);
                int var10 = def_getBlockLightValue_do(self, x, y, z - 1, 0);
                if (var7 > var6)
                    var6 = var7;
                if (var8 > var6)
                    var6 = var8;
                if (var9 > var6)
                    var6 = var9;
                if (var10 > var6)
                    var6 = var10;
                return var6;
            }
        }
        if (y < 0)
            return 0;
        if (y >= 128)
            y = 127;
        Chunk *var11 = self->vtable->getChunkFromChunkCoords(self, x >> 4, z >> 4);
        if (!var11)
            return 0;
        x &= 15;
        z &= 15;
        return Chunk_getBlockLightValue(var11, x, y, z, self->skylightSubtracted);
    } else {
        return 15;
    }
}

static int def_getBlockLightValue(World *self, int x, int y, int z) {
    return def_getBlockLightValue_do(self, x, y, z, 1);
}

static int def_getFullBlockLightValue(World *self, int x, int y, int z) {
    if (y < 0)
        return 0;
    if (y >= 128)
        y = 127;
    Chunk *c = world_getChunk(self, x >> 4, z >> 4);
    if (!c)
        return 0;
    return Chunk_getBlockLightValue(c, x & 15, y, z & 15, 0);
}
static int def_isBlockIndirectlyGettingPowered(World *self, int x, int y, int z) {
    return World_isBlockIndirectlyProvidingPowerTo(self, x, y - 1, z, 0)
               ? 1
               : (World_isBlockIndirectlyProvidingPowerTo(self, x, y + 1, z, 1)
                      ? 1
                      : (World_isBlockIndirectlyProvidingPowerTo(self, x, y, z - 1, 2)
                             ? 1
                             : (World_isBlockIndirectlyProvidingPowerTo(self, x, y, z + 1, 3)
                                    ? 1
                                    : (World_isBlockIndirectlyProvidingPowerTo(self, x - 1, y, z, 4)
                                           ? 1
                                           : World_isBlockIndirectlyProvidingPowerTo(self, x + 1, y,
                                                                                     z, 5)))));
}
static int def_getIndirectPowerLevelTo(World *self, int x, int y, int z, int side) {
    (void)self;
    (void)x;
    (void)y;
    (void)z;
    (void)side;
    return 0;
}
static int def_spawnEntityInWorld(World *self, Entity *entity) {
    return World_entityJoinedWorld(self, entity);
}
static void def_removeEntity(World *self, Entity *entity) {
    for (int i = 0; i < self->numEntities; i++) {
        if (self->loadedEntityList[i] == entity) {
            self->loadedEntityList[i] = self->loadedEntityList[--self->numEntities];
            return;
        }
    }
}

static void def_playSoundEffect(World *self, double x, double y, double z, const char *sound,
                                float vol, float pitch) {
    for (int i = 0; i < self->numWorldAccesses; i++) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[i];
        wa->vtable->playSound(wa, sound, x, y, z, vol, pitch);
    }
}

static void def_playAuxSFX(World *self, int effectId, int x, int y, int z, int data) {
    World_func_28107_a(self, NULL, effectId, x, y, z, data);
}
static int def_isRemote(World *self) { return self->isRemote; }
static void def_updateWeather(World *self);

static int iba_getBlockId(IBlockAccess *iba, int x, int y, int z) {
    return World_getBlockId((World *)iba, x, y, z);
}
static TileEntity *iba_getBlockTileEntity(IBlockAccess *iba, int x, int y, int z) {
    return World_getBlockTileEntity((World *)iba, x, y, z);
}

static float iba_getBrightness(IBlockAccess *iba, int x, int y, int z, int var4) {
    World *world = (World *)iba;
    int var5 = World_getBlockLightValue(world, x, y, z);
    if (var5 < var4)
        var5 = var4;
    return world->worldProvider->lightBrightnessTable[var5];
}
static float iba_getLightBrightness(IBlockAccess *iba, int x, int y, int z) {
    return (float)((World *)iba)->vtable->getLightBrightness((World *)iba, x, y, z);
}
static int iba_getBlockMetadata(IBlockAccess *iba, int x, int y, int z) {
    return World_getBlockMetadata((World *)iba, x, y, z);
}
static Material *iba_getBlockMaterial(IBlockAccess *iba, int x, int y, int z) {
    return World_getBlockMaterial((World *)iba, x, y, z);
}
static int iba_isBlockOpaqueCube(IBlockAccess *iba, int x, int y, int z) {
    return ((World *)iba)->vtable->isBlockOpaqueCube((World *)iba, x, y, z);
}
static int iba_isBlockNormalCube(IBlockAccess *iba, int x, int y, int z) {
    return ((World *)iba)->vtable->isBlockNormalCube((World *)iba, x, y, z);
}
static struct WorldChunkManager *iba_getWorldChunkManager(IBlockAccess *iba) {
    return World_getWorldChunkManager((World *)iba);
}

static IBlockAccessVtable World_ibaVtable = {
    .getBlockId = iba_getBlockId,
    .getBlockTileEntity = iba_getBlockTileEntity,
    .getBrightness = iba_getBrightness,
    .getLightBrightness = iba_getLightBrightness,
    .getBlockMetadata = iba_getBlockMetadata,
    .getBlockMaterial = iba_getBlockMaterial,
    .isBlockOpaqueCube = iba_isBlockOpaqueCube,
    .isBlockNormalCube = iba_isBlockNormalCube,
    .getWorldChunkManager = iba_getWorldChunkManager,
};

WorldVtable World_defaultVtable = {
    .getBlockId = def_getBlockId,
    .getBlockMetadata = def_getBlockMetadata,
    .getBlockTileEntity = def_getBlockTileEntity,
    .getLightBrightness = def_getLightBrightness,
    .isBlockOpaqueCube = def_isBlockOpaqueCube,
    .isBlockNormalCube = def_isBlockNormalCube,
    .setBlock = def_setBlock,
    .setBlockWithNotify = def_setBlockWithNotify,
    .setBlockAndMetadata = def_setBlockAndMetadata,
    .setBlockMetadataWithNotify = def_setBlockMetadataWithNotify,
    .setBlockToAir = def_setBlockToAir,
    .setBlockTileEntity = def_setBlockTileEntity,
    .removeBlockTileEntity = def_removeBlockTileEntity,
    .isAirBlock = def_isAirBlock,
    .blockExists = def_blockExists,
    .doesBlockHaveSolidTopSurface = def_doesBlockHaveSolidTopSurface,
    .scheduleBlockUpdate = def_scheduleBlockUpdate,
    .notifyBlocksOfNeighborChange = def_notifyBlocksOfNeighborChange,
    .notifyBlockOfNeighborChange = def_notifyBlockOfNeighborChange,
    .getHeightValue = def_getHeightValue,
    .getBlockLightValue = def_getBlockLightValue,
    .getFullBlockLightValue = def_getFullBlockLightValue,
    .isBlockIndirectlyGettingPowered = def_isBlockIndirectlyGettingPowered,
    .getIndirectPowerLevelTo = def_getIndirectPowerLevelTo,
    .spawnEntityInWorld = def_spawnEntityInWorld,
    .removeEntity = def_removeEntity,
    .playSoundEffect = def_playSoundEffect,
    .playAuxSFX = def_playAuxSFX,
    .getChunkFromBlockCoords = def_getChunkFromBlockCoords,
    .getChunkFromChunkCoords = def_getChunkFromChunkCoords,
    .isRemote = def_isRemote,
    .obtainEntitySkin = World_obtainEntitySkin,
    .releaseEntitySkin = World_releaseEntitySkin,
    .updateWeather = def_updateWeather,
    .getChunkProvider = World_getChunkProvider,
    .tick = World_tick,
    .setSpawnLocation = World_setSpawnLocation,
};

static void world_init_common(World *self) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long long now = (long long)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;

    self->iBlockAccess.vtable = &World_ibaVtable;
    self->vtable = &World_defaultVtable;
    self->worldHeight = CHUNK_SIZE_Y;

    self->field_1019_F = 16777215L;

    JavaRandom tmpRand;
    JavaRandom_new(&tmpRand);
    self->field_9437_g = JavaRandom_nextIntRaw(&tmpRand);

    self->field_9436_h = 1013904223;

    self->lockTimestamp = now;

    self->autosavePeriod = 40;

    JavaRandom_new(&self->rand);

    self->spawnHostileMobs = 1;
    self->spawnPeacefulMobs = 1;

    self->soundCounter = JavaRandom_nextInt(&self->rand, 12000);

    self->pathFinder = Pathfinder_create((IBlockAccess *)self);
}

void World_init(World *self, int64_t seed, int isRemote) {
    memset(self, 0, sizeof(*self));
    world_init_common(self);
    self->seed = seed;
    self->isRemote = isRemote;

    self->worldInfo = WorldInfo_create_from_seed_name(seed, "");
    self->worldProvider = WorldProvider_getProviderForDimension(0);
    WorldProvider_registerWorld(self->worldProvider, self);
    self->worldChunkManager = self->worldProvider->worldChunkMgr;
    for (int i = 0; i <= 15; i++)
        self->lightBrightnessTable[i] = self->worldProvider->lightBrightnessTable[i];
}

void World_init_with_save_and_provider(World *self, ISaveHandler *var1, const char *var2,
                                       WorldProvider *var3, int64_t var4) {
    const WorldVtable *saved_vtable = self->vtable;
    memset(self, 0, sizeof(*self));
    world_init_common(self);
    if (saved_vtable)
        self->vtable = saved_vtable;

    self->saveHandler = var1;
    self->ownsSaveHandler = 1;
    self->worldInfo = WorldInfo_create_from_seed_name(var4, var2);
    self->worldProvider = var3;
    self->field_28108_z = (MapStorage *)calloc(1, sizeof(MapStorage));
    MapStorage_init(self->field_28108_z, var1);
    WorldProvider_registerWorld(var3, self);
    self->worldChunkManager = var3->worldChunkMgr;
    for (int i = 0; i <= 15; i++)
        self->lightBrightnessTable[i] = var3->lightBrightnessTable[i];
    self->chunkProvider = self->vtable->getChunkProvider(self);
    World_calculateInitialSkylight(self);
    World_func_27163_E(self);
}

void World_init_from_save_provider(World *self, ISaveHandler *var1, const char *var2, int64_t var3,
                                   WorldProvider *var5) {

    const WorldVtable *saved_vtable = self->vtable;
    memset(self, 0, sizeof(*self));
    world_init_common(self);
    if (saved_vtable)
        self->vtable = saved_vtable;

    self->saveHandler = var1;
    self->ownsSaveHandler = 1;

    self->field_28108_z = (MapStorage *)calloc(1, sizeof(MapStorage));
    MapStorage_init(self->field_28108_z, var1);
    self->worldInfo = ISaveHandler_loadWorldInfo(var1);
    self->isNewWorld = (self->worldInfo == NULL);

    if (var5 != NULL) {
        self->worldProvider = var5;
    } else if (self->worldInfo != NULL && WorldInfo_getDimension(self->worldInfo) == -1) {
        self->worldProvider = WorldProvider_getProviderForDimension(-1);
    } else {
        self->worldProvider = WorldProvider_getProviderForDimension(0);
    }

    int var6 = 0;
    if (self->worldInfo == NULL) {
        self->worldInfo = WorldInfo_create_from_seed_name(var3, var2);
        var6 = 1;
    } else {
        WorldInfo_setWorldName(self->worldInfo, var2);
    }

    WorldProvider_registerWorld(self->worldProvider, self);
    self->worldChunkManager = self->worldProvider->worldChunkMgr;
    for (int i = 0; i <= 15; i++)
        self->lightBrightnessTable[i] = self->worldProvider->lightBrightnessTable[i];

    self->chunkProvider = self->vtable->getChunkProvider(self);
    if (var6) {
        World_getInitialSpawnLocation(self);
    }
    World_calculateInitialSkylight(self);
    World_func_27163_E(self);
}

World *World_create_from_save(ISaveHandler *var1, const char *var2, int64_t var3) {
    World *self = (World *)calloc(1, sizeof(World));
    if (!self)
        return NULL;
    World_init_from_save_provider(self, var1, var2, var3, NULL);
    return self;
}

IChunkProvider *World_getChunkProvider(World *self) {
    IChunkLoader *var1 = ISaveHandler_getChunkLoader(self->saveHandler, self->worldProvider);
    IChunkProvider *var2 = (IChunkProvider *)WorldProvider_getChunkProvider(self->worldProvider);
    return (IChunkProvider *)ChunkProvider_create(self, var1, var2);
}

void World_getInitialSpawnLocation(World *self) {
    self->findingSpawnPoint = 1;
    int var1 = 0;
    int8_t var2 = 64;
    int var3;
    for (var3 = 0; !WorldProvider_canCoordinateBeSpawn(self->worldProvider, var1, var3);
         var3 += JavaRandom_nextInt(&self->rand, 64) - JavaRandom_nextInt(&self->rand, 64)) {
        var1 += JavaRandom_nextInt(&self->rand, 64) - JavaRandom_nextInt(&self->rand, 64);
    }
    WorldInfo_setSpawn(self->worldInfo, var1, (int)var2, var3);
    self->findingSpawnPoint = 0;
}

void World_calculateInitialSkylight(World *self) {
    int var1 = World_calculateSkylightSubtracted(self, 1.0F);
    if (var1 != self->skylightSubtracted) {
        self->skylightSubtracted = var1;
    }
}

void World_func_27163_E(World *self) {
    if (WorldInfo_getRaining(self->worldInfo)) {
        self->rainingStrength = 1.0F;
        if (WorldInfo_getThundering(self->worldInfo)) {
            self->thunderingStrength = 1.0F;
        }
    }
}

void World_destroy(World *self) {
    for (int i = 0; i < self->numEntities; i++)
        Entity_destroy(self->loadedEntityList[i]);
    if (self->deadPlayer)
        Entity_destroy(self->deadPlayer);
    for (int i = 0; i < self->numUnloadedEntities; i++)
        Entity_destroy(self->unloadedEntityList[i]);
    self->numUnloadedEntities = 0;
    Entity_flush_destroy_queue_for_world(self);
    for (int i = 0; i < self->numLoadedTileEntities; i++)
        TileEntity_destroy(self->loadedTileEntityList[i]);

    free(self->loadedEntityList);
    free(self->playerEntities);
    free(self->pendingTickList);
    if (self->chunkProvider) {
        if (self->chunkProvider->vtable == &ChunkProviderClient_vtable)
            ChunkProviderClient_destroy((ChunkProviderClient *)self->chunkProvider);
        else
            ChunkProvider_destroy((ChunkProvider *)self->chunkProvider);
    }
    if (self->ownsSaveHandler && self->saveHandler)
        ISaveHandler_destroy(self->saveHandler);

    if (self->worldProvider)
        WorldProvider_destroy(self->worldProvider);
    if (self->worldInfo)
        WorldInfo_free(self->worldInfo);

    if (self->field_28108_z && !self->multiplayerWorld) {
        MapStorage_destroy(self->field_28108_z);
        free(self->field_28108_z);
    }
    if (self->multiplayerWorld) {
        WorldClient *wc = (WorldClient *)self;

        wc->sendQueue = NULL;
        MCHash_destroy(&wc->field_1055_D);
        EntitySet_destroy(&wc->field_20914_E);
        EntitySet_destroy(&wc->field_1053_F);
        free(wc->field_1057_z.entries);
    }
    if (self->pathFinder)
        Pathfinder_destroy(self->pathFinder);
    free(self->worldAccesses);
    free(self->positionsToUpdate);
    for (int i = 0; i < self->lightingToUpdate_count; ++i)
        free(self->lightingToUpdate[i]);
    free(self->lightingToUpdate);
    free(self->field_30900_E);
    free(self->loadedTileEntityList);
    free(self->weatherEffects);
    free(self->unloadedEntityList);
    free(self);
}

World *World_createFromWorld(World *var1, WorldProvider *var2) {
    World *self = (World *)calloc(1, sizeof(World));
    if (!self)
        return NULL;
    world_init_common(self);

    self->lockTimestamp = var1->lockTimestamp;

    self->saveHandler = var1->saveHandler;
    self->ownsSaveHandler = var1->ownsSaveHandler;
    var1->ownsSaveHandler = 0;

    self->worldInfo = WorldInfo_create_copy(var1->worldInfo);

    self->field_28108_z = (MapStorage *)calloc(1, sizeof(MapStorage));
    MapStorage_init(self->field_28108_z, self->saveHandler);

    self->worldProvider = var2;
    WorldProvider_registerWorld(var2, self);
    self->worldChunkManager = var2->worldChunkMgr;
    for (int i = 0; i <= 15; i++)
        self->lightBrightnessTable[i] = var2->lightBrightnessTable[i];

    self->chunkProvider = self->vtable->getChunkProvider(self);

    World_calculateInitialSkylight(self);

    World_func_27163_E(self);
    return self;
}

World *World_create(int64_t seed, int isRemote) {
    World *self = (World *)calloc(1, sizeof(World));
    World_init(self, seed, isRemote);
    return self;
}

void World_updateEntities(World *self) {

    int var1;
    Entity *var2;
    for (var1 = 0; var1 < self->numWeatherEffects; ++var1) {
        var2 = self->weatherEffects[var1];
        var2->vtable->onUpdate(var2);
        if (var2->isDead) {
            self->weatherEffects[var1] = self->weatherEffects[--self->numWeatherEffects];
            --var1;
        }
    }

    for (var1 = 0; var1 < self->numUnloadedEntities; var1++) {
        var2 = self->unloadedEntityList[var1];
        for (int j = 0; j < self->numEntities; j++) {
            if (self->loadedEntityList[j] == var2) {
                self->loadedEntityList[j] = self->loadedEntityList[--self->numEntities];
                break;
            }
        }
    }

    for (var1 = 0; var1 < self->numUnloadedEntities; var1++) {
        var2 = self->unloadedEntityList[var1];
        int var3 = var2->chunkCoordX;
        int var4 = var2->chunkCoordZ;
        if (var2->addedToChunk && World_chunkExists(self, var3, var4)) {
            Chunk *c = self->vtable->getChunkFromChunkCoords(self, var3, var4);
            if (c)
                Chunk_removeEntity(c, var2);
        }
    }

    for (var1 = 0; var1 < self->numUnloadedEntities; var1++) {
        Entity *ue = self->unloadedEntityList[var1];

        ue->isDead = 1;
        if (ue->isPlayer) {
            for (int pi = 0; pi < self->numPlayerEntities; pi++) {
                if ((Entity *)self->playerEntities[pi] == ue) {
                    self->playerEntities[pi] = self->playerEntities[--self->numPlayerEntities];
                    break;
                }
            }
        }
        self->vtable->releaseEntitySkin(self, ue);
        Entity_destroy(ue);
    }
    self->numUnloadedEntities = 0;

    for (var1 = 0; var1 < self->numEntities; ++var1) {
        var2 = self->loadedEntityList[var1];
        if (var2->ridingEntity != NULL) {
            if (!var2->ridingEntity->isDead && var2->ridingEntity->riddenByEntity == var2) {
                continue;
            }
            var2->ridingEntity->riddenByEntity = NULL;
            var2->ridingEntity = NULL;
        }
        if (!var2->isDead) {
            World_updateEntity(self, var2);
        }
        if (var2->isDead) {
            int var3 = var2->chunkCoordX;
            int var4 = var2->chunkCoordZ;
            if (var2->addedToChunk && World_chunkExists(self, var3, var4)) {
                Chunk *c = self->vtable->getChunkFromChunkCoords(self, var3, var4);
                if (c)
                    Chunk_removeEntity(c, var2);
            }
            self->loadedEntityList[var1] = self->loadedEntityList[--self->numEntities];
            --var1;
            if (var2->isPlayer) {
                for (int pi = 0; pi < self->numPlayerEntities; pi++) {
                    if ((Entity *)self->playerEntities[pi] == var2) {
                        self->playerEntities[pi] = self->playerEntities[--self->numPlayerEntities];
                        break;
                    }
                }
            }
            self->vtable->releaseEntitySkin(self, var2);
            if (var2->isPlayerSP || var2->isClientPlayerMP) {

                if (self->deadPlayer)
                    Entity_destroy(self->deadPlayer);
                self->deadPlayer = var2;
            } else {
                Entity_destroy(var2);
            }
        }
    }

    self->field_31055_L = 1;
    for (int i = 0; i < self->numLoadedTileEntities; i++) {
        TileEntity *var5 = self->loadedTileEntityList[i];
        if (!TileEntity_isInvalid(var5)) {
            TileEntity_updateEntity(var5);
        }
        if (TileEntity_isInvalid(var5)) {
            self->loadedTileEntityList[i] =
                self->loadedTileEntityList[--self->numLoadedTileEntities];
            --i;
            Chunk *var7 =
                self->vtable->getChunkFromChunkCoords(self, var5->xCoord >> 4, var5->zCoord >> 4);
            if (var7 != NULL) {
                Chunk_removeChunkBlockTileEntity(var7, var5->xCoord & 15, var5->yCoord,
                                                 var5->zCoord & 15);
            }
            TileEntity_destroy_deferred(var5);
        }
    }
    self->field_31055_L = 0;

    if (self->field_30900_E_count > 0) {
        for (int i = 0; i < self->field_30900_E_count; i++) {
            TileEntity *var8 = self->field_30900_E[i];
            if (!TileEntity_isInvalid(var8)) {
                int found = 0;
                for (int j = 0; j < self->numLoadedTileEntities; j++) {
                    if (self->loadedTileEntityList[j] == var8) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    if (self->numLoadedTileEntities >= self->loadedTileEntityCap) {
                        int newcap = self->loadedTileEntityCap ? self->loadedTileEntityCap * 2 : 16;
                        self->loadedTileEntityList = (TileEntity **)realloc(
                            self->loadedTileEntityList, (size_t)newcap * sizeof(TileEntity *));
                        self->loadedTileEntityCap = newcap;
                    }
                    self->loadedTileEntityList[self->numLoadedTileEntities++] = var8;
                }
                Chunk *var9 = self->vtable->getChunkFromChunkCoords(self, var8->xCoord >> 4,
                                                                    var8->zCoord >> 4);
                if (var9 != NULL) {
                    Chunk_setChunkBlockTileEntity(var9, var8->xCoord & 15, var8->yCoord,
                                                  var8->zCoord & 15, var8);
                }
                World_markBlockNeedsUpdate(self, var8->xCoord, var8->yCoord, var8->zCoord);
            }
        }
        self->field_30900_E_count = 0;
    }
}

int World_isAllPlayersFullyAsleep(World *self) {
    if (self->allPlayersSleeping && !self->multiplayerWorld) {
        for (int i = 0; i < self->numPlayerEntities; i++) {
            if (!EntityPlayer_isPlayerFullyAsleep(self->playerEntities[i])) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

void World_stopPrecipitation(World *self) {
    WorldInfo_setRainTime(self->worldInfo, 0);
    WorldInfo_setRaining(self->worldInfo, 0);
    WorldInfo_setThunderTime(self->worldInfo, 0);
    WorldInfo_setThundering(self->worldInfo, 0);
}

void World_wakeUpAllPlayers(World *self) {
    self->allPlayersSleeping = 0;
    for (int i = 0; i < self->numPlayerEntities; i++) {
        if (EntityPlayer_isPlayerSleeping(self->playerEntities[i])) {
            EntityPlayer_wakeUpPlayer(self->playerEntities[i], 0, 0, 1);
        }
    }
    World_stopPrecipitation(self);
}

void World_setAllowedMobSpawns(World *self, int var1, int var2) {
    self->spawnHostileMobs = var1;
    self->spawnPeacefulMobs = var2;
}

int World_TickUpdates(World *self, int var1) {
    int var2 = self->numPendingTicks;
    if (var2 > 1000)
        var2 = 1000;
    for (int var3 = 0; var3 < var2; ++var3) {
        NextTickListEntry *var4 = &self->pendingTickList[0];
        if (!var1 && var4->scheduledTime > WorldInfo_getWorldTime(self->worldInfo))
            break;
        NextTickListEntry entry = self->pendingTickList[0];
        --self->numPendingTicks;
        if (self->numPendingTicks > 0)
            memmove(&self->pendingTickList[0], &self->pendingTickList[1],
                    (size_t)self->numPendingTicks * sizeof(NextTickListEntry));
        int8_t var5 = 8;
        if (World_checkChunksExist(self, entry.xCoord - var5, entry.yCoord - var5,
                                   entry.zCoord - var5, entry.xCoord + var5, entry.yCoord + var5,
                                   entry.zCoord + var5)) {
            int var6 = World_getBlockId(self, entry.xCoord, entry.yCoord, entry.zCoord);
            if (var6 == entry.blockID && var6 > 0) {
                Block_blocksList[var6]->vtable->updateTick(Block_blocksList[var6], self,
                                                           entry.xCoord, entry.yCoord, entry.zCoord,
                                                           &self->rand);
            }
        }
    }
    return self->numPendingTicks != 0;
}

void World_updateBlocksAndPlayCaveSounds(World *self) {

    self->numPositionsToUpdate = 0;

    for (int var1 = 0; var1 < self->numPlayerEntities; ++var1) {
        EntityPlayer *var2 = self->playerEntities[var1];
        int var3 = MathHelper_floor_double(((Entity *)var2)->posX / 16.0);
        int var4 = MathHelper_floor_double(((Entity *)var2)->posZ / 16.0);
        int8_t var5 = 9;
        for (int var6 = -var5; var6 <= var5; ++var6) {
            for (int var7 = -var5; var7 <= var5; ++var7) {
                ChunkCoordIntPair pair;
                pair.chunkXPos = var6 + var3;
                pair.chunkZPos = var7 + var4;

                int dup = 0;
                for (int d = 0; d < self->numPositionsToUpdate; d++) {
                    if (self->positionsToUpdate[d].chunkXPos == pair.chunkXPos &&
                        self->positionsToUpdate[d].chunkZPos == pair.chunkZPos) {
                        dup = 1;
                        break;
                    }
                }
                if (!dup) {
                    if (self->numPositionsToUpdate >= self->positionsToUpdateCap) {
                        int newcap =
                            self->positionsToUpdateCap ? self->positionsToUpdateCap * 2 : 256;
                        self->positionsToUpdate = (ChunkCoordIntPair *)realloc(
                            self->positionsToUpdate, (size_t)newcap * sizeof(ChunkCoordIntPair));
                        self->positionsToUpdateCap = newcap;
                    }
                    self->positionsToUpdate[self->numPositionsToUpdate++] = pair;
                }
            }
        }
    }

    if (self->soundCounter > 0) {
        --self->soundCounter;
    }

    for (int pi = 0; pi < self->numPositionsToUpdate; pi++) {
        ChunkCoordIntPair *var13 = &self->positionsToUpdate[pi];
        int var3 = var13->chunkXPos * 16;
        int var4 = var13->chunkZPos * 16;
        Chunk *var14 =
            self->vtable->getChunkFromChunkCoords(self, var13->chunkXPos, var13->chunkZPos);
        if (!var14)
            continue;
        int var6, var7, var8, var9, var10;

        if (self->soundCounter == 0) {
            self->field_9437_g = (int)((unsigned int)self->field_9437_g * 3U + 1013904223U);
            var6 = self->field_9437_g >> 2;
            var7 = var6 & 15;
            var8 = var6 >> 8 & 15;
            var9 = var6 >> 16 & 127;
            var10 = Chunk_getBlockID(var14, var7, var9, var8);
            var7 += var3;
            var8 += var4;
            if (var10 == 0 &&
                self->vtable->getFullBlockLightValue(self, var7, var9, var8) <=
                    JavaRandom_nextInt(&self->rand, 8) &&
                World_getSavedLightValue(self, EnumSkyBlock_Sky, var7, var9, var8) <= 0) {
                Entity *var11 = World_getClosestPlayer(self, (double)var7 + 0.5, (double)var9 + 0.5,
                                                       (double)var8 + 0.5, 8.0);
                if (var11 != NULL &&
                    var11->vtable->getDistanceSq(var11, (double)var7 + 0.5, (double)var9 + 0.5,
                                                 (double)var8 + 0.5) > 4.0) {
                    World_playSoundEffect(self, (double)var7 + 0.5, (double)var9 + 0.5,
                                          (double)var8 + 0.5, "ambient.cave.cave", 0.7f,
                                          0.8f + JavaRandom_nextFloat(&self->rand) * 0.2f);
                    self->soundCounter = JavaRandom_nextInt(&self->rand, 12000) + 6000;
                }
            }
        }

        if (JavaRandom_nextInt(&self->rand, 100000) == 0 && World_func_27161_C(self) &&
            World_func_27160_B(self)) {
            self->field_9437_g = (int)((unsigned int)self->field_9437_g * 3U + 1013904223U);
            var6 = self->field_9437_g >> 2;
            var7 = var3 + (var6 & 15);
            var8 = var4 + (var6 >> 8 & 15);
            var9 = World_findTopSolidBlock(self, var7, var8);
            if (World_canBlockBeRainedOn(self, var7, var9, var8)) {
                EntityLightningBolt *bolt =
                    EntityLightningBolt_create(self, (double)var7, (double)var9, (double)var8);
                World_addWeatherEffect(self, (Entity *)bolt);
                self->field_27168_F = 2;
            }
        }

        if (JavaRandom_nextInt(&self->rand, 16) == 0) {
            self->field_9437_g = (int)((unsigned int)self->field_9437_g * 3U + 1013904223U);
            var6 = self->field_9437_g >> 2;
            var7 = var6 & 15;
            var8 = var6 >> 8 & 15;
            var9 = World_findTopSolidBlock(self, var7 + var3, var8 + var4);
            BiomeGenBase *biome =
                WorldChunkManager_getBiomeGenAt(self->worldChunkManager, var7 + var3, var8 + var4);
            if (BiomeGenBase_getEnableSnow(biome) && var9 >= 0 && var9 < 128 &&
                Chunk_getSavedLightValue(var14, EnumSkyBlock_Block, var7, var9, var8) < 10) {
                var10 = Chunk_getBlockID(var14, var7, var9 - 1, var8);
                int var15 = Chunk_getBlockID(var14, var7, var9, var8);
                if (World_func_27161_C(self) && var15 == 0 &&
                    Block_snow->vtable->canPlaceBlockAt(Block_snow, self, var7 + var3, var9,
                                                        var8 + var4) &&
                    var10 != 0 && var10 != Block_ice->blockID && Block_blocksList[var10] &&
                    Material_getIsSolid(Block_blocksList[var10]->blockMaterial)) {
                    World_setBlockWithNotify(self, var7 + var3, var9, var8 + var4,
                                             Block_snow->blockID);
                }
                if (var10 == Block_waterStill->blockID &&
                    Chunk_getBlockMetadata(var14, var7, var9 - 1, var8) == 0) {
                    World_setBlockWithNotify(self, var7 + var3, var9 - 1, var8 + var4,
                                             Block_ice->blockID);
                }
            }
        }

        for (var6 = 0; var6 < 80; ++var6) {
            self->field_9437_g = (int)((unsigned int)self->field_9437_g * 3U + 1013904223U);
            var7 = self->field_9437_g >> 2;
            var8 = var7 & 15;
            var9 = var7 >> 8 & 15;
            var10 = var7 >> 16 & 127;
            int var15 = var14->blocks[var8 << 11 | var9 << 7 | var10] & 255;
            if (Block_tickOnLoad[var15] && Block_blocksList[var15]) {
                Block_blocksList[var15]->vtable->updateTick(
                    Block_blocksList[var15], self, var8 + var3, var10, var9 + var4, &self->rand);
            }
        }
    }
}

void World_randomDisplayUpdates(World *self, int var1, int var2, int var3) {
    int8_t var4 = 16;
    JavaRandom var5;
    JavaRandom_new(&var5);
    for (int var6 = 0; var6 < 1000; ++var6) {
        int var7 =
            var1 + JavaRandom_nextInt(&self->rand, var4) - JavaRandom_nextInt(&self->rand, var4);
        int var8 =
            var2 + JavaRandom_nextInt(&self->rand, var4) - JavaRandom_nextInt(&self->rand, var4);
        int var9 =
            var3 + JavaRandom_nextInt(&self->rand, var4) - JavaRandom_nextInt(&self->rand, var4);
        int var10 = World_getBlockId(self, var7, var8, var9);
        if (var10 > 0) {
            Block_blocksList[var10]->vtable->randomDisplayTick(Block_blocksList[var10], self, var7,
                                                               var8, var9, &var5);
        }
    }
}

static void def_updateWeather(World *self) {
    if (!self->worldProvider->hasNoSky) {
        if (self->field_27168_F > 0) {
            --self->field_27168_F;
        }

        int var1 = WorldInfo_getThunderTime(self->worldInfo);
        if (var1 <= 0) {
            if (WorldInfo_getThundering(self->worldInfo)) {
                WorldInfo_setThunderTime(self->worldInfo,
                                         JavaRandom_nextInt(&self->rand, 12000) + 3600);
            } else {
                WorldInfo_setThunderTime(self->worldInfo,
                                         JavaRandom_nextInt(&self->rand, 168000) + 12000);
            }
        } else {
            --var1;
            WorldInfo_setThunderTime(self->worldInfo, var1);
            if (var1 <= 0) {
                WorldInfo_setThundering(self->worldInfo, !WorldInfo_getThundering(self->worldInfo));
            }
        }

        int var2i = WorldInfo_getRainTime(self->worldInfo);
        if (var2i <= 0) {
            if (WorldInfo_getRaining(self->worldInfo)) {
                WorldInfo_setRainTime(self->worldInfo,
                                      JavaRandom_nextInt(&self->rand, 12000) + 12000);
            } else {
                WorldInfo_setRainTime(self->worldInfo,
                                      JavaRandom_nextInt(&self->rand, 168000) + 12000);
            }
        } else {
            --var2i;
            WorldInfo_setRainTime(self->worldInfo, var2i);
            if (var2i <= 0) {
                WorldInfo_setRaining(self->worldInfo, !WorldInfo_getRaining(self->worldInfo));
            }
        }

        self->prevRainingStrength = self->rainingStrength;
        if (WorldInfo_getRaining(self->worldInfo)) {
            self->rainingStrength = (float)((double)self->rainingStrength + 0.01);
        } else {
            self->rainingStrength = (float)((double)self->rainingStrength - 0.01);
        }
        if (self->rainingStrength < 0.0f)
            self->rainingStrength = 0.0f;
        if (self->rainingStrength > 1.0f)
            self->rainingStrength = 1.0f;

        self->prevThunderingStrength = self->thunderingStrength;
        if (WorldInfo_getThundering(self->worldInfo)) {
            self->thunderingStrength = (float)((double)self->thunderingStrength + 0.01);
        } else {
            self->thunderingStrength = (float)((double)self->thunderingStrength - 0.01);
        }
        if (self->thunderingStrength < 0.0f)
            self->thunderingStrength = 0.0f;
        if (self->thunderingStrength > 1.0f)
            self->thunderingStrength = 1.0f;
    }
}

void World_tick(World *self) {

    self->vtable->updateWeather(self);

    if (World_isAllPlayersFullyAsleep(self)) {
        int var1b = 0;
        if (self->spawnHostileMobs && self->difficultySetting >= 1) {
            var1b = SpawnerAnimals_performSleepSpawning(self, self->playerEntities,
                                                        self->numPlayerEntities);
        }
        if (!var1b) {
            int64_t var2 = WorldInfo_getWorldTime(self->worldInfo) + 24000L;
            WorldInfo_setWorldTime(self->worldInfo, var2 - var2 % 24000L);
            World_wakeUpAllPlayers(self);
        }
    }

    SpawnerAnimals_performSpawning(self, self->spawnHostileMobs, self->spawnPeacefulMobs);

    if (self->chunkProvider)
        IChunkProvider_unload100OldestChunks(self->chunkProvider);

    int var4 = World_calculateSkylightSubtracted(self, 1.0f);
    if (var4 != self->skylightSubtracted) {
        self->skylightSubtracted = var4;
        for (int var5 = 0; var5 < self->numWorldAccesses; ++var5) {
            IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var5];
            wa->vtable->updateAllRenderers(wa);
        }
    }

    int64_t var2 = WorldInfo_getWorldTime(self->worldInfo) + 1L;
    if (var2 % (int64_t)self->autosavePeriod == 0L) {
        World_saveWorld(self, 0, NULL);
    }
    WorldInfo_setWorldTime(self->worldInfo, var2);
    self->worldTime = var2;

    World_TickUpdates(self, 0);

    World_updateBlocksAndPlayCaveSounds(self);
}

static void World_saveLevel(World *self) {
    World_checkSessionLock(self);
    if (self->saveHandler && self->worldInfo) {
        ISaveHandler_saveWorldInfoAndPlayer(self->saveHandler, self->worldInfo,
                                            self->playerEntities, self->numPlayerEntities);
    }

    if (self->field_28108_z)
        MapStorage_saveAllData(self->field_28108_z);
}

void World_saveWorld(World *self, int var1, IProgressUpdate *var2) {
    if (self->chunkProvider && IChunkProvider_canSave(self->chunkProvider)) {
        if (var2 != NULL)
            var2->func_594_b(var2, "Saving level");
        World_saveLevel(self);
        if (var2 != NULL)
            var2->displayLoadingString(var2, "Saving chunks");
        IChunkProvider_saveChunks(self->chunkProvider, var1, var2);
    }
}

void World_saveWorldIndirectly(World *self, IProgressUpdate *var1) {
    World_saveWorld(self, 1, var1);
}

void World_checkSessionLock(World *self) {
    if (self->saveHandler)
        ISaveHandler_func_22150_b(self->saveHandler);
}

WorldInfo *World_getWorldInfo(World *self) { return self->worldInfo; }

int World_getCollidingBoundingBoxes(World *self, struct Entity *entity, AxisAlignedBB *aabb,
                                    AxisAlignedBB *outList, int maxOut) {
    int count = 0;

    int var3 = MathHelper_floor_double(aabb->minX);
    int var4 = MathHelper_floor_double(aabb->maxX + 1.0);
    int var5 = MathHelper_floor_double(aabb->minY);
    int var6 = MathHelper_floor_double(aabb->maxY + 1.0);
    int var7 = MathHelper_floor_double(aabb->minZ);
    int var8 = MathHelper_floor_double(aabb->maxZ + 1.0);

    for (int var9 = var3; var9 < var4 && count < maxOut; var9++) {
        for (int var10 = var7; var10 < var8 && count < maxOut; var10++) {
            if (!World_blockExists(self, var9, 64, var10))
                continue;

            for (int var11 = var5 - 1; var11 < var6 && count < maxOut; var11++) {
                Block *var12 = Block_blocksList[World_getBlockId(self, var9, var11, var10)];
                if (var12 == NULL)
                    continue;

                CollidingList cl = {outList, count, maxOut};
                var12->vtable->getCollidingBoundingBoxes(var12, self, var9, var11, var10, aabb,
                                                         &cl);
                count = cl.count;
            }
        }
    }

    double var14 = 0.25;
    struct Entity *entList[512];
    int entCount = World_getEntitiesWithinAABBExcludingEntity(
        self, entity, AxisAlignedBB_expand(aabb, var14, var14, var14), entList, 512);
    for (int var16 = 0; var16 < entCount && count < maxOut; var16++) {
        struct Entity *ent = entList[var16];

        AxisAlignedBB *var13 = ent->vtable->getBoundingBox(ent);
        if (var13 != NULL && AxisAlignedBB_intersectsWith(var13, aabb))
            outList[count++] = *var13;
        if (count >= maxOut)
            break;

        if (entity != NULL)
            var13 = entity->vtable->getCollisionBox(entity, ent);
        else
            var13 = NULL;
        if (var13 != NULL && AxisAlignedBB_intersectsWith(var13, aabb))
            outList[count++] = *var13;
    }

    return count;
}

int World_getBlockId(World *s, int x, int y, int z) { return s->vtable->getBlockId(s, x, y, z); }
int World_getBlockMetadata(World *s, int x, int y, int z) {
    return s->vtable->getBlockMetadata(s, x, y, z);
}
int World_setBlock(World *s, int x, int y, int z, int id) {
    return s->vtable->setBlock(s, x, y, z, id);
}
int World_setBlockWithNotify(World *s, int x, int y, int z, int id) {
    return s->vtable->setBlockWithNotify(s, x, y, z, id);
}
int World_setBlockAndMetadata(World *s, int x, int y, int z, int id, int m) {
    return s->vtable->setBlockAndMetadata(s, x, y, z, id, m);
}
int World_setBlockMetadataWithNotify(World *s, int x, int y, int z, int m) {
    return s->vtable->setBlockMetadataWithNotify(s, x, y, z, m);
}
int World_setBlockToAir(World *s, int x, int y, int z) {
    return s->vtable->setBlockToAir(s, x, y, z);
}
TileEntity *World_getBlockTileEntity(World *s, int x, int y, int z) {
    return s->vtable->getBlockTileEntity(s, x, y, z);
}
void World_setBlockTileEntity(World *s, int x, int y, int z, TileEntity *t) {
    s->vtable->setBlockTileEntity(s, x, y, z, t);
}
void World_removeBlockTileEntity(World *s, int x, int y, int z) {
    s->vtable->removeBlockTileEntity(s, x, y, z);
}
int World_isAirBlock(World *s, int x, int y, int z) { return s->vtable->isAirBlock(s, x, y, z); }

int World_getFirstUncoveredBlock(World *s, int x, int z) {
    int var3;
    for (var3 = 63; !World_isAirBlock(s, x, var3 + 1, z); ++var3) {
    }
    return World_getBlockId(s, x, var3, z);
}

Material *World_getBlockMaterial(World *s, int x, int y, int z) {
    int var4 = World_getBlockId(s, x, y, z);
    if (var4 == 0 || !Block_blocksList[var4])
        return Material_air;
    return Block_blocksList[var4]->blockMaterial;
}
int World_blockExists(World *s, int x, int y, int z) { return s->vtable->blockExists(s, x, y, z); }
int World_isBlockNormalCube(World *s, int x, int y, int z) {
    return s->vtable->isBlockNormalCube(s, x, y, z);
}
int World_isBlockOpaqueCube(World *s, int x, int y, int z) {
    return s->vtable->isBlockOpaqueCube(s, x, y, z);
}
int World_isBlockSolid(World *s, int x, int y, int z) {
    return s->vtable->isBlockNormalCube(s, x, y, z);
}

long World_getRandomSeed(World *s) {
    return s->worldInfo ? (long)WorldInfo_getRandomSeed(s->worldInfo) : (long)s->seed;
}

int64_t World_getWorldTime(World *s) {
    return s->worldInfo ? WorldInfo_getWorldTime(s->worldInfo) : s->worldTime;
}
WorldChunkManager *World_getWorldChunkManager(World *s) { return s->worldChunkManager; }
int World_doesBlockHaveSolidTopSurface(World *s, int x, int y, int z) {
    return s->vtable->doesBlockHaveSolidTopSurface(s, x, y, z);
}
int World_getHeightValue(World *s, int x, int z) { return s->vtable->getHeightValue(s, x, z); }
int World_getBlockLightValue(World *s, int x, int y, int z) {
    return s->vtable->getBlockLightValue(s, x, y, z);
}
int World_isBlockIndirectlyGettingPowered(World *s, int x, int y, int z) {
    return s->vtable->isBlockIndirectlyGettingPowered(s, x, y, z);
}
void World_scheduleBlockUpdate(World *s, int x, int y, int z, int id, int d) {
    s->vtable->scheduleBlockUpdate(s, x, y, z, id, d);
}
void World_notifyBlocksOfNeighborChange(World *s, int x, int y, int z, int id) {
    s->vtable->notifyBlocksOfNeighborChange(s, x, y, z, id);
}
void World_notifyBlockOfNeighborChange(World *s, int x, int y, int z, int n) {
    s->vtable->notifyBlockOfNeighborChange(s, x, y, z, n);
}
int World_spawnEntityInWorld(World *s, Entity *e) { return s->vtable->spawnEntityInWorld(s, e); }
void World_removeEntity(World *s, Entity *e) { s->vtable->removeEntity(s, e); }
void World_playSoundEffect(World *s, double x, double y, double z, const char *snd, float v,
                           float p) {
    s->vtable->playSoundEffect(s, x, y, z, snd, v, p);
}
void World_playAuxSFX(World *s, int eid, int x, int y, int z, int data) {
    s->vtable->playAuxSFX(s, eid, x, y, z, data);
}

void World_onBlockHit(World *self, struct EntityPlayer *player, int x, int y, int z, int side) {
    if (side == 0)
        --y;
    else if (side == 1)
        ++y;
    else if (side == 2)
        --z;
    else if (side == 3)
        ++z;
    else if (side == 4)
        --x;
    else if (side == 5)
        ++x;
    extern Block *Block_fire;
    if (Block_fire && World_getBlockId(self, x, y, z) == Block_fire->blockID) {
        World_func_28107_a(self, player, 1004, x, y, z, 0);
        World_setBlockWithNotify(self, x, y, z, 0);
    }
}

float World_getRainStrength(World *self, float partialTick) {
    return self->prevRainingStrength +
           (self->rainingStrength - self->prevRainingStrength) * partialTick;
}

float World_getThunderStrength(World *self, float partialTick) {
    return self->prevThunderingStrength +
           (self->thunderingStrength - self->prevThunderingStrength) * partialTick;
}

Vec3D *World_getSkyColor(World *self, Entity *viewer, float partialTick) {
    static Vec3D col;
    float var3 = World_getCelestialAngle(self, partialTick);
    float var4 = MathHelper_cos(var3 * (float)(3.14159265358979323846 * 2.0)) * 2.0f + 0.5f;
    if (var4 < 0.0f)
        var4 = 0.0f;
    if (var4 > 1.0f)
        var4 = 1.0f;

    int var5 = MathHelper_floor_double(viewer->posX);
    int var6 = MathHelper_floor_double(viewer->posZ);
    float var7 = (float)WorldChunkManager_getTemperature(self->worldChunkManager, var5, var6);
    BiomeGenBase *biome = WorldChunkManager_getBiomeGenAt(self->worldChunkManager, var5, var6);
    int var8 = biome->vtable->getSkyColorByTemp(biome, var7);
    float var9 = (float)(var8 >> 16 & 255) / 255.0f;
    float var10 = (float)(var8 >> 8 & 255) / 255.0f;
    float var11 = (float)(var8 & 255) / 255.0f;
    var9 *= var4;
    var10 *= var4;
    var11 *= var4;

    float var12 = World_func_27162_g(self, partialTick);
    float var13, var14;
    if (var12 > 0.0f) {
        var13 = (var9 * 0.3f + var10 * 0.59f + var11 * 0.11f) * 0.6f;
        var14 = 1.0f - var12 * (12.0f / 16.0f);
        var9 = var9 * var14 + var13 * (1.0f - var14);
        var10 = var10 * var14 + var13 * (1.0f - var14);
        var11 = var11 * var14 + var13 * (1.0f - var14);
    }

    var13 = World_func_27166_f(self, partialTick);
    if (var13 > 0.0f) {
        var14 = (var9 * 0.3f + var10 * 0.59f + var11 * 0.11f) * 0.2f;
        float var15 = 1.0f - var13 * (12.0f / 16.0f);
        var9 = var9 * var15 + var14 * (1.0f - var15);
        var10 = var10 * var15 + var14 * (1.0f - var15);
        var11 = var11 * var15 + var14 * (1.0f - var15);
    }

    if (self->field_27172_i > 0) {
        var14 = (float)self->field_27172_i - partialTick;
        if (var14 > 1.0f)
            var14 = 1.0f;
        var14 *= 0.45f;
        var9 = var9 * (1.0f - var14) + 0.8f * var14;
        var10 = var10 * (1.0f - var14) + 0.8f * var14;
        var11 = var11 * (1.0f - var14) + 1.0f * var14;
    }

    col.xCoord = var9;
    col.yCoord = var10;
    col.zCoord = var11;
    return &col;
}

Vec3D *World_getFogColor(World *self, float partialTick) {
    float var2 = World_getCelestialAngle(self, partialTick);
    return WorldProvider_func_4096_a(self->worldProvider, var2, partialTick);
}

Vec3D *World_getCloudColor(World *self, float partialTick) {
    static Vec3D col;
    float var2 = World_getCelestialAngle(self, partialTick);
    float var3 = MathHelper_cos(var2 * (float)(3.14159265358979323846 * 2.0)) * 2.0f + 0.5f;
    if (var3 < 0.0f)
        var3 = 0.0f;
    if (var3 > 1.0f)
        var3 = 1.0f;

    float var4 = (float)(self->field_1019_F >> 16 & 255LL) / 255.0f;
    float var5 = (float)(self->field_1019_F >> 8 & 255LL) / 255.0f;
    float var6 = (float)(self->field_1019_F & 255LL) / 255.0f;

    float var7 = World_func_27162_g(self, partialTick);
    float var8, var9;
    if (var7 > 0.0f) {
        var8 = (var4 * 0.3f + var5 * 0.59f + var6 * 0.11f) * 0.6f;
        var9 = 1.0f - var7 * 0.95f;
        var4 = var4 * var9 + var8 * (1.0f - var9);
        var5 = var5 * var9 + var8 * (1.0f - var9);
        var6 = var6 * var9 + var8 * (1.0f - var9);
    }

    var4 *= var3 * 0.9f + 0.1f;
    var5 *= var3 * 0.9f + 0.1f;
    var6 *= var3 * 0.85f + 0.15f;

    var8 = World_func_27166_f(self, partialTick);
    if (var8 > 0.0f) {
        var9 = (var4 * 0.3f + var5 * 0.59f + var6 * 0.11f) * 0.2f;
        float var10 = 1.0f - var8 * 0.95f;
        var4 = var4 * var10 + var9 * (1.0f - var10);
        var5 = var5 * var10 + var9 * (1.0f - var10);
        var6 = var6 * var10 + var9 * (1.0f - var10);
    }

    col.xCoord = var4;
    col.yCoord = var5;
    col.zCoord = var6;
    return &col;
}

int World_findTopSolidBlock(World *self, int x, int z) {
    Chunk *var3 = self->vtable->getChunkFromBlockCoords(self, x, z);
    if (!var3)
        return -1;
    int lx = x & 15;
    int lz = z & 15;
    for (int var4 = 127; var4 > 0; --var4) {
        int var5 = Chunk_getBlockID(var3, lx, var4, lz);
        Material *var6 = (var5 == 0) ? Material_air : Block_blocksList[var5]->blockMaterial;
        if (Material_getIsSolid(var6) || Material_getIsLiquid(var6))
            return var4 + 1;
    }
    return -1;
}

int World_isNether(World *self) { return self->isNether; }

struct Entity *World_getClosestPlayer(World *self, double x, double y, double z, double dist) {
    double var9 = -1.0;
    struct EntityPlayer *var11 = NULL;
    for (int var12 = 0; var12 < self->numPlayerEntities; var12++) {
        struct EntityPlayer *var13 = self->playerEntities[var12];
        double var14 = ((Entity *)var13)->vtable->getDistanceSq((Entity *)var13, x, y, z);
        if ((dist < 0.0 || var14 < dist * dist) && (var9 == -1.0 || var14 < var9)) {
            var9 = var14;
            var11 = var13;
        }
    }
    return (struct Entity *)var11;
}

struct Entity *World_getClosestPlayerToEntity(World *self, struct Entity *entity, double dist) {
    return World_getClosestPlayer(self, entity->posX, entity->posY, entity->posZ, dist);
}

void World_playSoundAtEntity(World *self, struct Entity *entity, const char *name, float vol,
                             float pitch) {
    for (int var5 = 0; var5 < self->numWorldAccesses; var5++) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var5];
        wa->vtable->playSound(wa, name, entity->posX, entity->posY - (double)entity->yOffset,
                              entity->posZ, vol, pitch);
    }
}

int World_entityJoinedWorld(World *self, struct Entity *entity) {
    int var2 = MathHelper_floor_double(entity->posX / 16.0);
    int var3 = MathHelper_floor_double(entity->posZ / 16.0);

    int var4 = entity->isPlayer;
    if (!var4 && !World_chunkExists(self, var2, var3)) {
        return 0;
    }
    if (var4) {
        struct EntityPlayer *var5 = (struct EntityPlayer *)entity;
        if (self->numPlayerEntities >= self->playerEntitiesCap) {
            int newcap = self->playerEntitiesCap == 0 ? 4 : self->playerEntitiesCap * 2;
            self->playerEntities = (struct EntityPlayer **)realloc(
                self->playerEntities, (size_t)newcap * sizeof(struct EntityPlayer *));
            self->playerEntitiesCap = newcap;
        }
        self->playerEntities[self->numPlayerEntities++] = var5;
        World_updateAllPlayersSleepingFlag(self);
    }
    Chunk *chunk =
        (Chunk *)self->chunkProvider->vtable->provideChunk(self->chunkProvider, var2, var3);
    if (chunk)
        Chunk_addEntity(chunk, entity);
    if (self->numEntities >= self->entityCap) {
        int newcap = self->entityCap == 0 ? 16 : self->entityCap * 2;
        self->loadedEntityList =
            (Entity **)realloc(self->loadedEntityList, (size_t)newcap * sizeof(Entity *));
        self->entityCap = newcap;
    }
    self->loadedEntityList[self->numEntities++] = entity;
    self->vtable->obtainEntitySkin(self, entity);
    return 1;
}

int World_doChunksNearChunkExist(World *self, int x, int y, int z, int range) {
    return World_checkChunksExist(self, x - range, y - range, z - range, x + range, y + range,
                                  z + range);
}

int World_getEntitiesWithinAABBExcludingEntity(World *self, struct Entity *entity,
                                               AxisAlignedBB *bb, struct Entity **outList,
                                               int maxOut) {
    int var3 = MathHelper_floor_double((bb->minX - 2.0) / 16.0);
    int var4 = MathHelper_floor_double((bb->maxX + 2.0) / 16.0);
    int var5 = MathHelper_floor_double((bb->minZ - 2.0) / 16.0);
    int var6 = MathHelper_floor_double((bb->maxZ + 2.0) / 16.0);
    int total = 0;
    for (int var7 = var3; var7 <= var4; var7++) {
        for (int var8 = var5; var8 <= var6; var8++) {
            if (World_chunkExists(self, var7, var8)) {
                Chunk *chunk = (Chunk *)self->chunkProvider->vtable->provideChunk(
                    self->chunkProvider, var7, var8);
                if (chunk) {
                    total += Chunk_getEntitiesWithinAABBForEntity(chunk, entity, bb, outList, total,
                                                                  maxOut);
                }
            }
        }
    }
    return total;
}

void World_spawnParticle(World *self, const char *name, double x, double y, double z, double vx,
                         double vy, double vz) {
    for (int var14 = 0; var14 < self->numWorldAccesses; var14++) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var14];
        wa->vtable->spawnParticle(wa, name, x, y, z, vx, vy, vz);
    }
}

int World_checkIfAABBIsClear(World *self, AxisAlignedBB *bb) {
    struct Entity *list[256];
    int count = World_getEntitiesWithinAABBExcludingEntity(self, NULL, bb, list, 256);
    for (int var3 = 0; var3 < count; var3++) {
        struct Entity *var4 = list[var3];
        if (!var4->isDead && var4->preventEntitySpawning) {
            return 0;
        }
    }
    return 1;
}

int World_canBlockBePlacedAt(World *self, int var1, int var2, int var3, int var4, int var5,
                             int var6) {
    int var7 = World_getBlockId(self, var2, var3, var4);
    Block *var8 = Block_blocksList[var7];
    Block *var9 = Block_blocksList[var1];
    AxisAlignedBB *var10 =
        var9 ? var9->vtable->getCollisionBoundingBoxFromPool(var9, self, var2, var3, var4) : NULL;
    if (var5) {
        var10 = NULL;
    }
    if (var10 != NULL && !World_checkIfAABBIsClear(self, var10)) {
        return 0;
    } else {
        if (var8 == Block_waterMoving || var8 == Block_waterStill || var8 == Block_lavaMoving ||
            var8 == Block_lavaStill || var8 == Block_fire || var8 == Block_snow) {
            var8 = NULL;
        }
        return var1 > 0 && var8 == NULL && var9 != NULL &&
               var9->vtable->canPlaceBlockOnSide(var9, self, var2, var3, var4, var6);
    }
}

int World_getIsAnyLiquid(World *self, AxisAlignedBB *bb) {
    int var2 = MathHelper_floor_double(bb->minX);
    int var3 = MathHelper_floor_double(bb->maxX + 1.0);
    int var4 = MathHelper_floor_double(bb->minY);
    int var5 = MathHelper_floor_double(bb->maxY + 1.0);
    int var6 = MathHelper_floor_double(bb->minZ);
    int var7 = MathHelper_floor_double(bb->maxZ + 1.0);
    if (bb->minX < 0.0) {
        --var2;
    }
    if (bb->minY < 0.0) {
        --var4;
    }
    if (bb->minZ < 0.0) {
        --var6;
    }
    for (int var8 = var2; var8 < var3; var8++) {
        for (int var9 = var4; var9 < var5; var9++) {
            for (int var10 = var6; var10 < var7; var10++) {
                int id = World_getBlockId(self, var8, var9, var10);
                Block *var11 = Block_blocksList[id];
                if (var11 != NULL && Material_getIsLiquid(var11->blockMaterial)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int World_isAABBInMaterial(World *self, AxisAlignedBB *var1, Material *var2) {
    int var3 = MathHelper_floor_double(var1->minX);
    int var4 = MathHelper_floor_double(var1->maxX + 1.0);
    int var5 = MathHelper_floor_double(var1->minY);
    int var6 = MathHelper_floor_double(var1->maxY + 1.0);
    int var7 = MathHelper_floor_double(var1->minZ);
    int var8 = MathHelper_floor_double(var1->maxZ + 1.0);
    for (int var9 = var3; var9 < var4; ++var9) {
        for (int var10 = var5; var10 < var6; ++var10) {
            for (int var11 = var7; var11 < var8; ++var11) {
                int id = World_getBlockId(self, var9, var10, var11);
                Block *var12 = Block_blocksList[id];
                if (var12 != NULL && var12->blockMaterial == var2) {
                    int var13 = World_getBlockMetadata(self, var9, var10, var11);
                    double var14 = (double)(var10 + 1);
                    if (var13 < 8) {
                        var14 = (double)(var10 + 1) - (double)var13 / 8.0;
                    }
                    if (var14 >= var1->minY) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int World_handleMaterialAcceleration(World *self, AxisAlignedBB *bb, Material *mat,
                                     struct Entity *entity) {
    int var4 = MathHelper_floor_double(bb->minX);
    int var5 = MathHelper_floor_double(bb->maxX + 1.0);
    int var6 = MathHelper_floor_double(bb->minY);
    int var7 = MathHelper_floor_double(bb->maxY + 1.0);
    int var8 = MathHelper_floor_double(bb->minZ);
    int var9 = MathHelper_floor_double(bb->maxZ + 1.0);
    if (!World_checkChunksExist(self, var4, var6, var8, var5, var7, var9)) {
        return 0;
    }
    int var10 = 0;
    Vec3D *var11 = Vec3D_createVector(0.0, 0.0, 0.0);
    for (int var12 = var4; var12 < var5; var12++) {
        for (int var13 = var6; var13 < var7; var13++) {
            for (int var14 = var8; var14 < var9; var14++) {
                int id = World_getBlockId(self, var12, var13, var14);
                Block *var15 = Block_blocksList[id];
                if (var15 != NULL && var15->blockMaterial == mat) {
                    double var16 = (double)((float)(var13 + 1) -
                                            BlockFluid_getPercentAir(
                                                World_getBlockMetadata(self, var12, var13, var14)));
                    if ((double)var7 >= var16) {
                        var10 = 1;
                        var15->vtable->velocityToAddToEntity(var15, self, var12, var13, var14,
                                                             entity, var11);
                    }
                }
            }
        }
    }
    if (Vec3D_lengthVector(var11) > 0.0) {
        var11 = Vec3D_normalize(var11);
        double var18 = 0.014;
        entity->motionX += var11->xCoord * var18;
        entity->motionY += var11->yCoord * var18;
        entity->motionZ += var11->zCoord * var18;
    }
    return var10;
}

void World_createExplosion(World *self, struct Entity *entity, double x, double y, double z,
                           float strength) {
    World_newExplosion(self, entity, x, y, z, strength, 0);
}

int World_getEntitiesWithinAABB(World *self, int (*match)(struct Entity *), AxisAlignedBB *bb,
                                struct Entity **outList, int maxOut) {
    int var3 = MathHelper_floor_double((bb->minX - 2.0) / 16.0);
    int var4 = MathHelper_floor_double((bb->maxX + 2.0) / 16.0);
    int var5 = MathHelper_floor_double((bb->minZ - 2.0) / 16.0);
    int var6 = MathHelper_floor_double((bb->maxZ + 2.0) / 16.0);
    int total = 0;
    for (int var8 = var3; var8 <= var4; var8++) {
        for (int var9 = var5; var9 <= var6; var9++) {
            if (World_chunkExists(self, var8, var9)) {
                Chunk *chunk = (Chunk *)self->chunkProvider->vtable->provideChunk(
                    self->chunkProvider, var8, var9);
                if (chunk) {
                    total +=
                        Chunk_getEntitiesOfTypeWithinAAAB(chunk, match, bb, outList, total, maxOut);
                }
            }
        }
    }
    return total;
}

struct Entity *World_getPlayerEntityByName(World *self, const char *name) {
    for (int var2 = 0; var2 < self->numPlayerEntities; var2++) {
        if (strcmp(self->playerEntities[var2]->username, name) == 0) {
            return (struct Entity *)self->playerEntities[var2];
        }
    }
    return NULL;
}

struct PathEntity *World_getPathToEntity(World *self, struct Entity *var1, struct Entity *var2,
                                         float var3) {
    return Pathfinder_createEntityPathToEntity(self->pathFinder, var1, var2, var3);
}

struct PathEntity *World_getEntityPathToXYZ(World *self, struct Entity *var1, int var2, int var3,
                                            int var4, float var5) {
    return Pathfinder_createEntityPathToCoords(self->pathFinder, var1, var2, var3, var4, var5);
}

int World_addWeatherEffect(World *self, struct Entity *var1) {
    if (self->numWeatherEffects >= self->weatherEffectsCap) {
        int newCap = self->weatherEffectsCap == 0 ? 16 : self->weatherEffectsCap * 2;
        self->weatherEffects =
            (struct Entity **)realloc(self->weatherEffects, newCap * sizeof(struct Entity *));
        self->weatherEffectsCap = newCap;
    }
    self->weatherEffects[self->numWeatherEffects++] = var1;
    return 1;
}

void World_func_27158_h(World *self, float var1) {
    self->prevRainingStrength = var1;
    self->rainingStrength = var1;
}

int World_func_27160_B(World *self) { return (double)World_func_27166_f(self, 1.0f) > 0.9; }

void World_func_9425_a(World *self, struct Entity *entity, int8_t eventId) {
    (void)self;
    (void)entity;
    (void)eventId;
}

int World_isDaytime(World *self) { return self->skylightSubtracted < 4; }

int World_canBlockSeeTheSky(World *self, int x, int y, int z) {
    Chunk *chunk =
        (Chunk *)self->chunkProvider->vtable->provideChunk(self->chunkProvider, x >> 4, z >> 4);
    if (chunk == NULL)
        return 0;
    return Chunk_canBlockSeeTheSky(chunk, x & 15, y, z & 15);
}

void World_newExplosion(World *self, struct Entity *entity, double x, double y, double z,
                        float strength, int flag) {
    Explosion *var10 = Explosion_create(self, entity, x, y, z, strength);
    var10->isFlaming = flag;
    Explosion_doExplosionA(var10);
    Explosion_doExplosionB(var10, 1);
}

void World_markBlocksDirty(World *self, int x1, int y1, int z1, int x2, int y2, int z2) {
    for (int var7 = 0; var7 < self->numWorldAccesses; var7++) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var7];
        wa->vtable->markBlockRangeNeedsUpdate(wa, x1, y1, z1, x2, y2, z2);
    }
}

void World_markBlockAsNeedsUpdate(World *self, int x, int y, int z) {
    for (int var4 = 0; var4 < self->numWorldAccesses; var4++) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var4];
        wa->vtable->markBlockRangeNeedsUpdate(wa, x, y, z, x, y, z);
    }
}

void World_func_28106_e(World *self, int eventID, int x, int y, int z, int data) {
    World_func_28107_a(self, NULL, eventID, x, y, z, data);
}

void World_playRecord(World *self, const char *var1, int var2, int var3, int var4) {
    for (int var5 = 0; var5 < self->numWorldAccesses; ++var5) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var5];
        wa->vtable->playRecord(wa, var1, var2, var3, var4);
    }
}

int World_func_6466_a(World *self, struct EntityPlayer *var1, int var2, int var3, int var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    return 1;
}

void World_func_28107_a(World *self, struct EntityPlayer *player, int eventID, int x, int y, int z,
                        int data) {
    for (int var7 = 0; var7 < self->numWorldAccesses; var7++) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var7];
        wa->vtable->func_28136_a(wa, (EntityPlayer *)player, eventID, x, y, z, data);
    }
}

int World_canBlockBeRainedOn(World *self, int x, int y, int z) {
    if (!World_func_27161_C(self))
        return 0;
    if (!World_canBlockSeeTheSky(self, x, y, z))
        return 0;
    if (World_findTopSolidBlock(self, x, z) > y)
        return 0;
    BiomeGenBase *var4 = WorldChunkManager_getBiomeGenAt(self->worldChunkManager, x, z);
    if (BiomeGenBase_getEnableSnow(var4))
        return 0;
    return BiomeGenBase_canSpawnLightningBolt(var4);
}

int World_setBlockMetadata(World *self, int x, int y, int z, int meta) {
    if (x < -32000000 || z < -32000000 || x >= 32000000 || z > 32000000)
        return 0;
    if (y < 0 || y >= 128)
        return 0;
    Chunk *var5 =
        (Chunk *)self->chunkProvider->vtable->provideChunk(self->chunkProvider, x >> 4, z >> 4);
    if (var5 == NULL)
        return 0;
    Chunk_setBlockMetadata(var5, x & 15, y, z & 15, meta);
    return 1;
}

int World_setBlockAndMetadataWithNotify(World *self, int x, int y, int z, int id, int meta) {
    if (World_setBlockAndMetadata(self, x, y, z, id, meta)) {
        World_notifyBlockChange(self, x, y, z, id);
        return 1;
    }
    return 0;
}

float World_func_27162_g(World *self, float var1);

int World_func_27161_C(World *self) { return (double)World_func_27162_g(self, 1.0f) > 0.2; }

void World_setWorldTime(World *self, int64_t var1) {
    WorldInfo_setWorldTime(self->worldInfo, var1);
}

void World_setSpawnPoint(World *self, ChunkCoordinates *var1) {
    WorldInfo_setSpawn(self->worldInfo, var1->x, var1->y, var1->z);
}

ChunkCoordinates World_getSpawnPoint(World *self) {
    return ChunkCoordinates_create(WorldInfo_getSpawnX(self->worldInfo),
                                   WorldInfo_getSpawnY(self->worldInfo),
                                   WorldInfo_getSpawnZ(self->worldInfo));
}

float World_func_27162_g(World *self, float var1) {
    return self->prevRainingStrength + (self->rainingStrength - self->prevRainingStrength) * var1;
}

float World_getStarBrightness(World *self, float var1) {
    float var2 = World_getCelestialAngle(self, var1);
    float var3 = 1.0f - (MathHelper_cos(var2 * (float)M_PI * 2.0f) * 2.0f + 12.0f / 16.0f);
    if (var3 < 0.0f)
        var3 = 0.0f;
    if (var3 > 1.0f)
        var3 = 1.0f;
    return var3 * var3 * 0.5f;
}

static float World_func_27166_f(World *self, float var1) {
    return (self->prevThunderingStrength +
            (self->thunderingStrength - self->prevThunderingStrength) * var1) *
           World_func_27162_g(self, var1);
}

float World_getCelestialAngle(World *self, float var1) {
    return WorldProvider_calculateCelestialAngle(self->worldProvider,
                                                 WorldInfo_getWorldTime(self->worldInfo), var1);
}

int World_calculateSkylightSubtracted(World *self, float var1) {
    float var2 = World_getCelestialAngle(self, var1);
    float var3 = 1.0F - (MathHelper_cos(var2 * (float)M_PI * 2.0F) * 2.0F + 0.5F);
    if (var3 < 0.0F) {
        var3 = 0.0F;
    }
    if (var3 > 1.0F) {
        var3 = 1.0F;
    }
    var3 = 1.0F - var3;
    var3 = (float)((double)var3 * (1.0 - (double)(World_func_27162_g(self, var1) * 5.0F) / 16.0));
    var3 = (float)((double)var3 * (1.0 - (double)(World_func_27166_f(self, var1) * 5.0F) / 16.0));
    var3 = 1.0F - var3;
    return (int)(var3 * 11.0F);
}

void World_markBlockNeedsUpdate(World *self, int x, int y, int z) {
    for (int var4 = 0; var4 < self->numWorldAccesses; var4++) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var4];
        wa->vtable->markBlockAndNeighborsNeedsUpdate(wa, x, y, z);
    }
}

void World_notifyBlockChange(World *self, int x, int y, int z, int id) {
    World_markBlockNeedsUpdate(self, x, y, z);
    World_notifyBlocksOfNeighborChange(self, x, y, z, id);
}

void World_obtainEntitySkin(World *self, struct Entity *var1) {
    for (int var2 = 0; var2 < self->numWorldAccesses; var2++) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var2];
        wa->vtable->obtainEntitySkin(wa, var1);
    }
}

void World_releaseEntitySkin(World *self, struct Entity *var1) {
    for (int var2 = 0; var2 < self->numWorldAccesses; var2++) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var2];
        wa->vtable->releaseEntitySkin(wa, var1);
    }
}

void World_updateAllPlayersSleepingFlag(World *self) {
    self->allPlayersSleeping = (self->numPlayerEntities > 0);
    for (int i = 0; i < self->numPlayerEntities; i++) {
        if (!EntityPlayer_isPlayerSleeping(self->playerEntities[i])) {
            self->allPlayersSleeping = 0;
            break;
        }
    }
}

void World_setEntityDead(World *self, struct Entity *var1) {
    if (var1->riddenByEntity != NULL) {
        Entity_mountEntity(var1->riddenByEntity, NULL);
    }
    if (var1->ridingEntity != NULL) {
        Entity_mountEntity(var1, NULL);
    }
    Entity_setEntityDead(var1);
    if (var1->isPlayer) {
        for (int i = 0; i < self->numPlayerEntities; i++) {
            if ((Entity *)self->playerEntities[i] == var1) {
                self->playerEntities[i] = self->playerEntities[--self->numPlayerEntities];
                World_updateAllPlayersSleepingFlag(self);
                break;
            }
        }
    }
}

#include "MetadataChunkBlock.h"
#include <stdio.h>

int World_canExistingBlockSeeTheSky(World *self, int var1, int var2, int var3) {
    if (var1 < -32000000 || var3 < -32000000 || var1 >= 32000000 || var3 > 32000000)
        return 0;
    if (var2 < 0)
        return 0;
    if (var2 >= 128)
        return 1;
    if (!World_chunkExists(self, var1 >> 4, var3 >> 4))
        return 0;
    Chunk *var4 = self->vtable->getChunkFromChunkCoords(self, var1 >> 4, var3 >> 4);
    var1 &= 15;
    var3 &= 15;
    return Chunk_canBlockSeeTheSky(var4, var1, var2, var3);
}

int World_getSavedLightValue(World *self, EnumSkyBlock var1, int var2, int var3, int var4) {
    if (var3 < 0)
        var3 = 0;
    if (var3 >= 128)
        var3 = 127;
    if (var3 < 0 || var3 >= 128 || var2 < -32000000 || var4 < -32000000 || var2 >= 32000000 ||
        var4 > 32000000) {
        return EnumSkyBlock_field_1722_c[var1];
    }
    int var5 = var2 >> 4;
    int var6 = var4 >> 4;
    if (!World_chunkExists(self, var5, var6))
        return 0;
    Chunk *var7 = self->vtable->getChunkFromChunkCoords(self, var5, var6);
    return Chunk_getSavedLightValue(var7, var1, var2 & 15, var3, var4 & 15);
}

void World_setLightValue(World *self, EnumSkyBlock var1, int var2, int var3, int var4, int var5) {
    if (var2 < -32000000 || var4 < -32000000 || var2 >= 32000000 || var4 > 32000000)
        return;
    if (var3 < 0 || var3 >= 128)
        return;
    if (!World_chunkExists(self, var2 >> 4, var4 >> 4))
        return;
    Chunk *var6 = self->vtable->getChunkFromChunkCoords(self, var2 >> 4, var4 >> 4);
    Chunk_setLightValue(var6, var1, var2 & 15, var3, var4 & 15, var5);

    for (int var7 = 0; var7 < self->numWorldAccesses; ++var7) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var7];
        wa->vtable->markBlockAndNeighborsNeedsUpdate(wa, var2, var3, var4);
    }
}

void World_neighborLightPropagationChanged(World *self, EnumSkyBlock var1, int var2, int var3,
                                           int var4, int var5) {
    if (self->worldProvider->hasNoSky && var1 == EnumSkyBlock_Sky)
        return;
    if (!World_blockExists(self, var2, var3, var4))
        return;
    if (var1 == EnumSkyBlock_Sky) {
        if (World_canExistingBlockSeeTheSky(self, var2, var3, var4))
            var5 = 15;
    } else if (var1 == EnumSkyBlock_Block) {
        int var6 = World_getBlockId(self, var2, var3, var4);
        if (Block_lightValue[var6] > var5)
            var5 = Block_lightValue[var6];
    }
    if (World_getSavedLightValue(self, var1, var2, var3, var4) != var5) {
        World_scheduleLightingUpdate(self, var1, var2, var3, var4, var2, var3, var4);
    }
}

void World_scheduleLightingUpdate(World *self, EnumSkyBlock var1, int var2, int var3, int var4,
                                  int var5, int var6, int var7) {
    if (self->worldProvider->hasNoSky && var1 == EnumSkyBlock_Sky)
        return;
    ++World_lightingUpdatesScheduled;
    if (World_lightingUpdatesScheduled == 50) {
        --World_lightingUpdatesScheduled;
        return;
    }
    int var9 = (var5 + var2) / 2;
    int var10 = (var7 + var4) / 2;
    if (!World_blockExists(self, var9, 64, var10)) {
        --World_lightingUpdatesScheduled;
        return;
    }
    Chunk *mid = self->vtable->getChunkFromBlockCoords(self, var9, var10);
    if (Chunk_func_21167_h(mid)) {
        --World_lightingUpdatesScheduled;
        return;
    }

    int var11 = self->lightingToUpdate_count;
    int var12 = 5;
    if (var12 > var11)
        var12 = var11;
    for (int var13 = 0; var13 < var12; ++var13) {
        MetadataChunkBlock *var14 = self->lightingToUpdate[var11 - var13 - 1];
        if (var14->field_1299_a == var1 &&
            MetadataChunkBlock_func_866_a(var14, var2, var3, var4, var5, var6, var7)) {
            --World_lightingUpdatesScheduled;
            return;
        }
    }

    if (self->lightingToUpdate_count >= self->lightingToUpdate_cap) {
        int newcap = self->lightingToUpdate_cap ? self->lightingToUpdate_cap * 2 : 64;
        self->lightingToUpdate =
            realloc(self->lightingToUpdate, (size_t)newcap * sizeof(MetadataChunkBlock *));
        self->lightingToUpdate_cap = newcap;
    }
    MetadataChunkBlock *entry = calloc(1, sizeof(MetadataChunkBlock));
    MetadataChunkBlock_init(entry, var1, var2, var3, var4, var5, var6, var7);
    self->lightingToUpdate[self->lightingToUpdate_count++] = entry;
    if (self->lightingToUpdate_count > 1000000) {
        printf("More than 1000000 updates, aborting lighting updates\n");

        for (int i = 0; i < self->lightingToUpdate_count; ++i)
            free(self->lightingToUpdate[i]);
        self->lightingToUpdate_count = 0;
    }
    --World_lightingUpdatesScheduled;
}

int World_chunkExists(World *self, int cx, int cz) {
    if (!self->chunkProvider)
        return 0;
    return self->chunkProvider->vtable->chunkExists(self->chunkProvider, cx, cz);
}

int World_func_650_a(World *self, int var1) {
    if (!self->chunkProvider || !IChunkProvider_canSave(self->chunkProvider)) {
        return 1;
    }
    if (var1 == 0) {
        World_saveLevel(self);
    }
    return IChunkProvider_saveChunks(self->chunkProvider, 0, NULL);
}

void World_sendQuittingDisconnectingPacket(World *self) { (void)self; }

const char *World_func_687_d(World *self, char *buf, int bufLen) {
    snprintf(buf, (size_t)bufLen, "All: %d", self->numEntities);
    return buf;
}

const char *World_func_21119_g(World *self, char *buf, int bufLen) {
    if (self->chunkProvider) {
        const char *s = IChunkProvider_makeString(self->chunkProvider);
        if (s) {
            snprintf(buf, (size_t)bufLen, "%s", s);
            return buf;
        }
    }
    snprintf(buf, (size_t)bufLen, "N/A");
    return buf;
}

void World_emptyMethod1(World *self) { (void)self; }

void World_spawnPlayerWithLoadedChunks(World *self, struct Entity *var1) {
    if (self->worldInfo) {
        NBTTagCompound *var2 = (NBTTagCompound *)WorldInfo_getPlayerNBTTagCompound(self->worldInfo);
        if (var2 != NULL) {
            Entity_readFromNBT(var1, var2);
            WorldInfo_setPlayerNBTTagCompound(self->worldInfo, NULL);
        }
    }
    if (self->chunkProvider != NULL &&
        self->chunkProvider->vtable == &ChunkProviderLoadOrGenerate_vtable) {
        ChunkProviderLoadOrGenerate *var3 = (ChunkProviderLoadOrGenerate *)self->chunkProvider;
        int var4 = MathHelper_floor_float((float)((int)var1->posX)) >> 4;
        int var5 = MathHelper_floor_float((float)((int)var1->posZ)) >> 4;
        ChunkProviderLoadOrGenerate_setCurrentChunkOver(var3, var4, var5);
    }
    World_entityJoinedWorld(self, var1);
}

struct Entity *World_func_4085_a(World *self) {
    (void)self;
    return NULL;
}

int World_checkChunksExist(World *self, int var1, int var2, int var3, int var4, int var5,
                           int var6) {
    if (var5 >= 0 && var2 < 128) {
        var1 >>= 4;
        var2 >>= 4;
        var3 >>= 4;
        var4 >>= 4;
        var5 >>= 4;
        var6 >>= 4;
        for (int var7 = var1; var7 <= var4; var7++) {
            for (int var8 = var3; var8 <= var6; var8++) {
                if (!World_chunkExists(self, var7, var8))
                    return 0;
            }
        }
        return 1;
    } else {
        return 0;
    }
}

void World_updateEntityWithOptionalForce(World *self, struct Entity *var1, int var2) {
    int var3 = MathHelper_floor_double(var1->posX);
    int var4 = MathHelper_floor_double(var1->posZ);
    int var5 = 32;
    if (!var2 ||
        World_checkChunksExist(self, var3 - var5, 0, var4 - var5, var3 + var5, 128, var4 + var5)) {
        var1->lastTickPosX = var1->posX;
        var1->lastTickPosY = var1->posY;
        var1->lastTickPosZ = var1->posZ;
        var1->prevRotationYaw = var1->rotationYaw;
        var1->prevRotationPitch = var1->rotationPitch;
        if (var2 && var1->addedToChunk) {
            if (var1->ridingEntity != NULL) {
                Entity_updateRidden(var1);
            } else {
                var1->vtable->onUpdate(var1);
            }
        }
        if (isnan(var1->posX) || isinf(var1->posX))
            var1->posX = var1->lastTickPosX;
        if (isnan(var1->posY) || isinf(var1->posY))
            var1->posY = var1->lastTickPosY;
        if (isnan(var1->posZ) || isinf(var1->posZ))
            var1->posZ = var1->lastTickPosZ;
        if (isnan((double)var1->rotationPitch) || isinf((double)var1->rotationPitch))
            var1->rotationPitch = var1->prevRotationPitch;
        if (isnan((double)var1->rotationYaw) || isinf((double)var1->rotationYaw))
            var1->rotationYaw = var1->prevRotationYaw;
        int var6 = MathHelper_floor_double(var1->posX / 16.0);
        int var7 = MathHelper_floor_double(var1->posY / 16.0);
        int var8 = MathHelper_floor_double(var1->posZ / 16.0);
        if (!var1->addedToChunk || var1->chunkCoordX != var6 || var1->chunkCoordY != var7 ||
            var1->chunkCoordZ != var8) {
            if (var1->addedToChunk &&
                World_chunkExists(self, var1->chunkCoordX, var1->chunkCoordZ)) {
                Chunk_removeEntityAtIndex(self->vtable->getChunkFromChunkCoords(
                                              self, var1->chunkCoordX, var1->chunkCoordZ),
                                          var1, var1->chunkCoordY);
            }
            if (World_chunkExists(self, var6, var8)) {
                var1->addedToChunk = 1;
                Chunk_addEntity(self->vtable->getChunkFromChunkCoords(self, var6, var8), var1);
            } else {
                var1->addedToChunk = 0;
            }
        }
        if (var2 && var1->addedToChunk && var1->riddenByEntity != NULL) {
            if (!var1->riddenByEntity->isDead && var1->riddenByEntity->ridingEntity == var1) {
                World_updateEntity(self, var1->riddenByEntity);
            } else {
                var1->riddenByEntity->ridingEntity = NULL;
                var1->riddenByEntity = NULL;
            }
        }
    }
}

void World_updateEntity(World *self, struct Entity *var1) {
    World_updateEntityWithOptionalForce(self, var1, 1);
}

IChunkProvider *World_getIChunkProvider(World *self) { return self->chunkProvider; }

int World_updatingLighting(World *self) {
    if (self->lightingUpdatesCounter >= 50) {
        return 0;
    } else {
        ++self->lightingUpdatesCounter;
        int var2;
        {
            int var1 = 500;
            while (self->lightingToUpdate_count > 0) {
                --var1;
                if (var1 <= 0) {
                    var2 = 1;
                    --self->lightingUpdatesCounter;
                    return var2;
                }
                MetadataChunkBlock *mcb = self->lightingToUpdate[self->lightingToUpdate_count - 1];
                --self->lightingToUpdate_count;
                MetadataChunkBlock_func_4127_a(mcb, self);
                free(mcb);
            }
            var2 = 0;
        }
        --self->lightingUpdatesCounter;
        return var2;
    }
}

void World_func_656_j(World *self) {
    if (self->chunkProvider) {
        while (IChunkProvider_unload100OldestChunks(self->chunkProvider)) {
        }
    }
}

MovingObjectPosition *World_rayTraceBlocks(World *self, Vec3D *var1, Vec3D *var2) {
    return World_func_28105_a(self, var1, var2, 0, 0);
}

MovingObjectPosition *World_rayTraceBlocks_do(World *self, Vec3D *var1, Vec3D *var2, int var3) {
    return World_func_28105_a(self, var1, var2, var3, 0);
}

MovingObjectPosition *World_func_28105_a(World *self, Vec3D *var1, Vec3D *var2, int var3,
                                         int var4) {
    if (!isnan(var1->xCoord) && !isnan(var1->yCoord) && !isnan(var1->zCoord)) {
        if (!isnan(var2->xCoord) && !isnan(var2->yCoord) && !isnan(var2->zCoord)) {
            int var5 = MathHelper_floor_double(var2->xCoord);
            int var6 = MathHelper_floor_double(var2->yCoord);
            int var7 = MathHelper_floor_double(var2->zCoord);
            int var8 = MathHelper_floor_double(var1->xCoord);
            int var9 = MathHelper_floor_double(var1->yCoord);
            int var10 = MathHelper_floor_double(var1->zCoord);
            int var11 = World_getBlockId(self, var8, var9, var10);
            int var12 = World_getBlockMetadata(self, var8, var9, var10);
            Block *var13 = Block_blocksList[var11];
            if ((!var4 || var13 == NULL ||
                 var13->vtable->getCollisionBoundingBoxFromPool(var13, self, var8, var9, var10) !=
                     NULL) &&
                var11 > 0 && var13->vtable->canCollideCheck(var13, var12, var3)) {
                MovingObjectPosition *var14 =
                    var13->vtable->collisionRayTrace(var13, self, var8, var9, var10, var1, var2);
                if (var14 != NULL) {
                    return var14;
                }
            }

            var11 = 200;

            while (var11-- >= 0) {
                if (isnan(var1->xCoord) || isnan(var1->yCoord) || isnan(var1->zCoord)) {
                    return NULL;
                }

                if (var8 == var5 && var9 == var6 && var10 == var7) {
                    return NULL;
                }

                int var39 = 1;
                int var40 = 1;
                int var41 = 1;
                double var15 = 999.0;
                double var17 = 999.0;
                double var19 = 999.0;
                if (var5 > var8) {
                    var15 = (double)var8 + 1.0;
                } else if (var5 < var8) {
                    var15 = (double)var8 + 0.0;
                } else {
                    var39 = 0;
                }

                if (var6 > var9) {
                    var17 = (double)var9 + 1.0;
                } else if (var6 < var9) {
                    var17 = (double)var9 + 0.0;
                } else {
                    var40 = 0;
                }

                if (var7 > var10) {
                    var19 = (double)var10 + 1.0;
                } else if (var7 < var10) {
                    var19 = (double)var10 + 0.0;
                } else {
                    var41 = 0;
                }

                double var21 = 999.0;
                double var23 = 999.0;
                double var25 = 999.0;
                double var27 = var2->xCoord - var1->xCoord;
                double var29 = var2->yCoord - var1->yCoord;
                double var31 = var2->zCoord - var1->zCoord;
                if (var39) {
                    var21 = (var15 - var1->xCoord) / var27;
                }

                if (var40) {
                    var23 = (var17 - var1->yCoord) / var29;
                }

                if (var41) {
                    var25 = (var19 - var1->zCoord) / var31;
                }

                signed char var42;
                if (var21 < var23 && var21 < var25) {
                    if (var5 > var8) {
                        var42 = 4;
                    } else {
                        var42 = 5;
                    }

                    var1->xCoord = var15;
                    var1->yCoord += var29 * var21;
                    var1->zCoord += var31 * var21;
                } else if (var23 < var25) {
                    if (var6 > var9) {
                        var42 = 0;
                    } else {
                        var42 = 1;
                    }

                    var1->xCoord += var27 * var23;
                    var1->yCoord = var17;
                    var1->zCoord += var31 * var23;
                } else {
                    if (var7 > var10) {
                        var42 = 2;
                    } else {
                        var42 = 3;
                    }

                    var1->xCoord += var27 * var25;
                    var1->yCoord += var29 * var25;
                    var1->zCoord = var19;
                }

                Vec3D *var34 = Vec3D_createVector(var1->xCoord, var1->yCoord, var1->zCoord);
                var8 = (int)(var34->xCoord = (double)MathHelper_floor_double(var1->xCoord));
                if (var42 == 5) {
                    --var8;
                    ++var34->xCoord;
                }

                var9 = (int)(var34->yCoord = (double)MathHelper_floor_double(var1->yCoord));
                if (var42 == 1) {
                    --var9;
                    ++var34->yCoord;
                }

                var10 = (int)(var34->zCoord = (double)MathHelper_floor_double(var1->zCoord));
                if (var42 == 3) {
                    --var10;
                    ++var34->zCoord;
                }

                int var35 = World_getBlockId(self, var8, var9, var10);
                int var36 = World_getBlockMetadata(self, var8, var9, var10);
                Block *var37 = Block_blocksList[var35];
                if ((!var4 || var37 == NULL ||
                     var37->vtable->getCollisionBoundingBoxFromPool(var37, self, var8, var9,
                                                                    var10) != NULL) &&
                    var35 > 0 && var37->vtable->canCollideCheck(var37, var36, var3)) {
                    MovingObjectPosition *var38 = var37->vtable->collisionRayTrace(
                        var37, self, var8, var9, var10, var1, var2);
                    if (var38 != NULL) {
                        return var38;
                    }
                }
            }

            return NULL;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

void World_setItemData(World *self, const char *var1, MapDataBase *var2) {
    if (self->field_28108_z) {
        MapStorage_setData(self->field_28108_z, var1, var2);
    }
}

MapDataBase *World_loadItemData(World *self, MapDataBase *(*factory)(const char *),
                                const char *var2) {
    if (self->field_28108_z) {
        return MapStorage_loadData(self->field_28108_z, factory, var2);
    }
    return NULL;
}

int World_getUniqueDataId(World *self, const char *var1) {
    if (self->field_28108_z) {
        return MapStorage_getUniqueDataId(self->field_28108_z, var1);
    }
    return 0;
}

float World_getLightBrightness(World *self, int var1, int var2, int var3) {
    int idx = World_getBlockLightValue(self, var1, var2, var3);
    if (idx < 0)
        idx = 0;
    if (idx > 15)
        idx = 15;
    return self->lightBrightnessTable[idx];
}

int World_isBoundingBoxBurning(World *self, AxisAlignedBB *var1) {
    int var2 = MathHelper_floor_double(var1->minX);
    int var3 = MathHelper_floor_double(var1->maxX + 1.0);
    int var4 = MathHelper_floor_double(var1->minY);
    int var5 = MathHelper_floor_double(var1->maxY + 1.0);
    int var6 = MathHelper_floor_double(var1->minZ);
    int var7 = MathHelper_floor_double(var1->maxZ + 1.0);
    if (World_checkChunksExist(self, var2, var4, var6, var3, var5, var7)) {
        for (int var8 = var2; var8 < var3; ++var8) {
            for (int var9 = var4; var9 < var5; ++var9) {
                for (int var10 = var6; var10 < var7; ++var10) {
                    int var11 = World_getBlockId(self, var8, var9, var10);
                    if (var11 == Block_fire->blockID || var11 == Block_lavaMoving->blockID ||
                        var11 == Block_lavaStill->blockID) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int World_isMaterialInBB(World *self, AxisAlignedBB *var1, Material *var2) {
    int var3 = MathHelper_floor_double(var1->minX);
    int var4 = MathHelper_floor_double(var1->maxX + 1.0);
    int var5 = MathHelper_floor_double(var1->minY);
    int var6 = MathHelper_floor_double(var1->maxY + 1.0);
    int var7 = MathHelper_floor_double(var1->minZ);
    int var8 = MathHelper_floor_double(var1->maxZ + 1.0);
    for (int var9 = var3; var9 < var4; ++var9) {
        for (int var10 = var5; var10 < var6; ++var10) {
            for (int var11 = var7; var11 < var8; ++var11) {
                int id = World_getBlockId(self, var9, var10, var11);
                Block *var12 = Block_blocksList[id];
                if (var12 != NULL && var12->blockMaterial == var2) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void World_markBlocksDirtyVertical(World *self, int var1, int var2, int var3, int var4) {
    if (var3 > var4) {
        int var5 = var4;
        var4 = var3;
        var3 = var5;
    }
    World_markBlocksDirty(self, var1, var3, var2, var1, var4, var2);
}

void World_func_31054_a(World *self, TileEntity **var1, int count) {
    TileEntity ***dest_list;
    int *dest_count;
    int *dest_cap;
    if (self->field_31055_L) {
        dest_list = &self->field_30900_E;
        dest_count = &self->field_30900_E_count;
        dest_cap = &self->field_30900_E_cap;
    } else {
        dest_list = &self->loadedTileEntityList;
        dest_count = &self->numLoadedTileEntities;
        dest_cap = &self->loadedTileEntityCap;
    }
    for (int i = 0; i < count; i++) {
        if (*dest_count >= *dest_cap) {
            int newcap = *dest_cap == 0 ? 16 : *dest_cap * 2;
            *dest_list = (TileEntity **)realloc(*dest_list, (size_t)newcap * sizeof(TileEntity *));
            *dest_cap = newcap;
        }
        (*dest_list)[(*dest_count)++] = var1[i];
    }
}

void World_func_636_a(World *self, struct Entity **var1, int count) {
    for (int var2 = 0; var2 < count; var2++) {
        if (self->numEntities >= self->entityCap) {
            int newcap = self->entityCap == 0 ? 16 : self->entityCap * 2;
            self->loadedEntityList =
                (Entity **)realloc(self->loadedEntityList, (size_t)newcap * sizeof(Entity *));
            self->entityCap = newcap;
        }
        self->loadedEntityList[self->numEntities++] = var1[var2];
        self->vtable->obtainEntitySkin(self, var1[var2]);
    }
}

void World_func_632_b(World *self, struct Entity **var1, int count) {
    for (int i = 0; i < count; i++) {
        if (self->numUnloadedEntities >= self->unloadedEntityCap) {
            int newcap = self->unloadedEntityCap == 0 ? 16 : self->unloadedEntityCap * 2;
            self->unloadedEntityList =
                (Entity **)realloc(self->unloadedEntityList, (size_t)newcap * sizeof(Entity *));
            self->unloadedEntityCap = newcap;
        }
        self->unloadedEntityList[self->numUnloadedEntities++] = var1[i];
    }
}

float World_func_675_a(World *self, Vec3D *var1, AxisAlignedBB *var2) {
    double var3 = 1.0 / ((var2->maxX - var2->minX) * 2.0 + 1.0);
    double var5 = 1.0 / ((var2->maxY - var2->minY) * 2.0 + 1.0);
    double var7 = 1.0 / ((var2->maxZ - var2->minZ) * 2.0 + 1.0);
    int var9 = 0;
    int var10 = 0;
    for (float var11 = 0.0f; var11 <= 1.0f; var11 = (float)((double)var11 + var3)) {
        for (float var12 = 0.0f; var12 <= 1.0f; var12 = (float)((double)var12 + var5)) {
            for (float var13 = 0.0f; var13 <= 1.0f; var13 = (float)((double)var13 + var7)) {
                double var14 = var2->minX + (var2->maxX - var2->minX) * (double)var11;
                double var16 = var2->minY + (var2->maxY - var2->minY) * (double)var12;
                double var18 = var2->minZ + (var2->maxZ - var2->minZ) * (double)var13;
                Vec3D *v = Vec3D_createVector(var14, var16, var18);
                MovingObjectPosition *rtb = World_rayTraceBlocks(self, v, var1);
                if (rtb == NULL)
                    ++var9;
                MovingObjectPosition_free(rtb);
                ++var10;
            }
        }
    }
    return (float)var9 / (float)var10;
}

void World_playNoteAt(World *self, int x, int y, int z, int p1, int p2) {
    int var6 = World_getBlockId(self, x, y, z);
    if (var6 > 0) {
        Block *b = Block_blocksList[var6];
        if (b)
            b->vtable->playBlock(b, self, x, y, z, p1, p2);
    }
}

void World_func_698_b(World *self, int var1, int var2, int var3, struct TileEntity *var4) {
    if (World_blockExists(self, var1, var2, var3)) {
        Chunk *c = self->vtable->getChunkFromBlockCoords(self, var1, var3);
        if (c)
            Chunk_setChunkModified(c);
    }
    for (int var5 = 0; var5 < self->numWorldAccesses; ++var5) {
        IWorldAccess *wa = (IWorldAccess *)self->worldAccesses[var5];
        if (wa && wa->vtable->doNothingWithTileEntity)
            wa->vtable->doNothingWithTileEntity(wa, var1, var2, var3, var4);
    }
}

void World_setChunkData(World *self, int var1, int var2, int var3, int var4, int var5, int var6,
                        uint8_t *var7) {
    int var8 = var1 >> 4;
    int var9 = var3 >> 4;
    int var10 = (var1 + var4 - 1) >> 4;
    int var11 = (var3 + var6 - 1) >> 4;
    int var12 = 0;
    int var13 = var2;
    int var14 = var2 + var5;
    if (var13 < 0)
        var13 = 0;
    if (var14 > 128)
        var14 = 128;
    for (int var15 = var8; var15 <= var10; ++var15) {
        int var16 = var1 - var15 * 16;
        int var17 = var1 + var4 - var15 * 16;
        if (var16 < 0)
            var16 = 0;
        if (var17 > 16)
            var17 = 16;
        for (int var18 = var9; var18 <= var11; ++var18) {
            int var19 = var3 - var18 * 16;
            int var20 = var3 + var6 - var18 * 16;
            if (var19 < 0)
                var19 = 0;
            if (var20 > 16)
                var20 = 16;
            Chunk *c = self->vtable->getChunkFromChunkCoords(self, var15, var18);
            if (c)
                var12 =
                    Chunk_setChunkData(c, var7, var16, var13, var19, var17, var14, var20, var12);
            World_markBlocksDirty(self, var15 * 16 + var16, var13, var18 * 16 + var19,
                                  var15 * 16 + var17, var14, var18 * 16 + var20);
        }
    }
}

int World_countEntities(World *self, EntityClassId classId) {
    int var2 = 0;
    for (int var3 = 0; var3 < self->numEntities; var3++) {
        Entity *var4 = self->loadedEntityList[var3];
        if (!var4)
            continue;
        if (classId == ENTITY_CLASS_IMOB && var4->isMob)
            var2++;
        else if (classId == ENTITY_CLASS_ENTITY_ANIMAL && var4->isAnimal)
            var2++;
        else if (classId == ENTITY_CLASS_ENTITY_WATER_MOB && var4->isWaterMob)
            var2++;
    }
    return var2;
}

int World_isBlockProvidingPowerTo(World *self, int x, int y, int z, int side) {
    int var5 = World_getBlockId(self, x, y, z);
    if (var5 == 0)
        return 0;
    Block *b = Block_blocksList[var5];
    if (!b)
        return 0;
    return b->vtable->isIndirectlyPoweringTo(b, self, x, y, z, side);
}

int World_isBlockGettingPowered(World *self, int x, int y, int z) {
    return World_isBlockProvidingPowerTo(self, x, y - 1, z, 0)
               ? 1
               : (World_isBlockProvidingPowerTo(self, x, y + 1, z, 1)
                      ? 1
                      : (World_isBlockProvidingPowerTo(self, x, y, z - 1, 2)
                             ? 1
                             : (World_isBlockProvidingPowerTo(self, x, y, z + 1, 3)
                                    ? 1
                                    : (World_isBlockProvidingPowerTo(self, x - 1, y, z, 4)
                                           ? 1
                                           : World_isBlockProvidingPowerTo(self, x + 1, y, z,
                                                                           5)))));
}

int World_isBlockIndirectlyProvidingPowerTo(World *self, int x, int y, int z, int side) {
    if (World_isBlockNormalCube(self, x, y, z)) {
        return World_isBlockGettingPowered(self, x, y, z);
    } else {
        int var5 = World_getBlockId(self, x, y, z);
        if (var5 == 0)
            return 0;
        Block *b = Block_blocksList[var5];
        if (!b)
            return 0;
        return b->vtable->isPoweringTo(b, (IBlockAccess *)self, x, y, z, side);
    }
}

void World_setSpawnLocation(World *self) {
    if (!self->worldInfo)
        return;
    if (WorldInfo_getSpawnY(self->worldInfo) <= 0) {
        WorldInfo_setSpawnY(self->worldInfo, 64);
    }
    int var1 = WorldInfo_getSpawnX(self->worldInfo);
    int var2;
    for (var2 = WorldInfo_getSpawnZ(self->worldInfo);
         World_getFirstUncoveredBlock(self, var1, var2) == 0;
         var2 += JavaRandom_nextInt(&self->rand, 8) - JavaRandom_nextInt(&self->rand, 8)) {
        var1 += JavaRandom_nextInt(&self->rand, 8) - JavaRandom_nextInt(&self->rand, 8);
    }
    WorldInfo_setSpawnX(self->worldInfo, var1);
    WorldInfo_setSpawnZ(self->worldInfo, var2);
}

void World_joinEntityInSurroundings(World *self, struct Entity *var1) {
    int var2 = MathHelper_floor_double(var1->posX / 16.0);
    int var3 = MathHelper_floor_double(var1->posZ / 16.0);
    int8_t var4 = 2;
    for (int var5 = var2 - var4; var5 <= var2 + var4; ++var5) {
        for (int var6 = var3 - var4; var6 <= var3 + var4; ++var6) {
            self->vtable->getChunkFromChunkCoords(self, var5, var6);
        }
    }

    int found = 0;
    for (int i = 0; i < self->numEntities; i++) {
        if (self->loadedEntityList[i] == var1) {
            found = 1;
            break;
        }
    }
    if (!found) {
        if (self->numEntities >= self->entityCap) {
            int newcap = self->entityCap == 0 ? 16 : self->entityCap * 2;
            self->loadedEntityList =
                (Entity **)realloc(self->loadedEntityList, (size_t)newcap * sizeof(Entity *));
            self->entityCap = newcap;
        }
        self->loadedEntityList[self->numEntities++] = var1;
    }
}

void World_addWorldAccess(World *self, struct IWorldAccess *var1) {
    if (self->numWorldAccesses >= self->worldAccessesCap) {
        int newcap = self->worldAccessesCap == 0 ? 4 : self->worldAccessesCap * 2;
        self->worldAccesses =
            (void **)realloc(self->worldAccesses, (size_t)newcap * sizeof(void *));
        self->worldAccessesCap = newcap;
    }
    self->worldAccesses[self->numWorldAccesses++] = (void *)var1;
}

void World_removeWorldAccess(World *self, struct IWorldAccess *var1) {
    for (int i = 0; i < self->numWorldAccesses; i++) {
        if (self->worldAccesses[i] == (void *)var1) {
            self->worldAccesses[i] = self->worldAccesses[--self->numWorldAccesses];
            return;
        }
    }
}

void World_updateEntityList(World *self) {
    int var1;
    Entity *var2;
    int var3, var4;

    for (var1 = 0; var1 < self->numUnloadedEntities; var1++) {
        var2 = self->unloadedEntityList[var1];

        for (int j = 0; j < self->numEntities; j++) {
            if (self->loadedEntityList[j] == var2) {
                self->loadedEntityList[j] = self->loadedEntityList[--self->numEntities];
                break;
            }
        }
    }

    for (var1 = 0; var1 < self->numUnloadedEntities; var1++) {
        var2 = self->unloadedEntityList[var1];
        var3 = var2->chunkCoordX;
        var4 = var2->chunkCoordZ;
        if (var2->addedToChunk && World_chunkExists(self, var3, var4)) {

            Chunk *c = self->vtable->getChunkFromChunkCoords(self, var3, var4);
            if (c)
                Chunk_removeEntity(c, var2);
        }
    }

    for (var1 = 0; var1 < self->numUnloadedEntities; var1++) {
        self->vtable->releaseEntitySkin(self, self->unloadedEntityList[var1]);
    }

    self->numUnloadedEntities = 0;

    for (var1 = 0; var1 < self->numEntities; var1++) {
        var2 = self->loadedEntityList[var1];
        if (var2->ridingEntity != NULL) {
            if (!var2->ridingEntity->isDead && var2->ridingEntity->riddenByEntity == var2) {
                continue;
            }
            var2->ridingEntity->riddenByEntity = NULL;
            var2->ridingEntity = NULL;
        }
        if (var2->isDead) {
            var3 = var2->chunkCoordX;
            var4 = var2->chunkCoordZ;
            if (var2->addedToChunk && World_chunkExists(self, var3, var4)) {
                Chunk *c = self->vtable->getChunkFromChunkCoords(self, var3, var4);
                if (c)
                    Chunk_removeEntity(c, var2);
            }

            self->loadedEntityList[var1] = self->loadedEntityList[--self->numEntities];
            var1--;
            self->vtable->releaseEntitySkin(self, var2);
        }
    }
}
