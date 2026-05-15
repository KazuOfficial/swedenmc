#include "Pathfinder.h"
#include "Entity.h"
#include "Block.h"
#include "BlockDoor.h"
#include "Material.h"
#include "MathHelper.h"
#include <stdlib.h>

Pathfinder *Pathfinder_create(IBlockAccess *var1) {
    Pathfinder *self = (Pathfinder *)calloc(1, sizeof(Pathfinder));
    self->worldMap = var1;
    self->path = Path_create();
    MCHash_init(&self->pointMap);
    return self;
}

static void freePointMap(Pathfinder *self) {
    MCHash *m = &self->pointMap;
    for (int i = 0; i < m->slotsLen; i++) {
        for (MCHashEntry *e = m->slots[i]; e != NULL; e = e->nextEntry)
            free(e->valueEntry);
    }
    MCHash_clearMap(m);
}

void Pathfinder_destroy(Pathfinder *self) {
    freePointMap(self);
    Path_destroy(self->path);
    MCHash_destroy(&self->pointMap);
    free(self);
}

static PathPoint *openPoint(Pathfinder *self, int var1, int var2, int var3) {
    int var4 = PathPoint_func_22329_a(var1, var2, var3);
    PathPoint *var5 = (PathPoint *)MCHash_lookup(&self->pointMap, var4);
    if (var5 == NULL) {
        var5 = PathPoint_create(var1, var2, var3);
        MCHash_addKey(&self->pointMap, var4, var5);
    }
    return var5;
}

