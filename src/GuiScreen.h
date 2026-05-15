#ifndef GUI_SCREEN_H
#define GUI_SCREEN_H

#include "Gui.h"
#include "FontRenderer.h"
#include "GuiButton.h"
#include "GuiParticle.h"

struct Minecraft;

typedef struct GuiScreen GuiScreen;

typedef struct GuiScreenVtable {
    void (*initGui)(GuiScreen *self);
    void (*drawScreen)(GuiScreen *self, int mouseX, int mouseY, float partialTick);
    void (*updateScreen)(GuiScreen *self);
    int (*doesGuiPauseGame)(GuiScreen *self);
    void (*onGuiClosed)(GuiScreen *self);
    void (*handleKeyboardInput)(GuiScreen *self);
    void (*handleMouseInput)(GuiScreen *self);
    void (*actionPerformed)(GuiScreen *self, GuiButton *button);
    void (*keyTyped)(GuiScreen *self, char ch, int keyCode);
    void (*mouseClicked)(GuiScreen *self, int x, int y, int button);
    void (*mouseMovedOrUp)(GuiScreen *self, int x, int y, int button);

    void (*deleteWorld)(GuiScreen *self, int var1, int var2);

    void (*selectNextField)(GuiScreen *self);

    void (*drawGuiContainerForegroundLayer)(GuiScreen *self);

    void (*drawGuiContainerBackgroundLayer)(GuiScreen *self, float var1);

    void (*destroy)(GuiScreen *self);
} GuiScreenVtable;

#define GUI_TYPE_DEFAULT 0
#define GUI_TYPE_MAIN_MENU 1
#define GUI_TYPE_UNUSED 2
#define GUI_TYPE_CHAT 3
#define GUI_TYPE_GAME_OVER 4

#define GUI_TYPE_SLEEP_MP 5

struct GuiScreen {
    Gui guiBase;
    const GuiScreenVtable *vtable;
    struct Minecraft *mc;
    int guiType;
    FontRenderer *fontRenderer;
    int width;
    int height;
    GuiButton *buttonList[64];
    int buttonCount;

    int field_948_f;

    GuiParticle *field_25091_h;

    GuiButton *selectedButton;

    GuiScreen *parentScreen;
};

void GuiScreen_free(GuiScreen *self);
void GuiScreen_setWorldAndResolution(GuiScreen *self, struct Minecraft *mc, int w, int h);
GuiButton *GuiScreen_addButton(GuiScreen *self, GuiButton *button);
void GuiScreen_drawDefaultBackground(GuiScreen *self);
void GuiScreen_drawWorldBackground(GuiScreen *self, int var1);
void GuiScreen_drawBackground(GuiScreen *self, int var1);
void GuiScreen_handleInput(GuiScreen *self);

const char *GuiScreen_getClipboardString(void);

static inline void GuiScreen_drawString(GuiScreen *self, FontRenderer *var1, const char *var2,
                                        int var3, int var4, int var5) {
    Gui_drawString((Gui *)self, var1, var2, var3, var4, var5);
}

static inline void GuiScreen_drawCenteredString(GuiScreen *self, FontRenderer *var1,
                                                const char *var2, int var3, int var4, int var5) {
    Gui_drawCenteredString((Gui *)self, var1, var2, var3, var4, var5);
}

extern GuiScreenVtable GuiScreen_defaultVtable;
void GuiScreen_initVtable(GuiScreenVtable *vt);

#endif
