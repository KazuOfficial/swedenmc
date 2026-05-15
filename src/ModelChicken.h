#ifndef MODEL_CHICKEN_H
#define MODEL_CHICKEN_H

#include "ModelBase.h"
#include "ModelRenderer.h"

typedef struct ModelChicken {
    ModelBase base;
    ModelRenderer head;
    ModelRenderer body;
    ModelRenderer rightLeg;
    ModelRenderer leftLeg;
    ModelRenderer rightWing;
    ModelRenderer leftWing;
    ModelRenderer bill;
    ModelRenderer chin;
} ModelChicken;

extern ModelBase_vtable ModelChicken_defaultVtable;

void ModelChicken_init(ModelChicken *self);
void ModelChicken_destroy(ModelChicken *self);
void ModelChicken_render(ModelBase *self, float var1, float var2, float var3, float var4,
                         float var5, float var6);
void ModelChicken_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                    float var5, float var6);

#endif
