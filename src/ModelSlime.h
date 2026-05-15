#ifndef MODEL_SLIME_H
#define MODEL_SLIME_H

#include "ModelBase.h"
#include "ModelRenderer.h"

typedef struct ModelSlime {
    ModelBase base;
    ModelRenderer slimeBodies;
    ModelRenderer slimeRightEye;
    ModelRenderer slimeLeftEye;
    ModelRenderer slimeMouth;

    int hasEyes;
} ModelSlime;

extern ModelBase_vtable ModelSlime_defaultVtable;

void ModelSlime_init(ModelSlime *self, int var1);
void ModelSlime_destroy(ModelSlime *self);
void ModelSlime_render(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                       float var6);

#endif
