#include "ModelZombie.h"
#include "MathHelper.h"
#include <math.h>

ModelBiped_vtable ModelZombie_defaultVtable = {
    {ModelBiped_render, ModelZombie_setRotationAngles, ModelBase_setLivingAnimations},
    ModelBiped_renderEars,
    ModelBiped_renderCloak,
};

void ModelZombie_init(ModelZombie *self) {
    ModelBiped_init0(&self->base);
    self->base.base.vtable = (ModelBase_vtable *)&ModelZombie_defaultVtable;
}

void ModelZombie_setRotationAngles(ModelBase *base, float var1, float var2, float var3, float var4,
                                   float var5, float var6) {
    ModelZombie *self = (ModelZombie *)base;

    ModelBiped_setRotationAngles(base, var1, var2, var3, var4, var5, var6);

    float var7 = MathHelper_sin(self->base.base.onGround * (float)M_PI);
    float var8 = MathHelper_sin(
        (1.0F - (1.0F - self->base.base.onGround) * (1.0F - self->base.base.onGround)) *
        (float)M_PI);
    self->base.bipedRightArm.rotateAngleZ = 0.0F;
    self->base.bipedLeftArm.rotateAngleZ = 0.0F;
    self->base.bipedRightArm.rotateAngleY = -(0.1F - var7 * 0.6F);
    self->base.bipedLeftArm.rotateAngleY = 0.1F - var7 * 0.6F;
    self->base.bipedRightArm.rotateAngleX = (float)M_PI * -0.5F;
    self->base.bipedLeftArm.rotateAngleX = (float)M_PI * -0.5F;
    self->base.bipedRightArm.rotateAngleX -= var7 * 1.2F - var8 * 0.4F;
    self->base.bipedLeftArm.rotateAngleX -= var7 * 1.2F - var8 * 0.4F;
    self->base.bipedRightArm.rotateAngleZ += MathHelper_cos(var3 * 0.09F) * 0.05F + 0.05F;
    self->base.bipedLeftArm.rotateAngleZ -= MathHelper_cos(var3 * 0.09F) * 0.05F + 0.05F;
    self->base.bipedRightArm.rotateAngleX += MathHelper_sin(var3 * 0.067F) * 0.05F;
    self->base.bipedLeftArm.rotateAngleX -= MathHelper_sin(var3 * 0.067F) * 0.05F;
}
