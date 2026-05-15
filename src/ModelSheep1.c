#include "ModelSheep1.h"

void ModelSheep1_init(ModelSheep1 *self) {
    ModelQuadruped_init(&self->base, 12, 0.0F);

    ModelRenderer_destroy(&self->base.head);
    ModelRenderer_init(&self->base.head, 0, 0);
    ModelRenderer_addBoxExpanded(&self->base.head, -3.0F, -4.0F, -4.0F, 6, 6, 6, 0.6F);
    ModelRenderer_setRotationPoint(&self->base.head, 0.0F, 6.0F, -8.0F);

    ModelRenderer_destroy(&self->base.body);
    ModelRenderer_init(&self->base.body, 28, 8);
    ModelRenderer_addBoxExpanded(&self->base.body, -4.0F, -10.0F, -7.0F, 8, 16, 6, 1.75F);
    ModelRenderer_setRotationPoint(&self->base.body, 0.0F, 5.0F, 2.0F);

    float var1 = 0.5F;
    ModelRenderer_destroy(&self->base.leg1);
    ModelRenderer_init(&self->base.leg1, 0, 16);
    ModelRenderer_addBoxExpanded(&self->base.leg1, -2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
    ModelRenderer_setRotationPoint(&self->base.leg1, -3.0F, 12.0F, 7.0F);

    ModelRenderer_destroy(&self->base.leg2);
    ModelRenderer_init(&self->base.leg2, 0, 16);
    ModelRenderer_addBoxExpanded(&self->base.leg2, -2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
    ModelRenderer_setRotationPoint(&self->base.leg2, 3.0F, 12.0F, 7.0F);

    ModelRenderer_destroy(&self->base.leg3);
    ModelRenderer_init(&self->base.leg3, 0, 16);
    ModelRenderer_addBoxExpanded(&self->base.leg3, -2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
    ModelRenderer_setRotationPoint(&self->base.leg3, -3.0F, 12.0F, -5.0F);

    ModelRenderer_destroy(&self->base.leg4);
    ModelRenderer_init(&self->base.leg4, 0, 16);
    ModelRenderer_addBoxExpanded(&self->base.leg4, -2.0F, 0.0F, -2.0F, 4, 6, 4, var1);
    ModelRenderer_setRotationPoint(&self->base.leg4, 3.0F, 12.0F, -5.0F);
}
