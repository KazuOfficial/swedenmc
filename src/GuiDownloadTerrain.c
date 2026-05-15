#include "GuiDownloadTerrain.h"
#include "Minecraft.h"
#include "NetClientHandler.h"
#include "Packet0KeepAlive.h"
#include "StringTranslate.h"

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.keyTyped = GuiDownloadTerrain_keyTyped;
    s_vtable.initGui = GuiDownloadTerrain_initGui;
    s_vtable.updateScreen = GuiDownloadTerrain_updateScreen;
    s_vtable.actionPerformed = GuiDownloadTerrain_actionPerformed;
    s_vtable.drawScreen = GuiDownloadTerrain_drawScreen;
    s_vtable_init = 1;
}

void GuiDownloadTerrain_construct(GuiDownloadTerrain *self, struct NetClientHandler *var1) {
    ensure_vtable();
    self->netHandler = var1;
    self->updateCounter = 0;
    self->base.vtable = &s_vtable;
}

void GuiDownloadTerrain_keyTyped(GuiScreen *self_, char var1, int var2) {
    (void)self_;
    (void)var1;
    (void)var2;
}

void GuiDownloadTerrain_initGui(GuiScreen *self_) {
    GuiDownloadTerrain *self = (GuiDownloadTerrain *)self_;
    self->base.buttonCount = 0;
}

void GuiDownloadTerrain_updateScreen(GuiScreen *self_) {
    GuiDownloadTerrain *self = (GuiDownloadTerrain *)self_;
    ++self->updateCounter;
    if (self->updateCounter % 20 == 0) {
        NetClientHandler_addToSendQueue(self->netHandler, Packet0KeepAlive_create());
    }
    if (self->netHandler != NULL) {
        NetClientHandler_processReadPackets(self->netHandler);
    }
}

void GuiDownloadTerrain_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    (void)self_;
    (void)var1;
}

void GuiDownloadTerrain_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiDownloadTerrain *self = (GuiDownloadTerrain *)self_;
    GuiScreen_drawBackground((GuiScreen *)self, 0);
    StringTranslate *var4 = StringTranslate_getInstance();
    GuiScreen_drawCenteredString(
        (GuiScreen *)self, self->base.fontRenderer,
        StringTranslate_translateKey(var4, "multiplayer.downloadingTerrain"), self->base.width / 2,
        self->base.height / 2 - 50, 16777215);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}
