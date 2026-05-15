#ifndef CHUNK_H
#define CHUNK_H

#include "NibbleArray.h"
#include "GameForward.h"
#include "EnumSkyBlock.h"
#include "AxisAlignedBB.h"
#include <stdint.h>

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16

#define CHUNK_INDEX(x, y, z) (((x) << 11) | ((z) << 7) | (y))

typedef struct Chunk {
    World *worldObj;
    int xPosition;
    int zPosition;

    uint8_t blocks[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
    NibbleArray *data;
    NibbleArray *skylightMap;
    NibbleArray *blocklightMap;

    uint8_t heightMap[CHUNK_SIZE_X * CHUNK_SIZE_Z];
    int lowestBlockHeight;

    int isModified;
    int isTerrainPopulated;
    int hasEntities;
    int64_t lastSaveTime;
    int neverSave;
    int isChunkLoaded;
    int isEmptyChunk;

    void **tileEntities;
    int numTileEntities;
    int tileEntityCap;

#define CHUNK_ENTITY_SECTIONS 8
    struct Entity **entitySections[CHUNK_ENTITY_SECTIONS];
    int entitySectionCounts[CHUNK_ENTITY_SECTIONS];
    int entitySectionCaps[CHUNK_ENTITY_SECTIONS];
} Chunk;

Chunk *Chunk_create(World *world, int chunkX, int chunkZ);
Chunk *Chunk_create_with_blocks(World *world, uint8_t *blocks, int chunkX, int chunkZ);
void Chunk_destroy(Chunk *chunk);

int Chunk_getBlockID(Chunk *chunk, int x, int y, int z);
int Chunk_setBlockID(Chunk *chunk, int x, int y, int z, int id);
int Chunk_getBlockMetadata(Chunk *chunk, int x, int y, int z);
int Chunk_setBlockMetadata(Chunk *chunk, int x, int y, int z, int meta);

int Chunk_getSkyLight(Chunk *chunk, int x, int y, int z);
void Chunk_setSkyLight(Chunk *chunk, int x, int y, int z, int val);
int Chunk_getBlockLight(Chunk *chunk, int x, int y, int z);
void Chunk_setBlockLight(Chunk *chunk, int x, int y, int z, int val);

int Chunk_getHeightValue(Chunk *chunk, int x, int z);
void Chunk_generateHeightMap(Chunk *chunk);

void Chunk_onChunkLoad(Chunk *chunk);
void Chunk_onChunkUnload(Chunk *chunk);

int Chunk_needsSaving(Chunk *chunk, int forceSave);

void Chunk_setChunkModified(Chunk *chunk);

int Chunk_isAtLocation(Chunk *chunk, int chunkX, int chunkZ);

void Chunk_func_4143_d(Chunk *chunk);

void Chunk_func_25124_i(Chunk *chunk);

int Chunk_getBlockLightValue(Chunk *chunk, int x, int y, int z, int skylightSubtracted);

extern __thread int Chunk_isLit;

void Chunk_addEntity(Chunk *chunk, struct Entity *var1);

void Chunk_removeEntity(Chunk *chunk, struct Entity *var1);

void Chunk_removeEntityAtIndex(Chunk *chunk, struct Entity *var1, int var2);

int Chunk_getEntitiesWithinAABBForEntity(Chunk *chunk, struct Entity *var1, AxisAlignedBB *var2,
                                         struct Entity **outList, int outOffset, int outCap);

int Chunk_getEntitiesOfTypeWithinAAAB(Chunk *chunk, int (*match)(struct Entity *),
                                      AxisAlignedBB *var2, struct Entity **outList, int outOffset,
                                      int outCap);

TileEntity *Chunk_getChunkBlockTileEntity(Chunk *chunk, int x, int y, int z);
void Chunk_addTileEntity(Chunk *chunk, TileEntity *te);
void Chunk_setChunkBlockTileEntity(Chunk *chunk, int x, int y, int z, TileEntity *te);
void Chunk_removeChunkBlockTileEntity(Chunk *chunk, int x, int y, int z);

int Chunk_canBlockSeeTheSky(Chunk *chunk, int x, int y, int z);

void Chunk_func_1014_a(Chunk *chunk);

void Chunk_func_1024_c(Chunk *chunk);

int Chunk_func_21167_h(Chunk *chunk);

void Chunk_func_997_a(Chunk *chunk, int64_t var1, JavaRandom *out);

int Chunk_setBlockIDWithMetadata(Chunk *chunk, int x, int y, int z, int id, int meta);

int Chunk_setChunkData(Chunk *chunk, uint8_t *var1, int var2, int var3, int var4, int var5,
                       int var6, int var7, int var8);

int Chunk_getSavedLightValue(Chunk *chunk, EnumSkyBlock var1, int var2, int var3, int var4);

void Chunk_setLightValue(Chunk *chunk, EnumSkyBlock var1, int var2, int var3, int var4, int var5);

#endif
