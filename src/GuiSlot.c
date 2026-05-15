#include "Renderer.h"

#include "GuiSlot.h"
#include "GuiButton.h"
#include "Minecraft.h"
#include "Tessellator.h"
#include "RenderEngine.h"
#include <stdint.h>
#include <time.h>

static int64_t currentTimeMillis(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static void GuiSlot_func_27260_a_default(GuiSlot *self, int var1, int var2, Tessellator *var3) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
}

static void GuiSlot_func_27255_a_default(GuiSlot *self, int var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

static void GuiSlot_func_27257_b_default(GuiSlot *self, int var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

const GuiSlotVtable GuiSlot_defaultVtable_nonabstract = {NULL,
                                                         NULL,
                                                         NULL,
                                                         GuiSlot_getContentHeight_default,
                                                         NULL,
                                                         NULL,
                                                         GuiSlot_func_27260_a_default,
                                                         GuiSlot_func_27255_a_default,
                                                         GuiSlot_func_27257_b_default};

void GuiSlot_construct(GuiSlot *self, struct Minecraft *var1, int var2, int var3, int var4,
                       int var5, int var6) {
    self->mc = var1;
    self->width = var2;
    self->height = var3;
    self->top = var4;
    self->bottom = var5;
    self->posZ = var6;
    self->left = 0;
    self->right = var2;
    self->initialClickY = -2.0f;
    self->scrollMultiplier = 0.0f;
    self->amountScrolled = 0.0f;
    self->selectedElement = -1;
    self->lastClicked = 0;
    self->field_25123_p = 1;
    self->field_27262_q = 0;
    self->field_27261_r = 0;
    self->scrollUpButtonID = 0;
    self->scrollDownButtonID = 0;
}

void GuiSlot_func_27258_a(GuiSlot *self, int var1) { self->field_25123_p = var1; }

void GuiSlot_func_27259_a(GuiSlot *self, int var1, int var2) {
    self->field_27262_q = var1;
    self->field_27261_r = var2;
    if (!var1) {
        self->field_27261_r = 0;
    }
}

int GuiSlot_getContentHeight_default(GuiSlot *self) {
    return self->vtable->getSize(self) * self->posZ + self->field_27261_r;
}

int GuiSlot_func_27256_c(GuiSlot *self, int var1, int var2) {
    int var3 = self->width / 2 - 110;
    int var4 = self->width / 2 + 110;
    int var5 = var2 - self->top - self->field_27261_r + (int)self->amountScrolled - 4;
    int var6 = var5 / self->posZ;
    return (var1 >= var3 && var1 <= var4 && var6 >= 0 && var5 >= 0 &&
            var6 < self->vtable->getSize(self))
               ? var6
               : -1;
}

void GuiSlot_registerScrollButtons(GuiSlot *self, void *var1, int var2, int var3) {
    (void)var1;
    self->scrollUpButtonID = var2;
    self->scrollDownButtonID = var3;
}

static void GuiSlot_bindAmountScrolled(GuiSlot *self) {
    int var1 = self->vtable->getContentHeight(self) - (self->bottom - self->top - 4);
    if (var1 < 0) {
        var1 /= 2;
    }
    if (self->amountScrolled < 0.0f) {
        self->amountScrolled = 0.0f;
    }
    if (self->amountScrolled > (float)var1) {
        self->amountScrolled = (float)var1;
    }
}

void GuiSlot_actionPerformed(GuiSlot *self, struct GuiButton *var1) {
    if (var1->enabled) {
        if (var1->id == self->scrollUpButtonID) {
            self->amountScrolled -= (float)(self->posZ * 2 / 3);
            self->initialClickY = -2.0f;
            GuiSlot_bindAmountScrolled(self);
        } else if (var1->id == self->scrollDownButtonID) {
            self->amountScrolled += (float)(self->posZ * 2 / 3);
            self->initialClickY = -2.0f;
            GuiSlot_bindAmountScrolled(self);
        }
    }
}

static void GuiSlot_overlayBackground(GuiSlot *self, int var1, int var2, int var3, int var4) {
    Tessellator *var5 = &Tessellator_instance;
    unsigned int texId = RenderEngine_getTexture(self->mc->renderEngine, "/gui/background.png");
    R_bindTexture(texId);
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    float var6 = 32.0f;
    Tessellator_startDrawingQuads(var5);
    Tessellator_setColorRGBA_I(var5, 4210752, var4);
    Tessellator_addVertexWithUV(var5, 0.0, (double)var2, 0.0, 0.0, (double)((float)var2 / var6));
    Tessellator_addVertexWithUV(var5, (double)self->width, (double)var2, 0.0,
                                (double)((float)self->width / var6), (double)((float)var2 / var6));
    Tessellator_setColorRGBA_I(var5, 4210752, var3);
    Tessellator_addVertexWithUV(var5, (double)self->width, (double)var1, 0.0,
                                (double)((float)self->width / var6), (double)((float)var1 / var6));
    Tessellator_addVertexWithUV(var5, 0.0, (double)var1, 0.0, 0.0, (double)((float)var1 / var6));
    Tessellator_draw(var5);
}

void GuiSlot_drawScreen(GuiSlot *self, int var1, int var2, float var3) {
    (void)var3;
    self->vtable->drawBackground(self);
    int var4 = self->vtable->getSize(self);
    int var5 = self->width / 2 + 124;
    int var6 = var5 + 6;
    int var9, var10, var11, var13, var19;

    if (Minecraft_isMouseButtonDown(0)) {
        if (self->initialClickY == -1.0f) {
            int var7 = 1;
            if (var2 >= self->top && var2 <= self->bottom) {
                int var8 = self->width / 2 - 110;
                var9 = self->width / 2 + 110;
                var10 = var2 - self->top - self->field_27261_r + (int)self->amountScrolled - 4;
                var11 = var10 / self->posZ;
                if (var1 >= var8 && var1 <= var9 && var11 >= 0 && var10 >= 0 && var11 < var4) {
                    int var12 = (var11 == self->selectedElement &&
                                 currentTimeMillis() - self->lastClicked < 250L)
                                    ? 1
                                    : 0;

                    struct Minecraft *saved_mc = self->mc;
                    struct GuiScreen *saved_screen = saved_mc->currentScreen;
                    self->vtable->elementClicked(self, var11, var12);
                    if (saved_mc->currentScreen != saved_screen)
                        return;
                    self->selectedElement = var11;
                    self->lastClicked = currentTimeMillis();
                } else if (var1 >= var8 && var1 <= var9 && var10 < 0) {
                    if (self->vtable->func_27255_a)
                        self->vtable->func_27255_a(
                            self, var1 - var8, var2 - self->top + (int)self->amountScrolled - 4);
                    var7 = 0;
                }
                if (var1 >= var5 && var1 <= var6) {
                    self->scrollMultiplier = -1.0f;
                    var19 = self->vtable->getContentHeight(self) - (self->bottom - self->top - 4);
                    if (var19 < 1) {
                        var19 = 1;
                    }
                    var13 = (int)((float)((self->bottom - self->top) * (self->bottom - self->top)) /
                                  (float)self->vtable->getContentHeight(self));
                    if (var13 < 32) {
                        var13 = 32;
                    }
                    if (var13 > self->bottom - self->top - 8) {
                        var13 = self->bottom - self->top - 8;
                    }
                    self->scrollMultiplier /=
                        (float)(self->bottom - self->top - var13) / (float)var19;
                } else {
                    self->scrollMultiplier = 1.0f;
                }
                if (var7) {
                    self->initialClickY = (float)var2;
                } else {
                    self->initialClickY = -2.0f;
                }
            } else {
                self->initialClickY = -2.0f;
            }
        } else if (self->initialClickY >= 0.0f) {
            self->amountScrolled -= ((float)var2 - self->initialClickY) * self->scrollMultiplier;
            self->initialClickY = (float)var2;
        }
    } else {
        self->initialClickY = -1.0f;
    }

    GuiSlot_bindAmountScrolled(self);
    R_disable(R_LIGHTING);
    R_disable(R_FOG);
    Tessellator *var16 = &Tessellator_instance;
    unsigned int texId = RenderEngine_getTexture(self->mc->renderEngine, "/gui/background.png");
    R_bindTexture(texId);
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    float var17 = 32.0f;
    Tessellator_startDrawingQuads(var16);
    Tessellator_setColorOpaque_I(var16, 2105376);
    Tessellator_addVertexWithUV(
        var16, (double)self->left, (double)self->bottom, 0.0, (double)((float)self->left / var17),
        (double)((float)(self->bottom + (int)self->amountScrolled) / var17));
    Tessellator_addVertexWithUV(
        var16, (double)self->right, (double)self->bottom, 0.0, (double)((float)self->right / var17),
        (double)((float)(self->bottom + (int)self->amountScrolled) / var17));
    Tessellator_addVertexWithUV(var16, (double)self->right, (double)self->top, 0.0,
                                (double)((float)self->right / var17),
                                (double)((float)(self->top + (int)self->amountScrolled) / var17));
    Tessellator_addVertexWithUV(var16, (double)self->left, (double)self->top, 0.0,
                                (double)((float)self->left / var17),
                                (double)((float)(self->top + (int)self->amountScrolled) / var17));
    Tessellator_draw(var16);
    var9 = self->width / 2 - 92 - 16;
    var10 = self->top + 4 - (int)self->amountScrolled;
    if (self->field_27262_q) {
        if (self->vtable->func_27260_a)
            self->vtable->func_27260_a(self, var9, var10, var16);
    }

    int var14;
    for (var11 = 0; var11 < var4; ++var11) {
        var19 = var10 + var11 * self->posZ + self->field_27261_r;
        var13 = self->posZ - 4;
        if (var19 <= self->bottom && var19 + var13 >= self->top) {
            if (self->field_25123_p && self->vtable->isSelected(self, var11)) {
                var14 = self->width / 2 - 110;
                int var15 = self->width / 2 + 110;
                R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
                R_disable(R_TEXTURE_2D);
                Tessellator_startDrawingQuads(var16);
                Tessellator_setColorOpaque_I(var16, 8421504);
                Tessellator_addVertexWithUV(var16, (double)var14, (double)(var19 + var13 + 2), 0.0,
                                            0.0, 1.0);
                Tessellator_addVertexWithUV(var16, (double)var15, (double)(var19 + var13 + 2), 0.0,
                                            1.0, 1.0);
                Tessellator_addVertexWithUV(var16, (double)var15, (double)(var19 - 2), 0.0, 1.0,
                                            0.0);
                Tessellator_addVertexWithUV(var16, (double)var14, (double)(var19 - 2), 0.0, 0.0,
                                            0.0);
                Tessellator_setColorOpaque_I(var16, 0);
                Tessellator_addVertexWithUV(var16, (double)(var14 + 1), (double)(var19 + var13 + 1),
                                            0.0, 0.0, 1.0);
                Tessellator_addVertexWithUV(var16, (double)(var15 - 1), (double)(var19 + var13 + 1),
                                            0.0, 1.0, 1.0);
                Tessellator_addVertexWithUV(var16, (double)(var15 - 1), (double)(var19 - 1), 0.0,
                                            1.0, 0.0);
                Tessellator_addVertexWithUV(var16, (double)(var14 + 1), (double)(var19 - 1), 0.0,
                                            0.0, 0.0);
                Tessellator_draw(var16);
                R_enable(R_TEXTURE_2D);
            }
            self->vtable->drawSlot(self, var11, var9, var19, var13, var16);
        }
    }

    R_disable(R_DEPTH_TEST);

    int var18 = 4;
    GuiSlot_overlayBackground(self, 0, self->top, 255, 255);
    GuiSlot_overlayBackground(self, self->bottom, self->height, 255, 255);
    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    R_disable(R_ALPHA_TEST);
    R_shadeModel(R_SMOOTH);
    R_disable(R_TEXTURE_2D);
    Tessellator_startDrawingQuads(var16);
    Tessellator_setColorRGBA_I(var16, 0, 0);
    Tessellator_addVertexWithUV(var16, (double)self->left, (double)(self->top + var18), 0.0, 0.0,
                                1.0);
    Tessellator_addVertexWithUV(var16, (double)self->right, (double)(self->top + var18), 0.0, 1.0,
                                1.0);
    Tessellator_setColorRGBA_I(var16, 0, 255);
    Tessellator_addVertexWithUV(var16, (double)self->right, (double)self->top, 0.0, 1.0, 0.0);
    Tessellator_addVertexWithUV(var16, (double)self->left, (double)self->top, 0.0, 0.0, 0.0);
    Tessellator_draw(var16);
    Tessellator_startDrawingQuads(var16);
    Tessellator_setColorRGBA_I(var16, 0, 255);
    Tessellator_addVertexWithUV(var16, (double)self->left, (double)self->bottom, 0.0, 0.0, 1.0);
    Tessellator_addVertexWithUV(var16, (double)self->right, (double)self->bottom, 0.0, 1.0, 1.0);
    Tessellator_setColorRGBA_I(var16, 0, 0);
    Tessellator_addVertexWithUV(var16, (double)self->right, (double)(self->bottom - var18), 0.0,
                                1.0, 0.0);
    Tessellator_addVertexWithUV(var16, (double)self->left, (double)(self->bottom - var18), 0.0, 0.0,
                                0.0);
    Tessellator_draw(var16);
    var19 = self->vtable->getContentHeight(self) - (self->bottom - self->top - 4);
    if (var19 > 0) {
        var13 = (self->bottom - self->top) * (self->bottom - self->top) /
                self->vtable->getContentHeight(self);
        if (var13 < 32) {
            var13 = 32;
        }
        if (var13 > self->bottom - self->top - 8) {
            var13 = self->bottom - self->top - 8;
        }
        var14 = (int)self->amountScrolled * (self->bottom - self->top - var13) / var19 + self->top;
        if (var14 < self->top) {
            var14 = self->top;
        }
        Tessellator_startDrawingQuads(var16);
        Tessellator_setColorRGBA_I(var16, 0, 255);
        Tessellator_addVertexWithUV(var16, (double)var5, (double)self->bottom, 0.0, 0.0, 1.0);
        Tessellator_addVertexWithUV(var16, (double)var6, (double)self->bottom, 0.0, 1.0, 1.0);
        Tessellator_addVertexWithUV(var16, (double)var6, (double)self->top, 0.0, 1.0, 0.0);
        Tessellator_addVertexWithUV(var16, (double)var5, (double)self->top, 0.0, 0.0, 0.0);
        Tessellator_draw(var16);
        Tessellator_startDrawingQuads(var16);
        Tessellator_setColorRGBA_I(var16, 8421504, 255);
        Tessellator_addVertexWithUV(var16, (double)var5, (double)(var14 + var13), 0.0, 0.0, 1.0);
        Tessellator_addVertexWithUV(var16, (double)var6, (double)(var14 + var13), 0.0, 1.0, 1.0);
        Tessellator_addVertexWithUV(var16, (double)var6, (double)var14, 0.0, 1.0, 0.0);
        Tessellator_addVertexWithUV(var16, (double)var5, (double)var14, 0.0, 0.0, 0.0);
        Tessellator_draw(var16);
        Tessellator_startDrawingQuads(var16);
        Tessellator_setColorRGBA_I(var16, 12632256, 255);
        Tessellator_addVertexWithUV(var16, (double)var5, (double)(var14 + var13 - 1), 0.0, 0.0,
                                    1.0);
        Tessellator_addVertexWithUV(var16, (double)(var6 - 1), (double)(var14 + var13 - 1), 0.0,
                                    1.0, 1.0);
        Tessellator_addVertexWithUV(var16, (double)(var6 - 1), (double)var14, 0.0, 1.0, 0.0);
        Tessellator_addVertexWithUV(var16, (double)var5, (double)var14, 0.0, 0.0, 0.0);
        Tessellator_draw(var16);
    }

    if (self->vtable->func_27257_b)
        self->vtable->func_27257_b(self, var1, var2);
    R_enable(R_TEXTURE_2D);
    R_shadeModel(R_FLAT);
    R_enable(R_ALPHA_TEST);
    R_disable(R_BLEND);
}
