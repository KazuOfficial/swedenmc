#include "SignModel.h"

void SignModel_init(SignModel *self) {
    ModelRenderer_init(&self->signBoard, 0, 0);
    ModelRenderer_addBoxExpanded(&self->signBoard, -12.0f, -14.0f, -1.0f, 24, 12, 2, 0.0f);
    ModelRenderer_init(&self->signStick, 0, 14);
    ModelRenderer_addBoxExpanded(&self->signStick, -1.0f, -2.0f, -1.0f, 2, 14, 2, 0.0f);
}

void SignModel_func_887_a(SignModel *self) {
    ModelRenderer_render(&self->signBoard, 1.0f / 16.0f);
    ModelRenderer_render(&self->signStick, 1.0f / 16.0f);
}
