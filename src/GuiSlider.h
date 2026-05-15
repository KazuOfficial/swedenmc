#ifndef GUI_SLIDER_H
#define GUI_SLIDER_H

#include "GuiButton.h"
#include "EnumOptions.h"

typedef struct GuiSlider {
    GuiButton base;

    float sliderValue;

    int dragging;

    EnumOptions idFloat;
} GuiSlider;

void GuiSlider_construct(GuiSlider *self, int var1, int var2, int var3, EnumOptions var4,
                         const char *var5, float var6);

int GuiSlider_getHoverState(GuiButton *self, int var1);

void GuiSlider_mouseDragged(GuiButton *self, struct Minecraft *var1, int var2, int var3);

int GuiSlider_mousePressed(GuiButton *self, struct Minecraft *var1, int var2, int var3);

void GuiSlider_mouseReleased(GuiButton *self, int var1, int var2);

#endif
