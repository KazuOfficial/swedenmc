#ifndef RENDER_LIVING_H
#define RENDER_LIVING_H

#include "Render.h"
#include "EntityLiving.h"
#include "ModelBase.h"

struct RenderLiving;

typedef struct RenderLiving_vtable {
    Render_vtable base;

    void (*doRenderLiving)(struct RenderLiving *self, EntityLiving *var1, double var2, double var4,
                           double var6, float var8, float var9);

    void (*func_22012_b)(struct RenderLiving *self, EntityLiving *var1, double var2, double var4,
                         double var6);

    void (*rotateCorpse)(struct RenderLiving *self, EntityLiving *var1, float var2, float var3,
                         float var4);

    float (*func_167_c)(struct RenderLiving *self, EntityLiving *var1, float var2);

    float (*func_170_d)(struct RenderLiving *self, EntityLiving *var1, float var2);

    void (*renderEquippedItems)(struct RenderLiving *self, EntityLiving *var1, float var2);

    int (*func_27005_b)(struct RenderLiving *self, EntityLiving *var1, int var2, float var3);

    int (*shouldRenderPass)(struct RenderLiving *self, EntityLiving *var1, int var2, float var3);

    float (*getDeathMaxRotation)(struct RenderLiving *self, EntityLiving *var1);

    int (*getColorMultiplier)(struct RenderLiving *self, EntityLiving *var1, float var2,
                              float var3);

    void (*preRenderCallback)(struct RenderLiving *self, EntityLiving *var1, float var2);

    void (*passSpecialRender)(struct RenderLiving *self, EntityLiving *var1, double var2,
                              double var4, double var6);
} RenderLiving_vtable;

typedef struct RenderLiving {
    Render base;
    ModelBase *mainModel;
    ModelBase *renderPassModel;
} RenderLiving;

extern RenderLiving_vtable RenderLiving_defaultVtable;

void RenderLiving_init(RenderLiving *self, ModelBase *var1, float var2);

void RenderLiving_setRenderPassModel(RenderLiving *self, ModelBase *var1);

void RenderLiving_doRenderLiving(RenderLiving *self, EntityLiving *var1, double var2, double var4,
                                 double var6, float var8, float var9);
void RenderLiving_func_22012_b(RenderLiving *self, EntityLiving *var1, double var2, double var4,
                               double var6);
void RenderLiving_rotateCorpse(RenderLiving *self, EntityLiving *var1, float var2, float var3,
                               float var4);
float RenderLiving_func_167_c(RenderLiving *self, EntityLiving *var1, float var2);
float RenderLiving_func_170_d(RenderLiving *self, EntityLiving *var1, float var2);
void RenderLiving_renderEquippedItems(RenderLiving *self, EntityLiving *var1, float var2);
int RenderLiving_func_27005_b(RenderLiving *self, EntityLiving *var1, int var2, float var3);
int RenderLiving_shouldRenderPass(RenderLiving *self, EntityLiving *var1, int var2, float var3);
float RenderLiving_getDeathMaxRotation(RenderLiving *self, EntityLiving *var1);
int RenderLiving_getColorMultiplier(RenderLiving *self, EntityLiving *var1, float var2, float var3);
void RenderLiving_preRenderCallback(RenderLiving *self, EntityLiving *var1, float var2);
void RenderLiving_passSpecialRender(RenderLiving *self, EntityLiving *var1, double var2,
                                    double var4, double var6);
void RenderLiving_renderLivingLabel(RenderLiving *self, EntityLiving *var1, const char *var2,
                                    double var3, double var5, double var7, int var9);

#endif
