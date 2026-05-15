#ifndef TILE_ENTITY_RECORD_PLAYER_H
#define TILE_ENTITY_RECORD_PLAYER_H

#include "TileEntity.h"

typedef struct TileEntityRecordPlayer {
    TileEntity base;
    int record;
} TileEntityRecordPlayer;

TileEntityRecordPlayer *TileEntityRecordPlayer_create(void);

#endif
