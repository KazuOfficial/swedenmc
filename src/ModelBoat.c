#include "ModelBoat.h"
#include <math.h>

ModelBase_vtable ModelBoat_defaultVtable = {ModelBoat_render, ModelBase_setRotationAngles,
                                            ModelBase_setLivingAnimations};

void ModelBoat_init(ModelBoat *self) {
    self->base.vtable = &ModelBoat_defaultVtable;
    self->base.onGround = 0.0F;
    self->base.isRiding = 0;

    ModelRenderer_init(&self->boatSides[0], 0, 8);
    ModelRenderer_init(&self->boatSides[1], 0, 0);
    ModelRenderer_init(&self->boatSides[2], 0, 0);
    ModelRenderer_init(&self->boatSides[3], 0, 0);
    ModelRenderer_init(&self->boatSides[4], 0, 0);

    int var1 = 24;
    int var2 = 6;
    int var3 = 20;
    int var4 = 4;

    ModelRenderer_addBoxExpanded(&self->boatSides[0], (float)(-var1 / 2), (float)(-var3 / 2 + 2),
                                 -3.0F, var1, var3 - 4, 4, 0.0F);
    ModelRenderer_setRotationPoint(&self->boatSides[0], 0.0F, (float)(0 + var4), 0.0F);

    ModelRenderer_addBoxExpanded(&self->boatSides[1], (float)(-var1 / 2 + 2), (float)(-var2 - 1),
                                 -1.0F, var1 - 4, var2, 2, 0.0F);
    ModelRenderer_setRotationPoint(&self->boatSides[1], (float)(-var1 / 2 + 1), (float)(0 + var4),
                                   0.0F);

    ModelRenderer_addBoxExpanded(&self->boatSides[2], (float)(-var1 / 2 + 2), (float)(-var2 - 1),
                                 -1.0F, var1 - 4, var2, 2, 0.0F);
    ModelRenderer_setRotationPoint(&self->boatSides[2], (float)(var1 / 2 - 1), (float)(0 + var4),
                                   0.0F);

    ModelRenderer_addBoxExpanded(&self->boatSides[3], (float)(-var1 / 2 + 2), (float)(-var2 - 1),
                                 -1.0F, var1 - 4, var2, 2, 0.0F);
    ModelRenderer_setRotationPoint(&self->boatSides[3], 0.0F, (float)(0 + var4),
                                   (float)(-var3 / 2 + 1));

    ModelRenderer_addBoxExpanded(&self->boatSides[4], (float)(-var1 / 2 + 2), (float)(-var2 - 1),
                                 -1.0F, var1 - 4, var2, 2, 0.0F);
    ModelRenderer_setRotationPoint(&self->boatSides[4], 0.0F, (float)(0 + var4),
                                   (float)(var3 / 2 - 1));

    self->boatSides[0].rotateAngleX = (float)M_PI * 0.5F;
    self->boatSides[1].rotateAngleY = (float)M_PI * 3.0F / 2.0F;
    self->boatSides[2].rotateAngleY = (float)M_PI * 0.5F;
    self->boatSides[3].rotateAngleY = (float)M_PI;
}

void ModelBoat_render(ModelBase *base, float var1, float var2, float var3, float var4, float var5,
                      float var6) {
    ModelBoat *self = (ModelBoat *)base;
    (void)var1;
    (void)var2;
    (void)var3;
    (void)var4;
    (void)var5;
    for (int var7 = 0; var7 < 5; ++var7) {
        ModelRenderer_render(&self->boatSides[var7], var6);
    }
}
