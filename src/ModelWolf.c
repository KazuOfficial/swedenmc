#include "Renderer.h"

#include "ModelWolf.h"
#include "EntityWolf.h"
#include "MathHelper.h"
#include <math.h>

ModelBase_vtable ModelWolf_defaultVtable = {ModelWolf_render, ModelWolf_setRotationAngles,
                                            ModelWolf_setLivingAnimations};

void ModelWolf_init(ModelWolf *self) {
    self->base.vtable = &ModelWolf_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;

    float var1 = 0.0F;
    float var2 = 13.5F;

    ModelRenderer_init(&self->wolfHeadMain, 0, 0);
    ModelRenderer_addBoxExpanded(&self->wolfHeadMain, -3.0F, -3.0F, -2.0F, 6, 6, 4, var1);
    ModelRenderer_setRotationPoint(&self->wolfHeadMain, -1.0F, var2, -7.0F);

    ModelRenderer_init(&self->wolfBody, 18, 14);
    ModelRenderer_addBoxExpanded(&self->wolfBody, -4.0F, -2.0F, -3.0F, 6, 9, 6, var1);
    ModelRenderer_setRotationPoint(&self->wolfBody, 0.0F, 14.0F, 2.0F);

    ModelRenderer_init(&self->wolfMane, 21, 0);
    ModelRenderer_addBoxExpanded(&self->wolfMane, -4.0F, -3.0F, -3.0F, 8, 6, 7, var1);
    ModelRenderer_setRotationPoint(&self->wolfMane, -1.0F, 14.0F, 2.0F);

    ModelRenderer_init(&self->wolfLeg1, 0, 18);
    ModelRenderer_addBoxExpanded(&self->wolfLeg1, -1.0F, 0.0F, -1.0F, 2, 8, 2, var1);
    ModelRenderer_setRotationPoint(&self->wolfLeg1, -2.5F, 16.0F, 7.0F);

    ModelRenderer_init(&self->wolfLeg2, 0, 18);
    ModelRenderer_addBoxExpanded(&self->wolfLeg2, -1.0F, 0.0F, -1.0F, 2, 8, 2, var1);
    ModelRenderer_setRotationPoint(&self->wolfLeg2, 0.5F, 16.0F, 7.0F);

    ModelRenderer_init(&self->wolfLeg3, 0, 18);
    ModelRenderer_addBoxExpanded(&self->wolfLeg3, -1.0F, 0.0F, -1.0F, 2, 8, 2, var1);
    ModelRenderer_setRotationPoint(&self->wolfLeg3, -2.5F, 16.0F, -4.0F);

    ModelRenderer_init(&self->wolfLeg4, 0, 18);
    ModelRenderer_addBoxExpanded(&self->wolfLeg4, -1.0F, 0.0F, -1.0F, 2, 8, 2, var1);
    ModelRenderer_setRotationPoint(&self->wolfLeg4, 0.5F, 16.0F, -4.0F);

    ModelRenderer_init(&self->wolfTail, 9, 18);
    ModelRenderer_addBoxExpanded(&self->wolfTail, -1.0F, 0.0F, -1.0F, 2, 8, 2, var1);
    ModelRenderer_setRotationPoint(&self->wolfTail, -1.0F, 12.0F, 8.0F);

    ModelRenderer_init(&self->wolfRightEar, 16, 14);
    ModelRenderer_addBoxExpanded(&self->wolfRightEar, -3.0F, -5.0F, 0.0F, 2, 2, 1, var1);
    ModelRenderer_setRotationPoint(&self->wolfRightEar, -1.0F, var2, -7.0F);

    ModelRenderer_init(&self->wolfLeftEar, 16, 14);
    ModelRenderer_addBoxExpanded(&self->wolfLeftEar, 1.0F, -5.0F, 0.0F, 2, 2, 1, var1);
    ModelRenderer_setRotationPoint(&self->wolfLeftEar, -1.0F, var2, -7.0F);

    ModelRenderer_init(&self->wolfSnout, 0, 10);
    ModelRenderer_addBoxExpanded(&self->wolfSnout, -2.0F, 0.0F, -5.0F, 3, 3, 4, var1);
    ModelRenderer_setRotationPoint(&self->wolfSnout, -0.5F, var2, -7.0F);
}

