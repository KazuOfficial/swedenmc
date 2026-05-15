#include "ModelSpider.h"
#include "MathHelper.h"
#include <math.h>

ModelBase_vtable ModelSpider_defaultVtable = {ModelSpider_render, ModelSpider_setRotationAngles,
                                              ModelBase_setLivingAnimations};

void ModelSpider_init(ModelSpider *self) {
    self->base.vtable = &ModelSpider_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;

    float var1 = 0.0F;
    int var2 = 15;

    ModelRenderer_init(&self->spiderHead, 32, 4);
    ModelRenderer_addBoxExpanded(&self->spiderHead, -4.0F, -4.0F, -8.0F, 8, 8, 8, var1);
    ModelRenderer_setRotationPoint(&self->spiderHead, 0.0F, (float)(0 + var2), -3.0F);

    ModelRenderer_init(&self->spiderNeck, 0, 0);
    ModelRenderer_addBoxExpanded(&self->spiderNeck, -3.0F, -3.0F, -3.0F, 6, 6, 6, var1);
    ModelRenderer_setRotationPoint(&self->spiderNeck, 0.0F, (float)var2, 0.0F);

    ModelRenderer_init(&self->spiderBody, 0, 12);
    ModelRenderer_addBoxExpanded(&self->spiderBody, -5.0F, -4.0F, -6.0F, 10, 8, 12, var1);
    ModelRenderer_setRotationPoint(&self->spiderBody, 0.0F, (float)(0 + var2), 9.0F);

    ModelRenderer_init(&self->spiderLeg1, 18, 0);
    ModelRenderer_addBoxExpanded(&self->spiderLeg1, -15.0F, -1.0F, -1.0F, 16, 2, 2, var1);
    ModelRenderer_setRotationPoint(&self->spiderLeg1, -4.0F, (float)(0 + var2), 2.0F);

    ModelRenderer_init(&self->spiderLeg2, 18, 0);
    ModelRenderer_addBoxExpanded(&self->spiderLeg2, -1.0F, -1.0F, -1.0F, 16, 2, 2, var1);
    ModelRenderer_setRotationPoint(&self->spiderLeg2, 4.0F, (float)(0 + var2), 2.0F);

    ModelRenderer_init(&self->spiderLeg3, 18, 0);
    ModelRenderer_addBoxExpanded(&self->spiderLeg3, -15.0F, -1.0F, -1.0F, 16, 2, 2, var1);
    ModelRenderer_setRotationPoint(&self->spiderLeg3, -4.0F, (float)(0 + var2), 1.0F);

    ModelRenderer_init(&self->spiderLeg4, 18, 0);
    ModelRenderer_addBoxExpanded(&self->spiderLeg4, -1.0F, -1.0F, -1.0F, 16, 2, 2, var1);
    ModelRenderer_setRotationPoint(&self->spiderLeg4, 4.0F, (float)(0 + var2), 1.0F);

    ModelRenderer_init(&self->spiderLeg5, 18, 0);
    ModelRenderer_addBoxExpanded(&self->spiderLeg5, -15.0F, -1.0F, -1.0F, 16, 2, 2, var1);
    ModelRenderer_setRotationPoint(&self->spiderLeg5, -4.0F, (float)(0 + var2), 0.0F);

    ModelRenderer_init(&self->spiderLeg6, 18, 0);
    ModelRenderer_addBoxExpanded(&self->spiderLeg6, -1.0F, -1.0F, -1.0F, 16, 2, 2, var1);
    ModelRenderer_setRotationPoint(&self->spiderLeg6, 4.0F, (float)(0 + var2), 0.0F);

    ModelRenderer_init(&self->spiderLeg7, 18, 0);
    ModelRenderer_addBoxExpanded(&self->spiderLeg7, -15.0F, -1.0F, -1.0F, 16, 2, 2, var1);
    ModelRenderer_setRotationPoint(&self->spiderLeg7, -4.0F, (float)(0 + var2), -1.0F);

    ModelRenderer_init(&self->spiderLeg8, 18, 0);
    ModelRenderer_addBoxExpanded(&self->spiderLeg8, -1.0F, -1.0F, -1.0F, 16, 2, 2, var1);
    ModelRenderer_setRotationPoint(&self->spiderLeg8, 4.0F, (float)(0 + var2), -1.0F);
}

void ModelSpider_destroy(ModelSpider *self) {
    ModelRenderer_destroy(&self->spiderHead);
    ModelRenderer_destroy(&self->spiderNeck);
    ModelRenderer_destroy(&self->spiderBody);
    ModelRenderer_destroy(&self->spiderLeg1);
    ModelRenderer_destroy(&self->spiderLeg2);
    ModelRenderer_destroy(&self->spiderLeg3);
    ModelRenderer_destroy(&self->spiderLeg4);
    ModelRenderer_destroy(&self->spiderLeg5);
    ModelRenderer_destroy(&self->spiderLeg6);
    ModelRenderer_destroy(&self->spiderLeg7);
    ModelRenderer_destroy(&self->spiderLeg8);
}

