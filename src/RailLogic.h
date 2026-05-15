#ifndef RAIL_LOGIC_H
#define RAIL_LOGIC_H

#include "World.h"
#include "ChunkPosition.h"
#include "BlockRail.h"

typedef struct {
    ChunkPosition *items;
    int count;
    int capacity;
} ChunkPositionList;

typedef struct RailLogic {

    BlockRail *rail;

    World *worldObj;

    int trackX;
    int trackY;
    int trackZ;

    int isPoweredRail;

    ChunkPositionList connectedTracks;
} RailLogic;

RailLogic *RailLogic_create(BlockRail *var1, World *var2, int var3, int var4, int var5);
void RailLogic_destroy(RailLogic *self);

void RailLogic_func_792_a(RailLogic *self, int var1, int var2);

int RailLogic_getNAdjacentTracks(RailLogic *var0);

#endif
