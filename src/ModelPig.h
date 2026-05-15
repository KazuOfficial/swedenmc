#ifndef MODEL_PIG_H
#define MODEL_PIG_H

#include "ModelQuadruped.h"

typedef struct ModelPig {
    ModelQuadruped base;
} ModelPig;

void ModelPig_init(ModelPig *self);
void ModelPig_initF(ModelPig *self, float var1);

#endif
