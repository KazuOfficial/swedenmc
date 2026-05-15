#include "ModelCreeper.h"
#include "MathHelper.h"
#include <math.h>

ModelBase_vtable ModelCreeper_defaultVtable = {ModelCreeper_render, ModelCreeper_setRotationAngles,
                                               ModelBase_setLivingAnimations};

void ModelCreeper_init(ModelCreeper *self) { ModelCreeper_initF(self, 0.0F); }

void ModelCreeper_initF(ModelCreeper *self, float var1) {
    self->base.vtable = &ModelCreeper_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;

    int var2 = 4;
    ModelRenderer_init(&self->head, 0, 0);
    ModelRenderer_addBoxExpanded(&self->head, -4.0F, -8.0F, -4.0F, 8, 8, 8, var1);
    ModelRenderer_setRotationPoint(&self->head, 0.0F, (float)var2, 0.0F);

    ModelRenderer_init(&self->field_1270_b, 32, 0);
    ModelRenderer_addBoxExpanded(&self->field_1270_b, -4.0F, -8.0F, -4.0F, 8, 8, 8, var1 + 0.5F);
    ModelRenderer_setRotationPoint(&self->field_1270_b, 0.0F, (float)var2, 0.0F);

    ModelRenderer_init(&self->body, 16, 16);
    ModelRenderer_addBoxExpanded(&self->body, -4.0F, 0.0F, -2.0F, 8, 12, 4, var1);
    ModelRenderer_setRotationPoint(&self->body, 0.0F, (float)var2, 0.0F);

    ModelRenderer_init(&self->leg1, 0, 16);
    ModelRenderer_addBoxExpanded(&self->leg1, -2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
    ModelRenderer_setRotationPoint(&self->leg1, -2.0F, (float)(12 + var2), 4.0F);

    ModelRenderer_init(&self->leg2, 0, 16);
    ModelRenderer_addBoxExpanded(&self->leg2, -2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
    ModelRenderer_setRotationPoint(&self->leg2, 2.0F, (float)(12 + var2), 4.0F);

    ModelRenderer_init(&self->leg3, 0, 16);
    ModelRenderer_addBoxExpanded(&self->leg3, -2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
    ModelRenderer_setRotationPoint(&self->leg3, -2.0F, (float)(12 + var2), -4.0F);

    ModelRenderer_init(&self->leg4, 0, 16);
    ModelRenderer_addBoxExpanded(&self->leg4, -2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
    ModelRenderer_setRotationPoint(&self->leg4, 2.0F, (float)(12 + var2), -4.0F);
}

void ModelCreeper_destroy(ModelCreeper *self) {
    ModelRenderer_destroy(&self->head);
    ModelRenderer_destroy(&self->field_1270_b);
    ModelRenderer_destroy(&self->body);
    ModelRenderer_destroy(&self->leg1);
    ModelRenderer_destroy(&self->leg2);
    ModelRenderer_destroy(&self->leg3);
    ModelRenderer_destroy(&self->leg4);
}

void ModelCreeper_render(ModelBase *base, float var1, float var2, float var3, float var4,
                         float var5, float var6) {
    ModelCreeper *self = (ModelCreeper *)base;
    ModelCreeper_setRotationAngles(base, var1, var2, var3, var4, var5, var6);
    ModelRenderer_render(&self->head, var6);
    ModelRenderer_render(&self->body, var6);
    ModelRenderer_render(&self->leg1, var6);
    ModelRenderer_render(&self->leg2, var6);
    ModelRenderer_render(&self->leg3, var6);
    ModelRenderer_render(&self->leg4, var6);
}

void ModelCreeper_setRotationAngles(ModelBase *base, float var1, float var2, float var3, float var4,
                                    float var5, float var6) {
    ModelCreeper *self = (ModelCreeper *)base;
    (void)var3;
    (void)var6;
    self->head.rotateAngleY = var4 / (180.0F / (float)M_PI);
    self->head.rotateAngleX = var5 / (180.0F / (float)M_PI);
    self->leg1.rotateAngleX = MathHelper_cos(var1 * 0.6662F) * 1.4F * var2;
    self->leg2.rotateAngleX = MathHelper_cos(var1 * 0.6662F + (float)M_PI) * 1.4F * var2;
    self->leg3.rotateAngleX = MathHelper_cos(var1 * 0.6662F + (float)M_PI) * 1.4F * var2;
    self->leg4.rotateAngleX = MathHelper_cos(var1 * 0.6662F) * 1.4F * var2;
}
