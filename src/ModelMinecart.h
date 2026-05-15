#ifndef MODEL_MINECART_H
#define MODEL_MINECART_H

#include "ModelBase.h"
#include "ModelRenderer.h"

typedef struct ModelMinecart {
    ModelBase base;
    ModelRenderer sideModels[7];
} ModelMinecart;

extern ModelBase_vtable ModelMinecart_defaultVtable;

void ModelMinecart_init(ModelMinecart *self);
void ModelMinecart_render(ModelBase *self, float var1, float var2, float var3, float var4,
                          float var5, float var6);

#endif
