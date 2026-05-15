#include "ModelQuadruped.h"
#include "MathHelper.h"
#include <math.h>

ModelQuadruped_vtable ModelQuadruped_defaultVtable = {
    {ModelQuadruped_render, ModelQuadruped_setRotationAngles, ModelBase_setLivingAnimations}};

void ModelQuadruped_init(ModelQuadruped *self, int var1, float var2) {
    self->base.vtable = (ModelBase_vtable *)&ModelQuadruped_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;

    ModelRenderer_init(&self->head, 0, 0);
    ModelRenderer_addBoxExpanded(&self->head, -4.0F, -4.0F, -8.0F, 8, 8, 8, var2);
    ModelRenderer_setRotationPoint(&self->head, 0.0F, (float)(18 - var1), -6.0F);

    ModelRenderer_init(&self->body, 28, 8);
    ModelRenderer_addBoxExpanded(&self->body, -5.0F, -10.0F, -7.0F, 10, 16, 8, var2);
    ModelRenderer_setRotationPoint(&self->body, 0.0F, (float)(17 - var1), 2.0F);

    ModelRenderer_init(&self->leg1, 0, 16);
    ModelRenderer_addBoxExpanded(&self->leg1, -2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
    ModelRenderer_setRotationPoint(&self->leg1, -3.0F, (float)(24 - var1), 7.0F);

    ModelRenderer_init(&self->leg2, 0, 16);
    ModelRenderer_addBoxExpanded(&self->leg2, -2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
    ModelRenderer_setRotationPoint(&self->leg2, 3.0F, (float)(24 - var1), 7.0F);

    ModelRenderer_init(&self->leg3, 0, 16);
    ModelRenderer_addBoxExpanded(&self->leg3, -2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
    ModelRenderer_setRotationPoint(&self->leg3, -3.0F, (float)(24 - var1), -5.0F);

    ModelRenderer_init(&self->leg4, 0, 16);
    ModelRenderer_addBoxExpanded(&self->leg4, -2.0F, 0.0F, -2.0F, 4, var1, 4, var2);
    ModelRenderer_setRotationPoint(&self->leg4, 3.0F, (float)(24 - var1), -5.0F);
}

void ModelQuadruped_destroy(ModelQuadruped *self) {
    ModelRenderer_destroy(&self->head);
    ModelRenderer_destroy(&self->body);
    ModelRenderer_destroy(&self->leg1);
    ModelRenderer_destroy(&self->leg2);
    ModelRenderer_destroy(&self->leg3);
    ModelRenderer_destroy(&self->leg4);
}

void ModelQuadruped_render(ModelBase *base, float var1, float var2, float var3, float var4,
                           float var5, float var6) {
    ModelQuadruped *self = (ModelQuadruped *)base;
    self->base.vtable->setRotationAngles(base, var1, var2, var3, var4, var5, var6);
    ModelRenderer_render(&self->head, var6);
    ModelRenderer_render(&self->body, var6);
    ModelRenderer_render(&self->leg1, var6);
    ModelRenderer_render(&self->leg2, var6);
    ModelRenderer_render(&self->leg3, var6);
    ModelRenderer_render(&self->leg4, var6);
}

void ModelQuadruped_setRotationAngles(ModelBase *base, float var1, float var2, float var3,
                                      float var4, float var5, float var6) {
    ModelQuadruped *self = (ModelQuadruped *)base;
    (void)var3;
    (void)var6;
    self->head.rotateAngleX = var5 / (180.0F / (float)M_PI);
    self->head.rotateAngleY = var4 / (180.0F / (float)M_PI);
    self->body.rotateAngleX = (float)M_PI * 0.5F;
    self->leg1.rotateAngleX = MathHelper_cos(var1 * 0.6662F) * 1.4F * var2;
    self->leg2.rotateAngleX = MathHelper_cos(var1 * 0.6662F + (float)M_PI) * 1.4F * var2;
    self->leg3.rotateAngleX = MathHelper_cos(var1 * 0.6662F + (float)M_PI) * 1.4F * var2;
    self->leg4.rotateAngleX = MathHelper_cos(var1 * 0.6662F) * 1.4F * var2;
}
