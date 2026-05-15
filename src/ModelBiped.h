#ifndef MODEL_BIPED_H
#define MODEL_BIPED_H

#include "ModelBase.h"
#include "ModelRenderer.h"

typedef struct ModelBiped ModelBiped;

typedef struct ModelBiped_vtable {
    ModelBase_vtable base;

    void (*renderEars)(ModelBiped *self, float var1);

    void (*renderCloak)(ModelBiped *self, float var1);
} ModelBiped_vtable;

struct ModelBiped {
    ModelBase base;

    ModelRenderer bipedHead;
    ModelRenderer bipedHeadwear;
    ModelRenderer bipedBody;
    ModelRenderer bipedRightArm;
    ModelRenderer bipedLeftArm;
    ModelRenderer bipedRightLeg;
    ModelRenderer bipedLeftLeg;
    ModelRenderer bipedEars;
    ModelRenderer bipedCloak;

    int field_1279_h;
    int field_1278_i;
    int isSneak;
};

extern ModelBiped_vtable ModelBiped_defaultVtable;

void ModelBiped_init0(ModelBiped *self);

void ModelBiped_init1(ModelBiped *self, float var1);

void ModelBiped_init(ModelBiped *self, float var1, float var2);
void ModelBiped_destroy(ModelBiped *self);

void ModelBiped_render(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                       float var6);

void ModelBiped_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                  float var5, float var6);

void ModelBiped_renderEars(ModelBiped *self, float var1);

void ModelBiped_renderCloak(ModelBiped *self, float var1);

#endif
