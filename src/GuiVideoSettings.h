#ifndef GUI_VIDEO_SETTINGS_H
#define GUI_VIDEO_SETTINGS_H

#include "GuiScreen.h"
#include "GameSettings.h"
#include "EnumOptions.h"

typedef struct GuiVideoSettings {
    GuiScreen base;

    GuiScreen *field_22110_h;

    char field_22107_a[256];

    GameSettings *guiGameSettings;
} GuiVideoSettings;

extern const EnumOptions GuiVideoSettings_field_22108_k[];
extern const int GuiVideoSettings_field_22108_k_len;

void GuiVideoSettings_construct(GuiVideoSettings *self, GuiScreen *var1, GameSettings *var2);

void GuiVideoSettings_initGui(GuiVideoSettings *self);
void GuiVideoSettings_actionPerformed(GuiVideoSettings *self, GuiButton *var1);
void GuiVideoSettings_drawScreen(GuiVideoSettings *self, int var1, int var2, float var3);

#endif
