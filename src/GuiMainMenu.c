#include "Renderer.h"

#include "GuiMainMenu.h"
#include "Minecraft.h"
#include "MathHelper.h"
#include "FontRenderer.h"
#include "StringTranslate.h"
#include "Tessellator.h"
#include "GuiOptions.h"
#include "GuiSelectWorld.h"
#include "GuiMultiplayer.h"
#include "GuiTexturePacks.h"
#include <stdlib.h>
#include "JavaRandom.h"
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

static JavaRandom guiMainMenu_rand;
static int guiMainMenu_rand_inited = 0;

static void GuiMainMenu_init(GuiMainMenu *self) {

    if (!guiMainMenu_rand_inited) {
        JavaRandom_new(&guiMainMenu_rand);
        guiMainMenu_rand_inited = 1;
    }

    strncpy(self->splashText, "missingno", sizeof(self->splashText) - 1);
    self->splashText[sizeof(self->splashText) - 1] = '\0';
    self->updateCounter = 0.0F;
    self->multiplayerButton = NULL;

    {
        FILE *f = fopen("title/splashes.txt", "r");
        if (f) {
            char **lines = NULL;
            int nlines = 0;
            char buf[256];
            while (fgets(buf, sizeof(buf), f)) {

                int end = (int)strlen(buf) - 1;
                while (end >= 0 && (buf[end] == '\n' || buf[end] == '\r' || buf[end] == ' '))
                    buf[end--] = '\0';

                char *p = buf;
                while (*p == ' ')
                    ++p;
                if (*p == '\0')
                    continue;
                char **tmp = (char **)realloc(lines, (nlines + 1) * sizeof(char *));
                if (!tmp)
                    break;
                lines = tmp;
                lines[nlines++] = strdup(p);
            }
            fclose(f);
            if (nlines > 0) {
                int idx = JavaRandom_nextInt(&guiMainMenu_rand, nlines);
                strncpy(self->splashText, lines[idx], sizeof(self->splashText) - 1);
                self->splashText[sizeof(self->splashText) - 1] = '\0';
                for (int i = 0; i < nlines; ++i)
                    free(lines[i]);
                free(lines);
            }
        }
    }
}

void GuiMainMenu_updateScreen(GuiScreen *self_) {
    GuiMainMenu *self = (GuiMainMenu *)self_;
    ++self->updateCounter;
}

void GuiMainMenu_keyTyped(GuiScreen *self_, char var1, int var2) {
    (void)self_;
    (void)var1;
    (void)var2;
}

void GuiMainMenu_initGui(GuiScreen *self_) {
    GuiMainMenu *self = (GuiMainMenu *)self_;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int month = t->tm_mon + 1;
    int day = t->tm_mday;
    if (month == 11 && day == 9) {
        strncpy(self->splashText, "Happy birthday, ez!", sizeof(self->splashText) - 1);
    } else if (month == 6 && day == 1) {
        strncpy(self->splashText, "Happy birthday, Notch!", sizeof(self->splashText) - 1);
    } else if (month == 12 && day == 24) {
        strncpy(self->splashText, "Merry X-mas!", sizeof(self->splashText) - 1);
    } else if (month == 1 && day == 1) {
        strncpy(self->splashText, "Happy new year!", sizeof(self->splashText) - 1);
    }

    StringTranslate *var2 = StringTranslate_getInstance();
    int var4 = self->base.height / 4 + 48;
    GuiButton *btn1 = GuiButton_create(1, self->base.width / 2 - 100, var4,
                                       StringTranslate_translateKey(var2, "menu.singleplayer"));
    GuiButton *btn2 = GuiButton_create(2, self->base.width / 2 - 100, var4 + 24,
                                       StringTranslate_translateKey(var2, "menu.multiplayer"));
    GuiButton *btn3 = GuiButton_create(3, self->base.width / 2 - 100, var4 + 48,
                                       StringTranslate_translateKey(var2, "menu.mods"));
    GuiScreen_addButton((GuiScreen *)self, btn1);
    self->multiplayerButton = btn2;
    GuiScreen_addButton((GuiScreen *)self, btn2);
    GuiScreen_addButton((GuiScreen *)self, btn3);
    if (self->base.mc->hideQuitButton) {
        GuiButton *btn0 = GuiButton_create(0, self->base.width / 2 - 100, var4 + 72,
                                           StringTranslate_translateKey(var2, "menu.options"));
        GuiScreen_addButton((GuiScreen *)self, btn0);
    } else {
        GuiButton *btn0 =
            GuiButton_createSized(0, self->base.width / 2 - 100, var4 + 72 + 12, 98, 20,
                                  StringTranslate_translateKey(var2, "menu.options"));
        GuiButton *btn4 = GuiButton_createSized(4, self->base.width / 2 + 2, var4 + 72 + 12, 98, 20,
                                                StringTranslate_translateKey(var2, "menu.quit"));
        GuiScreen_addButton((GuiScreen *)self, btn0);
        GuiScreen_addButton((GuiScreen *)self, btn4);
    }
    if (self->base.mc->session == NULL) {
        self->multiplayerButton->enabled = 0;
    }
}

