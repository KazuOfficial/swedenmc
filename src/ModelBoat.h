#ifndef MODEL_BOAT_H
#define MODEL_BOAT_H

#include "ModelBase.h"
#include "ModelRenderer.h"

typedef struct ModelBoat {
    ModelBase base;
    ModelRenderer boatSides[5];
} ModelBoat;

extern ModelBase_vtable ModelBoat_defaultVtable;

void ModelBoat_init(ModelBoat *self);
void ModelBoat_render(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                      float var6);

#endif
