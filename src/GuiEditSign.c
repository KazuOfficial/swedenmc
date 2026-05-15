#include "Renderer.h"

#include "GuiEditSign.h"
#include "Minecraft.h"
#include "Block.h"
#include "TileEntity.h"
#include "TileEntityRenderer.h"
#include "NetClientHandler.h"
#include "EntityClientPlayerMP.h"
#include "Packet130UpdateSign.h"
#include "World.h"
#include <string.h>
#include <stdlib.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void s_vtable_ensure_init(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.initGui = (void (*)(GuiScreen *))GuiEditSign_initGui;
    s_vtable.drawScreen = (void (*)(GuiScreen *, int, int, float))GuiEditSign_drawScreen;
    s_vtable.updateScreen = (void (*)(GuiScreen *))GuiEditSign_updateScreen;
    s_vtable.onGuiClosed = (void (*)(GuiScreen *))GuiEditSign_onGuiClosed;
    s_vtable.actionPerformed =
        (void (*)(GuiScreen *, struct GuiButton *))GuiEditSign_actionPerformed;
    s_vtable.keyTyped = (void (*)(GuiScreen *, char, int))GuiEditSign_keyTyped;
    s_vtable_init = 1;
}

void GuiEditSign_construct(GuiEditSign *self, TileEntitySign *var1) {
    s_vtable_ensure_init();
    self->base.vtable = &s_vtable;
    strncpy(self->screenTitle, "Edit sign message:", sizeof(self->screenTitle) - 1);
    self->screenTitle[sizeof(self->screenTitle) - 1] = '\0';
    self->entitySign = var1;
    self->updateCounter = 0;
    self->editLine = 0;
}

void GuiEditSign_initGui(GuiEditSign *self) {
    self->base.buttonCount = 0;

    Minecraft_enableKeyRepeatEvents(1);
    GuiButton *btn0 =
        GuiButton_create(0, self->base.width / 2 - 100, self->base.height / 4 + 120, "Done");
    GuiScreen_addButton((GuiScreen *)self, btn0);
}

void GuiEditSign_onGuiClosed(GuiEditSign *self) {

    Minecraft_enableKeyRepeatEvents(0);
    if (self->base.mc->theWorld->multiplayerWorld) {

        NetClientHandler *var1 = ((EntityClientPlayerMP *)self->base.mc->thePlayer)->sendQueue;
        char *lines[4] = {self->entitySign->signText[0], self->entitySign->signText[1],
                          self->entitySign->signText[2], self->entitySign->signText[3]};
        NetClientHandler_addToSendQueue(
            var1, Packet130UpdateSign_createWith(self->entitySign->base.xCoord,
                                                 self->entitySign->base.yCoord,
                                                 self->entitySign->base.zCoord, lines));
    }
}

void GuiEditSign_updateScreen(GuiEditSign *self) { ++self->updateCounter; }

void GuiEditSign_actionPerformed(GuiEditSign *self, GuiButton *var1) {
    if (var1->enabled) {
        if (var1->id == 0) {
            TileEntity_onInventoryChanged((TileEntity *)self->entitySign);
            Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)NULL);
        }
    }
}

void GuiEditSign_keyTyped(GuiEditSign *self, char var1, int var2) {
    if (var2 == 200) {
        self->editLine = (self->editLine - 1) & 3;
    }
    if (var2 == 208 || var2 == 28) {
        self->editLine = (self->editLine + 1) & 3;
    }
    if (var2 == 14) {
        int len = (int)strlen(self->entitySign->signText[self->editLine]);
        if (len > 0) {
            self->entitySign->signText[self->editLine][len - 1] = '\0';
        }
    }
    if (strchr(GuiEditSign_allowedCharacters, var1) != NULL) {
        int len = (int)strlen(self->entitySign->signText[self->editLine]);
        if (len < 15) {
            self->entitySign->signText[self->editLine][len] = var1;
            self->entitySign->signText[self->editLine][len + 1] = '\0';
        }
    }
}

void GuiEditSign_drawScreen(GuiEditSign *self, int var1, int var2, float var3) {
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, self->screenTitle,
                                 self->base.width / 2, 40, 16777215);
    R_pushMatrix();
    R_translatef((float)(self->base.width / 2), 0.0F, 50.0F);
    float var4 = 93.75F;
    R_scalef(-var4, -var4, -var4);
    R_rotatef(180.0F, 0.0F, 1.0F, 0.0F);

    Block *var5 = NULL;
    if (self->entitySign->base.worldObj != NULL) {
        int id = World_getBlockId(self->entitySign->base.worldObj, self->entitySign->base.xCoord,
                                  self->entitySign->base.yCoord, self->entitySign->base.zCoord);
        var5 = Block_blocksList[id];
    }
    if (var5 == Block_signPost) {
        float var6 =
            (float)(TileEntity_getBlockMetadata((TileEntity *)self->entitySign) * 360) / 16.0F;
        R_rotatef(var6, 0.0F, 1.0F, 0.0F);
        R_translatef(0.0F, -1.0625F, 0.0F);
    } else {
        int var8 = TileEntity_getBlockMetadata((TileEntity *)self->entitySign);
        float var7 = 0.0F;
        if (var8 == 2) {
            var7 = 180.0F;
        }
        if (var8 == 4) {
            var7 = 90.0F;
        }
        if (var8 == 5) {
            var7 = -90.0F;
        }
        R_rotatef(var7, 0.0F, 1.0F, 0.0F);
        R_translatef(0.0F, -1.0625F, 0.0F);
    }
    if (self->updateCounter / 6 % 2 == 0) {
        self->entitySign->lineBeingEdited = self->editLine;
    }
    TileEntityRenderer_renderTileEntityAt(TileEntityRenderer_instance,
                                          (TileEntity *)self->entitySign, -0.5, -0.75, -0.5, 0.0F);
    self->entitySign->lineBeingEdited = -1;
    R_popMatrix();
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}
