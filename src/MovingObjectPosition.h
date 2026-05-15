#ifndef MOVING_OBJECT_POSITION_H
#define MOVING_OBJECT_POSITION_H

#include "Vec3D.h"
#include "EnumMovingObjectType.h"

struct Entity;

typedef struct {
    EnumMovingObjectType typeOfHit;
    int blockX;
    int blockY;
    int blockZ;
    int sideHit;
    Vec3D *hitVec;
    struct Entity *entityHit;
} MovingObjectPosition;

MovingObjectPosition *MovingObjectPosition_create(int x, int y, int z, int side, Vec3D *hitVec);

MovingObjectPosition *MovingObjectPosition_createEntity(struct Entity *entity);

void MovingObjectPosition_free(MovingObjectPosition *self);

#endif
