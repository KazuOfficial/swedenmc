#include "MovingObjectPosition.h"
#include "Entity.h"
#include <stdlib.h>

MovingObjectPosition *MovingObjectPosition_create(int x, int y, int z, int side, Vec3D *hitVec) {
    MovingObjectPosition *self = (MovingObjectPosition *)calloc(1, sizeof(MovingObjectPosition));
    self->typeOfHit = EnumMovingObjectType_TILE;
    self->blockX = x;
    self->blockY = y;
    self->blockZ = z;
    self->sideHit = side;
    self->hitVec = Vec3D_createVector(hitVec->xCoord, hitVec->yCoord, hitVec->zCoord);
    self->entityHit = NULL;
    return self;
}

MovingObjectPosition *MovingObjectPosition_createEntity(struct Entity *entity) {
    MovingObjectPosition *self = (MovingObjectPosition *)calloc(1, sizeof(MovingObjectPosition));
    self->typeOfHit = EnumMovingObjectType_ENTITY;
    self->entityHit = entity;
    self->hitVec = Vec3D_createVector(entity->posX, entity->posY, entity->posZ);
    self->blockX = 0;
    self->blockY = 0;
    self->blockZ = 0;
    self->sideHit = 0;
    return self;
}

void MovingObjectPosition_free(MovingObjectPosition *self) { free(self); }
