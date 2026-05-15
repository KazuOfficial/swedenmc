#ifndef WORLD_H
#define WORLD_H

#include "IBlockAccess.h"
#include "MapStorage.h"
#include "MapDataBase.h"
#include "Material.h"
#include "AxisAlignedBB.h"
#include "JavaRandom.h"
#include "GameForward.h"
#include "Chunk.h"
#include "TileEntity.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "NextTickListEntry.h"
#include "ISaveHandler.h"
#include "WorldInfo.h"
#include "ChunkCoordinates.h"
#include "IChunkProvider.h"
#include "IProgressUpdate.h"
#include "EnumCreatureType.h"
#include "ChunkCoordIntPair.h"
#include <stdint.h>

struct IWorldAccess;

#define WORLD_CHUNK_TABLE_SIZE 1024

struct World;
typedef struct World World;

struct MetadataChunkBlock;

typedef struct WorldVtable {

    int (*getBlockId)(World *self, int x, int y, int z);
    int (*getBlockMetadata)(World *self, int x, int y, int z);
    TileEntity *(*getBlockTileEntity)(World *self, int x, int y, int z);
    float (*getLightBrightness)(World *self, int x, int y, int z);
    int (*isBlockOpaqueCube)(World *self, int x, int y, int z);
    int (*isBlockNormalCube)(World *self, int x, int y, int z);

    int (*setBlock)(World *self, int x, int y, int z, int id);
    int (*setBlockWithNotify)(World *self, int x, int y, int z, int id);
    int (*setBlockAndMetadata)(World *self, int x, int y, int z, int id, int meta);
    int (*setBlockMetadataWithNotify)(World *self, int x, int y, int z, int meta);
    int (*setBlockToAir)(World *self, int x, int y, int z);
    void (*setBlockTileEntity)(World *self, int x, int y, int z, TileEntity *te);
    void (*removeBlockTileEntity)(World *self, int x, int y, int z);
    int (*isAirBlock)(World *self, int x, int y, int z);
    int (*blockExists)(World *self, int x, int y, int z);
    int (*doesBlockHaveSolidTopSurface)(World *self, int x, int y, int z);
    void (*scheduleBlockUpdate)(World *self, int x, int y, int z, int blockId, int delay);
    void (*notifyBlocksOfNeighborChange)(World *self, int x, int y, int z, int blockId);
    void (*notifyBlockOfNeighborChange)(World *self, int x, int y, int z, int neighborId);
    int (*getHeightValue)(World *self, int x, int z);
    int (*getBlockLightValue)(World *self, int x, int y, int z);
    int (*getFullBlockLightValue)(World *self, int x, int y, int z);
    int (*isBlockIndirectlyGettingPowered)(World *self, int x, int y, int z);
    int (*getIndirectPowerLevelTo)(World *self, int x, int y, int z, int side);
    int (*spawnEntityInWorld)(World *self, Entity *entity);
    void (*removeEntity)(World *self, Entity *entity);
    void (*playSoundEffect)(World *self, double x, double y, double z, const char *sound, float vol,
                            float pitch);
    void (*playAuxSFX)(World *self, int effectId, int x, int y, int z, int data);
    Chunk *(*getChunkFromBlockCoords)(World *self, int x, int z);
    Chunk *(*getChunkFromChunkCoords)(World *self, int cx, int cz);
    int (*isRemote)(World *self);

    void (*obtainEntitySkin)(World *self, struct Entity *var1);

    void (*releaseEntitySkin)(World *self, struct Entity *var1);

    void (*updateWeather)(World *self);

    IChunkProvider *(*getChunkProvider)(World *self);

    void (*tick)(World *self);

    void (*setSpawnLocation)(World *self);
} WorldVtable;

struct World {

    IBlockAccess iBlockAccess;

    const WorldVtable *vtable;

    JavaRandom rand;
    int64_t seed;
    int worldHeight;
    int isRemote;

    Chunk *chunkTable[WORLD_CHUNK_TABLE_SIZE];

    Entity **loadedEntityList;
    int numEntities;
    int entityCap;

    Entity **unloadedEntityList;
    int numUnloadedEntities;
    int unloadedEntityCap;

    TileEntity **loadedTileEntityList;
    int numLoadedTileEntities;
    int loadedTileEntityCap;

    TileEntity **field_30900_E;
    int field_30900_E_count;
    int field_30900_E_cap;

    int field_31055_L;

    NextTickListEntry *pendingTickList;
    int numPendingTicks;
    int pendingTickCap;

    int64_t worldTime;
    int64_t totalWorldTime;

