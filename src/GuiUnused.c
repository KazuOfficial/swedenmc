#include "GuiUnused.h"
#include <string.h>

static void s_initGui(GuiScreen *self) { GuiUnused_initGui((GuiUnused *)self); }
static void s_drawScreen(GuiScreen *self, int a, int b, float c) {
    GuiUnused_drawScreen((GuiUnused *)self, a, b, c);
}
static void s_keyTyped(GuiScreen *self, char c, int k) {
    GuiUnused_keyTyped((GuiUnused *)self, c, k);
}

static GuiScreenVtable s_vtable;
static int s_vtable_inited = 0;

static const GuiScreenVtable *GuiUnused_getVtable(void) {
    if (!s_vtable_inited) {
        GuiScreen_initVtable(&s_vtable);
        s_vtable.initGui = s_initGui;
        s_vtable.drawScreen = s_drawScreen;
        s_vtable.keyTyped = s_keyTyped;
        s_vtable_inited = 1;
    }
    return &s_vtable;
}

void GuiUnused_construct(GuiUnused *self) {
    memset(self, 0, sizeof(*self));
    self->base.vtable = GuiUnused_getVtable();
    self->base.guiType = GUI_TYPE_UNUSED;
}

void GuiUnused_initGui(GuiUnused *self) { (void)self; }

void GuiUnused_drawScreen(GuiUnused *self, int var1, int var2, float var3) {
    Gui_drawGradientRect((Gui *)self, 0, 0, self->base.width, self->base.height, -12574688,
                         -11530224);
    Gui_drawCenteredString((Gui *)self, self->base.fontRenderer, self->message1,
                           self->base.width / 2, 90, 16777215);
    Gui_drawCenteredString((Gui *)self, self->base.fontRenderer, self->message2,
                           self->base.width / 2, 110, 16777215);
    GuiScreen_defaultVtable.drawScreen(&self->base, var1, var2, var3);
}

void GuiUnused_keyTyped(GuiUnused *self, char var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}
