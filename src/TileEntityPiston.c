#include "TileEntityPiston.h"
#include "PistonBlockTextures.h"
#include "World.h"
#include "Block.h"
#include "Entity.h"
#include "AxisAlignedBB.h"
#include <stdlib.h>

#define PISTON_ENTITY_BUF 64
static Entity *piston_field_31018_m[PISTON_ENTITY_BUF];

static void piston_func_31010_a(TileEntityPiston *self, float var1, float var2) {
    TileEntity *base = &self->base;
    if (!self->field_31024_i) {
        --var1;
    } else {
        var1 = 1.0f - var1;
    }
    AxisAlignedBB *var3 = BlockPistonMoving_func_31035_a(
        Block_pistonMoving, base->worldObj, base->xCoord, base->yCoord, base->zCoord,
        self->storedBlockID, var1, self->field_31025_c);
    if (var3 != NULL) {
        int count = World_getEntitiesWithinAABBExcludingEntity(
            base->worldObj, NULL, var3, piston_field_31018_m, PISTON_ENTITY_BUF);
        if (count > 0) {
            double dx =
                (double)(var2 * (float)PistonBlockTextures_field_31056_b[self->field_31025_c]);
            double dy =
                (double)(var2 * (float)PistonBlockTextures_field_31059_c[self->field_31025_c]);
            double dz =
                (double)(var2 * (float)PistonBlockTextures_field_31058_d[self->field_31025_c]);
            for (int i = 0; i < count; ++i) {
                piston_field_31018_m[i]->vtable->moveEntity(piston_field_31018_m[i], dx, dy, dz);
            }
        }
    }
}

static void piston_updateEntity(TileEntity *base) {
    TileEntityPiston *self = (TileEntityPiston *)base;
    self->field_31020_l = self->field_31022_k;
    if (self->field_31020_l >= 1.0f) {
        piston_func_31010_a(self, 1.0f, 0.25f);
        World_removeBlockTileEntity(base->worldObj, base->xCoord, base->yCoord, base->zCoord);
        base->tileEntityInvalid = 1;
        if (World_getBlockId(base->worldObj, base->xCoord, base->yCoord, base->zCoord) ==
            Block_pistonMoving->blockID) {
            World_setBlockAndMetadataWithNotify(base->worldObj, base->xCoord, base->yCoord,
                                                base->zCoord, self->storedBlockID,
                                                self->storedMetadata);
        }
    } else {
        self->field_31022_k += 0.5f;
        if (self->field_31022_k >= 1.0f) {
            self->field_31022_k = 1.0f;
        }
        if (self->field_31024_i) {
            piston_func_31010_a(self, self->field_31022_k,
                                self->field_31022_k - self->field_31020_l + 1.0f / 16.0f);
        }
    }
}

static void piston_readFromNBT(TileEntity *base, NBTTagCompound *tag) {
    TileEntityPiston *self = (TileEntityPiston *)base;
    TileEntity_base_readFromNBT(base, tag);
    self->storedBlockID = NBTTagCompound_getInteger(tag, "blockId");
    self->storedMetadata = NBTTagCompound_getInteger(tag, "blockData");
    self->field_31025_c = NBTTagCompound_getInteger(tag, "facing");
    self->field_31020_l = self->field_31022_k = NBTTagCompound_getFloat(tag, "progress");
    self->field_31024_i = NBTTagCompound_getBoolean(tag, "extending");
}

static void piston_writeToNBT(TileEntity *base, NBTTagCompound *tag) {
    TileEntityPiston *self = (TileEntityPiston *)base;
    TileEntity_base_writeToNBT(base, tag);
    NBTTagCompound_setInteger(tag, "blockId", self->storedBlockID);
    NBTTagCompound_setInteger(tag, "blockData", self->storedMetadata);
    NBTTagCompound_setInteger(tag, "facing", self->field_31025_c);
    NBTTagCompound_setFloat(tag, "progress", self->field_31020_l);
    NBTTagCompound_setBoolean(tag, "extending", self->field_31024_i);
}

