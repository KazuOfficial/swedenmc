#ifndef MODEL_CREEPER_H
#define MODEL_CREEPER_H

#include "ModelBase.h"
#include "ModelRenderer.h"

typedef struct ModelCreeper {
    ModelBase base;
    ModelRenderer head;
    ModelRenderer field_1270_b;
    ModelRenderer body;
    ModelRenderer leg1;
    ModelRenderer leg2;
    ModelRenderer leg3;
    ModelRenderer leg4;
} ModelCreeper;

extern ModelBase_vtable ModelCreeper_defaultVtable;

void ModelCreeper_init(ModelCreeper *self);
void ModelCreeper_initF(ModelCreeper *self, float var1);
void ModelCreeper_destroy(ModelCreeper *self);
void ModelCreeper_render(ModelBase *self, float var1, float var2, float var3, float var4,
                         float var5, float var6);
void ModelCreeper_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                    float var5, float var6);

#endif