    int skylightSubtracted;

    float lightBrightnessTable[16];

    WorldProvider *worldProvider;

    WorldChunkManager *worldChunkManager;

    int multiplayerWorld;

    int isNewWorld;

    int scheduledUpdatesAreImmediate;

    float rainingStrength;
    float prevRainingStrength;
    float thunderingStrength;
    float prevThunderingStrength;
    int isNether;

    int editingBlocks;

    int findingSpawnPoint;

    ISaveHandler *saveHandler;
    int ownsSaveHandler;

    WorldInfo *worldInfo;

    int difficultySetting;

    struct EntityPlayer **playerEntities;
    int numPlayerEntities;
    int playerEntitiesCap;

    Entity *deadPlayer;

    int field_27172_i;

    int allPlayersSleeping;

    int field_27168_F;

    void **worldAccesses;
    int numWorldAccesses;
    int worldAccessesCap;

    struct MapStorage *field_28108_z;

    struct MetadataChunkBlock **lightingToUpdate;
    int lightingToUpdate_count;
    int lightingToUpdate_cap;

    IChunkProvider *chunkProvider;

    int lightingUpdatesCounter;

    int field_9437_g;

    int field_9436_h;

    long long lockTimestamp;

    int autosavePeriod;

    int spawnHostileMobs;

    int spawnPeacefulMobs;

    int soundCounter;

    long long field_1019_F;

    struct Pathfinder *pathFinder;

    struct Entity **weatherEffects;
    int numWeatherEffects;
    int weatherEffectsCap;

    ChunkCoordIntPair *positionsToUpdate;
    int numPositionsToUpdate;
    int positionsToUpdateCap;
};

void World_init(World *self, int64_t seed, int isRemote);

void World_init_with_save_and_provider(World *self, ISaveHandler *var1, const char *var2,
                                       WorldProvider *var3, int64_t var4);

void World_init_from_save_provider(World *self, ISaveHandler *var1, const char *var2, int64_t var3,
                                   WorldProvider *var5);

World *World_create_from_save(ISaveHandler *var1, const char *var2, int64_t var3);
void World_destroy(World *self);

World *World_createFromWorld(World *var1, WorldProvider *var2);
World *World_create(int64_t seed, int isRemote);

IChunkProvider *World_getChunkProvider(World *self);

void World_getInitialSpawnLocation(World *self);

void World_calculateInitialSkylight(World *self);

void World_func_27163_E(World *self);

extern WorldVtable World_defaultVtable;

Chunk *World_getOrCreateChunk(World *self, int cx, int cz);

int World_getBlockId(World *self, int x, int y, int z);
int World_getBlockMetadata(World *self, int x, int y, int z);
int World_setBlock(World *self, int x, int y, int z, int id);
int World_setBlockWithNotify(World *self, int x, int y, int z, int id);
int World_setBlockAndMetadata(World *self, int x, int y, int z, int id, int meta);
int World_setBlockMetadataWithNotify(World *self, int x, int y, int z, int meta);
int World_setBlockToAir(World *self, int x, int y, int z);
TileEntity *World_getBlockTileEntity(World *self, int x, int y, int z);
void World_setBlockTileEntity(World *self, int x, int y, int z, TileEntity *te);
void World_removeBlockTileEntity(World *self, int x, int y, int z);
int World_isAirBlock(World *self, int x, int y, int z);
int World_getFirstUncoveredBlock(World *self, int x, int z);
Material *World_getBlockMaterial(World *self, int x, int y, int z);
int World_blockExists(World *self, int x, int y, int z);
int World_isBlockNormalCube(World *self, int x, int y, int z);
int World_isBlockOpaqueCube(World *self, int x, int y, int z);
int World_isBlockSolid(World *self, int x, int y, int z);
long World_getRandomSeed(World *self);
int64_t World_getWorldTime(World *self);
WorldChunkManager *World_getWorldChunkManager(World *self);
int World_doesBlockHaveSolidTopSurface(World *self, int x, int y, int z);
int World_getHeightValue(World *self, int x, int z);
int World_getBlockLightValue(World *self, int x, int y, int z);
int World_isBlockIndirectlyGettingPowered(World *self, int x, int y, int z);

int World_isBlockProvidingPowerTo(World *self, int x, int y, int z, int side);

int World_isBlockGettingPowered(World *self, int x, int y, int z);

