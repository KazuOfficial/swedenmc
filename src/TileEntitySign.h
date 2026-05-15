#ifndef TILE_ENTITY_SIGN_H
#define TILE_ENTITY_SIGN_H

#include "TileEntity.h"

#define SIGN_LINES 4
#define SIGN_LINE_LEN 16

typedef struct TileEntitySign {
    TileEntity base;
    char signText[SIGN_LINES][SIGN_LINE_LEN];
    int lineBeingEdited;
    int field_25062_c;
} TileEntitySign;

TileEntitySign *TileEntitySign_create(void);

#endif
