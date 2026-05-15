#ifndef MODEL_SHEEP2_H
#define MODEL_SHEEP2_H

#include "ModelQuadruped.h"

typedef struct ModelSheep2 {
    ModelQuadruped base;
} ModelSheep2;

void ModelSheep2_init(ModelSheep2 *self);

#endif