static int piston_needsUpdate(TileEntity *self) {
    (void)self;
    return 1;
}
static void piston_destroy(TileEntity *base) { free(base); }

TileEntityPiston *TileEntityPiston_create(void) {
    static TileEntityVtable vt;
    static int once = 0;
    if (!once) {
        TileEntity_initVtable(&vt);
        vt.typeName = "Piston";
        vt.updateEntity = piston_updateEntity;
        vt.readFromNBT = piston_readFromNBT;
        vt.writeToNBT = piston_writeToNBT;
        vt.needsUpdate = piston_needsUpdate;
        vt.destroy = piston_destroy;
        once = 1;
    }
    TileEntityPiston *self = (TileEntityPiston *)calloc(1, sizeof(TileEntityPiston));
    TileEntity_init(&self->base);
    self->base.vtable = &vt;
    self->base.tileEntityTypeId = TE_TYPE_PISTON;
    return self;
}

TileEntityPiston *TileEntityPiston_createWith(int storedBlockID, int storedMetadata,
                                              int orientation, int extending,
                                              int shouldHeadBeRendered) {
    TileEntityPiston *self = TileEntityPiston_create();
    self->storedBlockID = storedBlockID;
    self->storedMetadata = storedMetadata;
    self->field_31025_c = orientation;
    self->field_31024_i = extending;
    self->field_31023_j = shouldHeadBeRendered;
    return self;
}

int TileEntityPiston_getStoredBlockID(TileEntityPiston *self) { return self->storedBlockID; }

int TileEntityPiston_getBlockMetadata(TileEntityPiston *self) { return self->storedMetadata; }

int TileEntityPiston_func_31015_b(TileEntityPiston *self) { return self->field_31024_i; }

int TileEntityPiston_func_31009_d(TileEntityPiston *self) { return self->field_31025_c; }

int TileEntityPiston_func_31012_k(TileEntityPiston *self) { return self->field_31023_j; }

float TileEntityPiston_func_31008_a(TileEntityPiston *self, float var1) {
    if (var1 > 1.0f)
        var1 = 1.0f;
    return self->field_31020_l + (self->field_31022_k - self->field_31020_l) * var1;
}

float TileEntityPiston_func_31017_b(TileEntityPiston *self, float var1) {
    float p = TileEntityPiston_func_31008_a(self, var1);
    return self->field_31024_i
               ? (p - 1.0f) * (float)PistonBlockTextures_field_31056_b[self->field_31025_c]
               : (1.0f - p) * (float)PistonBlockTextures_field_31056_b[self->field_31025_c];
}

float TileEntityPiston_func_31014_c(TileEntityPiston *self, float var1) {
    float p = TileEntityPiston_func_31008_a(self, var1);
    return self->field_31024_i
               ? (p - 1.0f) * (float)PistonBlockTextures_field_31059_c[self->field_31025_c]
               : (1.0f - p) * (float)PistonBlockTextures_field_31059_c[self->field_31025_c];
}

float TileEntityPiston_func_31013_d(TileEntityPiston *self, float var1) {
    float p = TileEntityPiston_func_31008_a(self, var1);
    return self->field_31024_i
               ? (p - 1.0f) * (float)PistonBlockTextures_field_31058_d[self->field_31025_c]
               : (1.0f - p) * (float)PistonBlockTextures_field_31058_d[self->field_31025_c];
}

void TileEntityPiston_func_31011_l(TileEntityPiston *self) {
    TileEntity *base = &self->base;
    if (self->field_31020_l < 1.0f) {
        self->field_31020_l = self->field_31022_k = 1.0f;
        World_removeBlockTileEntity(base->worldObj, base->xCoord, base->yCoord, base->zCoord);
        base->tileEntityInvalid = 1;
        if (World_getBlockId(base->worldObj, base->xCoord, base->yCoord, base->zCoord) ==
            Block_pistonMoving->blockID) {
            World_setBlockAndMetadataWithNotify(base->worldObj, base->xCoord, base->yCoord,
                                                base->zCoord, self->storedBlockID,
                                                self->storedMetadata);
        }
    }
}
