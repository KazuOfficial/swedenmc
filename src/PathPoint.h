#ifndef PATH_POINT_H
#define PATH_POINT_H

typedef struct PathPoint PathPoint;
struct PathPoint {

    int xCoord;

    int yCoord;

    int zCoord;

    int hash;

    int index;

    float totalPathDistance;

    float distanceToNext;

    float distanceToTarget;

    PathPoint *previous;

    int isFirst;
};

PathPoint *PathPoint_create(int var1, int var2, int var3);

int PathPoint_func_22329_a(int var0, int var1, int var2);

float PathPoint_distanceTo(PathPoint *self, PathPoint *var1);

int PathPoint_equals(PathPoint *self, PathPoint *var1);

int PathPoint_hashCode(PathPoint *self);

int PathPoint_isAssigned(PathPoint *self);

char *PathPoint_toString(PathPoint *self);

#endif
