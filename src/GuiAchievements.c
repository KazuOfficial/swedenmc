#include "Renderer.h"

#include "GuiAchievements.h"
#include "Minecraft.h"
#include "AchievementList.h"
#include "StatCollector.h"
#include "MathHelper.h"
#include "RenderHelper.h"
#include "RenderItem.h"
#include "Render.h"
#include "RenderEngine.h"
#include "Block.h"
#include "JavaRandom.h"
#include <math.h>
#include <time.h>
#include <stddef.h>

#define field_27126_s (AchievementList_minDisplayColumn * 24 - 112)
#define field_27125_t (AchievementList_minDisplayRow * 24 - 112)
#define field_27124_u (AchievementList_maxDisplayColumn * 24 - 77)
#define field_27123_v (AchievementList_maxDisplayRow * 24 - 77)

static int64_t millis_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000LL + (int64_t)ts.tv_nsec / 1000000LL;
}

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void s_vtable_ensure_init(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.initGui = (void (*)(GuiScreen *))GuiAchievements_initGui;
    s_vtable.drawScreen = (void (*)(GuiScreen *, int, int, float))GuiAchievements_drawScreen;
    s_vtable.updateScreen = (void (*)(GuiScreen *))GuiAchievements_updateScreen;
    s_vtable.doesGuiPauseGame = (int (*)(GuiScreen *))GuiAchievements_doesGuiPauseGame;
    s_vtable.actionPerformed =
        (void (*)(GuiScreen *, struct GuiButton *))GuiAchievements_actionPerformed;
    s_vtable.keyTyped = (void (*)(GuiScreen *, char, int))GuiAchievements_keyTyped;
    s_vtable_init = 1;
}

void GuiAchievements_construct(GuiAchievements *self, StatFileWriter *var1) {
    s_vtable_ensure_init();
    self->base.vtable = &s_vtable;
    self->field_27121_a = 256;
    self->field_27119_i = 202;
    self->field_27118_j = 0;
    self->field_27117_l = 0;
    self->field_27122_w = 0;
    self->field_27120_x = var1;
    short var2 = 141;
    short var3 = 141;
    self->field_27116_m = self->field_27114_o = self->field_27112_q =
        (double)(AchievementList_openInventory->displayColumn * 24 - var2 / 2 - 12);
    self->field_27115_n = self->field_27113_p = self->field_27111_r =
        (double)(AchievementList_openInventory->displayRow * 24 - var3 / 2);
}

void GuiAchievements_initGui(GuiAchievements *self) {
    self->base.buttonCount = 0;
    GuiSmallButton *btn = (GuiSmallButton *)calloc(1, sizeof(GuiSmallButton));
    GuiSmallButton_constructSized(btn, 1, self->base.width / 2 + 24, self->base.height / 2 + 74, 80,
                                  20, StatCollector_translateToLocal("gui.done"));
    GuiScreen_addButton((GuiScreen *)self, (GuiButton *)btn);
}

void GuiAchievements_actionPerformed(GuiAchievements *self, struct GuiButton *var1) {
    if (var1->id == 1) {
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)NULL);
        Minecraft_setIngameFocus(self->base.mc);
    }
    GuiScreen_defaultVtable.actionPerformed((GuiScreen *)self, var1);
}

void GuiAchievements_keyTyped(GuiAchievements *self, char var1, int var2) {
    if (var2 == self->base.mc->gameSettings->keyBindInventory.keyCode) {
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)NULL);
        Minecraft_setIngameFocus(self->base.mc);
    } else {
        GuiScreen_defaultVtable.keyTyped((GuiScreen *)self, var1, var2);
    }
}