void ModelWolf_destroy(ModelWolf *self) {
    ModelRenderer_destroy(&self->wolfHeadMain);
    ModelRenderer_destroy(&self->wolfBody);
    ModelRenderer_destroy(&self->wolfMane);
    ModelRenderer_destroy(&self->wolfLeg1);
    ModelRenderer_destroy(&self->wolfLeg2);
    ModelRenderer_destroy(&self->wolfLeg3);
    ModelRenderer_destroy(&self->wolfLeg4);
    ModelRenderer_destroy(&self->wolfRightEar);
    ModelRenderer_destroy(&self->wolfLeftEar);
    ModelRenderer_destroy(&self->wolfSnout);
    ModelRenderer_destroy(&self->wolfTail);
}

void ModelWolf_render(ModelBase *base, float var1, float var2, float var3, float var4, float var5,
                      float var6) {
    ModelWolf *self = (ModelWolf *)base;

    ModelWolf_setRotationAngles(base, var1, var2, var3, var4, var5, var6);
    ModelRenderer_renderWithRotation(&self->wolfHeadMain, var6);
    ModelRenderer_render(&self->wolfBody, var6);
    ModelRenderer_render(&self->wolfLeg1, var6);
    ModelRenderer_render(&self->wolfLeg2, var6);
    ModelRenderer_render(&self->wolfLeg3, var6);
    ModelRenderer_render(&self->wolfLeg4, var6);
    ModelRenderer_renderWithRotation(&self->wolfRightEar, var6);
    ModelRenderer_renderWithRotation(&self->wolfLeftEar, var6);
    ModelRenderer_renderWithRotation(&self->wolfSnout, var6);
    ModelRenderer_renderWithRotation(&self->wolfTail, var6);
    ModelRenderer_render(&self->wolfMane, var6);
}

