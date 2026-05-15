#include "ModelBiped.h"
#include "MathHelper.h"
#include <math.h>

ModelBiped_vtable ModelBiped_defaultVtable = {
    {ModelBiped_render, ModelBiped_setRotationAngles, ModelBase_setLivingAnimations},
    ModelBiped_renderEars,
    ModelBiped_renderCloak,
};

void ModelBiped_init0(ModelBiped *self) { ModelBiped_init1(self, 0.0F); }

void ModelBiped_init1(ModelBiped *self, float var1) { ModelBiped_init(self, var1, 0.0F); }

void ModelBiped_init(ModelBiped *self, float var1, float var2) {
    self->base.vtable = (ModelBase_vtable *)&ModelBiped_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;
    self->field_1279_h = 0;
    self->field_1278_i = 0;
    self->isSneak = 0;

    ModelRenderer_init(&self->bipedCloak, 0, 0);
    ModelRenderer_addBoxExpanded(&self->bipedCloak, -5.0F, 0.0F, -1.0F, 10, 16, 1, var1);

    ModelRenderer_init(&self->bipedEars, 24, 0);
    ModelRenderer_addBoxExpanded(&self->bipedEars, -3.0F, -6.0F, -1.0F, 6, 6, 1, var1);

    ModelRenderer_init(&self->bipedHead, 0, 0);
    ModelRenderer_addBoxExpanded(&self->bipedHead, -4.0F, -8.0F, -4.0F, 8, 8, 8, var1);
    ModelRenderer_setRotationPoint(&self->bipedHead, 0.0F, 0.0F + var2, 0.0F);

    ModelRenderer_init(&self->bipedHeadwear, 32, 0);
    ModelRenderer_addBoxExpanded(&self->bipedHeadwear, -4.0F, -8.0F, -4.0F, 8, 8, 8, var1 + 0.5F);
    ModelRenderer_setRotationPoint(&self->bipedHeadwear, 0.0F, 0.0F + var2, 0.0F);

    ModelRenderer_init(&self->bipedBody, 16, 16);
    ModelRenderer_addBoxExpanded(&self->bipedBody, -4.0F, 0.0F, -2.0F, 8, 12, 4, var1);
    ModelRenderer_setRotationPoint(&self->bipedBody, 0.0F, 0.0F + var2, 0.0F);

    ModelRenderer_init(&self->bipedRightArm, 40, 16);
    ModelRenderer_addBoxExpanded(&self->bipedRightArm, -3.0F, -2.0F, -2.0F, 4, 12, 4, var1);
    ModelRenderer_setRotationPoint(&self->bipedRightArm, -5.0F, 2.0F + var2, 0.0F);

    ModelRenderer_init(&self->bipedLeftArm, 40, 16);
    self->bipedLeftArm.mirror = 1;
    ModelRenderer_addBoxExpanded(&self->bipedLeftArm, -1.0F, -2.0F, -2.0F, 4, 12, 4, var1);
    ModelRenderer_setRotationPoint(&self->bipedLeftArm, 5.0F, 2.0F + var2, 0.0F);

    ModelRenderer_init(&self->bipedRightLeg, 0, 16);
    ModelRenderer_addBoxExpanded(&self->bipedRightLeg, -2.0F, 0.0F, -2.0F, 4, 12, 4, var1);
    ModelRenderer_setRotationPoint(&self->bipedRightLeg, -2.0F, 12.0F + var2, 0.0F);

    ModelRenderer_init(&self->bipedLeftLeg, 0, 16);
    self->bipedLeftLeg.mirror = 1;
    ModelRenderer_addBoxExpanded(&self->bipedLeftLeg, -2.0F, 0.0F, -2.0F, 4, 12, 4, var1);
    ModelRenderer_setRotationPoint(&self->bipedLeftLeg, 2.0F, 12.0F + var2, 0.0F);
}