void GuiAchievements_drawScreen(GuiAchievements *self, int var1, int var2, float var3) {
    if (Minecraft_isMouseButtonDown(0)) {
        int var4 = (self->base.width - self->field_27121_a) / 2;
        int var5 = (self->base.height - self->field_27119_i) / 2;
        int var6 = var4 + 8;
        int var7 = var5 + 17;
        if ((self->field_27122_w == 0 || self->field_27122_w == 1) && var1 >= var6 &&
            var1 < var6 + 224 && var2 >= var7 && var2 < var7 + 155) {
            if (self->field_27122_w == 0) {
                self->field_27122_w = 1;
            } else {
                self->field_27114_o -= (double)(var1 - self->field_27118_j);
                self->field_27113_p -= (double)(var2 - self->field_27117_l);
                self->field_27112_q = self->field_27116_m = self->field_27114_o;
                self->field_27111_r = self->field_27115_n = self->field_27113_p;
            }

            self->field_27118_j = var1;
            self->field_27117_l = var2;
        }

        if (self->field_27112_q < (double)field_27126_s) {
            self->field_27112_q = (double)field_27126_s;
        }

        if (self->field_27111_r < (double)field_27125_t) {
            self->field_27111_r = (double)field_27125_t;
        }

        if (self->field_27112_q >= (double)field_27124_u) {
            self->field_27112_q = (double)(field_27124_u - 1);
        }

        if (self->field_27111_r >= (double)field_27123_v) {
            self->field_27111_r = (double)(field_27123_v - 1);
        }
    } else {
        self->field_27122_w = 0;
    }

    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    GuiAchievements_func_27109_b(self, var1, var2, var3);
    R_disable(R_LIGHTING);
    R_disable(R_DEPTH_TEST);
    GuiAchievements_func_27110_k(self);
    R_enable(R_LIGHTING);
    R_enable(R_DEPTH_TEST);
}

void GuiAchievements_updateScreen(GuiAchievements *self) {
    self->field_27116_m = self->field_27114_o;
    self->field_27115_n = self->field_27113_p;
    double var1 = self->field_27112_q - self->field_27114_o;
    double var3 = self->field_27111_r - self->field_27113_p;
    if (var1 * var1 + var3 * var3 < 4.0) {
        self->field_27114_o += var1;
        self->field_27113_p += var3;
    } else {
        self->field_27114_o += var1 * 0.85;
        self->field_27113_p += var3 * 0.85;
    }
}

void GuiAchievements_func_27110_k(GuiAchievements *self) {
    int var1 = (self->base.width - self->field_27121_a) / 2;
    int var2 = (self->base.height - self->field_27119_i) / 2;
    FontRenderer_drawString(self->base.fontRenderer, "Achievements", var1 + 15, var2 + 5, 4210752);
}

