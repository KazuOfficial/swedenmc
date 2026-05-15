#include "GuiMultiplayer.h"
#include "Minecraft.h"
#include "GameSettings.h"
#include "GuiConnecting.h"
#include "StringTranslate.h"
#include "GuiTextField.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.updateScreen = GuiMultiplayer_updateScreen;
    s_vtable.initGui = GuiMultiplayer_initGui;
    s_vtable.onGuiClosed = GuiMultiplayer_onGuiClosed;
    s_vtable.actionPerformed = GuiMultiplayer_actionPerformed;
    s_vtable.keyTyped = GuiMultiplayer_keyTyped;
    s_vtable.mouseClicked = GuiMultiplayer_mouseClicked;
    s_vtable.drawScreen = GuiMultiplayer_drawScreen;
    s_vtable_init = 1;
}

void GuiMultiplayer_construct(GuiMultiplayer *self, GuiScreen *var1) {
    ensure_vtable();
    self->parentScreen = var1;
    self->base.parentScreen = var1;
    self->field_22111_h = NULL;
    self->base.vtable = &s_vtable;
}

void GuiMultiplayer_updateScreen(GuiScreen *self_) {
    GuiMultiplayer *self = (GuiMultiplayer *)self_;
    GuiTextField_updateCursorCounter(self->field_22111_h);
}

void GuiMultiplayer_initGui(GuiScreen *self_) {
    GuiMultiplayer *self = (GuiMultiplayer *)self_;
    StringTranslate *var1 = StringTranslate_getInstance();
    Minecraft_enableKeyRepeatEvents(1);

    GuiButton *btn0 =
        GuiButton_create(0, self->base.width / 2 - 100, self->base.height / 4 + 96 + 12,
                         StringTranslate_translateKey(var1, "multiplayer.connect"));
    GuiButton *btn1 =
        GuiButton_create(1, self->base.width / 2 - 100, self->base.height / 4 + 120 + 12,
                         StringTranslate_translateKey(var1, "gui.cancel"));
    GuiScreen_addButton((GuiScreen *)self, btn0);
    GuiScreen_addButton((GuiScreen *)self, btn1);

    char var2[256];
    strncpy(var2, self->base.mc->gameSettings->lastServer, sizeof(var2) - 1);
    var2[sizeof(var2) - 1] = '\0';

    for (int i = 0; var2[i]; ++i) {
        if (var2[i] == '_')
            var2[i] = ':';
    }
    self->base.buttonList[0]->enabled = (strlen(var2) > 0) ? 1 : 0;
    free(self->field_22111_h);
    self->field_22111_h = (GuiTextField *)calloc(1, sizeof(GuiTextField));
    GuiTextField_construct(self->field_22111_h, (GuiScreen *)self, self->base.fontRenderer,
                           self->base.width / 2 - 100, self->base.height / 4 - 10 + 50 + 18, 200,
                           20, var2);
    self->field_22111_h->isFocused = 1;
    GuiTextField_setMaxStringLength(self->field_22111_h, 128);
}

void GuiMultiplayer_onGuiClosed(GuiScreen *self_) {
    GuiMultiplayer *self = (GuiMultiplayer *)self_;
    Minecraft_enableKeyRepeatEvents(0);
    free(self->field_22111_h);
    self->field_22111_h = NULL;
}