void ModelBiped_destroy(ModelBiped *self) {
    ModelRenderer_destroy(&self->bipedHead);
    ModelRenderer_destroy(&self->bipedHeadwear);
    ModelRenderer_destroy(&self->bipedBody);
    ModelRenderer_destroy(&self->bipedRightArm);
    ModelRenderer_destroy(&self->bipedLeftArm);
    ModelRenderer_destroy(&self->bipedRightLeg);
    ModelRenderer_destroy(&self->bipedLeftLeg);
    ModelRenderer_destroy(&self->bipedEars);
    ModelRenderer_destroy(&self->bipedCloak);
}

void ModelBiped_render(ModelBase *base, float var1, float var2, float var3, float var4, float var5,
                       float var6) {
    ModelBiped *self = (ModelBiped *)base;
    self->base.vtable->setRotationAngles(base, var1, var2, var3, var4, var5, var6);
    ModelRenderer_render(&self->bipedHead, var6);
    ModelRenderer_render(&self->bipedBody, var6);
    ModelRenderer_render(&self->bipedRightArm, var6);
    ModelRenderer_render(&self->bipedLeftArm, var6);
    ModelRenderer_render(&self->bipedRightLeg, var6);
    ModelRenderer_render(&self->bipedLeftLeg, var6);
    ModelRenderer_render(&self->bipedHeadwear, var6);
}

