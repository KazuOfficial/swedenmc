#include "PathEntity.h"
#include "Entity.h"
#include "Vec3D.h"
#include <stdlib.h>

PathEntity *PathEntity_create(PathPoint **var1, int len) {
    PathEntity *self = (PathEntity *)calloc(1, sizeof(PathEntity));
    self->points = var1;
    self->pathLength = len;
    self->pathIndex = 0;
    return self;
}

void PathEntity_destroy(PathEntity *self) {
    if (!self)
        return;
    for (int i = 0; i < self->pathLength; i++)
        free(self->points[i]);
    free(self->points);
    free(self);
}

void PathEntity_incrementPathIndex(PathEntity *self) { ++self->pathIndex; }

int PathEntity_isFinished(PathEntity *self) { return self->pathIndex >= self->pathLength; }

PathPoint *PathEntity_func_22328_c(PathEntity *self) {
    return self->pathLength > 0 ? self->points[self->pathLength - 1] : NULL;
}

Vec3D *PathEntity_getPosition(PathEntity *self, struct Entity *var1) {
    double var2 =
        (double)self->points[self->pathIndex]->xCoord + (double)((int)(var1->width + 1.0f)) * 0.5;
    double var4 = (double)self->points[self->pathIndex]->yCoord;
    double var6 =
        (double)self->points[self->pathIndex]->zCoord + (double)((int)(var1->width + 1.0f)) * 0.5;
    return Vec3D_createVector(var2, var4, var6);
}
