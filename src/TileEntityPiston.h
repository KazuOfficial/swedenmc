#ifndef TILE_ENTITY_PISTON_H
#define TILE_ENTITY_PISTON_H

#include "TileEntity.h"

typedef struct TileEntityPiston {
    TileEntity base;

    int storedBlockID;

    int storedMetadata;

    int field_31025_c;

    int field_31024_i;

    int field_31023_j;

    float field_31022_k;

    float field_31020_l;
} TileEntityPiston;

TileEntityPiston *TileEntityPiston_create(void);

TileEntityPiston *TileEntityPiston_createWith(int storedBlockID, int storedMetadata,
                                              int orientation, int extending,
                                              int shouldHeadBeRendered);

int TileEntityPiston_getStoredBlockID(TileEntityPiston *self);

int TileEntityPiston_getBlockMetadata(TileEntityPiston *self);

int TileEntityPiston_func_31015_b(TileEntityPiston *self);

int TileEntityPiston_func_31009_d(TileEntityPiston *self);

int TileEntityPiston_func_31012_k(TileEntityPiston *self);

float TileEntityPiston_func_31008_a(TileEntityPiston *self, float var1);

float TileEntityPiston_func_31017_b(TileEntityPiston *self, float var1);

float TileEntityPiston_func_31014_c(TileEntityPiston *self, float var1);

float TileEntityPiston_func_31013_d(TileEntityPiston *self, float var1);

void TileEntityPiston_func_31011_l(TileEntityPiston *self);

#endif
