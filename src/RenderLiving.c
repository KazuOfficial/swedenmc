#include "Renderer.h"

#include "RenderLiving.h"
#include "Render.h"
#include "MathHelper.h"
#include "Tessellator.h"
#include "FontRenderer.h"
#include "Minecraft.h"
#include "Entity.h"
#include <string.h>
#include <stdlib.h>

#define VTABLE(self) ((RenderLiving_vtable *)((self)->base.vtable))

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {

    RenderLiving *self = (RenderLiving *)base;
    VTABLE(self)->doRenderLiving(self, (EntityLiving *)var1, var2, var4, var6, var8, var9);
}

void RenderLiving_doRenderLiving(RenderLiving *self, EntityLiving *var1, double var2, double var4,
                                 double var6, float var8, float var9) {
    (void)var8;
    R_pushMatrix();
    R_disable(R_CULL_FACE);
    self->mainModel->onGround = VTABLE(self)->func_167_c(self, var1, var9);
    if (self->renderPassModel != NULL) {
        self->renderPassModel->onGround = self->mainModel->onGround;
    }

    self->mainModel->isRiding = Entity_isRiding(&var1->base);
    if (self->renderPassModel != NULL) {
        self->renderPassModel->isRiding = self->mainModel->isRiding;
    }

    {
        float var10 =
            var1->prevRenderYawOffset + (var1->renderYawOffset - var1->prevRenderYawOffset) * var9;
        float var11 = var1->base.prevRotationYaw +
                      (var1->base.rotationYaw - var1->base.prevRotationYaw) * var9;
        float var12 = var1->base.prevRotationPitch +
                      (var1->base.rotationPitch - var1->base.prevRotationPitch) * var9;
        VTABLE(self)->func_22012_b(self, var1, var2, var4, var6);
        float var13 = VTABLE(self)->func_170_d(self, var1, var9);
        VTABLE(self)->rotateCorpse(self, var1, var13, var10, var9);
        float var14 = 1.0f / 16.0f;
        R_enable(R_RESCALE_NORMAL);
        R_scalef(-1.0f, -1.0f, 1.0f);
        VTABLE(self)->preRenderCallback(self, var1, var9);
        R_translatef(0.0f, -24.0f * var14 - 0.0078125f, 0.0f);
        float var15 = var1->field_705_Q + (var1->field_704_R - var1->field_705_Q) * var9;
        float var16 = var1->field_703_S - var1->field_704_R * (1.0f - var9);
        if (var15 > 1.0f) {
            var15 = 1.0f;
        }

        Render_loadDownloadableImageTexture(
            &self->base, var1->base.skinUrl,
            ((EntityLivingVtable *)var1->base.vtable)->getEntityTexture(var1));
        R_enable(R_ALPHA_TEST);
        self->mainModel->vtable->setLivingAnimations(self->mainModel, var1, var16, var15, var9);
        self->mainModel->vtable->render(self->mainModel, var16, var15, var13, var11 - var10, var12,
                                        var14);

        for (int var17 = 0; var17 < 4; ++var17) {
            if (VTABLE(self)->shouldRenderPass(self, var1, var17, var9)) {
                self->renderPassModel->vtable->render(self->renderPassModel, var16, var15, var13,
                                                      var11 - var10, var12, var14);
                R_disable(R_BLEND);
                R_enable(R_ALPHA_TEST);
            }
        }

        VTABLE(self)->renderEquippedItems(self, var1, var9);
        float var25 = var1->base.vtable->getEntityBrightness(&var1->base, var9);
        int var18 = VTABLE(self)->getColorMultiplier(self, var1, var25, var9);
        if ((var18 >> 24 & 255) > 0 || var1->hurtTime > 0 || var1->deathTime > 0) {
            R_disable(R_TEXTURE_2D);
            R_disable(R_ALPHA_TEST);
            R_enable(R_BLEND);
            R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
            R_depthFunc(R_EQUAL);
            if (var1->hurtTime > 0 || var1->deathTime > 0) {
                R_color4f(var25, 0.0f, 0.0f, 0.4f);
                self->mainModel->vtable->render(self->mainModel, var16, var15, var13, var11 - var10,
                                                var12, var14);
                for (int var19 = 0; var19 < 4; ++var19) {
                    if (VTABLE(self)->func_27005_b(self, var1, var19, var9)) {
                        R_color4f(var25, 0.0f, 0.0f, 0.4f);
                        self->renderPassModel->vtable->render(self->renderPassModel, var16, var15,
                                                              var13, var11 - var10, var12, var14);
                    }
                }
            }
            if ((var18 >> 24 & 255) > 0) {
                float var26 = (float)(var18 >> 16 & 255) / 255.0f;
                float var20 = (float)(var18 >> 8 & 255) / 255.0f;
                float var21 = (float)(var18 & 255) / 255.0f;
                float var22 = (float)(var18 >> 24 & 255) / 255.0f;
                R_color4f(var26, var20, var21, var22);
                self->mainModel->vtable->render(self->mainModel, var16, var15, var13, var11 - var10,
                                                var12, var14);
                for (int var23 = 0; var23 < 4; ++var23) {
                    if (VTABLE(self)->func_27005_b(self, var1, var23, var9)) {
                        R_color4f(var26, var20, var21, var22);
                        self->renderPassModel->vtable->render(self->renderPassModel, var16, var15,
                                                              var13, var11 - var10, var12, var14);
                    }
                }
            }
            R_depthFunc(R_LEQUAL);
            R_disable(R_BLEND);
            R_enable(R_ALPHA_TEST);
            R_enable(R_TEXTURE_2D);
        }
        R_disable(R_RESCALE_NORMAL);
    }

    R_enable(R_CULL_FACE);
    R_popMatrix();
    VTABLE(self)->passSpecialRender(self, var1, var2, var4, var6);
}

