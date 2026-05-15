#ifndef MODEL_ZOMBIE_H
#define MODEL_ZOMBIE_H

#include "ModelBiped.h"

typedef struct ModelZombie {
    ModelBiped base;
} ModelZombie;

extern ModelBiped_vtable ModelZombie_defaultVtable;

void ModelZombie_init(ModelZombie *self);

void ModelZombie_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                   float var5, float var6);

#endif
