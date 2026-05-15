#ifndef MODEL_COW_H
#define MODEL_COW_H

#include "ModelQuadruped.h"

typedef struct ModelCow {
    ModelQuadruped base;
    ModelRenderer udders;
    ModelRenderer horn1;
    ModelRenderer horn2;
} ModelCow;

extern ModelQuadruped_vtable ModelCow_defaultVtable;

void ModelCow_init(ModelCow *self);
void ModelCow_destroy(ModelCow *self);
void ModelCow_render(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                     float var6);
void ModelCow_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                float var5, float var6);

#endif
