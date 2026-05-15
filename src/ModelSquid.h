#ifndef MODEL_SQUID_H
#define MODEL_SQUID_H

#include "ModelBase.h"
#include "ModelRenderer.h"

typedef struct ModelSquid {
    ModelBase base;
    ModelRenderer squidBody;
    ModelRenderer squidTentacles[8];
} ModelSquid;

extern ModelBase_vtable ModelSquid_defaultVtable;

void ModelSquid_init(ModelSquid *self);
void ModelSquid_destroy(ModelSquid *self);
void ModelSquid_render(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                       float var6);
void ModelSquid_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                  float var5, float var6);

#endif
