#include "Renderer.h"

#include "GuiAchievement.h"
#include "StatCollector.h"
#include "RenderHelper.h"
#include "FontRenderer.h"
#include <time.h>
#include <stddef.h>

#include "RenderItem.h"
#include "ScaledResolution.h"
#include "RenderEngine.h"

static int64_t millis_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000LL + (int64_t)ts.tv_nsec / 1000000LL;
}

void GuiAchievement_construct(GuiAchievement *self, Minecraft *var1) {
    Gui_init(&self->base);
    self->theGame = var1;
    self->achievementWindowWidth = 0;
    self->achievementWindowHeight = 0;
    self->field_25085_d = NULL;
    self->field_25084_e = NULL;
    self->theAchievement = NULL;
    self->field_25083_f = 0L;
    self->field_27103_i = 0;

    self->itemRender = RenderItem_create();
}

void GuiAchievement_queueTakenAchievement(GuiAchievement *self, Achievement *var1) {
    self->field_25085_d = StatCollector_translateToLocal("achievement.get");
    self->field_25084_e = var1->base.statName;
    self->field_25083_f = millis_now();
    self->theAchievement = var1;
    self->field_27103_i = 0;
}

void GuiAchievement_queueAchievementInformation(GuiAchievement *self, Achievement *var1) {
    self->field_25085_d = var1->base.statName;
    self->field_25084_e = Achievement_getDescription(var1);
    self->field_25083_f = millis_now() - 2500LL;
    self->theAchievement = var1;
    self->field_27103_i = 1;
}

void GuiAchievement_updateAchievementWindowScale(GuiAchievement *self) {
    R_viewport(0, 0, self->theGame->displayWidth, self->theGame->displayHeight);
    R_matrixMode(R_PROJECTION);
    R_loadIdentity();
    R_matrixMode(R_MODELVIEW);
    R_loadIdentity();
    self->achievementWindowWidth = self->theGame->displayWidth;
    self->achievementWindowHeight = self->theGame->displayHeight;

    ScaledResolution var1;
    ScaledResolution_init(&var1, self->theGame->gameSettings, self->theGame->displayWidth,
                          self->theGame->displayHeight);
    self->achievementWindowWidth = ScaledResolution_getScaledWidth(&var1);
    self->achievementWindowHeight = ScaledResolution_getScaledHeight(&var1);
    R_clear(R_DEPTH_BUFFER_BIT);
    R_matrixMode(R_PROJECTION);
    R_loadIdentity();
    R_ortho(0.0, (double)self->achievementWindowWidth, (double)self->achievementWindowHeight, 0.0,
            1000.0, 3000.0);
    R_matrixMode(R_MODELVIEW);
    R_loadIdentity();
    R_translatef(0.0F, 0.0F, -2000.0F);
}

void GuiAchievement_updateAchievementWindow(GuiAchievement *self) {
    if (Minecraft_hasPaidCheckTime > 0L) {
        R_disable(R_DEPTH_TEST);
        R_depthMask(R_FALSE);
        RenderHelper_disableStandardItemLighting();
        GuiAchievement_updateAchievementWindowScale(self);
        const char *var1 = "Minecraft Beta 1.7.3   Unlicensed Copy :(";
        const char *var2 = "(Or logged in from another location)";
        const char *var3 = "Purchase at minecraft.net";
        FontRenderer_drawStringWithShadow(self->theGame->fontRenderer, var1, 2, 2, 16777215);
        FontRenderer_drawStringWithShadow(self->theGame->fontRenderer, var2, 2, 11, 16777215);
        FontRenderer_drawStringWithShadow(self->theGame->fontRenderer, var3, 2, 20, 16777215);
        R_depthMask(R_TRUE);
        R_enable(R_DEPTH_TEST);
    }

    if (self->theAchievement != NULL && self->field_25083_f != 0L) {
        double var8 = (double)(millis_now() - self->field_25083_f) / 3000.0;
        if (self->field_27103_i || self->field_27103_i || (var8 >= 0.0 && var8 <= 1.0)) {
            GuiAchievement_updateAchievementWindowScale(self);
            R_disable(R_DEPTH_TEST);
            R_depthMask(R_FALSE);
            double var9 = var8 * 2.0;
            if (var9 > 1.0) {
                var9 = 2.0 - var9;
            }

            var9 *= 4.0;
            var9 = 1.0 - var9;
            if (var9 < 0.0) {
                var9 = 0.0;
            }

            var9 *= var9;
            var9 *= var9;
            int var5 = self->achievementWindowWidth - 160;
            double var9_36 = var9 * 36.0;
            int var6 = 0 - (int)(var9_36 > 2147483647.0 ? 2147483647.0 : var9_36);
            unsigned int var7 =
                RenderEngine_getTexture(self->theGame->renderEngine, "/achievement/bg.png");
            R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
            R_enable(R_TEXTURE_2D);
            R_bindTexture(var7);
            R_disable(R_LIGHTING);
            Gui_drawTexturedModalRect(&self->base, var5, var6, 96, 202, 160, 32);
            if (self->field_27103_i) {
                FontRenderer_func_27278_a(self->theGame->fontRenderer, self->field_25084_e,
                                          var5 + 30, var6 + 7, 120, -1);
            } else {
                FontRenderer_drawString(self->theGame->fontRenderer, self->field_25085_d, var5 + 30,
                                        var6 + 7, -256);
                FontRenderer_drawString(self->theGame->fontRenderer, self->field_25084_e, var5 + 30,
                                        var6 + 18, -1);
            }

            R_pushMatrix();
            R_rotatef(180.0F, 1.0F, 0.0F, 0.0F);
            RenderHelper_enableStandardItemLighting();
            R_popMatrix();
            R_disable(R_LIGHTING);
            R_enable(R_RESCALE_NORMAL);
            R_enable(R_COLOR_MATERIAL);
            R_enable(R_LIGHTING);

            RenderItem_renderItemIntoGUI(self->itemRender, self->theGame->fontRenderer,
                                         self->theGame->renderEngine,
                                         &self->theAchievement->theItemStack, var5 + 8, var6 + 8);
            R_disable(R_LIGHTING);
            R_depthMask(R_TRUE);
            R_enable(R_DEPTH_TEST);
        } else {
            self->field_25083_f = 0L;
        }
    }
}