void GuiAchievements_func_27109_b(GuiAchievements *self, int var1, int var2, float var3) {
    int var4 = MathHelper_floor_double(self->field_27116_m +
                                       (self->field_27114_o - self->field_27116_m) * (double)var3);
    int var5 = MathHelper_floor_double(self->field_27115_n +
                                       (self->field_27113_p - self->field_27115_n) * (double)var3);
    if (var4 < field_27126_s) {
        var4 = field_27126_s;
    }

    if (var5 < field_27125_t) {
        var5 = field_27125_t;
    }

    if (var4 >= field_27124_u) {
        var4 = field_27124_u - 1;
    }

    if (var5 >= field_27123_v) {
        var5 = field_27123_v - 1;
    }

    unsigned int var6 = RenderEngine_getTexture(self->base.mc->renderEngine, "/terrain.png");
    unsigned int var7 = RenderEngine_getTexture(self->base.mc->renderEngine, "/achievement/bg.png");
    int var8 = (self->base.width - self->field_27121_a) / 2;
    int var9 = (self->base.height - self->field_27119_i) / 2;
    int var10 = var8 + 16;
    int var11 = var9 + 17;
    ((Gui *)self)->zLevel = 0.0F;
    R_depthFunc(R_GEQUAL);
    R_pushMatrix();
    R_translatef(0.0F, 0.0F, -200.0F);
    R_enable(R_TEXTURE_2D);
    R_disable(R_LIGHTING);
    R_enable(R_RESCALE_NORMAL);
    R_enable(R_COLOR_MATERIAL);
    RenderEngine_bindTexture(self->base.mc->renderEngine, var6);
    int var12 = (var4 + 288) >> 4;
    int var13 = (var5 + 288) >> 4;
    int var14 = (var4 + 288) % 16;
    int var15 = (var5 + 288) % 16;
    JavaRandom var21;
    JavaRandom_init(&var21, 0LL);

    for (int var22 = 0; var22 * 16 - var15 < 155; ++var22) {
        float var23 = 0.6F - (float)(var13 + var22) / 25.0F * 0.3F;
        R_color4f(var23, var23, var23, 1.0F);

        for (int var24 = 0; var24 * 16 - var14 < 224; ++var24) {
            JavaRandom_setSeed(&var21, (int64_t)(1234 + var12 + var24));
            JavaRandom_nextIntRaw(&var21);
            int var25 = JavaRandom_nextInt(&var21, 1 + var13 + var22) + (var13 + var22) / 2;
            int var26 = Block_sand->blockIndexInTexture;
            if (var25 <= 37 && var13 + var22 != 35) {
                if (var25 == 22) {
                    if (JavaRandom_nextInt(&var21, 2) == 0) {
                        var26 = Block_oreDiamond->blockIndexInTexture;
                    } else {
                        var26 = Block_oreRedstone->blockIndexInTexture;
                    }
                } else if (var25 == 10) {
                    var26 = Block_oreIron->blockIndexInTexture;
                } else if (var25 == 8) {
                    var26 = Block_oreCoal->blockIndexInTexture;
                } else if (var25 > 4) {
                    var26 = Block_stone->blockIndexInTexture;
                } else if (var25 > 0) {
                    var26 = Block_dirt->blockIndexInTexture;
                }
            } else {
                var26 = Block_bedrock->blockIndexInTexture;
            }

            Gui_drawTexturedModalRect((Gui *)self, var10 + var24 * 16 - var14,
                                      var11 + var22 * 16 - var15, var26 % 16 << 4, var26 >> 4 << 4,
                                      16, 16);
        }
    }

    R_enable(R_DEPTH_TEST);
    R_depthFunc(R_LEQUAL);
    R_disable(R_TEXTURE_2D);

    int var16;
    int var17;
    int var33;
    int var38;
    for (var12 = 0; var12 < AchievementList_achievementList_count; ++var12) {
        Achievement *var28 = AchievementList_achievementList[var12];
        if (var28->parentAchievement != NULL) {
            var14 = var28->displayColumn * 24 - var4 + 11 + var10;
            var15 = var28->displayRow * 24 - var5 + 11 + var11;
            var16 = var28->parentAchievement->displayColumn * 24 - var4 + 11 + var10;
            var17 = var28->parentAchievement->displayRow * 24 - var5 + 11 + var11;
            int var18 = 0;
            int var19 = StatFileWriter_hasAchievementUnlocked(self->field_27120_x, var28);
            int var20 = StatFileWriter_func_27181_b(self->field_27120_x, var28);
            var38 = sin((double)(millis_now() % 600LL) / 600.0 * M_PI * 2.0) > 0.6 ? 255 : 130;
            if (var19) {
                var33 = -9408400;
            } else if (var20) {
                var33 = (int)(0x0000ff00U + ((unsigned int)var38 << 24));
            } else {
                var33 = -16777216;
            }

            Gui_func_27100_a((Gui *)self, var14, var16, var15, var33);
            Gui_func_27099_b((Gui *)self, var16, var15, var17, var33);
            (void)var18;
        }
    }

    Achievement *var27 = NULL;

    RenderItem *var29 = RenderItem_create();
    R_pushMatrix();
    R_rotatef(180.0F, 1.0F, 0.0F, 0.0F);
    RenderHelper_enableStandardItemLighting();
    R_popMatrix();
    R_disable(R_LIGHTING);
    R_enable(R_RESCALE_NORMAL);
    R_enable(R_COLOR_MATERIAL);

    int var34;
    for (var14 = 0; var14 < AchievementList_achievementList_count; ++var14) {
        Achievement *var30 = AchievementList_achievementList[var14];
        var16 = var30->displayColumn * 24 - var4;
        var17 = var30->displayRow * 24 - var5;
        if (var16 >= -24 && var17 >= -24 && var16 <= 224 && var17 <= 155) {
            float var35;
            if (StatFileWriter_hasAchievementUnlocked(self->field_27120_x, var30)) {
                var35 = 1.0F;
                R_color4f(var35, var35, var35, 1.0F);
            } else if (StatFileWriter_func_27181_b(self->field_27120_x, var30)) {
                var35 =
                    sin((double)(millis_now() % 600LL) / 600.0 * M_PI * 2.0) < 0.6 ? 0.6F : 0.8F;
                R_color4f(var35, var35, var35, 1.0F);
            } else {
                var35 = 0.3F;
                R_color4f(var35, var35, var35, 1.0F);
            }

            RenderEngine_bindTexture(self->base.mc->renderEngine, var7);
            var33 = var10 + var16;
            var34 = var11 + var17;
            if (Achievement_getSpecial(var30)) {
                Gui_drawTexturedModalRect((Gui *)self, var33 - 2, var34 - 2, 26, 202, 26, 26);
            } else {
                Gui_drawTexturedModalRect((Gui *)self, var33 - 2, var34 - 2, 0, 202, 26, 26);
            }

            if (!StatFileWriter_func_27181_b(self->field_27120_x, var30)) {
                float var36 = 0.1F;
                R_color4f(var36, var36, var36, 1.0F);

                var29->field_27004_a = 0;
            }

            R_enable(R_LIGHTING);
            R_enable(R_CULL_FACE);

            RenderItem_renderItemIntoGUI(var29, self->base.mc->fontRenderer,
                                         self->base.mc->renderEngine, &var30->theItemStack,
                                         var33 + 3, var34 + 3);
            R_disable(R_LIGHTING);
            if (!StatFileWriter_func_27181_b(self->field_27120_x, var30)) {

                var29->field_27004_a = 1;
            }

            R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
            if (var1 >= var10 && var2 >= var11 && var1 < var10 + 224 && var2 < var11 + 155 &&
                var1 >= var33 && var1 <= var33 + 22 && var2 >= var34 && var2 <= var34 + 22) {
                var27 = var30;
            }
        }
    }

    R_disable(R_DEPTH_TEST);
    R_enable(R_BLEND);
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
    RenderEngine_bindTexture(self->base.mc->renderEngine, var7);
    Gui_drawTexturedModalRect((Gui *)self, var8, var9, 0, 0, self->field_27121_a,
                              self->field_27119_i);
    R_popMatrix();
    ((Gui *)self)->zLevel = 0.0F;
    R_depthFunc(R_LEQUAL);
    R_disable(R_DEPTH_TEST);
    R_enable(R_TEXTURE_2D);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
    if (var27 != NULL) {
        const char *var31 = var27->base.statName;
        const char *var32 = Achievement_getDescription(var27);
        var17 = var1 + 12;
        var33 = var2 - 4;
        if (StatFileWriter_func_27181_b(self->field_27120_x, var27)) {
            var34 = var31 && self->base.fontRenderer
                        ? FontRenderer_getStringWidth(self->base.fontRenderer, var31)
                        : 0;
            var34 = var34 > 120 ? var34 : 120;
            int var37 = FontRenderer_func_27277_a(self->base.fontRenderer, var32, var34);
            if (StatFileWriter_hasAchievementUnlocked(self->field_27120_x, var27)) {
                var37 += 12;
            }

            Gui_drawGradientRect((Gui *)self, var17 - 3, var33 - 3, var17 + var34 + 3,
                                 var33 + var37 + 3 + 12, -1073741824, -1073741824);
            FontRenderer_func_27278_a(self->base.fontRenderer, var32, var17, var33 + 12, var34,
                                      -6250336);
            if (StatFileWriter_hasAchievementUnlocked(self->field_27120_x, var27)) {
                FontRenderer_drawStringWithShadow(
                    self->base.fontRenderer, StatCollector_translateToLocal("achievement.taken"),
                    var17, var33 + var37 + 4, -7302913);
            }
        } else {
            var34 = var31 && self->base.fontRenderer
                        ? FontRenderer_getStringWidth(self->base.fontRenderer, var31)
                        : 0;
            var34 = var34 > 120 ? var34 : 120;
            const char *var39 = StatCollector_translateToLocalFormatted(
                "achievement.requires", var27->parentAchievement->base.statName);
            var38 = FontRenderer_func_27277_a(self->base.fontRenderer, var39, var34);
            Gui_drawGradientRect((Gui *)self, var17 - 3, var33 - 3, var17 + var34 + 3,
                                 var33 + var38 + 12 + 3, -1073741824, -1073741824);
            FontRenderer_func_27278_a(self->base.fontRenderer, var39, var17, var33 + 12, var34,
                                      -9416624);
        }

        FontRenderer_drawStringWithShadow(
            self->base.fontRenderer, var31, var17, var33,
            StatFileWriter_func_27181_b(self->field_27120_x, var27)
                ? (Achievement_getSpecial(var27) ? -128 : -1)
                : (Achievement_getSpecial(var27) ? -8355776 : -8355712));
    }

    R_enable(R_DEPTH_TEST);
    R_enable(R_LIGHTING);
    RenderHelper_disableStandardItemLighting();
    Render_destroy(&var29->base);
    free(var29->renderBlocks);
    free(var29);
}

int GuiAchievements_doesGuiPauseGame(GuiAchievements *self) {
    (void)self;
    return 1;
}
