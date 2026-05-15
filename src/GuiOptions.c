#include "GuiOptions.h"
#include "Minecraft.h"
#include "GuiControls.h"
#include "GuiVideoSettings.h"
#include "GuiSmallButton.h"
#include "GuiSlider.h"
#include "StringTranslate.h"
#include <string.h>
#include <stdlib.h>

EnumOptions GuiOptions_field_22135_k[GUI_OPTIONS_FIELD_22135_K_SIZE] = {
    EnumOptions_MUSIC, EnumOptions_SOUND, EnumOptions_INVERT_MOUSE, EnumOptions_SENSITIVITY,
    EnumOptions_DIFFICULTY};

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.initGui = GuiOptions_initGui;
    s_vtable.actionPerformed = GuiOptions_actionPerformed;
    s_vtable.drawScreen = GuiOptions_drawScreen;
    s_vtable_init = 1;
}

void GuiOptions_construct(GuiOptions *self, GuiScreen *var1, GameSettings *var2) {
    ensure_vtable();
    strncpy(self->screenTitle, "Options", sizeof(self->screenTitle) - 1);
    self->screenTitle[sizeof(self->screenTitle) - 1] = '\0';
    self->parentScreen = var1;
    self->base.parentScreen = var1;
    self->options = var2;
    self->base.vtable = &s_vtable;
}

void GuiOptions_initGui(GuiScreen *self_) {
    GuiOptions *self = (GuiOptions *)self_;
    StringTranslate *var1 = StringTranslate_getInstance();
    strncpy(self->screenTitle, StringTranslate_translateKey(var1, "options.title"),
            sizeof(self->screenTitle) - 1);
    int var2 = 0;
    EnumOptions *var3 = GuiOptions_field_22135_k;
    int var4 = GUI_OPTIONS_FIELD_22135_K_SIZE;
    for (int var5 = 0; var5 < var4; ++var5) {
        EnumOptions var6 = var3[var5];
        if (!EnumOptions_getEnumFloat(var6)) {
            GuiSmallButton *btn = (GuiSmallButton *)calloc(1, sizeof(GuiSmallButton));
            GuiSmallButton_constructEnum(btn, EnumOptions_returnEnumOrdinal(var6),
                                         self->base.width / 2 - 155 + var2 % 2 * 160,
                                         self->base.height / 6 + 24 * (var2 >> 1), var6,
                                         GameSettings_getKeyBinding(self->options, var6));
            GuiScreen_addButton((GuiScreen *)self, (GuiButton *)btn);
        } else {
            GuiSlider *btn = (GuiSlider *)calloc(1, sizeof(GuiSlider));
            GuiSlider_construct(btn, EnumOptions_returnEnumOrdinal(var6),
                                self->base.width / 2 - 155 + var2 % 2 * 160,
                                self->base.height / 6 + 24 * (var2 >> 1), var6,
                                GameSettings_getKeyBinding(self->options, var6),
                                GameSettings_getOptionFloatValue(self->options, var6));
            GuiScreen_addButton((GuiScreen *)self, (GuiButton *)btn);
        }
        ++var2;
    }
    GuiButton *btn101 =
        GuiButton_create(101, self->base.width / 2 - 100, self->base.height / 6 + 96 + 12,
                         StringTranslate_translateKey(var1, "options.video"));
    GuiButton *btn100 =
        GuiButton_create(100, self->base.width / 2 - 100, self->base.height / 6 + 120 + 12,
                         StringTranslate_translateKey(var1, "options.controls"));
    GuiButton *btn200 =
        GuiButton_create(200, self->base.width / 2 - 100, self->base.height / 6 + 168,
                         StringTranslate_translateKey(var1, "gui.done"));
    GuiScreen_addButton((GuiScreen *)self, btn101);
    GuiScreen_addButton((GuiScreen *)self, btn100);
    GuiScreen_addButton((GuiScreen *)self, btn200);
}

void GuiOptions_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiOptions *self = (GuiOptions *)self_;
    if (var1->enabled) {
        if (var1->id < 100) {

            GameSettings_setOptionValue(self->options, ((GuiSmallButton *)var1)->enumOptions, 1);
            EnumOptions *opt = EnumOptions_getEnumOptions(var1->id);
            if (opt)
                strncpy(var1->displayString, GameSettings_getKeyBinding(self->options, *opt),
                        sizeof(var1->displayString) - 1);
        }
        if (var1->id == 101) {
            GameSettings_saveOptions(self->base.mc->gameSettings);
            GuiVideoSettings *s = (GuiVideoSettings *)calloc(1, sizeof(GuiVideoSettings));
            GuiVideoSettings_construct(s, (GuiScreen *)self, self->options);
            Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)s);
        }
        if (var1->id == 100) {
            GameSettings_saveOptions(self->base.mc->gameSettings);
            GuiControls *ctrl = (GuiControls *)calloc(1, sizeof(GuiControls));
            GuiControls_construct(ctrl, (GuiScreen *)self, self->options);
            Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)ctrl);
        }
        if (var1->id == 200) {
            GameSettings_saveOptions(self->base.mc->gameSettings);
            Minecraft_displayGuiScreen(self->base.mc, self->parentScreen);
        }
    }
}

void GuiOptions_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiOptions *self = (GuiOptions *)self_;
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, self->screenTitle,
                                 self->base.width / 2, 20, 16777215);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}
