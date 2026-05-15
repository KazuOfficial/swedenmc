#include "ModelGhast.h"
#include "MathHelper.h"
#include "JavaRandom.h"

ModelBase_vtable ModelGhast_defaultVtable = {ModelGhast_render, ModelGhast_setRotationAngles,
                                             ModelBase_setLivingAnimations};

void ModelGhast_init(ModelGhast *self) {
    self->base.vtable = &ModelGhast_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;

    int var1 = -16;
    ModelRenderer_init(&self->body, 0, 0);
    ModelRenderer_addBox(&self->body, -8.0F, -8.0F, -8.0F, 16, 16, 16);
    self->body.rotationPointY += (float)(24 + var1);

    JavaRandom var2;
    JavaRandom_init(&var2, 1660L);

    for (int var3 = 0; var3 < 9; ++var3) {
        ModelRenderer_init(&self->tentacles[var3], 0, 0);
        float var4 =
            (((float)(var3 % 3) - (float)(var3 / 3 % 2) * 0.5F + 0.25F) / 2.0F * 2.0F - 1.0F) *
            5.0F;
        float var5 = ((float)(var3 / 3) / 2.0F * 2.0F - 1.0F) * 5.0F;
        int var6 = JavaRandom_nextInt(&var2, 7) + 8;
        ModelRenderer_addBox(&self->tentacles[var3], -1.0F, 0.0F, -1.0F, 2, var6, 2);
        self->tentacles[var3].rotationPointX = var4;
        self->tentacles[var3].rotationPointZ = var5;
        self->tentacles[var3].rotationPointY = (float)(31 + var1);
    }
}

void ModelGhast_destroy(ModelGhast *self) {
    ModelRenderer_destroy(&self->body);
    for (int i = 0; i < 9; i++)
        ModelRenderer_destroy(&self->tentacles[i]);
}

void ModelGhast_render(ModelBase *base, float var1, float var2, float var3, float var4, float var5,
                       float var6) {
    ModelGhast *self = (ModelGhast *)base;
    ModelGhast_setRotationAngles(base, var1, var2, var3, var4, var5, var6);
    ModelRenderer_render(&self->body, var6);
    for (int var7 = 0; var7 < 9; ++var7) {
        ModelRenderer_render(&self->tentacles[var7], var6);
    }
}

void ModelGhast_setRotationAngles(ModelBase *base, float var1, float var2, float var3, float var4,
                                  float var5, float var6) {
    ModelGhast *self = (ModelGhast *)base;
    (void)var1;
    (void)var2;
    (void)var4;
    (void)var5;
    (void)var6;
    for (int var7 = 0; var7 < 9; ++var7) {
        self->tentacles[var7].rotateAngleX =
            0.2F * MathHelper_sin(var3 * 0.3F + (float)var7) + 0.4F;
    }
}
