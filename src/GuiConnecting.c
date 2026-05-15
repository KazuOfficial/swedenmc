#include "GuiConnecting.h"
#include "Minecraft.h"
#include "GuiMainMenu.h"
#include "StringTranslate.h"
#include "ThreadConnectToServer.h"
#include "NetClientHandler.h"
#include <stdio.h>
#include <stddef.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.updateScreen = GuiConnecting_updateScreen;
    s_vtable.keyTyped = GuiConnecting_keyTyped;
    s_vtable.initGui = GuiConnecting_initGui;
    s_vtable.actionPerformed = GuiConnecting_actionPerformed;
    s_vtable.drawScreen = GuiConnecting_drawScreen;
    s_vtable_init = 1;
}

void GuiConnecting_construct(GuiConnecting *self, struct Minecraft *var1, const char *var2,
                             int var3) {
    ensure_vtable();
    self->clientHandler = NULL;
    self->cancelled = 0;
    self->base.vtable = &s_vtable;
    printf("Connecting to %s, %d\n", var2, var3);
    Minecraft_changeWorld1(var1, NULL);
    ThreadConnectToServer_create(self, var1, var2, var3);
}

void GuiConnecting_updateScreen(GuiScreen *self_) {
    GuiConnecting *self = (GuiConnecting *)self_;
    if (self->clientHandler != NULL) {
        NetClientHandler_processReadPackets(self->clientHandler);
    }
}

void GuiConnecting_keyTyped(GuiScreen *self, char var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

void GuiConnecting_initGui(GuiScreen *self_) {
    GuiConnecting *self = (GuiConnecting *)self_;
    self->base.buttonCount = 0;
    StringTranslate *var1 = StringTranslate_getInstance();
    GuiButton *btn =
        GuiButton_create(0, self->base.width / 2 - 100, self->base.height / 4 + 120 + 12,
                         StringTranslate_translateKey(var1, "gui.cancel"));
    GuiScreen_addButton((GuiScreen *)self, btn);
}

void GuiConnecting_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiConnecting *self = (GuiConnecting *)self_;
    if (var1->id == 0) {
        self->cancelled = 1;
        if (self->clientHandler != NULL) {
            NetClientHandler_disconnect(self->clientHandler);
        }
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)GuiMainMenu_create());
    }
}

void GuiConnecting_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiConnecting *self = (GuiConnecting *)self_;
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    StringTranslate *var4 = StringTranslate_getInstance();
    if (self->clientHandler == NULL) {
        Gui_drawCenteredString((Gui *)self, self->base.fontRenderer,
                               StringTranslate_translateKey(var4, "connect.connecting"),
                               self->base.width / 2, self->base.height / 2 - 50, 16777215);
        Gui_drawCenteredString((Gui *)self, self->base.fontRenderer, "", self->base.width / 2,
                               self->base.height / 2 - 10, 16777215);
    } else {
        Gui_drawCenteredString((Gui *)self, self->base.fontRenderer,
                               StringTranslate_translateKey(var4, "connect.authorizing"),
                               self->base.width / 2, self->base.height / 2 - 50, 16777215);
        Gui_drawCenteredString((Gui *)self, self->base.fontRenderer,
                               self->clientHandler->field_1209_a, self->base.width / 2,
                               self->base.height / 2 - 10, 16777215);
    }
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}

struct NetClientHandler *GuiConnecting_setNetClientHandler(GuiConnecting *var0,
                                                           struct NetClientHandler *var1) {
    return var0->clientHandler = var1;
}

int GuiConnecting_isCancelled(GuiConnecting *var0) { return var0->cancelled; }

struct NetClientHandler *GuiConnecting_getNetClientHandler(GuiConnecting *var0) {
    return var0->clientHandler;
}
