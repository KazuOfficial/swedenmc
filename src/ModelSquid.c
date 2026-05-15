#include "ModelSquid.h"
#include <math.h>

ModelBase_vtable ModelSquid_defaultVtable = {ModelSquid_render, ModelSquid_setRotationAngles,
                                             ModelBase_setLivingAnimations};

void ModelSquid_init(ModelSquid *self) {
    self->base.vtable = &ModelSquid_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;

    int var1 = -16;
    ModelRenderer_init(&self->squidBody, 0, 0);
    ModelRenderer_addBox(&self->squidBody, -6.0F, -8.0F, -6.0F, 12, 16, 12);
    self->squidBody.rotationPointY += (float)(24 + var1);

    for (int var2 = 0; var2 < 8; ++var2) {
        ModelRenderer_init(&self->squidTentacles[var2], 48, 0);
        double var3 = (double)var2 * M_PI * 2.0 / (double)8;
        float var5 = (float)cos(var3) * 5.0F;
        float var6 = (float)sin(var3) * 5.0F;
        ModelRenderer_addBox(&self->squidTentacles[var2], -1.0F, 0.0F, -1.0F, 2, 18, 2);
        self->squidTentacles[var2].rotationPointX = var5;
        self->squidTentacles[var2].rotationPointZ = var6;
        self->squidTentacles[var2].rotationPointY = (float)(31 + var1);

        var3 = (double)var2 * M_PI * -2.0 / (double)8 + M_PI * 0.5;
        self->squidTentacles[var2].rotateAngleY = (float)var3;
    }
}

void ModelSquid_destroy(ModelSquid *self) {
    ModelRenderer_destroy(&self->squidBody);
    for (int i = 0; i < 8; i++)
        ModelRenderer_destroy(&self->squidTentacles[i]);
}

void ModelSquid_render(ModelBase *base, float var1, float var2, float var3, float var4, float var5,
                       float var6) {
    ModelSquid *self = (ModelSquid *)base;
    ModelSquid_setRotationAngles(base, var1, var2, var3, var4, var5, var6);
    ModelRenderer_render(&self->squidBody, var6);
    for (int var7 = 0; var7 < 8; ++var7) {
        ModelRenderer_render(&self->squidTentacles[var7], var6);
    }
}

void ModelSquid_setRotationAngles(ModelBase *base, float var1, float var2, float var3, float var4,
                                  float var5, float var6) {
    ModelSquid *self = (ModelSquid *)base;
    (void)var1;
    (void)var2;
    (void)var4;
    (void)var5;
    (void)var6;
    for (int var7 = 0; var7 < 8; ++var7) {
        self->squidTentacles[var7].rotateAngleX = var3;
    }
}
