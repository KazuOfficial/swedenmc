#include "ModelPig.h"

void ModelPig_init(ModelPig *self) { ModelQuadruped_init(&self->base, 6, 0.0F); }

void ModelPig_initF(ModelPig *self, float var1) { ModelQuadruped_init(&self->base, 6, var1); }
