#include "Renderer.h"

#include "GuiScreen.h"
#include "Gui.h"
#include "Minecraft.h"
#include "GuiParticle.h"
#include "Tessellator.h"
#include "SoundManager.h"
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdlib.h>

static void default_initGui(GuiScreen *self) { (void)self; }

static void default_drawScreen(GuiScreen *self, int mouseX, int mouseY, float partialTick) {
    (void)partialTick;
    for (int var4 = 0; var4 < self->buttonCount; ++var4) {
        GuiButton *var5 = self->buttonList[var4];
        GuiButton_drawButton(var5, self->mc, mouseX, mouseY);
    }
}

static void default_updateScreen(GuiScreen *self) { (void)self; }

static int default_doesGuiPauseGame(GuiScreen *self) {
    (void)self;
    return 1;
}

static void default_onGuiClosed(GuiScreen *self) { (void)self; }

static void default_handleKeyboardInput(GuiScreen *self) {
    if (Minecraft_getKeyboardEventKeyState()) {
        if (Minecraft_getKeyboardEventKey() == GLFW_KEY_F11) {
            Minecraft_toggleFullscreen(self->mc);
            return;
        }
        self->vtable->keyTyped(self, Minecraft_getKeyboardEventCharacter(),
                               Minecraft_getKeyboardEventKey());
    }
}

static void default_handleMouseInput(GuiScreen *self) {

    int rawX = Minecraft_getMouseEventX();
    int rawY = Minecraft_getMouseEventY();
    int var1 = rawX * self->width / self->mc->displayWidth;

    int var2 = rawY * self->height / self->mc->displayHeight - 1;
    if (Minecraft_getMouseEventButtonState()) {
        self->vtable->mouseClicked(self, var1, var2, Minecraft_getMouseEventButton());
    } else {
        self->vtable->mouseMovedOrUp(self, var1, var2, Minecraft_getMouseEventButton());
    }
}

static void default_actionPerformed(GuiScreen *self, GuiButton *button) {
    (void)self;
    (void)button;
}

static void default_keyTyped(GuiScreen *self, char ch, int keyCode) {
    (void)ch;
    if (keyCode == GLFW_KEY_ESCAPE) {
        Minecraft_displayGuiScreen(self->mc, NULL);
        Minecraft_setIngameFocus(self->mc);
    }
}

static void default_mouseClicked(GuiScreen *self, int x, int y, int button) {
    if (button == 0) {
        for (int var4 = 0; var4 < self->buttonCount; ++var4) {
            GuiButton *var5 = self->buttonList[var4];
            if (var5->vtable->mousePressed(var5, self->mc, x, y)) {
                self->selectedButton = var5;

                SoundManager_playSoundFX(self->mc->sndManager, "random.click", 1.0F, 1.0F);
                self->vtable->actionPerformed(self, var5);
            }
        }
    }
}

static void default_mouseMovedOrUp(GuiScreen *self, int x, int y, int button) {
    if (self->selectedButton != NULL && button == 0) {
        self->selectedButton->vtable->mouseReleased(self->selectedButton, x, y);
        self->selectedButton = NULL;
    }
}

static void default_deleteWorld(GuiScreen *self, int var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

static void default_selectNextField(GuiScreen *self) { (void)self; }

GuiScreenVtable GuiScreen_defaultVtable = {default_initGui,
                                           default_drawScreen,
                                           default_updateScreen,
                                           default_doesGuiPauseGame,
                                           default_onGuiClosed,
                                           default_handleKeyboardInput,
                                           default_handleMouseInput,
                                           default_actionPerformed,
                                           default_keyTyped,
                                           default_mouseClicked,
                                           default_mouseMovedOrUp,
                                           default_deleteWorld,
                                           default_selectNextField,
                                           NULL,
                                           NULL,
                                           NULL};

void GuiScreen_initVtable(GuiScreenVtable *vt) { *vt = GuiScreen_defaultVtable; }

void GuiScreen_free(GuiScreen *self) {
    if (!self)
        return;
    if (self->vtable && self->vtable->destroy)
        self->vtable->destroy(self);
    for (int i = 0; i < self->buttonCount; i++) {
        GuiButton_free(self->buttonList[i]);
        self->buttonList[i] = NULL;
    }
    self->buttonCount = 0;
    free(self->field_25091_h);
    self->field_25091_h = NULL;

    GuiScreen *parent = self->parentScreen;
    self->parentScreen = NULL;
    free(self);
    GuiScreen_free(parent);
}

void GuiScreen_setWorldAndResolution(GuiScreen *self, struct Minecraft *mc, int w, int h) {

    if (self->field_25091_h) {
        free(self->field_25091_h);
    }
    self->field_25091_h = (GuiParticle *)calloc(1, sizeof(GuiParticle));
    if (self->field_25091_h && mc) {
        GuiParticle_construct(self->field_25091_h, mc);
    }
    self->mc = mc;
    self->fontRenderer = mc ? mc->fontRenderer : NULL;
    self->width = w;
    self->height = h;

    for (int i = 0; i < self->buttonCount; i++) {
        if (self->buttonList[i]) {
            GuiButton_free(self->buttonList[i]);
            self->buttonList[i] = NULL;
        }
    }
    self->buttonCount = 0;
    self->selectedButton = NULL;

    if (self->vtable && self->vtable->initGui)
        self->vtable->initGui(self);
}

GuiButton *GuiScreen_addButton(GuiScreen *self, GuiButton *button) {
    if (self->buttonCount < 64)
        self->buttonList[self->buttonCount++] = button;
    return button;
}

void GuiScreen_handleInput(GuiScreen *self) {

    struct Minecraft *mc = self->mc;
    while (Minecraft_mouseNext()) {
        self->vtable->handleMouseInput(self);
        if (mc->currentScreen != self)
            return;
    }
    while (Minecraft_keyboardNext()) {
        self->vtable->handleKeyboardInput(self);
        if (mc->currentScreen != self)
            return;
    }
}

void GuiScreen_drawDefaultBackground(GuiScreen *self) { GuiScreen_drawWorldBackground(self, 0); }

void GuiScreen_drawWorldBackground(GuiScreen *self, int var1) {
    if (self->mc != NULL && self->mc->theWorld != NULL) {
        Gui_drawGradientRect(&self->guiBase, 0, 0, self->width, self->height, -1072689136,
                             -804253680);
    } else {
        GuiScreen_drawBackground(self, var1);
    }
}

void GuiScreen_drawBackground(GuiScreen *self, int var1) {
    R_disable(R_LIGHTING);
    R_disable(R_FOG);
    Tessellator *var2 = &Tessellator_instance;
    R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/gui/background.png"));
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    float var3 = 32.0f;
    Tessellator_startDrawingQuads(var2);
    Tessellator_setColorOpaque_I(var2, 4210752);
    Tessellator_addVertexWithUV(var2, 0.0, (double)self->height, 0.0, 0.0,
                                (double)((float)self->height / var3 + (float)var1));
    Tessellator_addVertexWithUV(var2, (double)self->width, (double)self->height, 0.0,
                                (double)((float)self->width / var3),
                                (double)((float)self->height / var3 + (float)var1));
    Tessellator_addVertexWithUV(var2, (double)self->width, 0.0, 0.0,
                                (double)((float)self->width / var3), (double)(0 + var1));
    Tessellator_addVertexWithUV(var2, 0.0, 0.0, 0.0, 0.0, (double)(0 + var1));
    Tessellator_draw(var2);
}

const char *GuiScreen_getClipboardString(void) { return glfwGetClipboardString(NULL); }
