#ifndef MODEL_SKELETON_H
#define MODEL_SKELETON_H

#include "ModelZombie.h"

typedef struct ModelSkeleton {
    ModelZombie base;
} ModelSkeleton;

void ModelSkeleton_init(ModelSkeleton *self);

#endif
