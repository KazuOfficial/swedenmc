#ifndef MODEL_QUADRUPED_H
#define MODEL_QUADRUPED_H

#include "ModelBase.h"
#include "ModelRenderer.h"

typedef struct ModelQuadruped ModelQuadruped;

typedef struct ModelQuadruped_vtable {
    ModelBase_vtable base;
} ModelQuadruped_vtable;

struct ModelQuadruped {
    ModelBase base;

    ModelRenderer head;

    ModelRenderer body;

    ModelRenderer leg1;
    ModelRenderer leg2;
    ModelRenderer leg3;
    ModelRenderer leg4;
};

extern ModelQuadruped_vtable ModelQuadruped_defaultVtable;

void ModelQuadruped_init(ModelQuadruped *self, int var1, float var2);
void ModelQuadruped_destroy(ModelQuadruped *self);

void ModelQuadruped_render(ModelBase *self, float var1, float var2, float var3, float var4,
                           float var5, float var6);

void ModelQuadruped_setRotationAngles(ModelBase *self, float var1, float var2, float var3,
                                      float var4, float var5, float var6);

#endif
