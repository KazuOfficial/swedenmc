#include "Renderer.h"

#include "LoadingScreenRenderer.h"
#include "Minecraft.h"
#include "ScaledResolution.h"
#include "Tessellator.h"
#include "FontRenderer.h"
#include "RenderEngine.h"
#include "MinecraftError.h"
#include <GLFW/glfw3.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <stdint.h>
#include <time.h>

static int64_t get_millis(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)ts.tv_sec * 1000LL + (int64_t)(ts.tv_nsec / 1000000);
}

static void lsr_func_594_b(IProgressUpdate *self, const char *str) {
    LoadingScreenRenderer_func_594_b((LoadingScreenRenderer *)self, str);
}
static void lsr_displayLoadingString(IProgressUpdate *self, const char *str) {
    LoadingScreenRenderer_displayLoadingString((LoadingScreenRenderer *)self, str);
}
static void lsr_setLoadingProgress(IProgressUpdate *self, int progress) {
    LoadingScreenRenderer_setLoadingProgress((LoadingScreenRenderer *)self, progress);
}

void LoadingScreenRenderer_init(LoadingScreenRenderer *self, Minecraft *var1) {
    self->iProgressUpdate.func_594_b = lsr_func_594_b;
    self->iProgressUpdate.displayLoadingString = lsr_displayLoadingString;
    self->iProgressUpdate.setLoadingProgress = lsr_setLoadingProgress;
    self->mc = var1;
    self->field_1004_a = "";
    self->field_1007_c = "";
    self->field_1006_d = get_millis();
    self->field_1005_e = 0;
}

void LoadingScreenRenderer_printText(LoadingScreenRenderer *self, const char *var1) {
    self->field_1005_e = 0;
    LoadingScreenRenderer_func_597_c(self, var1);
}

void LoadingScreenRenderer_func_594_b(LoadingScreenRenderer *self, const char *var1) {
    (void)var1;
    self->field_1005_e = 1;
    LoadingScreenRenderer_func_597_c(self, self->field_1007_c);
}

void LoadingScreenRenderer_func_597_c(LoadingScreenRenderer *self, const char *var1) {
    if (!self->mc->running) {
        if (!self->field_1005_e) {
            MinecraftError_throw();
        }
    } else {
        self->field_1007_c = var1;
        ScaledResolution var2;
        ScaledResolution_init(&var2, self->mc->gameSettings, self->mc->displayWidth,
                              self->mc->displayHeight);
        R_clear(R_DEPTH_BUFFER_BIT);
        R_matrixMode(R_PROJECTION);
        R_loadIdentity();
        R_ortho(0.0, var2.field_25121_a, var2.field_25120_b, 0.0, 100.0, 300.0);
        R_matrixMode(R_MODELVIEW);
        R_loadIdentity();
        R_translatef(0.0F, 0.0F, -200.0F);
    }
}

void LoadingScreenRenderer_displayLoadingString(LoadingScreenRenderer *self, const char *var1) {
    if (!self->mc->running) {
        if (!self->field_1005_e) {
            MinecraftError_throw();
        }
    } else {
        self->field_1006_d = 0L;
        self->field_1004_a = var1;
        LoadingScreenRenderer_setLoadingProgress(self, -1);
        self->field_1006_d = 0L;
    }
}

void LoadingScreenRenderer_setLoadingProgress(LoadingScreenRenderer *self, int var1) {
    if (!self->mc->running) {
        if (!self->field_1005_e) {
            MinecraftError_throw();
        }
    } else {
        int64_t var2 = get_millis();
        if (var2 - self->field_1006_d >= 20L) {
            self->field_1006_d = var2;
            ScaledResolution var4;
            ScaledResolution_init(&var4, self->mc->gameSettings, self->mc->displayWidth,
                                  self->mc->displayHeight);
            int var5 = ScaledResolution_getScaledWidth(&var4);
            int var6 = ScaledResolution_getScaledHeight(&var4);
            R_clear(R_DEPTH_BUFFER_BIT);
            R_matrixMode(R_PROJECTION);
            R_loadIdentity();
            R_ortho(0.0, var4.field_25121_a, var4.field_25120_b, 0.0, 100.0, 300.0);
            R_matrixMode(R_MODELVIEW);
            R_loadIdentity();
            R_translatef(0.0F, 0.0F, -200.0F);
            R_clear(R_DEPTH_BUFFER_BIT | R_COLOR_BUFFER_BIT);
            Tessellator *var7 = &Tessellator_instance;
            int var8 = RenderEngine_getTexture(self->mc->renderEngine, "/gui/background.png");
            R_bindTexture(var8);
            float var9 = 32.0F;
            Tessellator_startDrawingQuads(var7);
            Tessellator_setColorOpaque_I(var7, 4210752);
            Tessellator_addVertexWithUV(var7, 0.0, (double)var6, 0.0, 0.0,
                                        (double)((float)var6 / var9));
            Tessellator_addVertexWithUV(var7, (double)var5, (double)var6, 0.0,
                                        (double)((float)var5 / var9), (double)((float)var6 / var9));
            Tessellator_addVertexWithUV(var7, (double)var5, 0.0, 0.0, (double)((float)var5 / var9),
                                        0.0);
            Tessellator_addVertexWithUV(var7, 0.0, 0.0, 0.0, 0.0, 0.0);
            Tessellator_draw(var7);
            if (var1 >= 0) {
                int var10 = 100;
                int var11 = 2;
                int var12 = var5 / 2 - var10 / 2;
                int var13 = var6 / 2 + 16;
                R_disable(R_TEXTURE_2D);
                Tessellator_startDrawingQuads(var7);
                Tessellator_setColorOpaque_I(var7, 8421504);
                Tessellator_addVertex(var7, (double)var12, (double)var13, 0.0);
                Tessellator_addVertex(var7, (double)var12, (double)(var13 + var11), 0.0);
                Tessellator_addVertex(var7, (double)(var12 + var10), (double)(var13 + var11), 0.0);
                Tessellator_addVertex(var7, (double)(var12 + var10), (double)var13, 0.0);
                Tessellator_setColorOpaque_I(var7, 8454016);
                Tessellator_addVertex(var7, (double)var12, (double)var13, 0.0);
                Tessellator_addVertex(var7, (double)var12, (double)(var13 + var11), 0.0);
                Tessellator_addVertex(var7, (double)(var12 + var1), (double)(var13 + var11), 0.0);
                Tessellator_addVertex(var7, (double)(var12 + var1), (double)var13, 0.0);
                Tessellator_draw(var7);
                R_enable(R_TEXTURE_2D);
            }
            FontRenderer_drawStringWithShadow(
                self->mc->fontRenderer, self->field_1007_c,
                (var5 - FontRenderer_getStringWidth(self->mc->fontRenderer, self->field_1007_c)) /
                    2,
                var6 / 2 - 4 - 16, 16777215);
            FontRenderer_drawStringWithShadow(
                self->mc->fontRenderer, self->field_1004_a,
                (var5 - FontRenderer_getStringWidth(self->mc->fontRenderer, self->field_1004_a)) /
                    2,
                var6 / 2 - 4 + 8, 16777215);

            R_endFrame();
            R_beginFrame();
            glfwPollEvents();
#ifdef __EMSCRIPTEN__
            emscripten_sleep(0);
#endif
        }
    }
}
