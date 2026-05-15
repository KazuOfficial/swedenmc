#include "ModelCow.h"
#include <math.h>

ModelQuadruped_vtable ModelCow_defaultVtable = {
    {ModelCow_render, ModelCow_setRotationAngles, ModelBase_setLivingAnimations}};

void ModelCow_init(ModelCow *self) {
    ModelQuadruped_init(&self->base, 12, 0.0F);
    self->base.base.vtable = (ModelBase_vtable *)&ModelCow_defaultVtable;

    ModelRenderer_destroy(&self->base.head);
    ModelRenderer_init(&self->base.head, 0, 0);
    ModelRenderer_addBox(&self->base.head, -4.0F, -4.0F, -6.0F, 8, 8, 6);
    ModelRenderer_setRotationPoint(&self->base.head, 0.0F, 4.0F, -8.0F);

    ModelRenderer_init(&self->horn1, 22, 0);
    ModelRenderer_addBox(&self->horn1, -4.0F, -5.0F, -4.0F, 1, 3, 1);
    ModelRenderer_setRotationPoint(&self->horn1, 0.0F, 3.0F, -7.0F);

    ModelRenderer_init(&self->horn2, 22, 0);
    ModelRenderer_addBox(&self->horn2, 3.0F, -5.0F, -4.0F, 1, 3, 1);
    ModelRenderer_setRotationPoint(&self->horn2, 0.0F, 3.0F, -7.0F);

    ModelRenderer_init(&self->udders, 52, 0);
    ModelRenderer_addBox(&self->udders, -2.0F, -3.0F, 0.0F, 4, 6, 2);
    ModelRenderer_setRotationPoint(&self->udders, 0.0F, 14.0F, 6.0F);
    self->udders.rotateAngleX = (float)M_PI * 0.5F;

    ModelRenderer_destroy(&self->base.body);
    ModelRenderer_init(&self->base.body, 18, 4);
    ModelRenderer_addBox(&self->base.body, -6.0F, -10.0F, -7.0F, 12, 18, 10);
    ModelRenderer_setRotationPoint(&self->base.body, 0.0F, 5.0F, 2.0F);

    --self->base.leg1.rotationPointX;
    ++self->base.leg2.rotationPointX;

    --self->base.leg3.rotationPointX;
    ++self->base.leg4.rotationPointX;
    --self->base.leg3.rotationPointZ;
    --self->base.leg4.rotationPointZ;
}

void ModelCow_destroy(ModelCow *self) {
    ModelQuadruped_destroy(&self->base);
    ModelRenderer_destroy(&self->horn1);
    ModelRenderer_destroy(&self->horn2);
    ModelRenderer_destroy(&self->udders);
}

void ModelCow_render(ModelBase *base, float var1, float var2, float var3, float var4, float var5,
                     float var6) {
    ModelCow *self = (ModelCow *)base;

    ModelQuadruped_render(base, var1, var2, var3, var4, var5, var6);
    ModelRenderer_render(&self->horn1, var6);
    ModelRenderer_render(&self->horn2, var6);
    ModelRenderer_render(&self->udders, var6);
}

void ModelCow_setRotationAngles(ModelBase *base, float var1, float var2, float var3, float var4,
                                float var5, float var6) {
    ModelCow *self = (ModelCow *)base;

    ModelQuadruped_setRotationAngles(base, var1, var2, var3, var4, var5, var6);
    self->horn1.rotateAngleY = self->base.head.rotateAngleY;
    self->horn1.rotateAngleX = self->base.head.rotateAngleX;
    self->horn2.rotateAngleY = self->base.head.rotateAngleY;
    self->horn2.rotateAngleX = self->base.head.rotateAngleX;
}