void ModelBiped_setRotationAngles(ModelBase *base, float var1, float var2, float var3, float var4,
                                  float var5, float var6) {
    ModelBiped *self = (ModelBiped *)base;
    (void)var6;
    self->bipedHead.rotateAngleY = var4 / (180.0F / (float)M_PI);
    self->bipedHead.rotateAngleX = var5 / (180.0F / (float)M_PI);
    self->bipedHeadwear.rotateAngleY = self->bipedHead.rotateAngleY;
    self->bipedHeadwear.rotateAngleX = self->bipedHead.rotateAngleX;

    self->bipedRightArm.rotateAngleX =
        MathHelper_cos(var1 * 0.6662F + (float)M_PI) * 2.0F * var2 * 0.5F;
    self->bipedLeftArm.rotateAngleX = MathHelper_cos(var1 * 0.6662F) * 2.0F * var2 * 0.5F;
    self->bipedRightArm.rotateAngleZ = 0.0F;
    self->bipedLeftArm.rotateAngleZ = 0.0F;
    self->bipedRightLeg.rotateAngleX = MathHelper_cos(var1 * 0.6662F) * 1.4F * var2;
    self->bipedLeftLeg.rotateAngleX = MathHelper_cos(var1 * 0.6662F + (float)M_PI) * 1.4F * var2;
    self->bipedRightLeg.rotateAngleY = 0.0F;
    self->bipedLeftLeg.rotateAngleY = 0.0F;

    if (self->base.isRiding) {
        self->bipedRightArm.rotateAngleX += (float)M_PI * -0.2F;
        self->bipedLeftArm.rotateAngleX += (float)M_PI * -0.2F;
        self->bipedRightLeg.rotateAngleX = (float)M_PI * -0.4F;
        self->bipedLeftLeg.rotateAngleX = (float)M_PI * -0.4F;
        self->bipedRightLeg.rotateAngleY = (float)M_PI * 0.1F;
        self->bipedLeftLeg.rotateAngleY = (float)M_PI * -0.1F;
    }

    if (self->field_1279_h) {
        self->bipedLeftArm.rotateAngleX =
            self->bipedLeftArm.rotateAngleX * 0.5F - (float)M_PI * 0.1F;
    }
    if (self->field_1278_i) {
        self->bipedRightArm.rotateAngleX =
            self->bipedRightArm.rotateAngleX * 0.5F - (float)M_PI * 0.1F;
    }

    self->bipedRightArm.rotateAngleY = 0.0F;
    self->bipedLeftArm.rotateAngleY = 0.0F;

    if (self->base.onGround > -9990.0F) {
        float var7 = self->base.onGround;
        self->bipedBody.rotateAngleY =
            MathHelper_sin(MathHelper_sqrt_float(var7) * (float)M_PI * 2.0F) * 0.2F;
        self->bipedRightArm.rotationPointZ = MathHelper_sin(self->bipedBody.rotateAngleY) * 5.0F;
        self->bipedRightArm.rotationPointX = -MathHelper_cos(self->bipedBody.rotateAngleY) * 5.0F;
        self->bipedLeftArm.rotationPointZ = -MathHelper_sin(self->bipedBody.rotateAngleY) * 5.0F;
        self->bipedLeftArm.rotationPointX = MathHelper_cos(self->bipedBody.rotateAngleY) * 5.0F;
        self->bipedRightArm.rotateAngleY += self->bipedBody.rotateAngleY;
        self->bipedLeftArm.rotateAngleY += self->bipedBody.rotateAngleY;
        self->bipedLeftArm.rotateAngleX += self->bipedBody.rotateAngleY;
        var7 = 1.0F - self->base.onGround;
        var7 *= var7;
        var7 *= var7;
        var7 = 1.0F - var7;
        float var8 = MathHelper_sin(var7 * (float)M_PI);
        float var9 = MathHelper_sin(self->base.onGround * (float)M_PI) *
                     -(self->bipedHead.rotateAngleX - 0.7F) * (12.0F / 16.0F);
        self->bipedRightArm.rotateAngleX =
            (float)((double)self->bipedRightArm.rotateAngleX - ((double)var8 * 1.2 + (double)var9));
        self->bipedRightArm.rotateAngleY += self->bipedBody.rotateAngleY * 2.0F;
        self->bipedRightArm.rotateAngleZ =
            MathHelper_sin(self->base.onGround * (float)M_PI) * -0.4F;
    }

    if (self->isSneak) {
        self->bipedBody.rotateAngleX = 0.5F;
        self->bipedRightLeg.rotateAngleX -= 0.0F;
        self->bipedLeftLeg.rotateAngleX -= 0.0F;
        self->bipedRightArm.rotateAngleX += 0.4F;
        self->bipedLeftArm.rotateAngleX += 0.4F;
        self->bipedRightLeg.rotationPointZ = 4.0F;
        self->bipedLeftLeg.rotationPointZ = 4.0F;
        self->bipedRightLeg.rotationPointY = 9.0F;
        self->bipedLeftLeg.rotationPointY = 9.0F;
        self->bipedHead.rotationPointY = 1.0F;
    } else {
        self->bipedBody.rotateAngleX = 0.0F;
        self->bipedRightLeg.rotationPointZ = 0.0F;
        self->bipedLeftLeg.rotationPointZ = 0.0F;
        self->bipedRightLeg.rotationPointY = 12.0F;
        self->bipedLeftLeg.rotationPointY = 12.0F;
        self->bipedHead.rotationPointY = 0.0F;
    }

    self->bipedRightArm.rotateAngleZ += MathHelper_cos(var3 * 0.09F) * 0.05F + 0.05F;
    self->bipedLeftArm.rotateAngleZ -= MathHelper_cos(var3 * 0.09F) * 0.05F + 0.05F;
    self->bipedRightArm.rotateAngleX += MathHelper_sin(var3 * 0.067F) * 0.05F;
    self->bipedLeftArm.rotateAngleX -= MathHelper_sin(var3 * 0.067F) * 0.05F;
}

void ModelBiped_renderEars(ModelBiped *self, float var1) {
    self->bipedEars.rotateAngleY = self->bipedHead.rotateAngleY;
    self->bipedEars.rotateAngleX = self->bipedHead.rotateAngleX;
    self->bipedEars.rotationPointX = 0.0F;
    self->bipedEars.rotationPointY = 0.0F;
    ModelRenderer_render(&self->bipedEars, var1);
}

void ModelBiped_renderCloak(ModelBiped *self, float var1) {
    ModelRenderer_render(&self->bipedCloak, var1);
}
