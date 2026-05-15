#ifndef PATH_H
#define PATH_H

#include "PathPoint.h"

typedef struct Path {

    PathPoint **pathPoints;
    int pathPointsLen;

    int count;
} Path;

Path *Path_create(void);
void Path_destroy(Path *self);

PathPoint *Path_addPoint(Path *self, PathPoint *var1);

void Path_clearPath(Path *self);

PathPoint *Path_dequeue(Path *self);

void Path_changeDistance(Path *self, PathPoint *var1, float var2);

int Path_isPathEmpty(Path *self);

#endif