void GuiMultiplayer_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiMultiplayer *self = (GuiMultiplayer *)self_;
    if (var1->enabled) {
        if (var1->id == 1) {
            Minecraft_displayGuiScreen(self->base.mc, self->parentScreen);
        } else if (var1->id == 0) {
            char var2[256];
            strncpy(var2, GuiTextField_getText(self->field_22111_h), sizeof(var2) - 1);
            var2[sizeof(var2) - 1] = '\0';

            int start = 0;
            while (var2[start] == ' ')
                ++start;
            int end = (int)strlen(var2) - 1;
            while (end > start && var2[end] == ' ')
                --end;
            var2[end + 1] = '\0';
            memmove(var2, var2 + start, strlen(var2 + start) + 1);

            char saved[256];
            strncpy(saved, var2, sizeof(saved) - 1);
            saved[sizeof(saved) - 1] = '\0';
            for (int i = 0; saved[i]; ++i) {
                if (saved[i] == ':')
                    saved[i] = '_';
            }
            strncpy(self->base.mc->gameSettings->lastServer, saved,
                    sizeof(self->base.mc->gameSettings->lastServer) - 1);

            GameSettings_saveOptions(self->base.mc->gameSettings);

            char host[256] = {0};
            int port = 25565;
            if (var2[0] == '[') {

                char *closeBracket = strchr(var2, ']');
                if (closeBracket != NULL && closeBracket > var2 + 1) {
                    int hostLen = (int)(closeBracket - var2 - 1);
                    strncpy(host, var2 + 1, (size_t)hostLen);
                    host[hostLen] = '\0';
                    char *colonAfter = closeBracket + 1;
                    while (*colonAfter == ' ')
                        ++colonAfter;
                    if (*colonAfter == ':' && strlen(colonAfter + 1) > 0) {
                        port = GuiMultiplayer_parseIntWithDefault(self, colonAfter + 1, 25565);
                    }
                } else {
                    strncpy(host, var2, sizeof(host) - 1);
                }
            } else {

                char *colon = strchr(var2, ':');
                if (colon != NULL) {
                    int hostLen = (int)(colon - var2);
                    strncpy(host, var2, (size_t)hostLen);
                    host[hostLen] = '\0';
                    port = GuiMultiplayer_parseIntWithDefault(self, colon + 1, 25565);
                } else {
                    strncpy(host, var2, sizeof(host) - 1);
                }
            }

            GuiConnecting *connecting = (GuiConnecting *)calloc(1, sizeof(GuiConnecting));
            GuiConnecting_construct(connecting, self->base.mc, host, port);
            Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)connecting);
        }
    }
}

int GuiMultiplayer_parseIntWithDefault(GuiMultiplayer *self, const char *var1, int var2) {
    (void)self;
    if (!var1 || !*var1)
        return var2;
    char *end;
    long val = strtol(var1, &end, 10);
    if (end == var1)
        return var2;
    return (int)val;
}

void GuiMultiplayer_keyTyped(GuiScreen *self_, char var1, int var2) {
    GuiMultiplayer *self = (GuiMultiplayer *)self_;
    GuiTextField_textboxKeyTyped(self->field_22111_h, var1, var2);
    if (var1 == 13) {
        if (self->base.buttonCount > 0)
            GuiMultiplayer_actionPerformed((GuiScreen *)self, self->base.buttonList[0]);
        return;
    }
    self->base.buttonList[0]->enabled =
        (strlen(GuiTextField_getText(self->field_22111_h)) > 0) ? 1 : 0;
}

void GuiMultiplayer_mouseClicked(GuiScreen *self_, int var1, int var2, int var3) {
    GuiMultiplayer *self = (GuiMultiplayer *)self_;
    GuiScreen_defaultVtable.mouseClicked((GuiScreen *)self, var1, var2, var3);
    if (self->field_22111_h)
        GuiTextField_mouseClicked(self->field_22111_h, var1, var2, var3);
}

void GuiMultiplayer_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiMultiplayer *self = (GuiMultiplayer *)self_;
    StringTranslate *var4 = StringTranslate_getInstance();
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer,
                                 StringTranslate_translateKey(var4, "multiplayer.title"),
                                 self->base.width / 2, self->base.height / 4 - 60 + 20, 16777215);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         StringTranslate_translateKey(var4, "multiplayer.info1"),
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 0, 10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         StringTranslate_translateKey(var4, "multiplayer.info2"),
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 9, 10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         StringTranslate_translateKey(var4, "multiplayer.ipinfo"),
                         self->base.width / 2 - 140, self->base.height / 4 - 60 + 60 + 36,
                         10526880);
    GuiTextField_drawTextBox(self->field_22111_h);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}
