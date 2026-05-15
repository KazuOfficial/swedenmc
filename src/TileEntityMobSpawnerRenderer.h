#ifndef TILE_ENTITY_MOB_SPAWNER_RENDERER_H
#define TILE_ENTITY_MOB_SPAWNER_RENDERER_H

#include "TileEntitySpecialRenderer.h"
#include "TileEntityMobSpawner.h"

#define TE_MOB_SPAWNER_RENDERER_MAP_SIZE 32
typedef struct {
    char mobID[64];
    Entity *entity;
} TEMobSpawnerEntry;

typedef struct TileEntityMobSpawnerRenderer {
    TileEntitySpecialRenderer base;
    TEMobSpawnerEntry entityHashMap[TE_MOB_SPAWNER_RENDERER_MAP_SIZE];
    int entityHashMapSize;
} TileEntityMobSpawnerRenderer;

TileEntitySpecialRenderer *TileEntityMobSpawnerRenderer_create(void);

#endif