void RenderLiving_func_22012_b(RenderLiving *self, EntityLiving *var1, double var2, double var4,
                               double var6) {
    (void)self;
    (void)var1;
    R_translatef((float)var2, (float)var4, (float)var6);
}

void RenderLiving_rotateCorpse(RenderLiving *self, EntityLiving *var1, float var2, float var3,
                               float var4) {
    (void)var2;
    R_rotatef(180.0f - var3, 0.0f, 1.0f, 0.0f);
    if (var1->deathTime > 0) {
        float var5 = ((float)var1->deathTime + var4 - 1.0f) / 20.0f * 1.6f;
        var5 = MathHelper_sqrt_float(var5);
        if (var5 > 1.0f) {
            var5 = 1.0f;
        }
        R_rotatef(var5 * VTABLE(self)->getDeathMaxRotation(self, var1), 0.0f, 0.0f, 1.0f);
    }
}

float RenderLiving_func_167_c(RenderLiving *self, EntityLiving *var1, float var2) {
    (void)self;
    return EntityLiving_getSwingProgress(var1, var2);
}

float RenderLiving_func_170_d(RenderLiving *self, EntityLiving *var1, float var2) {
    (void)self;
    return (float)var1->base.ticksExisted + var2;
}

void RenderLiving_renderEquippedItems(RenderLiving *self, EntityLiving *var1, float var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

int RenderLiving_func_27005_b(RenderLiving *self, EntityLiving *var1, int var2, float var3) {
    return VTABLE(self)->shouldRenderPass(self, var1, var2, var3);
}

int RenderLiving_shouldRenderPass(RenderLiving *self, EntityLiving *var1, int var2, float var3) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    return 0;
}

float RenderLiving_getDeathMaxRotation(RenderLiving *self, EntityLiving *var1) {
    (void)self;
    (void)var1;
    return 90.0f;
}

int RenderLiving_getColorMultiplier(RenderLiving *self, EntityLiving *var1, float var2,
                                    float var3) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    return 0;
}

