#include "ModelSlime.h"

ModelBase_vtable ModelSlime_defaultVtable = {ModelSlime_render, ModelBase_setRotationAngles,
                                             ModelBase_setLivingAnimations};

void ModelSlime_init(ModelSlime *self, int var1) {
    self->base.vtable = &ModelSlime_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;
    self->hasEyes = 0;

    ModelRenderer_init(&self->slimeBodies, 0, var1);
    ModelRenderer_addBox(&self->slimeBodies, -4.0F, 16.0F, -4.0F, 8, 8, 8);

    if (var1 > 0) {

        ModelRenderer_destroy(&self->slimeBodies);
        ModelRenderer_init(&self->slimeBodies, 0, var1);
        ModelRenderer_addBox(&self->slimeBodies, -3.0F, 17.0F, -3.0F, 6, 6, 6);

        ModelRenderer_init(&self->slimeRightEye, 32, 0);
        ModelRenderer_addBox(&self->slimeRightEye, -3.25F, 18.0F, -3.5F, 2, 2, 2);

        ModelRenderer_init(&self->slimeLeftEye, 32, 4);
        ModelRenderer_addBox(&self->slimeLeftEye, 1.25F, 18.0F, -3.5F, 2, 2, 2);

        ModelRenderer_init(&self->slimeMouth, 32, 8);
        ModelRenderer_addBox(&self->slimeMouth, 0.0F, 21.0F, -3.5F, 1, 1, 1);

        self->hasEyes = 1;
    }
}

void ModelSlime_destroy(ModelSlime *self) {
    ModelRenderer_destroy(&self->slimeBodies);
    if (self->hasEyes) {
        ModelRenderer_destroy(&self->slimeRightEye);
        ModelRenderer_destroy(&self->slimeLeftEye);
        ModelRenderer_destroy(&self->slimeMouth);
    }
}

void ModelSlime_render(ModelBase *base, float var1, float var2, float var3, float var4, float var5,
                       float var6) {
    ModelSlime *self = (ModelSlime *)base;

    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    (void)var5;
    ModelRenderer_render(&self->slimeBodies, var6);
    if (self->hasEyes) {
        ModelRenderer_render(&self->slimeRightEye, var6);
        ModelRenderer_render(&self->slimeLeftEye, var6);
        ModelRenderer_render(&self->slimeMouth, var6);
    }
}
