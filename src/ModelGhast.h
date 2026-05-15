#ifndef MODEL_GHAST_H
#define MODEL_GHAST_H

#include "ModelBase.h"
#include "ModelRenderer.h"

typedef struct ModelGhast {
    ModelBase base;
    ModelRenderer body;
    ModelRenderer tentacles[9];
} ModelGhast;

extern ModelBase_vtable ModelGhast_defaultVtable;

void ModelGhast_init(ModelGhast *self);
void ModelGhast_destroy(ModelGhast *self);
void ModelGhast_render(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                       float var6);
void ModelGhast_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                  float var5, float var6);

#endif
