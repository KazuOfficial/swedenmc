#include "RailLogic.h"
#include "Block.h"
#include <stdlib.h>
#include <string.h>

static void cpl_init(ChunkPositionList *self) {
    self->capacity = 4;
    self->count = 0;
    self->items = (ChunkPosition *)malloc(self->capacity * sizeof(ChunkPosition));
}

static void cpl_free(ChunkPositionList *self) {
    free(self->items);
    self->items = NULL;
    self->count = 0;
    self->capacity = 0;
}

static void cpl_clear(ChunkPositionList *self) { self->count = 0; }

static void cpl_add(ChunkPositionList *self, ChunkPosition cp) {
    if (self->count == self->capacity) {
        self->capacity *= 2;
        self->items = (ChunkPosition *)realloc(self->items, self->capacity * sizeof(ChunkPosition));
    }
    self->items[self->count++] = cp;
}

static ChunkPosition cpl_get(ChunkPositionList *self, int i) { return self->items[i]; }

static void cpl_set(ChunkPositionList *self, int i, ChunkPosition cp) { self->items[i] = cp; }

static void cpl_remove(ChunkPositionList *self, int i) {
    memmove(&self->items[i], &self->items[i + 1], (self->count - i - 1) * sizeof(ChunkPosition));
    --self->count;
}

static int cpl_size(ChunkPositionList *self) { return self->count; }

static void setConnections(RailLogic *self, int var1) {
    cpl_clear(&self->connectedTracks);
    if (var1 == 0) {
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX, self->trackY, self->trackZ - 1));
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX, self->trackY, self->trackZ + 1));
    } else if (var1 == 1) {
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX - 1, self->trackY, self->trackZ));
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX + 1, self->trackY, self->trackZ));
    } else if (var1 == 2) {
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX - 1, self->trackY, self->trackZ));
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX + 1, self->trackY + 1, self->trackZ));
    } else if (var1 == 3) {
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX - 1, self->trackY + 1, self->trackZ));
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX + 1, self->trackY, self->trackZ));
    } else if (var1 == 4) {
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX, self->trackY + 1, self->trackZ - 1));
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX, self->trackY, self->trackZ + 1));
    } else if (var1 == 5) {
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX, self->trackY, self->trackZ - 1));
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX, self->trackY + 1, self->trackZ + 1));
    } else if (var1 == 6) {
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX + 1, self->trackY, self->trackZ));
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX, self->trackY, self->trackZ + 1));
    } else if (var1 == 7) {
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX - 1, self->trackY, self->trackZ));
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX, self->trackY, self->trackZ + 1));
    } else if (var1 == 8) {
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX - 1, self->trackY, self->trackZ));
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX, self->trackY, self->trackZ - 1));
    } else if (var1 == 9) {
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX + 1, self->trackY, self->trackZ));
        cpl_add(&self->connectedTracks,
                ChunkPosition_create(self->trackX, self->trackY, self->trackZ - 1));
    }
}

static int isMinecartTrack(RailLogic *self, int var1, int var2, int var3) {
    return BlockRail_isRailBlockAt(self->worldObj, var1, var2, var3)
               ? 1
               : (BlockRail_isRailBlockAt(self->worldObj, var1, var2 + 1, var3)
                      ? 1
                      : BlockRail_isRailBlockAt(self->worldObj, var1, var2 - 1, var3));
}

static RailLogic *getMinecartTrackLogic(RailLogic *self, ChunkPosition var1) {
    if (BlockRail_isRailBlockAt(self->worldObj, var1.x, var1.y, var1.z))
        return RailLogic_create(self->rail, self->worldObj, var1.x, var1.y, var1.z);
    if (BlockRail_isRailBlockAt(self->worldObj, var1.x, var1.y + 1, var1.z))
        return RailLogic_create(self->rail, self->worldObj, var1.x, var1.y + 1, var1.z);
    if (BlockRail_isRailBlockAt(self->worldObj, var1.x, var1.y - 1, var1.z))
        return RailLogic_create(self->rail, self->worldObj, var1.x, var1.y - 1, var1.z);
    return NULL;
}

static int isConnectedTo(RailLogic *self, RailLogic *var1) {
    for (int var2 = 0; var2 < cpl_size(&self->connectedTracks); ++var2) {
        ChunkPosition var3 = cpl_get(&self->connectedTracks, var2);
        if (var3.x == var1->trackX && var3.z == var1->trackZ)
            return 1;
    }
    return 0;
}

