#include "ModelSkeleton.h"

void ModelSkeleton_init(ModelSkeleton *self) {
    ModelZombie_init(&self->base);

    float var1 = 0.0F;
    self->base.base.bipedRightArm.mirror = 0;
    ModelRenderer_destroy(&self->base.base.bipedRightArm);
    ModelRenderer_init(&self->base.base.bipedRightArm, 40, 16);
    ModelRenderer_addBoxExpanded(&self->base.base.bipedRightArm, -1.0F, -2.0F, -1.0F, 2, 12, 2,
                                 var1);
    ModelRenderer_setRotationPoint(&self->base.base.bipedRightArm, -5.0F, 2.0F, 0.0F);

    self->base.base.bipedLeftArm.mirror = 1;
    ModelRenderer_destroy(&self->base.base.bipedLeftArm);
    ModelRenderer_init(&self->base.base.bipedLeftArm, 40, 16);
    ModelRenderer_addBoxExpanded(&self->base.base.bipedLeftArm, -1.0F, -2.0F, -1.0F, 2, 12, 2,
                                 var1);
    ModelRenderer_setRotationPoint(&self->base.base.bipedLeftArm, 5.0F, 2.0F, 0.0F);

    self->base.base.bipedRightLeg.mirror = 0;
    ModelRenderer_destroy(&self->base.base.bipedRightLeg);
    ModelRenderer_init(&self->base.base.bipedRightLeg, 0, 16);
    ModelRenderer_addBoxExpanded(&self->base.base.bipedRightLeg, -1.0F, 0.0F, -1.0F, 2, 12, 2,
                                 var1);
    ModelRenderer_setRotationPoint(&self->base.base.bipedRightLeg, -2.0F, 12.0F, 0.0F);

    self->base.base.bipedLeftLeg.mirror = 1;
    ModelRenderer_destroy(&self->base.base.bipedLeftLeg);
    ModelRenderer_init(&self->base.base.bipedLeftLeg, 0, 16);
    ModelRenderer_addBoxExpanded(&self->base.base.bipedLeftLeg, -1.0F, 0.0F, -1.0F, 2, 12, 2, var1);
    ModelRenderer_setRotationPoint(&self->base.base.bipedLeftLeg, 2.0F, 12.0F, 0.0F);
}
