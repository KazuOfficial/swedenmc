#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "Gui.h"

struct Minecraft;

typedef struct GuiButton GuiButton;

typedef struct GuiButtonVtable {

    int (*getHoverState)(GuiButton *self, int var1);

    void (*mouseDragged)(GuiButton *self, struct Minecraft *var1, int var2, int var3);

    void (*mouseReleased)(GuiButton *self, int var1, int var2);

    int (*mousePressed)(GuiButton *self, struct Minecraft *var1, int var2, int var3);
} GuiButtonVtable;

struct GuiButton {
    Gui guiBase;
    const GuiButtonVtable *vtable;

    int width;

    int height;

    int xPosition;

    int yPosition;

    char displayString[256];

    int id;

    int enabled;

    int enabled2;
};

extern const GuiButtonVtable GuiButton_defaultVtable;

GuiButton *GuiButton_create(int id, int x, int y, const char *text);
GuiButton *GuiButton_createSized(int id, int x, int y, int w, int h, const char *text);
void GuiButton_free(GuiButton *self);

void GuiButton_drawButton(GuiButton *self, struct Minecraft *mc, int mouseX, int mouseY);

int GuiButton_getHoverState_default(GuiButton *self, int var1);
void GuiButton_mouseDragged_default(GuiButton *self, struct Minecraft *var1, int var2, int var3);
void GuiButton_mouseReleased_default(GuiButton *self, int var1, int var2);
int GuiButton_mousePressed_default(GuiButton *self, struct Minecraft *var1, int var2, int var3);

#endif
