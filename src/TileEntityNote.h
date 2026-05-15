#ifndef TILE_ENTITY_NOTE_H
#define TILE_ENTITY_NOTE_H

#include "TileEntity.h"
#include <stdint.h>

typedef struct TileEntityNote {
    TileEntity base;
    int8_t note;
    int previousRedstoneState;
} TileEntityNote;

TileEntityNote *TileEntityNote_create(void);
void TileEntityNote_changePitch(TileEntityNote *self);
void TileEntityNote_triggerNote(TileEntityNote *self, World *world, int x, int y, int z);

#endif
