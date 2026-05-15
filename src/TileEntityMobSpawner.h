#ifndef TILE_ENTITY_MOB_SPAWNER_H
#define TILE_ENTITY_MOB_SPAWNER_H

#include "TileEntity.h"

typedef struct TileEntityMobSpawner {
    TileEntity base;
    int delay;
    char mobID[64];

    double yaw;

    double yaw2;
} TileEntityMobSpawner;

TileEntityMobSpawner *TileEntityMobSpawner_create(void);

const char *TileEntityMobSpawner_getMobID(TileEntityMobSpawner *self);

void TileEntityMobSpawner_setMobID(TileEntityMobSpawner *self, const char *var1);

#endif