void RenderLiving_preRenderCallback(RenderLiving *self, EntityLiving *var1, float var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

void RenderLiving_passSpecialRender(RenderLiving *self, EntityLiving *var1, double var2,
                                    double var4, double var6) {
    if (Minecraft_isDebugInfoEnabled()) {
        char buf[16];

        snprintf(buf, sizeof(buf), "%d", var1->base.entityId);
        RenderLiving_renderLivingLabel(self, var1, buf, var2, var4, var6, 64);
    }
}

void RenderLiving_renderLivingLabel(RenderLiving *self, EntityLiving *var1, const char *var2,
                                    double var3, double var5, double var7, int var9) {
    float var10 =
        Entity_getDistanceToEntity(&var1->base, &self->base.renderManager->livingPlayer->base);
    if (var10 <= (float)var9) {
        FontRenderer *var11 = (FontRenderer *)Render_getFontRendererFromRenderManager(&self->base);
        float var12 = 1.6f;
        float var13 = (float)(1.0 / 60.0) * var12;
        R_pushMatrix();
        R_translatef((float)var3 + 0.0f, (float)var5 + 2.3f, (float)var7);
        R_normal3f(0.0f, 1.0f, 0.0f);
        R_rotatef(-self->base.renderManager->playerViewY, 0.0f, 1.0f, 0.0f);
        R_rotatef(self->base.renderManager->playerViewX, 1.0f, 0.0f, 0.0f);
        R_scalef(-var13, -var13, var13);
        R_disable(R_LIGHTING);
        R_depthMask(R_FALSE);
        R_disable(R_DEPTH_TEST);
        R_enable(R_BLEND);
        R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
        Tessellator *var14 = &Tessellator_instance;
        int var15 = 0;
        if (strcmp(var2, "deadmau5") == 0) {
            var15 = -10;
        }
        R_disable(R_TEXTURE_2D);
        Tessellator_startDrawingQuads(var14);
        int var16 = FontRenderer_getStringWidth(var11, var2) / 2;
        Tessellator_setColorRGBA_F(var14, 0.0f, 0.0f, 0.0f, 0.25f);
        Tessellator_addVertex(var14, (double)(-var16 - 1), (double)(-1 + var15), 0.0);
        Tessellator_addVertex(var14, (double)(-var16 - 1), (double)(8 + var15), 0.0);
        Tessellator_addVertex(var14, (double)(var16 + 1), (double)(8 + var15), 0.0);
        Tessellator_addVertex(var14, (double)(var16 + 1), (double)(-1 + var15), 0.0);
        Tessellator_draw(var14);
        R_enable(R_TEXTURE_2D);
        FontRenderer_drawString(var11, var2, -FontRenderer_getStringWidth(var11, var2) / 2, var15,
                                553648127);
        R_enable(R_DEPTH_TEST);
        R_depthMask(R_TRUE);
        FontRenderer_drawString(var11, var2, -FontRenderer_getStringWidth(var11, var2) / 2, var15,
                                -1);
        R_enable(R_LIGHTING);
        R_disable(R_BLEND);
        R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
        R_popMatrix();
    }
}

void RenderLiving_setRenderPassModel(RenderLiving *self, ModelBase *var1) {
    self->renderPassModel = var1;
}

RenderLiving_vtable RenderLiving_defaultVtable = {
    .base = {.doRender = vt_doRender},
    .doRenderLiving = RenderLiving_doRenderLiving,
    .func_22012_b = RenderLiving_func_22012_b,
    .rotateCorpse = RenderLiving_rotateCorpse,
    .func_167_c = RenderLiving_func_167_c,
    .func_170_d = RenderLiving_func_170_d,
    .renderEquippedItems = RenderLiving_renderEquippedItems,
    .func_27005_b = RenderLiving_func_27005_b,
    .shouldRenderPass = RenderLiving_shouldRenderPass,
    .getDeathMaxRotation = RenderLiving_getDeathMaxRotation,
    .getColorMultiplier = RenderLiving_getColorMultiplier,
    .preRenderCallback = RenderLiving_preRenderCallback,
    .passSpecialRender = RenderLiving_passSpecialRender,
};

void RenderLiving_init(RenderLiving *self, ModelBase *var1, float var2) {
    Render_init(&self->base);
    self->base.vtable = (const Render_vtable *)&RenderLiving_defaultVtable;
    self->mainModel = var1;
    self->renderPassModel = NULL;
    self->base.shadowSize = var2;
}