int World_isBlockIndirectlyProvidingPowerTo(World *self, int x, int y, int z, int side);
void World_scheduleBlockUpdate(World *self, int x, int y, int z, int blockId, int delay);
void World_notifyBlocksOfNeighborChange(World *self, int x, int y, int z, int blockId);
void World_notifyBlockOfNeighborChange(World *self, int x, int y, int z, int neighborId);
int World_spawnEntityInWorld(World *self, Entity *entity);
void World_removeEntity(World *self, Entity *entity);
void World_playSoundEffect(World *self, double x, double y, double z, const char *sound, float vol,
                           float pitch);
void World_playAuxSFX(World *self, int effectId, int x, int y, int z, int data);

struct EntityPlayer;
void World_onBlockHit(World *self, struct EntityPlayer *player, int x, int y, int z, int side);

void World_tick(World *self);

void World_updateEntities(World *self);

int World_isAllPlayersFullyAsleep(World *self);

void World_wakeUpAllPlayers(World *self);

void World_stopPrecipitation(World *self);

void World_setAllowedMobSpawns(World *self, int var1, int var2);

int World_TickUpdates(World *self, int var1);

void World_updateBlocksAndPlayCaveSounds(World *self);

void World_randomDisplayUpdates(World *self, int var1, int var2, int var3);

void World_saveWorld(World *self, int var1, IProgressUpdate *var2);

void World_saveWorldIndirectly(World *self, IProgressUpdate *var1);

void World_checkSessionLock(World *self);

WorldInfo *World_getWorldInfo(World *self);

int World_getCollidingBoundingBoxes(World *self, struct Entity *entity, AxisAlignedBB *aabb,
                                    AxisAlignedBB *outList, int maxOut);

float World_getRainStrength(World *self, float partialTick);
float World_getThunderStrength(World *self, float partialTick);
Vec3D *World_getSkyColor(World *self, Entity *viewer, float partialTick);
Vec3D *World_getFogColor(World *self, float partialTick);
Vec3D *World_getCloudColor(World *self, float partialTick);
int World_findTopSolidBlock(World *self, int x, int z);

struct Entity *World_getClosestPlayer(World *self, double x, double y, double z, double dist);

struct Entity *World_getClosestPlayerToEntity(World *self, struct Entity *entity, double dist);

void World_playSoundAtEntity(World *self, struct Entity *entity, const char *name, float vol,
                             float pitch);

void World_spawnParticle(World *self, const char *name, double x, double y, double z, double vx,
                         double vy, double vz);

void World_createExplosion(World *self, struct Entity *entity, double x, double y, double z,
                           float strength);

void World_newExplosion(World *self, struct Entity *entity, double x, double y, double z,
                        float strength, int flag);

int World_entityJoinedWorld(World *self, struct Entity *entity);
int World_isNether(World *self);

int World_doChunksNearChunkExist(World *self, int x, int y, int z, int range);

int World_checkIfAABBIsClear(World *self, AxisAlignedBB *bb);

int World_canBlockBePlacedAt(World *self, int blockID, int x, int y, int z, int isFlipped,
                             int side);

int World_getIsAnyLiquid(World *self, AxisAlignedBB *bb);

int World_isAABBInMaterial(World *self, AxisAlignedBB *var1, Material *var2);

int World_handleMaterialAcceleration(World *self, AxisAlignedBB *bb, Material *mat,
                                     struct Entity *entity);

int World_getEntitiesWithinAABBExcludingEntity(World *self, struct Entity *entity,
                                               AxisAlignedBB *bb, struct Entity **outList,
                                               int maxOut);

int World_getEntitiesWithinAABB(World *self, int (*match)(struct Entity *), AxisAlignedBB *bb,
                                struct Entity **outList, int maxOut);

struct Entity *World_getPlayerEntityByName(World *self, const char *name);

struct PathEntity *World_getPathToEntity(World *self, struct Entity *entity, struct Entity *target,
                                         float range);

struct PathEntity *World_getEntityPathToXYZ(World *self, struct Entity *var1, int var2, int var3,
                                            int var4, float var5);

int World_addWeatherEffect(World *self, struct Entity *var1);

void World_func_27158_h(World *self, float var1);

int World_func_27160_B(World *self);

void World_func_9425_a(World *self, struct Entity *entity, int8_t eventId);

int World_isDaytime(World *self);

int World_canBlockSeeTheSky(World *self, int x, int y, int z);

void World_markBlocksDirty(World *self, int x1, int y1, int z1, int x2, int y2, int z2);

void World_markBlockAsNeedsUpdate(World *self, int x, int y, int z);

void World_func_28106_e(World *self, int eventID, int x, int y, int z, int data);

void World_playRecord(World *self, const char *var1, int var2, int var3, int var4);

