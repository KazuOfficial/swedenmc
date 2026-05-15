#ifndef MODEL_WOLF_H
#define MODEL_WOLF_H

#include "ModelBase.h"
#include "ModelRenderer.h"

struct EntityLiving;

typedef struct ModelWolf {
    ModelBase base;
    ModelRenderer wolfHeadMain;
    ModelRenderer wolfBody;
    ModelRenderer wolfLeg1;
    ModelRenderer wolfLeg2;
    ModelRenderer wolfLeg3;
    ModelRenderer wolfLeg4;
    ModelRenderer wolfRightEar;
    ModelRenderer wolfLeftEar;
    ModelRenderer wolfSnout;
    ModelRenderer wolfTail;
    ModelRenderer wolfMane;
} ModelWolf;

extern ModelBase_vtable ModelWolf_defaultVtable;

void ModelWolf_init(ModelWolf *self);
void ModelWolf_destroy(ModelWolf *self);
void ModelWolf_render(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                      float var6);
void ModelWolf_setLivingAnimations(ModelBase *self, struct EntityLiving *var1, float var2,
                                   float var3, float var4);
void ModelWolf_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                 float var5, float var6);

#endif
