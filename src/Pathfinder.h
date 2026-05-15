#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "PathEntity.h"
#include "Path.h"
#include "MCHash.h"
#include "IBlockAccess.h"

struct Entity;

typedef struct Pathfinder {

    IBlockAccess *worldMap;

    Path *path;

    MCHash pointMap;

    PathPoint *pathOptions[32];
} Pathfinder;

Pathfinder *Pathfinder_create(IBlockAccess *var1);
void Pathfinder_destroy(Pathfinder *self);

PathEntity *Pathfinder_createEntityPathToEntity(Pathfinder *self, struct Entity *var1,
                                                struct Entity *var2, float var3);

PathEntity *Pathfinder_createEntityPathToCoords(Pathfinder *self, struct Entity *var1, int var2,
                                                int var3, int var4, float var5);

#endif
