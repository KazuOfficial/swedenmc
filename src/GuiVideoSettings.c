#include "GuiVideoSettings.h"
#include "GuiSmallButton.h"
#include "GuiSlider.h"
#include "StringTranslate.h"
#include "ScaledResolution.h"
#include "Minecraft.h"
#include <string.h>
#include <stdlib.h>

const EnumOptions GuiVideoSettings_field_22108_k[] = {
    EnumOptions_GRAPHICS,        EnumOptions_RENDER_DISTANCE, EnumOptions_AMBIENT_OCCLUSION,
    EnumOptions_FRAMERATE_LIMIT, EnumOptions_ANAGLYPH,        EnumOptions_VIEW_BOBBING,
    EnumOptions_GUI_SCALE,       EnumOptions_ADVANCED_OPENGL};
const int GuiVideoSettings_field_22108_k_len = 8;

static void s_initGui(GuiScreen *self) { GuiVideoSettings_initGui((GuiVideoSettings *)self); }
static void s_actionPerformed(GuiScreen *self, GuiButton *btn) {
    GuiVideoSettings_actionPerformed((GuiVideoSettings *)self, btn);
}
static void s_drawScreen(GuiScreen *self, int a, int b, float c) {
    GuiVideoSettings_drawScreen((GuiVideoSettings *)self, a, b, c);
}

static GuiScreenVtable s_vtable;
static int s_vtable_inited = 0;

static const GuiScreenVtable *GuiVideoSettings_getVtable(void) {
    if (!s_vtable_inited) {
        GuiScreen_initVtable(&s_vtable);
        s_vtable.initGui = s_initGui;
        s_vtable.actionPerformed = s_actionPerformed;
        s_vtable.drawScreen = s_drawScreen;
        s_vtable_inited = 1;
    }
    return &s_vtable;
}

void GuiVideoSettings_construct(GuiVideoSettings *self, GuiScreen *var1, GameSettings *var2) {
    memset(self, 0, sizeof(*self));
    self->base.vtable = GuiVideoSettings_getVtable();
    self->field_22110_h = var1;
    self->base.parentScreen = var1;
    self->guiGameSettings = var2;
    strncpy(self->field_22107_a, "Video Settings", sizeof(self->field_22107_a) - 1);
}

void GuiVideoSettings_initGui(GuiVideoSettings *self) {
    StringTranslate *var1 = StringTranslate_getInstance();
    strncpy(self->field_22107_a, StringTranslate_translateKey(var1, "options.videoTitle"),
            sizeof(self->field_22107_a) - 1);
    int var2 = 0;
    const EnumOptions *var3 = GuiVideoSettings_field_22108_k;
    int var4 = GuiVideoSettings_field_22108_k_len;
    for (int var5 = 0; var5 < var4; ++var5) {
        EnumOptions var6 = var3[var5];
        if (!EnumOptions_getEnumFloat(var6)) {
            GuiSmallButton *btn = (GuiSmallButton *)calloc(1, sizeof(GuiSmallButton));
            GuiSmallButton_constructEnum(btn, EnumOptions_returnEnumOrdinal(var6),
                                         self->base.width / 2 - 155 + var2 % 2 * 160,
                                         self->base.height / 6 + 24 * (var2 >> 1), var6,
                                         GameSettings_getKeyBinding(self->guiGameSettings, var6));
            GuiScreen_addButton(&self->base, (GuiButton *)btn);
        } else {
            GuiSlider *sld = (GuiSlider *)calloc(1, sizeof(GuiSlider));
            GuiSlider_construct(sld, EnumOptions_returnEnumOrdinal(var6),
                                self->base.width / 2 - 155 + var2 % 2 * 160,
                                self->base.height / 6 + 24 * (var2 >> 1), var6,
                                GameSettings_getKeyBinding(self->guiGameSettings, var6),
                                GameSettings_getOptionFloatValue(self->guiGameSettings, var6));
            GuiScreen_addButton(&self->base, (GuiButton *)sld);
        }
        ++var2;
    }
    GuiButton *btnDone =
        GuiButton_create(200, self->base.width / 2 - 100, self->base.height / 6 + 168,
                         StringTranslate_translateKey(var1, "gui.done"));
    GuiScreen_addButton(&self->base, btnDone);
}

void GuiVideoSettings_actionPerformed(GuiVideoSettings *self, GuiButton *var1) {
    if (var1->enabled) {
        if (var1->id < 100) {

            GameSettings_setOptionValue(self->guiGameSettings,
                                        ((GuiSmallButton *)var1)->enumOptions, 1);
            EnumOptions *opt = EnumOptions_getEnumOptions(var1->id);
            if (opt)
                strncpy(var1->displayString,
                        GameSettings_getKeyBinding(self->guiGameSettings, *opt),
                        sizeof(var1->displayString) - 1);
        }
        if (var1->id == 200) {
            GameSettings_saveOptions(self->base.mc->gameSettings);
            Minecraft_displayGuiScreen(self->base.mc, self->field_22110_h);
        }
        ScaledResolution var2;
        ScaledResolution_init(&var2, self->base.mc->gameSettings, self->base.mc->displayWidth,
                              self->base.mc->displayHeight);
        int var3 = ScaledResolution_getScaledWidth(&var2);
        int var4 = ScaledResolution_getScaledHeight(&var2);
        GuiScreen_setWorldAndResolution(&self->base, self->base.mc, var3, var4);
    }
}

void GuiVideoSettings_drawScreen(GuiVideoSettings *self, int var1, int var2, float var3) {
    GuiScreen_drawDefaultBackground(&self->base);
    GuiScreen_drawCenteredString(&self->base, self->base.fontRenderer, self->field_22107_a,
                                 self->base.width / 2, 20, 16777215);
    GuiScreen_defaultVtable.drawScreen(&self->base, var1, var2, var3);
}
