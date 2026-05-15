#include "GuiChat.h"
#include <GLFW/glfw3.h>
#include "Minecraft.h"
#include "GuiIngame.h"
#include "ChatAllowedCharacters.h"
#include "EntityClientPlayerMP.h"
#include <string.h>
#include <stdio.h>

#define field_20082_i ChatAllowedCharacters_allowedCharacters

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.initGui = GuiChat_initGui;
    s_vtable.onGuiClosed = GuiChat_onGuiClosed;
    s_vtable.updateScreen = GuiChat_updateScreen;
    s_vtable.keyTyped = GuiChat_keyTyped;
    s_vtable.drawScreen = GuiChat_drawScreen;
    s_vtable.mouseClicked = GuiChat_mouseClicked;
    s_vtable_init = 1;
}

void GuiChat_construct(GuiChat *self) {
    ensure_vtable();
    self->base.vtable = &s_vtable;
    self->base.guiType = GUI_TYPE_CHAT;
    self->message[0] = '\0';
    self->updateCounter = 0;
}

void GuiChat_initGui(GuiScreen *self_) {
    (void)self_;

    Minecraft_enableKeyRepeatEvents(1);
}

void GuiChat_onGuiClosed(GuiScreen *self_) {
    (void)self_;

    Minecraft_enableKeyRepeatEvents(0);
}

void GuiChat_updateScreen(GuiScreen *self_) {
    GuiChat *self = (GuiChat *)self_;
    ++self->updateCounter;
}

void GuiChat_keyTyped(GuiScreen *self_, char var1, int var2) {
    GuiChat *self = (GuiChat *)self_;
    if (var2 == GLFW_KEY_ESCAPE) {
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)NULL);
    } else if (var2 == GLFW_KEY_ENTER || var2 == GLFW_KEY_KP_ENTER) {

        char var3[101];
        strncpy(var3, self->message, 100);
        var3[100] = '\0';

        int start = 0;
        while (var3[start] == ' ')
            ++start;
        int end = (int)strlen(var3) - 1;
        while (end > start && var3[end] == ' ')
            --end;
        var3[end + 1] = '\0';
        char *var3_trimmed = var3 + start;
        if (strlen(var3_trimmed) > 0) {
            char var4[101];
            strncpy(var4, self->message, 100);
            var4[100] = '\0';
            int s = 0;
            while (var4[s] == ' ')
                ++s;
            int e = (int)strlen(var4) - 1;
            while (e > s && var4[e] == ' ')
                --e;
            var4[e + 1] = '\0';
            char *var4_trimmed = var4 + s;
            if (!Minecraft_lineIsCommand(self->base.mc, var4_trimmed)) {
                EntityClientPlayerMP_sendChatMessage(
                    (EntityClientPlayerMP *)self->base.mc->thePlayer, var4_trimmed);
            }
        }
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)NULL);
    } else {
        if (var2 == GLFW_KEY_BACKSPACE && strlen(self->message) > 0) {
            self->message[strlen(self->message) - 1] = '\0';
        }

        if (field_20082_i != NULL && strchr(field_20082_i, var1) != NULL &&
            strlen(self->message) < 100) {
            size_t len = strlen(self->message);
            self->message[len] = var1;
            self->message[len + 1] = '\0';
        }
    }
}

void GuiChat_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiChat *self = (GuiChat *)self_;
    Gui_drawRect((Gui *)self, 2, self->base.height - 14, self->base.width - 2,
                 self->base.height - 2, (int)0x80000000);
    char displayStr[256];
    snprintf(displayStr, sizeof(displayStr), "> %s%s", self->message,
             (self->updateCounter / 6 % 2 == 0 ? "_" : ""));
    Gui_drawString((Gui *)self, self->base.fontRenderer, displayStr, 4, self->base.height - 12,
                   14737632);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}

void GuiChat_mouseClicked(GuiScreen *self_, int var1, int var2, int var3) {
    GuiChat *self = (GuiChat *)self_;
    if (var3 == 0) {
        if (self->base.mc->ingameGUI->field_933_a != NULL) {
            if (strlen(self->message) > 0 && self->message[strlen(self->message) - 1] != ' ') {
                size_t len = strlen(self->message);
                if (len < 100) {
                    self->message[len] = ' ';
                    self->message[len + 1] = '\0';
                }
            }
            strncat(self->message, self->base.mc->ingameGUI->field_933_a,
                    100 - strlen(self->message));
            int var4 = 100;
            if ((int)strlen(self->message) > var4) {
                self->message[var4] = '\0';
            }
        } else {
            GuiScreen_defaultVtable.mouseClicked((GuiScreen *)self, var1, var2, var3);
        }
    }
}