void ModelWolf_setLivingAnimations(ModelBase *base, struct EntityLiving *var1, float var2,
                                   float var3, float var4) {
    ModelWolf *self = (ModelWolf *)base;
    EntityWolf *var5 = (EntityWolf *)var1;

    if (EntityWolf_isWolfAngry(var5)) {
        self->wolfTail.rotateAngleY = 0.0F;
    } else {
        self->wolfTail.rotateAngleY = MathHelper_cos(var2 * 0.6662F) * 1.4F * var3;
    }

    if (EntityWolf_isWolfSitting(var5)) {
        ModelRenderer_setRotationPoint(&self->wolfMane, -1.0F, 16.0F, -3.0F);
        self->wolfMane.rotateAngleX = (float)M_PI * 0.4F;
        self->wolfMane.rotateAngleY = 0.0F;
        ModelRenderer_setRotationPoint(&self->wolfBody, 0.0F, 18.0F, 0.0F);
        self->wolfBody.rotateAngleX = (float)M_PI * 0.25F;
        ModelRenderer_setRotationPoint(&self->wolfTail, -1.0F, 21.0F, 6.0F);
        ModelRenderer_setRotationPoint(&self->wolfLeg1, -2.5F, 22.0F, 2.0F);
        self->wolfLeg1.rotateAngleX = (float)M_PI * 3.0F / 2.0F;
        ModelRenderer_setRotationPoint(&self->wolfLeg2, 0.5F, 22.0F, 2.0F);
        self->wolfLeg2.rotateAngleX = (float)M_PI * 3.0F / 2.0F;
        self->wolfLeg3.rotateAngleX = (float)M_PI * 1.85F;
        ModelRenderer_setRotationPoint(&self->wolfLeg3, -2.49F, 17.0F, -4.0F);
        self->wolfLeg4.rotateAngleX = (float)M_PI * 1.85F;
        ModelRenderer_setRotationPoint(&self->wolfLeg4, 0.51F, 17.0F, -4.0F);
    } else {
        ModelRenderer_setRotationPoint(&self->wolfBody, 0.0F, 14.0F, 2.0F);
        self->wolfBody.rotateAngleX = (float)M_PI * 0.5F;
        ModelRenderer_setRotationPoint(&self->wolfMane, -1.0F, 14.0F, -3.0F);
        self->wolfMane.rotateAngleX = self->wolfBody.rotateAngleX;
        ModelRenderer_setRotationPoint(&self->wolfTail, -1.0F, 12.0F, 8.0F);
        ModelRenderer_setRotationPoint(&self->wolfLeg1, -2.5F, 16.0F, 7.0F);
        ModelRenderer_setRotationPoint(&self->wolfLeg2, 0.5F, 16.0F, 7.0F);
        ModelRenderer_setRotationPoint(&self->wolfLeg3, -2.5F, 16.0F, -4.0F);
        ModelRenderer_setRotationPoint(&self->wolfLeg4, 0.5F, 16.0F, -4.0F);
        self->wolfLeg1.rotateAngleX = MathHelper_cos(var2 * 0.6662F) * 1.4F * var3;
        self->wolfLeg2.rotateAngleX = MathHelper_cos(var2 * 0.6662F + (float)M_PI) * 1.4F * var3;
        self->wolfLeg3.rotateAngleX = MathHelper_cos(var2 * 0.6662F + (float)M_PI) * 1.4F * var3;
        self->wolfLeg4.rotateAngleX = MathHelper_cos(var2 * 0.6662F) * 1.4F * var3;
    }

    float var6 =
        EntityWolf_getInterestedAngle(var5, var4) + EntityWolf_getShakeAngle(var5, var4, 0.0F);
    self->wolfHeadMain.rotateAngleZ = var6;
    self->wolfRightEar.rotateAngleZ = var6;
    self->wolfLeftEar.rotateAngleZ = var6;
    self->wolfSnout.rotateAngleZ = var6;
    self->wolfMane.rotateAngleZ = EntityWolf_getShakeAngle(var5, var4, -0.08F);
    self->wolfBody.rotateAngleZ = EntityWolf_getShakeAngle(var5, var4, -0.16F);
    self->wolfTail.rotateAngleZ = EntityWolf_getShakeAngle(var5, var4, -0.2F);

    if (EntityWolf_getWolfShaking(var5)) {

        float var7 = ((Entity *)var5)->vtable->getEntityBrightness((Entity *)var5, var4) *
                     EntityWolf_getShadingWhileShaking(var5, var4);
        R_color3f(var7, var7, var7);
    }
}

void ModelWolf_setRotationAngles(ModelBase *base, float var1, float var2, float var3, float var4,
                                 float var5, float var6) {
    ModelWolf *self = (ModelWolf *)base;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var6;

    self->wolfHeadMain.rotateAngleX = var5 / (180.0F / (float)M_PI);
    self->wolfHeadMain.rotateAngleY = var4 / (180.0F / (float)M_PI);
    self->wolfRightEar.rotateAngleY = self->wolfHeadMain.rotateAngleY;
    self->wolfRightEar.rotateAngleX = self->wolfHeadMain.rotateAngleX;
    self->wolfLeftEar.rotateAngleY = self->wolfHeadMain.rotateAngleY;
    self->wolfLeftEar.rotateAngleX = self->wolfHeadMain.rotateAngleX;
    self->wolfSnout.rotateAngleY = self->wolfHeadMain.rotateAngleY;
    self->wolfSnout.rotateAngleX = self->wolfHeadMain.rotateAngleX;
    self->wolfTail.rotateAngleX = var3;
}
