#include "GuiCreateWorld.h"
#include "Minecraft.h"
#include "ISaveFormat.h"
#include "MathHelper.h"
#include "ChatAllowedCharacters.h"
#include "PlayerControllerSP.h"
#include "JavaRandom.h"
#include "StringTranslate.h"
#include "GuiTextField.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void cw_destroy(GuiScreen *base) {
    GuiCreateWorld *self = (GuiCreateWorld *)base;
    free(self->textboxWorldName);
    self->textboxWorldName = NULL;
    free(self->textboxSeed);
    self->textboxSeed = NULL;
}

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.destroy = cw_destroy;
    s_vtable.updateScreen = GuiCreateWorld_updateScreen;
    s_vtable.initGui = GuiCreateWorld_initGui;
    s_vtable.onGuiClosed = GuiCreateWorld_onGuiClosed;
    s_vtable.actionPerformed = GuiCreateWorld_actionPerformed;
    s_vtable.keyTyped = GuiCreateWorld_keyTyped;
    s_vtable.mouseClicked = GuiCreateWorld_mouseClicked;
    s_vtable.drawScreen = GuiCreateWorld_drawScreen;
    s_vtable.selectNextField = GuiCreateWorld_selectNextField;
    s_vtable_init = 1;
}

void GuiCreateWorld_construct(GuiCreateWorld *self, GuiScreen *var1) {
    ensure_vtable();
    self->field_22131_a = var1;
    self->base.parentScreen = var1;
    self->textboxWorldName = NULL;
    self->textboxSeed = NULL;
    self->folderName[0] = '\0';
    self->createClicked = 0;
    self->base.vtable = &s_vtable;
}

void GuiCreateWorld_updateScreen(GuiScreen *self_) {
    GuiCreateWorld *self = (GuiCreateWorld *)self_;
    GuiTextField_updateCursorCounter(self->textboxWorldName);
    GuiTextField_updateCursorCounter(self->textboxSeed);
}

void GuiCreateWorld_initGui(GuiScreen *self_) {
    GuiCreateWorld *self = (GuiCreateWorld *)self_;
    StringTranslate *var1 = StringTranslate_getInstance();

    Minecraft_enableKeyRepeatEvents(1);
    self->base.buttonCount = 0;
    GuiButton *btn0 =
        GuiButton_create(0, self->base.width / 2 - 100, self->base.height / 4 + 96 + 12,
                         StringTranslate_translateKey(var1, "selectWorld.create"));
    GuiButton *btn1 =
        GuiButton_create(1, self->base.width / 2 - 100, self->base.height / 4 + 120 + 12,
                         StringTranslate_translateKey(var1, "gui.cancel"));
    GuiScreen_addButton((GuiScreen *)self, btn0);
    GuiScreen_addButton((GuiScreen *)self, btn1);
    self->textboxWorldName = (GuiTextField *)calloc(1, sizeof(GuiTextField));
    GuiTextField_construct(self->textboxWorldName, (GuiScreen *)self, self->base.fontRenderer,
                           self->base.width / 2 - 100, 60, 200, 20,
                           StringTranslate_translateKey(var1, "selectWorld.newWorld"));
    self->textboxWorldName->isFocused = 1;
    GuiTextField_setMaxStringLength(self->textboxWorldName, 32);
    self->textboxSeed = (GuiTextField *)calloc(1, sizeof(GuiTextField));
    GuiTextField_construct(self->textboxSeed, (GuiScreen *)self, self->base.fontRenderer,
                           self->base.width / 2 - 100, 116, 200, 20, "");
    GuiCreateWorld_func_22129_j(self);
}

void GuiCreateWorld_func_22129_j(GuiCreateWorld *self) {
    const char *text = self->textboxWorldName ? GuiTextField_getText(self->textboxWorldName) : "";

    while (*text == ' ')
        ++text;
    strncpy(self->folderName, text, sizeof(self->folderName) - 1);
    self->folderName[sizeof(self->folderName) - 1] = '\0';
    int len = (int)strlen(self->folderName);
    while (len > 0 && self->folderName[len - 1] == ' ')
        self->folderName[--len] = '\0';

    for (int var3 = 0; var3 < ChatAllowedCharacters_allowedCharactersArray_length; ++var3) {
        char var4 = ChatAllowedCharacters_allowedCharactersArray[var3];
        for (int i = 0; self->folderName[i]; ++i) {
            if (self->folderName[i] == var4)
                self->folderName[i] = '_';
        }
    }

    if (MathHelper_stringNullOrLengthZero(self->folderName)) {
        strncpy(self->folderName, "World", sizeof(self->folderName) - 1);
    }

    struct ISaveFormat *saveLoader = Minecraft_getSaveLoader(self->base.mc);
    if (saveLoader) {
        GuiCreateWorld_generateUnusedFolderName(saveLoader, self->folderName, self->folderName,
                                                (int)sizeof(self->folderName));
    }
}

