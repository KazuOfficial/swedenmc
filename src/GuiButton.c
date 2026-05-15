#include "Renderer.h"

#include "GuiButton.h"
#include "Minecraft.h"
#include <stdlib.h>
#include <string.h>

int GuiButton_getHoverState_default(GuiButton *self, int var1) {
    int var2 = 1;
    if (!self->enabled) {
        var2 = 0;
    } else if (var1) {
        var2 = 2;
    }
    return var2;
}

void GuiButton_mouseDragged_default(GuiButton *self, struct Minecraft *var1, int var2, int var3) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
}

void GuiButton_mouseReleased_default(GuiButton *self, int var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

int GuiButton_mousePressed_default(GuiButton *self, struct Minecraft *var1, int var2, int var3) {
    (void)var1;
    return self->enabled && var2 >= self->xPosition && var3 >= self->yPosition &&
           var2 < self->xPosition + self->width && var3 < self->yPosition + self->height;
}

const GuiButtonVtable GuiButton_defaultVtable = {
    GuiButton_getHoverState_default, GuiButton_mouseDragged_default,
    GuiButton_mouseReleased_default, GuiButton_mousePressed_default};

GuiButton *GuiButton_create(int id, int x, int y, const char *text) {
    return GuiButton_createSized(id, x, y, 200, 20, text);
}

GuiButton *GuiButton_createSized(int id, int x, int y, int w, int h, const char *text) {
    GuiButton *self = (GuiButton *)calloc(1, sizeof(GuiButton));
    if (!self)
        return NULL;
    self->vtable = &GuiButton_defaultVtable;
    self->width = w;
    self->height = h;
    self->enabled = 1;
    self->enabled2 = 1;
    self->id = id;
    self->xPosition = x;
    self->yPosition = y;
    if (text)
        strncpy(self->displayString, text, sizeof(self->displayString) - 1);
    return self;
}

void GuiButton_free(GuiButton *self) {
    if (!self)
        return;
    free(self);
}

void GuiButton_drawButton(GuiButton *self, struct Minecraft *mc, int mouseX, int mouseY) {
    if (self->enabled2) {
        FontRenderer *var4 = mc->fontRenderer;
        unsigned int texId = RenderEngine_getTexture(mc->renderEngine, "/gui/gui.png");
        R_bindTexture(texId);
        R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
        int var5 =
            (mouseX >= self->xPosition && mouseY >= self->yPosition &&
             mouseX < self->xPosition + self->width && mouseY < self->yPosition + self->height)
                ? 1
                : 0;
        int var6 = self->vtable->getHoverState(self, var5);
        Gui_drawTexturedModalRect(&self->guiBase, self->xPosition, self->yPosition, 0,
                                  46 + var6 * 20, self->width / 2, self->height);
        Gui_drawTexturedModalRect(&self->guiBase, self->xPosition + self->width / 2,
                                  self->yPosition, 200 - self->width / 2, 46 + var6 * 20,
                                  self->width / 2, self->height);
        self->vtable->mouseDragged(self, mc, mouseX, mouseY);
        if (!self->enabled) {
            Gui_drawCenteredString(&self->guiBase, var4, self->displayString,
                                   self->xPosition + self->width / 2,
                                   self->yPosition + (self->height - 8) / 2, -6250336);
        } else if (var5) {
            Gui_drawCenteredString(&self->guiBase, var4, self->displayString,
                                   self->xPosition + self->width / 2,
                                   self->yPosition + (self->height - 8) / 2, 16777120);
        } else {
            Gui_drawCenteredString(&self->guiBase, var4, self->displayString,
                                   self->xPosition + self->width / 2,
                                   self->yPosition + (self->height - 8) / 2, 14737632);
        }
    }
}
