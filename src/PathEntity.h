#ifndef PATH_ENTITY_H
#define PATH_ENTITY_H

#include "PathPoint.h"
#include "Vec3D.h"

struct Entity;

typedef struct PathEntity {

    PathPoint **points;

    int pathLength;

    int pathIndex;
} PathEntity;

PathEntity *PathEntity_create(PathPoint **var1, int len);
void PathEntity_destroy(PathEntity *self);

void PathEntity_incrementPathIndex(PathEntity *self);

int PathEntity_isFinished(PathEntity *self);

PathPoint *PathEntity_func_22328_c(PathEntity *self);

Vec3D *PathEntity_getPosition(PathEntity *self, struct Entity *var1);

#endif