static int isInTrack(RailLogic *self, int var1, int var2, int var3) {
    (void)var2;
    for (int var4 = 0; var4 < cpl_size(&self->connectedTracks); ++var4) {
        ChunkPosition var5 = cpl_get(&self->connectedTracks, var4);
        if (var5.x == var1 && var5.z == var3)
            return 1;
    }
    return 0;
}

static int getAdjacentTracks(RailLogic *self) {
    int var1 = 0;
    if (isMinecartTrack(self, self->trackX, self->trackY, self->trackZ - 1))
        ++var1;
    if (isMinecartTrack(self, self->trackX, self->trackY, self->trackZ + 1))
        ++var1;
    if (isMinecartTrack(self, self->trackX - 1, self->trackY, self->trackZ))
        ++var1;
    if (isMinecartTrack(self, self->trackX + 1, self->trackY, self->trackZ))
        ++var1;
    return var1;
}

static int handleKeyPress(RailLogic *self, RailLogic *var1) {
    if (isConnectedTo(self, var1))
        return 1;
    if (cpl_size(&self->connectedTracks) == 2)
        return 0;
    if (cpl_size(&self->connectedTracks) == 0)
        return 1;
    ChunkPosition var2 = cpl_get(&self->connectedTracks, 0);

    (void)var1;
    (void)var2;
    return 1;
}

static void func_785_b(RailLogic *self) {
    for (int var1 = 0; var1 < cpl_size(&self->connectedTracks); ++var1) {
        RailLogic *var2 = getMinecartTrackLogic(self, cpl_get(&self->connectedTracks, var1));
        if (var2 != NULL && isConnectedTo(var2, self)) {
            cpl_set(&self->connectedTracks, var1,
                    ChunkPosition_create(var2->trackX, var2->trackY, var2->trackZ));
            RailLogic_destroy(var2);
        } else {
            if (var2 != NULL)
                RailLogic_destroy(var2);
            cpl_remove(&self->connectedTracks, var1--);
        }
    }
}

static void func_788_d(RailLogic *self, RailLogic *var1) {
    cpl_add(&self->connectedTracks, ChunkPosition_create(var1->trackX, var1->trackY, var1->trackZ));
    int var2 = isInTrack(self, self->trackX, self->trackY, self->trackZ - 1);
    int var3 = isInTrack(self, self->trackX, self->trackY, self->trackZ + 1);
    int var4 = isInTrack(self, self->trackX - 1, self->trackY, self->trackZ);
    int var5 = isInTrack(self, self->trackX + 1, self->trackY, self->trackZ);
    int var6 = -1;
    if (var2 || var3)
        var6 = 0;
    if (var4 || var5)
        var6 = 1;
    if (!self->isPoweredRail) {
        if (var3 && var5 && !var2 && !var4)
            var6 = 6;
        if (var3 && var4 && !var2 && !var5)
            var6 = 7;
        if (var2 && var4 && !var3 && !var5)
            var6 = 8;
        if (var2 && var5 && !var3 && !var4)
            var6 = 9;
    }
    if (var6 == 0) {
        if (BlockRail_isRailBlockAt(self->worldObj, self->trackX, self->trackY + 1,
                                    self->trackZ - 1))
            var6 = 4;
        if (BlockRail_isRailBlockAt(self->worldObj, self->trackX, self->trackY + 1,
                                    self->trackZ + 1))
            var6 = 5;
    }
    if (var6 == 1) {
        if (BlockRail_isRailBlockAt(self->worldObj, self->trackX + 1, self->trackY + 1,
                                    self->trackZ))
            var6 = 2;
        if (BlockRail_isRailBlockAt(self->worldObj, self->trackX - 1, self->trackY + 1,
                                    self->trackZ))
            var6 = 3;
    }
    if (var6 < 0)
        var6 = 0;
    int var7 = var6;
    if (self->isPoweredRail) {
        var7 =
            (World_getBlockMetadata(self->worldObj, self->trackX, self->trackY, self->trackZ) & 8) |
            var6;
    }
    World_setBlockMetadataWithNotify(self->worldObj, self->trackX, self->trackY, self->trackZ,
                                     var7);
}

static int func_786_c(RailLogic *self, int var1, int var2, int var3) {
    RailLogic *var4 = getMinecartTrackLogic(self, ChunkPosition_create(var1, var2, var3));
    if (var4 == NULL)
        return 0;
    func_785_b(var4);
    int result = handleKeyPress(var4, self);
    RailLogic_destroy(var4);
    return result;
}