const char *GuiCreateWorld_generateUnusedFolderName(struct ISaveFormat *var0, const char *var1,
                                                    char *outBuf, int bufLen) {
    if (outBuf != var1) {
        strncpy(outBuf, var1, bufLen - 1);
        outBuf[bufLen - 1] = '\0';
    }

    WorldInfo *wi;
    while ((wi = ISaveFormat_func_22173_b(var0, outBuf)) != NULL) {
        WorldInfo_free(wi);
        int len = (int)strlen(outBuf);
        if (len < bufLen - 1) {
            outBuf[len] = '-';
            outBuf[len + 1] = '\0';
        } else {
            break;
        }
    }
    return outBuf;
}

void GuiCreateWorld_onGuiClosed(GuiScreen *self_) {
    (void)self_;

    Minecraft_enableKeyRepeatEvents(0);
}

void GuiCreateWorld_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiCreateWorld *self = (GuiCreateWorld *)self_;
    if (var1->enabled) {
        if (var1->id == 1) {
            Minecraft_displayGuiScreen(self->base.mc, self->field_22131_a);
        } else if (var1->id == 0) {
            Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)NULL);
            if (self->createClicked) {
                return;
            }
            self->createClicked = 1;
            JavaRandom rng;
            JavaRandom_init(&rng, 0LL);
            int64_t var2 = JavaRandom_nextLong(&rng);
            const char *var4 = GuiTextField_getText(self->textboxSeed);
            if (!MathHelper_stringNullOrLengthZero(var4)) {
                char *endptr;
                int64_t var5 = (int64_t)strtoll(var4, &endptr, 10);
                if (*endptr == '\0') {

                    if (var5 != 0L) {
                        var2 = var5;
                    }
                } else {

                    int64_t hash = 0;
                    for (const char *p = var4; *p; ++p) {
                        hash = 31 * hash + (unsigned char)*p;
                    }
                    var2 = (int64_t)(int)hash;
                }
            }
            PlayerController_destroy(self->base.mc->playerController);
            self->base.mc->playerController =
                (PlayerController *)PlayerControllerSP_create(self->base.mc);

            Minecraft_startWorld(self->base.mc, self->folderName,
                                 GuiTextField_getText(self->textboxWorldName), var2);
            Minecraft_displayGuiScreen(self->base.mc, (GuiScreen *)NULL);
        }
    }
}

void GuiCreateWorld_keyTyped(GuiScreen *self_, char var1, int var2) {
    GuiCreateWorld *self = (GuiCreateWorld *)self_;
    if (self->textboxWorldName->isFocused) {
        GuiTextField_textboxKeyTyped(self->textboxWorldName, var1, var2);
    } else {
        GuiTextField_textboxKeyTyped(self->textboxSeed, var1, var2);
    }
    if (var1 == 13) {
        GuiCreateWorld_actionPerformed((GuiScreen *)self, self->base.buttonList[0]);
    }
    self->base.buttonList[0]->enabled =
        (strlen(GuiTextField_getText(self->textboxWorldName)) > 0) ? 1 : 0;
    GuiCreateWorld_func_22129_j(self);
}

void GuiCreateWorld_mouseClicked(GuiScreen *self_, int var1, int var2, int var3) {
    GuiCreateWorld *self = (GuiCreateWorld *)self_;
    GuiScreen_defaultVtable.mouseClicked((GuiScreen *)self, var1, var2, var3);
    GuiTextField_mouseClicked(self->textboxWorldName, var1, var2, var3);
    GuiTextField_mouseClicked(self->textboxSeed, var1, var2, var3);
}

void GuiCreateWorld_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiCreateWorld *self = (GuiCreateWorld *)self_;
    StringTranslate *var4 = StringTranslate_getInstance();
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    GuiScreen_drawCenteredString((GuiScreen *)self, self->base.fontRenderer,
                                 StringTranslate_translateKey(var4, "selectWorld.create"),
                                 self->base.width / 2, self->base.height / 4 - 60 + 20, 16777215);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         StringTranslate_translateKey(var4, "selectWorld.enterName"),
                         self->base.width / 2 - 100, 47, 10526880);
    char resultLine[512];
    snprintf(resultLine, sizeof(resultLine), "%s %s",
             StringTranslate_translateKey(var4, "selectWorld.resultFolder"), self->folderName);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer, resultLine,
                         self->base.width / 2 - 100, 85, 10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         StringTranslate_translateKey(var4, "selectWorld.enterSeed"),
                         self->base.width / 2 - 100, 104, 10526880);
    GuiScreen_drawString((GuiScreen *)self, self->base.fontRenderer,
                         StringTranslate_translateKey(var4, "selectWorld.seedInfo"),
                         self->base.width / 2 - 100, 140, 10526880);
    GuiTextField_drawTextBox(self->textboxWorldName);
    GuiTextField_drawTextBox(self->textboxSeed);
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
}

void GuiCreateWorld_selectNextField(GuiScreen *self_) {
    GuiCreateWorld *self = (GuiCreateWorld *)self_;
    if (self->textboxWorldName->isFocused) {
        GuiTextField_setFocused(self->textboxWorldName, 0);
        GuiTextField_setFocused(self->textboxSeed, 1);
    } else {
        GuiTextField_setFocused(self->textboxWorldName, 1);
        GuiTextField_setFocused(self->textboxSeed, 0);
    }
}
