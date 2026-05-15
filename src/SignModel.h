#ifndef SIGN_MODEL_H
#define SIGN_MODEL_H

#include "ModelRenderer.h"

typedef struct SignModel {

    ModelRenderer signBoard;

    ModelRenderer signStick;
} SignModel;

void SignModel_init(SignModel *self);

void SignModel_func_887_a(SignModel *self);

#endif
