#include "ModelBase.h"

void ModelBase_render(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                      float var6) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    (void)var5;
    (void)var6;
}

void ModelBase_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                 float var5, float var6) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    (void)var5;
    (void)var6;
}

void ModelBase_setLivingAnimations(ModelBase *self, struct EntityLiving *var1, float var2,
                                   float var3, float var4) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
}

ModelBase_vtable ModelBase_defaultVtable = {
    ModelBase_render,
    ModelBase_setRotationAngles,
    ModelBase_setLivingAnimations,
};