int World_func_6466_a(World *self, struct EntityPlayer *var1, int var2, int var3, int var4);

void World_func_28107_a(World *self, struct EntityPlayer *player, int eventID, int x, int y, int z,
                        int data);

int World_setBlockMetadata(World *self, int x, int y, int z, int meta);

int World_setBlockAndMetadataWithNotify(World *self, int x, int y, int z, int id, int meta);

int World_func_27161_C(World *self);

int World_canBlockBeRainedOn(World *self, int x, int y, int z);

void World_setWorldTime(World *self, int64_t var1);

int World_calculateSkylightSubtracted(World *self, float var1);

void World_setSpawnPoint(World *self, ChunkCoordinates *var1);

void World_setEntityDead(World *self, struct Entity *var1);

void World_markBlockNeedsUpdate(World *self, int x, int y, int z);

void World_obtainEntitySkin(World *self, struct Entity *var1);

void World_releaseEntitySkin(World *self, struct Entity *var1);

void World_notifyBlockChange(World *self, int x, int y, int z, int id);

void World_updateAllPlayersSleepingFlag(World *self);

int World_chunkExists(World *self, int cx, int cz);

void World_scheduleLightingUpdate(World *self, EnumSkyBlock var1, int var2, int var3, int var4,
                                  int var5, int var6, int var7);

int World_canExistingBlockSeeTheSky(World *self, int x, int y, int z);

int World_getSavedLightValue(World *self, EnumSkyBlock type, int x, int y, int z);

void World_setLightValue(World *self, EnumSkyBlock type, int x, int y, int z, int val);

void World_neighborLightPropagationChanged(World *self, EnumSkyBlock type, int x, int y, int z,
                                           int val);

float World_getCelestialAngle(World *self, float var1);

float World_getStarBrightness(World *self, float var1);

float World_func_27162_g(World *self, float var1);

ChunkCoordinates World_getSpawnPoint(World *self);

int World_countEntities(World *self, EntityClassId classId);

int World_func_650_a(World *self, int var1);

void World_sendQuittingDisconnectingPacket(World *self);

const char *World_func_687_d(World *self, char *buf, int bufLen);

const char *World_func_21119_g(World *self, char *buf, int bufLen);

void World_emptyMethod1(World *self);

int World_checkChunksExist(World *self, int var1, int var2, int var3, int var4, int var5, int var6);

void World_updateEntityWithOptionalForce(World *self, struct Entity *var1, int var2);

void World_updateEntity(World *self, struct Entity *var1);

void World_spawnPlayerWithLoadedChunks(World *self, struct Entity *var1);

struct Entity *World_func_4085_a(World *self);

IChunkProvider *World_getIChunkProvider(World *self);

int World_updatingLighting(World *self);

void World_func_656_j(World *self);

MovingObjectPosition *World_rayTraceBlocks(World *self, Vec3D *var1, Vec3D *var2);

MovingObjectPosition *World_rayTraceBlocks_do(World *self, Vec3D *var1, Vec3D *var2, int var3);

MovingObjectPosition *World_func_28105_a(World *self, Vec3D *var1, Vec3D *var2, int var3, int var4);

void World_setItemData(World *self, const char *var1, MapDataBase *var2);

MapDataBase *World_loadItemData(World *self, MapDataBase *(*factory)(const char *),
                                const char *var2);

int World_getUniqueDataId(World *self, const char *var1);

float World_getLightBrightness(World *self, int var1, int var2, int var3);

int World_isBoundingBoxBurning(World *self, AxisAlignedBB *var1);

int World_isMaterialInBB(World *self, AxisAlignedBB *var1, Material *var2);

void World_markBlocksDirtyVertical(World *self, int var1, int var2, int var3, int var4);

void World_func_31054_a(World *self, TileEntity **var1, int count);

void World_func_636_a(World *self, struct Entity **var1, int count);

void World_func_632_b(World *self, struct Entity **var1, int count);

float World_func_675_a(World *self, Vec3D *var1, AxisAlignedBB *var2);

void World_playNoteAt(World *self, int x, int y, int z, int p1, int p2);

void World_func_698_b(World *self, int var1, int var2, int var3, struct TileEntity *var4);

void World_setChunkData(World *self, int var1, int var2, int var3, int var4, int var5, int var6,
                        uint8_t *var7);

void World_setSpawnLocation(World *self);

void World_updateEntityList(World *self);

void World_joinEntityInSurroundings(World *self, struct Entity *var1);

void World_addWorldAccess(World *self, struct IWorldAccess *var1);

void World_removeWorldAccess(World *self, struct IWorldAccess *var1);

#endif
