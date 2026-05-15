#include "Path.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define PATH_INITIAL_SIZE 1024

Path *Path_create(void) {
    Path *self = (Path *)calloc(1, sizeof(Path));
    self->pathPoints = (PathPoint **)calloc(PATH_INITIAL_SIZE, sizeof(PathPoint *));
    self->pathPointsLen = PATH_INITIAL_SIZE;
    self->count = 0;
    return self;
}

void Path_destroy(Path *self) {
    free(self->pathPoints);
    free(self);
}

static void sortBack(Path *self, int var1) {
    PathPoint *var2 = self->pathPoints[var1];
    float var3 = var2->distanceToTarget;
    while (var1 > 0) {
        int var4 = (var1 - 1) >> 1;
        PathPoint *var5 = self->pathPoints[var4];
        if (var3 >= var5->distanceToTarget)
            break;
        self->pathPoints[var1] = var5;
        var5->index = var1;
        var1 = var4;
    }
    self->pathPoints[var1] = var2;
    var2->index = var1;
}

static void sortForward(Path *self, int var1) {
    PathPoint *var2 = self->pathPoints[var1];
    float var3 = var2->distanceToTarget;
    while (1) {
        int var4 = 1 + (var1 << 1);
        int var5 = var4 + 1;
        if (var4 >= self->count)
            break;
        PathPoint *var6 = self->pathPoints[var4];
        float var7 = var6->distanceToTarget;
        PathPoint *var8;
        float var9;
        if (var5 >= self->count) {
            var8 = NULL;
            var9 = INFINITY;
        } else {
            var8 = self->pathPoints[var5];
            var9 = var8->distanceToTarget;
        }
        if (var7 < var9) {
            if (var7 >= var3)
                break;
            self->pathPoints[var1] = var6;
            var6->index = var1;
            var1 = var4;
        } else {
            if (var9 >= var3)
                break;
            self->pathPoints[var1] = var8;
            var8->index = var1;
            var1 = var5;
        }
    }
    self->pathPoints[var1] = var2;
    var2->index = var1;
}

PathPoint *Path_addPoint(Path *self, PathPoint *var1) {
    if (var1->index >= 0) {
        fprintf(stderr, "IllegalStateException: OW KNOWS!\n");
        abort();
    }
    if (self->count == self->pathPointsLen) {
        int newLen = self->count << 1;
        PathPoint **var2 = (PathPoint **)calloc(newLen, sizeof(PathPoint *));
        memcpy(var2, self->pathPoints, self->count * sizeof(PathPoint *));
        free(self->pathPoints);
        self->pathPoints = var2;
        self->pathPointsLen = newLen;
    }
    self->pathPoints[self->count] = var1;
    var1->index = self->count;
    sortBack(self, self->count++);
    return var1;
}

void Path_clearPath(Path *self) { self->count = 0; }

PathPoint *Path_dequeue(Path *self) {
    PathPoint *var1 = self->pathPoints[0];
    self->pathPoints[0] = self->pathPoints[--self->count];
    self->pathPoints[self->count] = NULL;
    if (self->count > 0) {
        sortForward(self, 0);
    }
    var1->index = -1;
    return var1;
}

void Path_changeDistance(Path *self, PathPoint *var1, float var2) {
    float var3 = var1->distanceToTarget;
    var1->distanceToTarget = var2;
    if (var2 < var3) {
        sortBack(self, var1->index);
    } else {
        sortForward(self, var1->index);
    }
}

int Path_isPathEmpty(Path *self) { return self->count == 0; }
