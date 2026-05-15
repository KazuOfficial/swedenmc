#ifndef GUI_TEXT_FIELD_H
#define GUI_TEXT_FIELD_H

#include "Gui.h"
#include "FontRenderer.h"
#include "GuiScreen.h"

typedef struct GuiTextField {
    Gui base;

    FontRenderer *fontRenderer;

    int xPos;

    int yPos;

    int width;

    int height;

    char text[256];

    int maxStringLength;

    int cursorCounter;

    int isFocused;

    int isEnabled;

    GuiScreen *parentGuiScreen;
} GuiTextField;

void GuiTextField_construct(GuiTextField *self, GuiScreen *var1, FontRenderer *var2, int var3,
                            int var4, int var5, int var6, const char *var7);

void GuiTextField_setText(GuiTextField *self, const char *var1);

const char *GuiTextField_getText(GuiTextField *self);

void GuiTextField_updateCursorCounter(GuiTextField *self);

void GuiTextField_textboxKeyTyped(GuiTextField *self, char var1, int var2);

void GuiTextField_mouseClicked(GuiTextField *self, int var1, int var2, int var3);

void GuiTextField_setFocused(GuiTextField *self, int var1);

void GuiTextField_drawTextBox(GuiTextField *self);

void GuiTextField_setMaxStringLength(GuiTextField *self, int var1);

#endif
