#include "ModelSheep2.h"

void ModelSheep2_init(ModelSheep2 *self) {
    ModelQuadruped_init(&self->base, 12, 0.0F);

    ModelRenderer_destroy(&self->base.head);
    ModelRenderer_init(&self->base.head, 0, 0);
    ModelRenderer_addBox(&self->base.head, -3.0F, -4.0F, -6.0F, 6, 6, 8);
    ModelRenderer_setRotationPoint(&self->base.head, 0.0F, 6.0F, -8.0F);

    ModelRenderer_destroy(&self->base.body);
    ModelRenderer_init(&self->base.body, 28, 8);
    ModelRenderer_addBox(&self->base.body, -4.0F, -10.0F, -7.0F, 8, 16, 6);
    ModelRenderer_setRotationPoint(&self->base.body, 0.0F, 5.0F, 2.0F);
}