RailLogic *RailLogic_create(BlockRail *var1, World *var2, int var3, int var4, int var5) {
    RailLogic *self = (RailLogic *)calloc(1, sizeof(RailLogic));
    self->rail = var1;
    self->worldObj = var2;
    self->trackX = var3;
    self->trackY = var4;
    self->trackZ = var5;
    cpl_init(&self->connectedTracks);
    int var6 = World_getBlockId(var2, var3, var4, var5);
    int var7 = World_getBlockMetadata(var2, var3, var4, var5);
    if (Block_blocksList[var6] && BlockRail_getIsPowered(Block_blocksList[var6])) {
        self->isPoweredRail = 1;
        var7 &= -9;
    } else {
        self->isPoweredRail = 0;
    }
    setConnections(self, var7);
    return self;
}

void RailLogic_destroy(RailLogic *self) {
    cpl_free(&self->connectedTracks);
    free(self);
}

void RailLogic_func_792_a(RailLogic *self, int var1, int var2) {
    int var3 = func_786_c(self, self->trackX, self->trackY, self->trackZ - 1);
    int var4 = func_786_c(self, self->trackX, self->trackY, self->trackZ + 1);
    int var5 = func_786_c(self, self->trackX - 1, self->trackY, self->trackZ);
    int var6 = func_786_c(self, self->trackX + 1, self->trackY, self->trackZ);
    int var7 = -1;
    if ((var3 || var4) && !var5 && !var6)
        var7 = 0;
    if ((var5 || var6) && !var3 && !var4)
        var7 = 1;
    if (!self->isPoweredRail) {
        if (var4 && var6 && !var3 && !var5)
            var7 = 6;
        if (var4 && var5 && !var3 && !var6)
            var7 = 7;
        if (var3 && var5 && !var4 && !var6)
            var7 = 8;
        if (var3 && var6 && !var4 && !var5)
            var7 = 9;
    }
    if (var7 == -1) {
        if (var3 || var4)
            var7 = 0;
        if (var5 || var6)
            var7 = 1;
        if (!self->isPoweredRail) {
            if (var1) {
                if (var4 && var6)
                    var7 = 6;
                if (var5 && var4)
                    var7 = 7;
                if (var6 && var3)
                    var7 = 9;
                if (var3 && var5)
                    var7 = 8;
            } else {
                if (var3 && var5)
                    var7 = 8;
                if (var6 && var3)
                    var7 = 9;
                if (var5 && var4)
                    var7 = 7;
                if (var4 && var6)
                    var7 = 6;
            }
        }
    }
    if (var7 == 0) {
        if (BlockRail_isRailBlockAt(self->worldObj, self->trackX, self->trackY + 1,
                                    self->trackZ - 1))
            var7 = 4;
        if (BlockRail_isRailBlockAt(self->worldObj, self->trackX, self->trackY + 1,
                                    self->trackZ + 1))
            var7 = 5;
    }
    if (var7 == 1) {
        if (BlockRail_isRailBlockAt(self->worldObj, self->trackX + 1, self->trackY + 1,
                                    self->trackZ))
            var7 = 2;
        if (BlockRail_isRailBlockAt(self->worldObj, self->trackX - 1, self->trackY + 1,
                                    self->trackZ))
            var7 = 3;
    }
    if (var7 < 0)
        var7 = 0;
    setConnections(self, var7);
    int var8 = var7;
    if (self->isPoweredRail) {
        var8 =
            (World_getBlockMetadata(self->worldObj, self->trackX, self->trackY, self->trackZ) & 8) |
            var7;
    }
    if (var2 ||
        World_getBlockMetadata(self->worldObj, self->trackX, self->trackY, self->trackZ) != var8) {
        World_setBlockMetadataWithNotify(self->worldObj, self->trackX, self->trackY, self->trackZ,
                                         var8);
        for (int var9 = 0; var9 < cpl_size(&self->connectedTracks); ++var9) {
            RailLogic *var10 = getMinecartTrackLogic(self, cpl_get(&self->connectedTracks, var9));
            if (var10 != NULL) {
                func_785_b(var10);
                if (handleKeyPress(var10, self)) {
                    func_788_d(var10, self);
                }
                RailLogic_destroy(var10);
            }
        }
    }
}

int RailLogic_getNAdjacentTracks(RailLogic *var0) { return getAdjacentTracks(var0); }
