#include "Renderer.h"

#include "GuiSlider.h"
#include "GuiButton.h"
#include "Minecraft.h"
#include "GameSettings.h"
#include <string.h>

static const GuiButtonVtable s_GuiSlider_vtable = {GuiSlider_getHoverState, GuiSlider_mouseDragged,
                                                   GuiSlider_mouseReleased, GuiSlider_mousePressed};

void GuiSlider_construct(GuiSlider *self, int var1, int var2, int var3, EnumOptions var4,
                         const char *var5, float var6) {

    GuiButton *base = &self->base;
    base->vtable = &s_GuiSlider_vtable;
    base->width = 150;
    base->height = 20;
    base->enabled = 1;
    base->enabled2 = 1;
    base->id = var1;
    base->xPosition = var2;
    base->yPosition = var3;
    if (var5)
        strncpy(base->displayString, var5, sizeof(base->displayString) - 1);
    self->idFloat = var4;
    self->sliderValue = var6;
    self->dragging = 0;
}

int GuiSlider_getHoverState(GuiButton *self, int var1) {
    (void)self;
    (void)var1;
    return 0;
}

void GuiSlider_mouseDragged(GuiButton *base, struct Minecraft *var1, int var2, int var3) {
    (void)var3;
    GuiSlider *self = (GuiSlider *)base;
    if (base->enabled2) {
        if (self->dragging) {
            self->sliderValue = (float)(var2 - (base->xPosition + 4)) / (float)(base->width - 8);
            if (self->sliderValue < 0.0f) {
                self->sliderValue = 0.0f;
            }
            if (self->sliderValue > 1.0f) {
                self->sliderValue = 1.0f;
            }
            GameSettings_setOptionFloatValue(var1->gameSettings, self->idFloat, self->sliderValue);
            strncpy(base->displayString,
                    GameSettings_getKeyBinding(var1->gameSettings, self->idFloat),
                    sizeof(base->displayString) - 1);
        }
        R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
        Gui_drawTexturedModalRect(
            &base->guiBase, base->xPosition + (int)(self->sliderValue * (float)(base->width - 8)),
            base->yPosition, 0, 66, 4, 20);
        Gui_drawTexturedModalRect(&base->guiBase,
                                  base->xPosition +
                                      (int)(self->sliderValue * (float)(base->width - 8)) + 4,
                                  base->yPosition, 196, 66, 4, 20);
    }
}

int GuiSlider_mousePressed(GuiButton *base, struct Minecraft *var1, int var2, int var3) {
    GuiSlider *self = (GuiSlider *)base;
    if (GuiButton_mousePressed_default(base, var1, var2, var3)) {
        self->sliderValue = (float)(var2 - (base->xPosition + 4)) / (float)(base->width - 8);
        if (self->sliderValue < 0.0f) {
            self->sliderValue = 0.0f;
        }
        if (self->sliderValue > 1.0f) {
            self->sliderValue = 1.0f;
        }
        GameSettings_setOptionFloatValue(var1->gameSettings, self->idFloat, self->sliderValue);
        strncpy(base->displayString, GameSettings_getKeyBinding(var1->gameSettings, self->idFloat),
                sizeof(base->displayString) - 1);
        self->dragging = 1;
        return 1;
    } else {
        return 0;
    }
}

void GuiSlider_mouseReleased(GuiButton *base, int var1, int var2) {
    (void)var1;
    (void)var2;
    GuiSlider *self = (GuiSlider *)base;
    self->dragging = 0;
}
