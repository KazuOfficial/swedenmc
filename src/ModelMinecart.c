#include "ModelMinecart.h"
#include <math.h>

ModelBase_vtable ModelMinecart_defaultVtable = {ModelMinecart_render, ModelBase_setRotationAngles,
                                                ModelBase_setLivingAnimations};

void ModelMinecart_init(ModelMinecart *self) {
    self->base.vtable = &ModelMinecart_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;

    ModelRenderer_init(&self->sideModels[0], 0, 10);
    ModelRenderer_init(&self->sideModels[1], 0, 0);
    ModelRenderer_init(&self->sideModels[2], 0, 0);
    ModelRenderer_init(&self->sideModels[3], 0, 0);
    ModelRenderer_init(&self->sideModels[4], 0, 0);
    ModelRenderer_init(&self->sideModels[5], 44, 10);

    int var1 = 20;
    int var2 = 8;
    int var3 = 16;
    int var4 = 4;

    ModelRenderer_addBoxExpanded(&self->sideModels[0], (float)(-var1 / 2), (float)(-var3 / 2),
                                 -1.0F, var1, var3, 2, 0.0F);
    ModelRenderer_setRotationPoint(&self->sideModels[0], 0.0F, (float)(0 + var4), 0.0F);

    ModelRenderer_addBoxExpanded(&self->sideModels[5], (float)(-var1 / 2 + 1),
                                 (float)(-var3 / 2 + 1), -1.0F, var1 - 2, var3 - 2, 1, 0.0F);
    ModelRenderer_setRotationPoint(&self->sideModels[5], 0.0F, (float)(0 + var4), 0.0F);

    ModelRenderer_addBoxExpanded(&self->sideModels[1], (float)(-var1 / 2 + 2), (float)(-var2 - 1),
                                 -1.0F, var1 - 4, var2, 2, 0.0F);
    ModelRenderer_setRotationPoint(&self->sideModels[1], (float)(-var1 / 2 + 1), (float)(0 + var4),
                                   0.0F);

    ModelRenderer_addBoxExpanded(&self->sideModels[2], (float)(-var1 / 2 + 2), (float)(-var2 - 1),
                                 -1.0F, var1 - 4, var2, 2, 0.0F);
    ModelRenderer_setRotationPoint(&self->sideModels[2], (float)(var1 / 2 - 1), (float)(0 + var4),
                                   0.0F);

    ModelRenderer_addBoxExpanded(&self->sideModels[3], (float)(-var1 / 2 + 2), (float)(-var2 - 1),
                                 -1.0F, var1 - 4, var2, 2, 0.0F);
    ModelRenderer_setRotationPoint(&self->sideModels[3], 0.0F, (float)(0 + var4),
                                   (float)(-var3 / 2 + 1));

    ModelRenderer_addBoxExpanded(&self->sideModels[4], (float)(-var1 / 2 + 2), (float)(-var2 - 1),
                                 -1.0F, var1 - 4, var2, 2, 0.0F);
    ModelRenderer_setRotationPoint(&self->sideModels[4], 0.0F, (float)(0 + var4),
                                   (float)(var3 / 2 - 1));

    self->sideModels[0].rotateAngleX = (float)M_PI * 0.5F;
    self->sideModels[1].rotateAngleY = (float)M_PI * 3.0F / 2.0F;
    self->sideModels[2].rotateAngleY = (float)M_PI * 0.5F;
    self->sideModels[3].rotateAngleY = (float)M_PI;
    self->sideModels[5].rotateAngleX = (float)M_PI * -0.5F;
}

void ModelMinecart_render(ModelBase *base, float var1, float var2, float var3, float var4,
                          float var5, float var6) {
    ModelMinecart *self = (ModelMinecart *)base;
    (void)var1;
    (void)var2;
    (void)var4;
    (void)var5;
    self->sideModels[5].rotationPointY = 4.0F - var3;
    for (int var7 = 0; var7 < 6; ++var7) {
        ModelRenderer_render(&self->sideModels[var7], var6);
    }
}