static int getVerticalOffset(Pathfinder *self, Entity *var1, int var2, int var3, int var4,
                             PathPoint *var5) {
    (void)var1;
    for (int var6 = var2; var6 < var2 + var5->xCoord; ++var6) {
        for (int var7 = var3; var7 < var3 + var5->yCoord; ++var7) {
            for (int var8 = var4; var8 < var4 + var5->zCoord; ++var8) {
                int var9 = self->worldMap->vtable->getBlockId(self->worldMap, var6, var7, var8);
                if (var9 > 0 && Block_blocksList[var9]) {
                    if (var9 != Block_doorSteel->blockID && var9 != Block_doorWood->blockID) {
                        Material *var11 = Block_blocksList[var9]->blockMaterial;
                        if (Material_getIsSolid(var11)) {
                            return 0;
                        }
                        if (var11 == Material_water) {
                            return -1;
                        }
                        if (var11 == Material_lava) {
                            return -2;
                        }
                    } else {
                        int var10 = self->worldMap->vtable->getBlockMetadata(self->worldMap, var6,
                                                                             var7, var8);
                        if (!BlockDoor_isOpen(var10)) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

static PathPoint *getSafePoint(Pathfinder *self, Entity *var1, int var2, int var3, int var4,
                               PathPoint *var5, int var6) {
    PathPoint *var7 = NULL;
    if (getVerticalOffset(self, var1, var2, var3, var4, var5) == 1) {
        var7 = openPoint(self, var2, var3, var4);
    }
    if (var7 == NULL && var6 > 0 &&
        getVerticalOffset(self, var1, var2, var3 + var6, var4, var5) == 1) {
        var7 = openPoint(self, var2, var3 + var6, var4);
        var3 += var6;
    }
    if (var7 != NULL) {
        int var8 = 0;
        int var9 = 0;
        while (var3 > 0) {
            var9 = getVerticalOffset(self, var1, var2, var3 - 1, var4, var5);
            if (var9 != 1)
                break;
            ++var8;
            if (var8 >= 4)
                return NULL;
            --var3;
            if (var3 > 0) {
                var7 = openPoint(self, var2, var3, var4);
            }
        }
        if (var9 == -2)
            return NULL;
    }
    return var7;
}

static int findPathOptions(Pathfinder *self, Entity *var1, PathPoint *var2, PathPoint *var3,
                           PathPoint *var4, float var5) {
    int var6 = 0;
    int var7 = 0;
    if (getVerticalOffset(self, var1, var2->xCoord, var2->yCoord + 1, var2->zCoord, var3) == 1) {
        var7 = 1;
    }
    PathPoint *var8 =
        getSafePoint(self, var1, var2->xCoord, var2->yCoord, var2->zCoord + 1, var3, var7);
    PathPoint *var9 =
        getSafePoint(self, var1, var2->xCoord - 1, var2->yCoord, var2->zCoord, var3, var7);
    PathPoint *var10 =
        getSafePoint(self, var1, var2->xCoord + 1, var2->yCoord, var2->zCoord, var3, var7);
    PathPoint *var11 =
        getSafePoint(self, var1, var2->xCoord, var2->yCoord, var2->zCoord - 1, var3, var7);
    if (var8 != NULL && !var8->isFirst && PathPoint_distanceTo(var8, var4) < var5)
        self->pathOptions[var6++] = var8;
    if (var9 != NULL && !var9->isFirst && PathPoint_distanceTo(var9, var4) < var5)
        self->pathOptions[var6++] = var9;
    if (var10 != NULL && !var10->isFirst && PathPoint_distanceTo(var10, var4) < var5)
        self->pathOptions[var6++] = var10;
    if (var11 != NULL && !var11->isFirst && PathPoint_distanceTo(var11, var4) < var5)
        self->pathOptions[var6++] = var11;
    return var6;
}

static PathEntity *createEntityPath(PathPoint *var1, PathPoint *var2) {
    (void)var1;
    int var3 = 1;
    PathPoint *var4;
    for (var4 = var2; var4->previous != NULL; var4 = var4->previous) {
        ++var3;
    }

    PathPoint **var5 = (PathPoint **)calloc(var3, sizeof(PathPoint *));
    var4 = var2;
    int len = var3;
    --var3;
    for (var5[var3] = var2; var4->previous != NULL; var5[var3] = var4) {
        var4 = var4->previous;
        --var3;
    }

    for (int i = 0; i < len; i++) {
        PathPoint *src = var5[i];
        var5[i] = PathPoint_create(src->xCoord, src->yCoord, src->zCoord);
    }
    return PathEntity_create(var5, len);
}

static PathEntity *addToPath(Pathfinder *self, Entity *var1, PathPoint *var2, PathPoint *var3,
                             PathPoint *var4, float var5) {
    var2->totalPathDistance = 0.0f;
    var2->distanceToNext = PathPoint_distanceTo(var2, var3);
    var2->distanceToTarget = var2->distanceToNext;
    Path_clearPath(self->path);
    Path_addPoint(self->path, var2);
    PathPoint *var6 = var2;
    while (!Path_isPathEmpty(self->path)) {
        PathPoint *var7 = Path_dequeue(self->path);
        if (PathPoint_equals(var7, var3)) {
            return createEntityPath(var2, var3);
        }
        if (PathPoint_distanceTo(var7, var3) < PathPoint_distanceTo(var6, var3)) {
            var6 = var7;
        }
        var7->isFirst = 1;
        int var8 = findPathOptions(self, var1, var7, var4, var3, var5);
        for (int var9 = 0; var9 < var8; ++var9) {
            PathPoint *var10 = self->pathOptions[var9];
            float var11 = var7->totalPathDistance + PathPoint_distanceTo(var7, var10);
            if (!PathPoint_isAssigned(var10) || var11 < var10->totalPathDistance) {
                var10->previous = var7;
                var10->totalPathDistance = var11;
                var10->distanceToNext = PathPoint_distanceTo(var10, var3);
                if (PathPoint_isAssigned(var10)) {
                    Path_changeDistance(self->path, var10,
                                        var10->totalPathDistance + var10->distanceToNext);
                } else {
                    var10->distanceToTarget = var10->totalPathDistance + var10->distanceToNext;
                    Path_addPoint(self->path, var10);
                }
            }
        }
    }
    if (var6 == var2)
        return NULL;
    return createEntityPath(var2, var6);
}

static PathEntity *createEntityPathTo_d(Pathfinder *self, Entity *var1, double var2, double var4,
                                        double var6, float var8) {
    Path_clearPath(self->path);
    freePointMap(self);
    PathPoint *var9 = openPoint(self, MathHelper_floor_double(var1->boundingBox.minX),
                                MathHelper_floor_double(var1->boundingBox.minY),
                                MathHelper_floor_double(var1->boundingBox.minZ));
    PathPoint *var10 = openPoint(self, MathHelper_floor_double(var2 - (double)(var1->width / 2.0f)),
                                 MathHelper_floor_double(var4),
                                 MathHelper_floor_double(var6 - (double)(var1->width / 2.0f)));
    PathPoint *var11 = PathPoint_create(MathHelper_floor_float(var1->width + 1.0f),
                                        MathHelper_floor_float(var1->height + 1.0f),
                                        MathHelper_floor_float(var1->width + 1.0f));
    PathEntity *var12 = addToPath(self, var1, var9, var10, var11, var8);
    free(var11);
    return var12;
}

PathEntity *Pathfinder_createEntityPathToEntity(Pathfinder *self, Entity *var1, Entity *var2,
                                                float var3) {
    return createEntityPathTo_d(self, var1, var2->posX, var2->boundingBox.minY, var2->posZ, var3);
}

PathEntity *Pathfinder_createEntityPathToCoords(Pathfinder *self, Entity *var1, int var2, int var3,
                                                int var4, float var5) {
    return createEntityPathTo_d(self, var1, (double)((float)var2 + 0.5f),
                                (double)((float)var3 + 0.5f), (double)((float)var4 + 0.5f), var5);
}
