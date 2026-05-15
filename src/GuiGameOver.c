#include "Renderer.h"

#include "GuiGameOver.h"
#include "Minecraft.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "GuiMainMenu.h"
#include "Gui.h"
#include <stdlib.h>
#include <stdio.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.initGui = GuiGameOver_initGui;
    s_vtable.keyTyped = GuiGameOver_keyTyped;
    s_vtable.actionPerformed = GuiGameOver_actionPerformed;
    s_vtable.drawScreen = GuiGameOver_drawScreen;
    s_vtable.doesGuiPauseGame = GuiGameOver_doesGuiPauseGame;
    s_vtable_init = 1;
}

GuiGameOver *GuiGameOver_create(void) {
    ensure_vtable();
    GuiGameOver *self = (GuiGameOver *)calloc(1, sizeof(GuiGameOver));
    if (!self)
        return NULL;
    self->base.vtable = &s_vtable;
    self->base.guiType = GUI_TYPE_GAME_OVER;
    return self;
}

void GuiGameOver_initGui(GuiScreen *self_) {
    GuiGameOver *self = (GuiGameOver *)self_;
    self->base.buttonCount = 0;
    GuiButton *btn1 =
        GuiButton_create(1, self->base.width / 2 - 100, self->base.height / 4 + 72, "Respawn");
    GuiButton *btn2 =
        GuiButton_create(2, self->base.width / 2 - 100, self->base.height / 4 + 96, "Title menu");
    GuiScreen_addButton((GuiScreen *)self, btn1);
    GuiScreen_addButton((GuiScreen *)self, btn2);
    if (self->base.mc->session == NULL) {
        self->base.buttonList[1]->enabled = 0;
    }
}

void GuiGameOver_keyTyped(GuiScreen *self_, char var1, int var2) {
    (void)self_;
    (void)var1;
    (void)var2;
}

void GuiGameOver_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiGameOver *self = (GuiGameOver *)self_;
    if (var1->id == 0) {
    }
    if (var1->id == 1) {
        if (self->base.mc->thePlayer != NULL) {
            EntityPlayer *ep = (EntityPlayer *)self->base.mc->thePlayer;
            ((const EntityPlayerVtable *)((Entity *)ep)->vtable)->respawnPlayer(ep);
        }
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)NULL);
    }
    if (var1->id == 2) {
        Minecraft_changeWorld1(self->base.mc, (World *)NULL);
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)GuiMainMenu_create());
    }
}

void GuiGameOver_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiGameOver *self = (GuiGameOver *)self_;
    Gui_drawGradientRect((Gui *)self, 0, 0, self->base.width, self->base.height, 1615855616,
                         -1602211792);
    R_pushMatrix();
    R_scalef(2.0F, 2.0F, 2.0F);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, "Game over!",
                                 self->base.width / 2 / 2, 30, 16777215);
    R_popMatrix();
    char scoreStr[64];
    int score = self->base.mc->thePlayer
                    ? EntityPlayer_getScore((EntityPlayer *)self->base.mc->thePlayer)
                    : 0;
    snprintf(scoreStr, sizeof(scoreStr), "Score: &e%d", score);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, scoreStr,
                                 self->base.width / 2, 100, 16777215);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}

int GuiGameOver_doesGuiPauseGame(GuiScreen *self_) {
    (void)self_;
    return 0;
}
