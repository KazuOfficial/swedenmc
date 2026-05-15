#ifndef MODEL_SHEEP1_H
#define MODEL_SHEEP1_H

#include "ModelQuadruped.h"

typedef struct ModelSheep1 {
    ModelQuadruped base;
} ModelSheep1;

void ModelSheep1_init(ModelSheep1 *self);

#endif
