#ifndef IWORLD_ACCESS_H
#define IWORLD_ACCESS_H

#include "GameForward.h"

typedef struct IWorldAccessVtable {
    void (*markBlockAndNeighborsNeedsUpdate)(struct IWorldAccess *self, int var1, int var2,
                                             int var3);
    void (*markBlockRangeNeedsUpdate)(struct IWorldAccess *self, int var1, int var2, int var3,
                                      int var4, int var5, int var6);
    void (*playSound)(struct IWorldAccess *self, const char *var1, double var2, double var4,
                      double var6, float var8, float var9);
    void (*spawnParticle)(struct IWorldAccess *self, const char *var1, double var2, double var4,
                          double var6, double var8, double var10, double var12);
    void (*obtainEntitySkin)(struct IWorldAccess *self, struct Entity *var1);
    void (*releaseEntitySkin)(struct IWorldAccess *self, struct Entity *var1);
    void (*updateAllRenderers)(struct IWorldAccess *self);
    void (*playRecord)(struct IWorldAccess *self, const char *var1, int var2, int var3, int var4);
    void (*doNothingWithTileEntity)(struct IWorldAccess *self, int var1, int var2, int var3,
                                    TileEntity *var4);
    void (*func_28136_a)(struct IWorldAccess *self, EntityPlayer *var1, int var2, int var3,
                         int var4, int var5, int var6);
} IWorldAccessVtable;

typedef struct IWorldAccess {
    const IWorldAccessVtable *vtable;
} IWorldAccess;

#endif
