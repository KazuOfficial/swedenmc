#ifndef TILE_ENTITY_H
#define TILE_ENTITY_H

#include "GameForward.h"
#include "NBTTagCompound.h"

struct TileEntity;
typedef struct TileEntity TileEntity;

typedef struct TileEntityVtable {
    const char *typeName;
    void (*updateEntity)(TileEntity *self);
    int (*needsUpdate)(TileEntity *self);
    void (*validate)(TileEntity *self);
    void (*invalidate)(TileEntity *self);
    int (*isInvalid)(TileEntity *self);
    void (*writeToNBT)(TileEntity *self, NBTTagCompound *tag);
    void (*readFromNBT)(TileEntity *self, NBTTagCompound *tag);
    void (*destroy)(TileEntity *self);
} TileEntityVtable;

#define TE_TYPE_BASE 0
#define TE_TYPE_SIGN 1
#define TE_TYPE_MOB_SPAWNER 2
#define TE_TYPE_PISTON 3

struct TileEntity {
    const TileEntityVtable *vtable;
    int xCoord, yCoord, zCoord;
    World *worldObj;
    int tileEntityInvalid;

    int tileEntityTypeId;
};

extern TileEntityVtable TileEntity_defaultVtable;

void TileEntity_init(TileEntity *self);
void TileEntity_initVtable(TileEntityVtable *vt);

void TileEntity_updateEntity(TileEntity *self);
int TileEntity_needsUpdate(TileEntity *self);
void TileEntity_validate(TileEntity *self);
void TileEntity_invalidate(TileEntity *self);
int TileEntity_isInvalid(TileEntity *self);
void TileEntity_writeToNBT(TileEntity *self, NBTTagCompound *tag);
void TileEntity_readFromNBT(TileEntity *self, NBTTagCompound *tag);
void TileEntity_base_writeToNBT(TileEntity *self, NBTTagCompound *tag);
void TileEntity_base_readFromNBT(TileEntity *self, NBTTagCompound *tag);
void TileEntity_destroy(TileEntity *self);

void TileEntity_destroy_deferred(TileEntity *self);

void TileEntity_flush_destroy_queue(void);

int TileEntity_is_queued_for_free(TileEntity *te);

int TileEntity_getBlockMetadata(TileEntity *self);
Block *TileEntity_getBlockType(TileEntity *self);

double TileEntity_getDistanceFrom(TileEntity *self, double var1, double var3, double var5);

TileEntity *TileEntity_createAndLoadEntity(NBTTagCompound *tag);

void TileEntity_onInventoryChanged(TileEntity *self);

#endif
