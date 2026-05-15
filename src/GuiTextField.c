#include "GuiTextField.h"
#include "ChatAllowedCharacters.h"
#include <string.h>
#include <GLFW/glfw3.h>

void GuiTextField_construct(GuiTextField *self, GuiScreen *var1, FontRenderer *var2, int var3,
                            int var4, int var5, int var6, const char *var7) {
    memset(self, 0, sizeof(*self));
    self->isEnabled = 1;
    self->parentGuiScreen = var1;
    self->fontRenderer = var2;
    self->xPos = var3;
    self->yPos = var4;
    self->width = var5;
    self->height = var6;
    GuiTextField_setText(self, var7);
}

void GuiTextField_setText(GuiTextField *self, const char *var1) {
    if (var1) {
        strncpy(self->text, var1, sizeof(self->text) - 1);
        self->text[sizeof(self->text) - 1] = '\0';
    } else {
        self->text[0] = '\0';
    }
}

const char *GuiTextField_getText(GuiTextField *self) { return self->text; }

void GuiTextField_updateCursorCounter(GuiTextField *self) { ++self->cursorCounter; }

void GuiTextField_textboxKeyTyped(GuiTextField *self, char var1, int var2) {
    if (self->isEnabled && self->isFocused) {
        if (var1 == 9) {

            if (self->parentGuiScreen && self->parentGuiScreen->vtable->selectNextField)
                self->parentGuiScreen->vtable->selectNextField(self->parentGuiScreen);
        }

        if (var1 == 22) {

            const char *var3 = GuiScreen_getClipboardString();
            if (var3 == NULL) {
                var3 = "";
            }
            int var4 = 32 - (int)strlen(self->text);
            if (var4 > (int)strlen(var3)) {
                var4 = (int)strlen(var3);
            }
            if (var4 > 0) {
                strncat(self->text, var3, (size_t)var4);
            }
        }

        if (var2 == GLFW_KEY_BACKSPACE && strlen(self->text) > 0) {
            self->text[strlen(self->text) - 1] = '\0';
        }

        if (ChatAllowedCharacters_allowedCharacters != NULL &&
            strchr(ChatAllowedCharacters_allowedCharacters, var1) != NULL &&
            ((int)strlen(self->text) < self->maxStringLength || self->maxStringLength == 0)) {
            size_t len = strlen(self->text);
            if (len + 1 < sizeof(self->text)) {
                self->text[len] = var1;
                self->text[len + 1] = '\0';
            }
        }
    }
}

void GuiTextField_mouseClicked(GuiTextField *self, int var1, int var2, int var3) {
    (void)var3;
    int var4 = self->isEnabled && var1 >= self->xPos && var1 < self->xPos + self->width &&
               var2 >= self->yPos && var2 < self->yPos + self->height;
    GuiTextField_setFocused(self, var4);
}

void GuiTextField_setFocused(GuiTextField *self, int var1) {
    if (var1 && !self->isFocused) {
        self->cursorCounter = 0;
    }
    self->isFocused = var1;
}

void GuiTextField_drawTextBox(GuiTextField *self) {
    Gui_drawRect(&self->base, self->xPos - 1, self->yPos - 1, self->xPos + self->width + 1,
                 self->yPos + self->height + 1, -6250336);
    Gui_drawRect(&self->base, self->xPos, self->yPos, self->xPos + self->width,
                 self->yPos + self->height, -16777216);
    if (self->isEnabled) {
        int var1 = self->isFocused && self->cursorCounter / 6 % 2 == 0;
        char buf[258];
        strncpy(buf, self->text, sizeof(buf) - 2);
        buf[sizeof(buf) - 2] = '\0';
        if (var1) {
            size_t len = strlen(buf);
            buf[len] = '_';
            buf[len + 1] = '\0';
        }
        Gui_drawString(&self->base, self->fontRenderer, buf, self->xPos + 4,
                       self->yPos + (self->height - 8) / 2, 14737632);
    } else {
        Gui_drawString(&self->base, self->fontRenderer, self->text, self->xPos + 4,
                       self->yPos + (self->height - 8) / 2, 7368816);
    }
}

void GuiTextField_setMaxStringLength(GuiTextField *self, int var1) { self->maxStringLength = var1; }
