#include "GuiSleepMP.h"
#include <GLFW/glfw3.h>
#include "GuiChat.h"
#include "Minecraft.h"
#include "EntityClientPlayerMP.h"
#include "NetClientHandler.h"
#include "Packet19EntityAction.h"
#include "StringTranslate.h"
#include <string.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;

    GuiChat tmp;
    GuiChat_construct(&tmp);
    s_vtable = *tmp.base.vtable;

    s_vtable.initGui = GuiSleepMP_initGui;
    s_vtable.onGuiClosed = GuiSleepMP_onGuiClosed;
    s_vtable.keyTyped = GuiSleepMP_keyTyped;
    s_vtable.drawScreen = GuiSleepMP_drawScreen;
    s_vtable.actionPerformed = GuiSleepMP_actionPerformed;
    s_vtable_init = 1;
}

void GuiSleepMP_construct(GuiSleepMP *self) {
    GuiChat_construct(&self->base);
    ensure_vtable();
    self->base.base.vtable = &s_vtable;
    self->base.base.guiType = GUI_TYPE_SLEEP_MP;
}

void GuiSleepMP_initGui(GuiScreen *self_) {
    GuiSleepMP *self = (GuiSleepMP *)self_;
    Minecraft_enableKeyRepeatEvents(1);
    StringTranslate *var1 = StringTranslate_getInstance();
    GuiScreen_addButton(
        (GuiScreen *)self,
        GuiButton_create(1, self->base.base.width / 2 - 100, self->base.base.height - 40,
                         StringTranslate_translateKey(var1, "multiplayer.stopSleeping")));
}

void GuiSleepMP_onGuiClosed(GuiScreen *self_) {
    (void)self_;
    Minecraft_enableKeyRepeatEvents(0);
}

void GuiSleepMP_keyTyped(GuiScreen *self_, char var1, int var2) {
    GuiSleepMP *self = (GuiSleepMP *)self_;
    if (var2 == GLFW_KEY_ESCAPE) {
        GuiSleepMP_func_22115_j(self);
    } else if (var2 == GLFW_KEY_ENTER || var2 == GLFW_KEY_KP_ENTER) {

        char var3[101];
        strncpy(var3, self->base.message, sizeof(var3) - 1);
        var3[sizeof(var3) - 1] = '\0';
        int start = 0;
        while (var3[start] == ' ')
            ++start;
        int end = (int)strlen(var3) - 1;
        while (end > start && var3[end] == ' ')
            --end;
        var3[end + 1] = '\0';
        if (strlen(var3 + start) > 0) {

            EntityClientPlayerMP_sendChatMessage(
                (EntityClientPlayerMP *)self->base.base.mc->thePlayer, var3 + start);
        }
        self->base.message[0] = '\0';
    } else {
        GuiChat_keyTyped((GuiScreen *)&self->base, var1, var2);
    }
}

void GuiSleepMP_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiSleepMP *self = (GuiSleepMP *)self_;
    GuiChat_drawScreen((GuiScreen *)&self->base, var1, var2, var3);
}

void GuiSleepMP_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiSleepMP *self = (GuiSleepMP *)self_;
    if (var1->id == 1) {
        GuiSleepMP_func_22115_j(self);
    } else {

        GuiScreen_defaultVtable.actionPerformed((GuiScreen *)self, var1);
    }
}

void GuiSleepMP_func_22115_j(GuiSleepMP *self) {
    if (((Entity *)self->base.base.mc->thePlayer)->isClientPlayerMP) {
        NetClientHandler *var1 = ((EntityClientPlayerMP *)self->base.base.mc->thePlayer)->sendQueue;
        NetClientHandler_addToSendQueue(
            var1, Packet19EntityAction_createWith((Entity *)self->base.base.mc->thePlayer, 3));
    }
}