void GuiMainMenu_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiMainMenu *self = (GuiMainMenu *)self_;
    if (var1->id == 0) {
        GuiOptions *s = (GuiOptions *)calloc(1, sizeof(GuiOptions));
        GuiOptions_construct(s, (GuiScreen *)self, self->base.mc->gameSettings);
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)s);
    }
    if (var1->id == 1) {
        GuiSelectWorld *s = (GuiSelectWorld *)calloc(1, sizeof(GuiSelectWorld));
        GuiSelectWorld_construct(s, (GuiScreen *)self);
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)s);
    }
    if (var1->id == 2) {
        GuiMultiplayer *s = (GuiMultiplayer *)calloc(1, sizeof(GuiMultiplayer));
        GuiMultiplayer_construct(s, (GuiScreen *)self);
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)s);
    }
    if (var1->id == 3) {
        GuiTexturePacks *s = (GuiTexturePacks *)calloc(1, sizeof(GuiTexturePacks));
        GuiTexturePacks_construct(s, (GuiScreen *)self);
        Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)s);
    }
    if (var1->id == 4) {

        self->base.mc->running = 0;
    }
}

void GuiMainMenu_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiMainMenu *self = (GuiMainMenu *)self_;
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    Tessellator *var4 = &Tessellator_instance;
    short var5 = 274;
    int var6 = self->base.width / 2 - var5 / 2;
    int var7 = 30;
    R_bindTexture(RenderEngine_getTexture(self->base.mc->renderEngine, "/title/mclogo.png"));
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
    Gui_drawTexturedModalRect((Gui *)self, var6 + 0, var7 + 0, 0, 0, 155, 44);
    Gui_drawTexturedModalRect((Gui *)self, var6 + 155, var7 + 0, 0, 45, 155, 44);
    Tessellator_setColorOpaque_I(var4, 16777215);
    R_pushMatrix();
    R_translatef((float)(self->base.width / 2 + 90), 70.0F, 0.0F);
    R_rotatef(-20.0F, 0.0F, 0.0F, 1.0F);

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    int64_t millis = (int64_t)(ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL);
    float var8 =
        1.8F - MathHelper_abs(
                   MathHelper_sin((float)(millis % 1000L) / 1000.0F * (float)M_PI * 2.0F) * 0.1F);
    var8 = var8 * 100.0F /
           (float)(FontRenderer_getStringWidth(self->base.fontRenderer, self->splashText) + 32);
    R_scalef(var8, var8, var8);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer, self->splashText, 0,
                                 -8, 16776960);
    R_popMatrix();
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer, "Minecraft Beta 1.7.3", 2, 2,
                         5263440);
    const char *var9 = "Copyright Mojang AB. Do not distribute.";
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer, var9,
                         self->base.width -
                             FontRenderer_getStringWidth(self->base.fontRenderer, var9) - 2,
                         self->base.height - 10, 16777215);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.initGui = GuiMainMenu_initGui;
    s_vtable.updateScreen = GuiMainMenu_updateScreen;
    s_vtable.keyTyped = GuiMainMenu_keyTyped;
    s_vtable.actionPerformed = GuiMainMenu_actionPerformed;
    s_vtable.drawScreen = GuiMainMenu_drawScreen;
    s_vtable_init = 1;
}

GuiMainMenu *GuiMainMenu_create(void) {
    ensure_vtable();
    GuiMainMenu *self = (GuiMainMenu *)calloc(1, sizeof(GuiMainMenu));
    if (!self)
        return NULL;
    GuiMainMenu_init(self);
    self->base.vtable = &s_vtable;
    self->base.guiType = GUI_TYPE_MAIN_MENU;
    return self;
}

void GuiMainMenu_free(GuiMainMenu *self) {
    if (!self)
        return;
    for (int i = 0; i < self->base.buttonCount; i++) {
        GuiButton_free(self->base.buttonList[i]);
        self->base.buttonList[i] = NULL;
    }
    self->base.buttonCount = 0;
    free(self);
}
