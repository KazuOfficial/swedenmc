#include "ModelChicken.h"
#include "MathHelper.h"
#include <math.h>

ModelBase_vtable ModelChicken_defaultVtable = {ModelChicken_render, ModelChicken_setRotationAngles,
                                               ModelBase_setLivingAnimations};

void ModelChicken_init(ModelChicken *self) {
    self->base.vtable = &ModelChicken_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;

    int var1 = 16;
    ModelRenderer_init(&self->head, 0, 0);
    ModelRenderer_addBox(&self->head, -2.0F, -6.0F, -2.0F, 4, 6, 3);
    ModelRenderer_setRotationPoint(&self->head, 0.0F, (float)(-1 + var1), -4.0F);

    ModelRenderer_init(&self->bill, 14, 0);
    ModelRenderer_addBox(&self->bill, -2.0F, -4.0F, -4.0F, 4, 2, 2);
    ModelRenderer_setRotationPoint(&self->bill, 0.0F, (float)(-1 + var1), -4.0F);

    ModelRenderer_init(&self->chin, 14, 4);
    ModelRenderer_addBox(&self->chin, -1.0F, -2.0F, -3.0F, 2, 2, 2);
    ModelRenderer_setRotationPoint(&self->chin, 0.0F, (float)(-1 + var1), -4.0F);

    ModelRenderer_init(&self->body, 0, 9);
    ModelRenderer_addBox(&self->body, -3.0F, -4.0F, -3.0F, 6, 8, 6);
    ModelRenderer_setRotationPoint(&self->body, 0.0F, (float)(0 + var1), 0.0F);

    ModelRenderer_init(&self->rightLeg, 26, 0);
    ModelRenderer_addBox(&self->rightLeg, -1.0F, 0.0F, -3.0F, 3, 5, 3);
    ModelRenderer_setRotationPoint(&self->rightLeg, -2.0F, (float)(3 + var1), 1.0F);

    ModelRenderer_init(&self->leftLeg, 26, 0);
    ModelRenderer_addBox(&self->leftLeg, -1.0F, 0.0F, -3.0F, 3, 5, 3);
    ModelRenderer_setRotationPoint(&self->leftLeg, 1.0F, (float)(3 + var1), 1.0F);

    ModelRenderer_init(&self->rightWing, 24, 13);
    ModelRenderer_addBox(&self->rightWing, 0.0F, 0.0F, -3.0F, 1, 4, 6);
    ModelRenderer_setRotationPoint(&self->rightWing, -4.0F, (float)(-3 + var1), 0.0F);

    ModelRenderer_init(&self->leftWing, 24, 13);
    ModelRenderer_addBox(&self->leftWing, -1.0F, 0.0F, -3.0F, 1, 4, 6);
    ModelRenderer_setRotationPoint(&self->leftWing, 4.0F, (float)(-3 + var1), 0.0F);
}

void ModelChicken_destroy(ModelChicken *self) {
    ModelRenderer_destroy(&self->head);
    ModelRenderer_destroy(&self->bill);
    ModelRenderer_destroy(&self->chin);
    ModelRenderer_destroy(&self->body);
    ModelRenderer_destroy(&self->rightLeg);
    ModelRenderer_destroy(&self->leftLeg);
    ModelRenderer_destroy(&self->rightWing);
    ModelRenderer_destroy(&self->leftWing);
}

void ModelChicken_render(ModelBase *base, float var1, float var2, float var3, float var4,
                         float var5, float var6) {
    ModelChicken *self = (ModelChicken *)base;
    ModelChicken_setRotationAngles(base, var1, var2, var3, var4, var5, var6);
    ModelRenderer_render(&self->head, var6);
    ModelRenderer_render(&self->bill, var6);
    ModelRenderer_render(&self->chin, var6);
    ModelRenderer_render(&self->body, var6);
    ModelRenderer_render(&self->rightLeg, var6);
    ModelRenderer_render(&self->leftLeg, var6);
    ModelRenderer_render(&self->rightWing, var6);
    ModelRenderer_render(&self->leftWing, var6);
}

void ModelChicken_setRotationAngles(ModelBase *base, float var1, float var2, float var3, float var4,
                                    float var5, float var6) {
    ModelChicken *self = (ModelChicken *)base;
    (void)var6;
    self->head.rotateAngleX = -(var5 / (180.0F / (float)M_PI));
    self->head.rotateAngleY = var4 / (180.0F / (float)M_PI);
    self->bill.rotateAngleX = self->head.rotateAngleX;
    self->bill.rotateAngleY = self->head.rotateAngleY;
    self->chin.rotateAngleX = self->head.rotateAngleX;
    self->chin.rotateAngleY = self->head.rotateAngleY;
    self->body.rotateAngleX = (float)M_PI * 0.5F;
    self->rightLeg.rotateAngleX = MathHelper_cos(var1 * 0.6662F) * 1.4F * var2;
    self->leftLeg.rotateAngleX = MathHelper_cos(var1 * 0.6662F + (float)M_PI) * 1.4F * var2;
    self->rightWing.rotateAngleZ = var3;
    self->leftWing.rotateAngleZ = -var3;
}