void ModelSpider_render(ModelBase *base, float var1, float var2, float var3, float var4, float var5,
                        float var6) {
    ModelSpider *self = (ModelSpider *)base;
    ModelSpider_setRotationAngles(base, var1, var2, var3, var4, var5, var6);
    ModelRenderer_render(&self->spiderHead, var6);
    ModelRenderer_render(&self->spiderNeck, var6);
    ModelRenderer_render(&self->spiderBody, var6);
    ModelRenderer_render(&self->spiderLeg1, var6);
    ModelRenderer_render(&self->spiderLeg2, var6);
    ModelRenderer_render(&self->spiderLeg3, var6);
    ModelRenderer_render(&self->spiderLeg4, var6);
    ModelRenderer_render(&self->spiderLeg5, var6);
    ModelRenderer_render(&self->spiderLeg6, var6);
    ModelRenderer_render(&self->spiderLeg7, var6);
    ModelRenderer_render(&self->spiderLeg8, var6);
}

void ModelSpider_setRotationAngles(ModelBase *base, float var1, float var2, float var3, float var4,
                                   float var5, float var6) {
    ModelSpider *self = (ModelSpider *)base;
    (void)var3;
    (void)var6;
    self->spiderHead.rotateAngleY = var4 / (180.0F / (float)M_PI);
    self->spiderHead.rotateAngleX = var5 / (180.0F / (float)M_PI);

    float var7 = (float)M_PI * 0.25F;
    self->spiderLeg1.rotateAngleZ = -var7;
    self->spiderLeg2.rotateAngleZ = var7;
    self->spiderLeg3.rotateAngleZ = -var7 * 0.74F;
    self->spiderLeg4.rotateAngleZ = var7 * 0.74F;
    self->spiderLeg5.rotateAngleZ = -var7 * 0.74F;
    self->spiderLeg6.rotateAngleZ = var7 * 0.74F;
    self->spiderLeg7.rotateAngleZ = -var7;
    self->spiderLeg8.rotateAngleZ = var7;

    float var8 = -0.0F;
    float var9 = (float)M_PI * 0.125F;
    self->spiderLeg1.rotateAngleY = var9 * 2.0F + var8;
    self->spiderLeg2.rotateAngleY = -var9 * 2.0F - var8;
    self->spiderLeg3.rotateAngleY = var9 * 1.0F + var8;
    self->spiderLeg4.rotateAngleY = -var9 * 1.0F - var8;
    self->spiderLeg5.rotateAngleY = -var9 * 1.0F + var8;
    self->spiderLeg6.rotateAngleY = var9 * 1.0F - var8;
    self->spiderLeg7.rotateAngleY = -var9 * 2.0F + var8;
    self->spiderLeg8.rotateAngleY = var9 * 2.0F - var8;

    float var10 = -(MathHelper_cos(var1 * 0.6662F * 2.0F + 0.0F) * 0.4F) * var2;
    float var11 = -(MathHelper_cos(var1 * 0.6662F * 2.0F + (float)M_PI) * 0.4F) * var2;
    float var12 = -(MathHelper_cos(var1 * 0.6662F * 2.0F + (float)M_PI * 0.5F) * 0.4F) * var2;
    float var13 =
        -(MathHelper_cos(var1 * 0.6662F * 2.0F + (float)M_PI * 3.0F / 2.0F) * 0.4F) * var2;
    float var14 = (float)fabs(MathHelper_sin(var1 * 0.6662F + 0.0F) * 0.4F) * var2;
    float var15 = (float)fabs(MathHelper_sin(var1 * 0.6662F + (float)M_PI) * 0.4F) * var2;
    float var16 = (float)fabs(MathHelper_sin(var1 * 0.6662F + (float)M_PI * 0.5F) * 0.4F) * var2;
    float var17 =
        (float)fabs(MathHelper_sin(var1 * 0.6662F + (float)M_PI * 3.0F / 2.0F) * 0.4F) * var2;

    self->spiderLeg1.rotateAngleY += var10;
    self->spiderLeg2.rotateAngleY += -var10;
    self->spiderLeg3.rotateAngleY += var11;
    self->spiderLeg4.rotateAngleY += -var11;
    self->spiderLeg5.rotateAngleY += var12;
    self->spiderLeg6.rotateAngleY += -var12;
    self->spiderLeg7.rotateAngleY += var13;
    self->spiderLeg8.rotateAngleY += -var13;
    self->spiderLeg1.rotateAngleZ += var14;
    self->spiderLeg2.rotateAngleZ += -var14;
    self->spiderLeg3.rotateAngleZ += var15;
    self->spiderLeg4.rotateAngleZ += -var15;
    self->spiderLeg5.rotateAngleZ += var16;
    self->spiderLeg6.rotateAngleZ += -var16;
    self->spiderLeg7.rotateAngleZ += var17;
    self->spiderLeg8.rotateAngleZ += -var17;
}
