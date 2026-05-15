#ifndef MODEL_SPIDER_H
#define MODEL_SPIDER_H

#include "ModelBase.h"
#include "ModelRenderer.h"

typedef struct ModelSpider {
    ModelBase base;
    ModelRenderer spiderHead;
    ModelRenderer spiderNeck;
    ModelRenderer spiderBody;
    ModelRenderer spiderLeg1;
    ModelRenderer spiderLeg2;
    ModelRenderer spiderLeg3;
    ModelRenderer spiderLeg4;
    ModelRenderer spiderLeg5;
    ModelRenderer spiderLeg6;
    ModelRenderer spiderLeg7;
    ModelRenderer spiderLeg8;
} ModelSpider;

extern ModelBase_vtable ModelSpider_defaultVtable;

void ModelSpider_init(ModelSpider *self);
void ModelSpider_destroy(ModelSpider *self);
void ModelSpider_render(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                        float var6);
void ModelSpider_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                   float var5, float var6);

#endif
