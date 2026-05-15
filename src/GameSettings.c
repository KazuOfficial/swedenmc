#include "GameSettings.h"
#include "StringTranslate.h"
#include "StatCollector.h"
#include "SoundManager.h"
#include "RenderGlobal.h"
#include "Minecraft.h"
#include <GLFW/glfw3.h>
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char *GameSettings_RENDER_DISTANCES[4] = {
    "options.renderDistance.far", "options.renderDistance.normal", "options.renderDistance.short",
    "options.renderDistance.tiny"};

const char *GameSettings_DIFFICULTIES[4] = {"options.difficulty.peaceful",
                                            "options.difficulty.easy", "options.difficulty.normal",
                                            "options.difficulty.hard"};

const char *GameSettings_GUISCALES[4] = {"options.guiScale.auto", "options.guiScale.small",
                                         "options.guiScale.normal", "options.guiScale.large"};

const char *GameSettings_LIMIT_FRAMERATES[3] = {"performance.max", "performance.balanced",
                                                "performance.powersaver"};

#define KEY_W 87
#define KEY_A 65
#define KEY_S 83
#define KEY_D 68
#define KEY_SPACE 32
#define KEY_LSHIFT 340
#define KEY_Q 81
#define KEY_E 69
#define KEY_T 84
#define KEY_F 70

void GameSettings_init(GameSettings *self, struct Minecraft *mc, const char *optionsDir) {
    memset(self, 0, sizeof(GameSettings));

    self->mc = mc;
    if (optionsDir)
        snprintf(self->optionsFile, sizeof(self->optionsFile), "%s/options.txt", optionsDir);
    else
        snprintf(self->optionsFile, sizeof(self->optionsFile), "options.txt");

    self->musicVolume = 1.0f;
    self->soundVolume = 1.0f;
    self->mouseSensitivity = 0.5f;
    self->renderDistance = 0;
    self->viewBobbing = 1;
    self->fancyGraphics = 1;
    self->ambientOcclusion = 1;
    self->limitFramerate = 1;
    self->difficulty = 2;
    self->field_22272_F = 1.0f;
    self->field_22271_G = 1.0f;

    self->keyBindForward = (KeyBinding){"key.forward", KEY_W, 0};
    self->keyBindLeft = (KeyBinding){"key.left", KEY_A, 0};
    self->keyBindBack = (KeyBinding){"key.back", KEY_S, 0};
    self->keyBindRight = (KeyBinding){"key.right", KEY_D, 0};
    self->keyBindJump = (KeyBinding){"key.jump", KEY_SPACE, 0};
    self->keyBindSneak = (KeyBinding){"key.sneak", KEY_LSHIFT, 0};
    self->keyBindDrop = (KeyBinding){"key.drop", KEY_Q, 0};
    self->keyBindInventory = (KeyBinding){"key.inventory", KEY_E, 0};
    self->keyBindChat = (KeyBinding){"key.chat", KEY_T, 0};
    self->keyBindToggleFog = (KeyBinding){"key.fog", KEY_F, 0};

    self->keyBindings[0] = &self->keyBindForward;
    self->keyBindings[1] = &self->keyBindLeft;
    self->keyBindings[2] = &self->keyBindBack;
    self->keyBindings[3] = &self->keyBindRight;
    self->keyBindings[4] = &self->keyBindJump;
    self->keyBindings[5] = &self->keyBindSneak;
    self->keyBindings[6] = &self->keyBindDrop;
    self->keyBindings[7] = &self->keyBindInventory;
    self->keyBindings[8] = &self->keyBindChat;
    self->keyBindings[9] = &self->keyBindToggleFog;

    strncpy(self->skin, "Default", sizeof(self->skin) - 1);

    GameSettings_loadOptions(self);
}

void GameSettings_loadOptions(GameSettings *self) {
    FILE *f = fopen(self->optionsFile, "r");
    if (!f)
        return;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char *colon = strchr(line, ':');
        if (!colon)
            continue;
        *colon = '\0';
        const char *key = line;
        const char *val = colon + 1;
        char v[128];
        snprintf(v, sizeof(v), "%s", val);
        size_t l = strlen(v);
        while (l > 0 && (v[l - 1] == '\n' || v[l - 1] == '\r'))
            v[--l] = '\0';

        if (!strcmp(key, "music"))
            self->musicVolume = (float)atof(v);
        else if (!strcmp(key, "sound"))
            self->soundVolume = (float)atof(v);
        else if (!strcmp(key, "mouseSensitivity"))
            self->mouseSensitivity = (float)atof(v);
        else if (!strcmp(key, "invertYMouse"))
            self->invertMouse = atoi(v);
        else if (!strcmp(key, "viewDistance"))
            self->renderDistance = atoi(v);
        else if (!strcmp(key, "guiScale"))
            self->guiScale = atoi(v);
        else if (!strcmp(key, "bobView"))
            self->viewBobbing = atoi(v);
        else if (!strcmp(key, "anaglyph3d"))
            self->anaglyph = atoi(v);
        else if (!strcmp(key, "advancedOpengl"))
            self->advancedOpengl = atoi(v);
        else if (!strcmp(key, "fpsLimit"))
            self->limitFramerate = atoi(v);
        else if (!strcmp(key, "difficulty"))
            self->difficulty = atoi(v);
        else if (!strcmp(key, "fancyGraphics"))
            self->fancyGraphics = atoi(v);
        else if (!strcmp(key, "ao"))
            self->ambientOcclusion = atoi(v);
        else if (!strcmp(key, "skin"))
            strncpy(self->skin, v, sizeof(self->skin) - 1);
        else if (!strcmp(key, "lastServer") && strlen(v) > 0)
            strncpy(self->lastServer, v, sizeof(self->lastServer) - 1);

        for (int i = 0; i < NUM_KEY_BINDINGS; i++) {
            if (self->keyBindings[i]) {
                char expected[128];
                snprintf(expected, sizeof(expected), "key_%s",
                         self->keyBindings[i]->keyDescription);
                if (!strcmp(key, expected)) {
                    self->keyBindings[i]->keyCode = atoi(v);
                    break;
                }
            }
        }
    }
    fclose(f);
}

void GameSettings_saveOptions(GameSettings *self) {
    FILE *f = fopen(self->optionsFile, "w");
    if (!f) {
        printf("Failed to save options\n");
        return;
    }
    fprintf(f, "music:%.2f\n", self->musicVolume);
    fprintf(f, "sound:%.2f\n", self->soundVolume);
    fprintf(f, "invertYMouse:%d\n", self->invertMouse);
    fprintf(f, "mouseSensitivity:%.2f\n", self->mouseSensitivity);
    fprintf(f, "viewDistance:%d\n", self->renderDistance);
    fprintf(f, "guiScale:%d\n", self->guiScale);
    fprintf(f, "bobView:%d\n", self->viewBobbing);
    fprintf(f, "anaglyph3d:%d\n", self->anaglyph);
    fprintf(f, "advancedOpengl:%d\n", self->advancedOpengl);
    fprintf(f, "fpsLimit:%d\n", self->limitFramerate);
    fprintf(f, "difficulty:%d\n", self->difficulty);
    fprintf(f, "fancyGraphics:%d\n", self->fancyGraphics);
    fprintf(f, "ao:%d\n", self->ambientOcclusion);
    fprintf(f, "skin:%s\n", self->skin);
    fprintf(f, "lastServer:%s\n", self->lastServer);

    for (int i = 0; i < NUM_KEY_BINDINGS; i++) {
        if (self->keyBindings[i])
            fprintf(f, "key_%s:%d\n", self->keyBindings[i]->keyDescription,
                    self->keyBindings[i]->keyCode);
    }
    fclose(f);
}

const char *GameSettings_getKeyBindingDescription(GameSettings *self, int var1) {
    StringTranslate *var2 = StringTranslate_getInstance();
    return StringTranslate_translateKey(var2, self->keyBindings[var1]->keyDescription);
}

const char *GameSettings_getOptionDisplayString(GameSettings *self, int var1) {
    int keyCode = self->keyBindings[var1]->keyCode;
    const char *name = glfwGetKeyName(keyCode, 0);
    if (name) {

        static char buf[64];
        strncpy(buf, name, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        for (int i = 0; buf[i]; i++)
            buf[i] = (char)toupper((unsigned char)buf[i]);
        return buf;
    }

    switch (keyCode) {
    case GLFW_KEY_SPACE:
        return "SPACE";
    case GLFW_KEY_LEFT_SHIFT:
        return "LSHIFT";
    case GLFW_KEY_RIGHT_SHIFT:
        return "RSHIFT";
    case GLFW_KEY_LEFT_CONTROL:
        return "LCONTROL";
    case GLFW_KEY_LEFT_ALT:
        return "LMENU";
    case GLFW_KEY_ENTER:
        return "RETURN";
    case GLFW_KEY_ESCAPE:
        return "ESCAPE";
    case GLFW_KEY_TAB:
        return "TAB";
    case GLFW_KEY_BACKSPACE:
        return "BACK";
    case GLFW_KEY_UP:
        return "UP";
    case GLFW_KEY_DOWN:
        return "DOWN";
    case GLFW_KEY_LEFT:
        return "LEFT";
    case GLFW_KEY_RIGHT:
        return "RIGHT";
    default:
        return "?";
    }
}

void GameSettings_setKeyBinding(GameSettings *self, int var1, int var2) {
    self->keyBindings[var1]->keyCode = var2;
    GameSettings_saveOptions(self);
}

void GameSettings_setOptionFloatValue(GameSettings *self, EnumOptions var1, float var2) {
    if (var1 == EnumOptions_MUSIC) {
        self->musicVolume = var2;
        if (self->mc && self->mc->sndManager)
            SoundManager_onSoundOptionsChanged(self->mc->sndManager);
    }
    if (var1 == EnumOptions_SOUND) {
        self->soundVolume = var2;
        if (self->mc && self->mc->sndManager)
            SoundManager_onSoundOptionsChanged(self->mc->sndManager);
    }
    if (var1 == EnumOptions_SENSITIVITY) {
        self->mouseSensitivity = var2;
    }
}

void GameSettings_setOptionValue(GameSettings *self, EnumOptions var1, int var2) {
    if (var1 == EnumOptions_INVERT_MOUSE) {
        self->invertMouse = !self->invertMouse;
    }
    if (var1 == EnumOptions_RENDER_DISTANCE) {
        self->renderDistance = (self->renderDistance + var2) & 3;
    }
    if (var1 == EnumOptions_GUI_SCALE) {
        self->guiScale = (self->guiScale + var2) & 3;
    }
    if (var1 == EnumOptions_VIEW_BOBBING) {
        self->viewBobbing = !self->viewBobbing;
    }
    if (var1 == EnumOptions_ADVANCED_OPENGL) {
        self->advancedOpengl = !self->advancedOpengl;
        if (self->mc && self->mc->renderGlobal)
            RenderGlobal_loadRenderers(self->mc->renderGlobal);
    }
    if (var1 == EnumOptions_ANAGLYPH) {
        self->anaglyph = !self->anaglyph;
        RenderEngine_refreshTextures(self->mc->renderEngine);
    }
    if (var1 == EnumOptions_FRAMERATE_LIMIT) {
        self->limitFramerate = (self->limitFramerate + var2 + 3) % 3;
    }
    if (var1 == EnumOptions_DIFFICULTY) {
        self->difficulty = (self->difficulty + var2) & 3;
    }
    if (var1 == EnumOptions_GRAPHICS) {
        self->fancyGraphics = !self->fancyGraphics;
        if (self->mc && self->mc->renderGlobal)
            RenderGlobal_loadRenderers(self->mc->renderGlobal);
    }
    if (var1 == EnumOptions_AMBIENT_OCCLUSION) {
        self->ambientOcclusion = !self->ambientOcclusion;
        if (self->mc && self->mc->renderGlobal)
            RenderGlobal_loadRenderers(self->mc->renderGlobal);
    }
    GameSettings_saveOptions(self);
}

float GameSettings_getOptionFloatValue(GameSettings *self, EnumOptions var1) {
    return var1 == EnumOptions_MUSIC
               ? self->musicVolume
               : (var1 == EnumOptions_SOUND
                      ? self->soundVolume
                      : (var1 == EnumOptions_SENSITIVITY ? self->mouseSensitivity : 0.0f));
}

int GameSettings_getOptionOrdinalValue(GameSettings *self, EnumOptions var1) {
    switch (var1) {
    case EnumOptions_INVERT_MOUSE:
        return self->invertMouse;
    case EnumOptions_VIEW_BOBBING:
        return self->viewBobbing;
    case EnumOptions_ANAGLYPH:
        return self->anaglyph;
    case EnumOptions_ADVANCED_OPENGL:
        return self->advancedOpengl;
    case EnumOptions_AMBIENT_OCCLUSION:
        return self->ambientOcclusion;
    default:
        return 0;
    }
}

const char *GameSettings_getKeyBinding(GameSettings *self, EnumOptions var1) {
    StringTranslate *var2 = StringTranslate_getInstance();
    static char buf[256];
    const char *var3_str = StringTranslate_translateKey(var2, EnumOptions_getEnumString(var1));

    char var3[200];
    snprintf(var3, sizeof(var3), "%s: ", var3_str);

    if (EnumOptions_getEnumFloat(var1)) {
        float var5 = GameSettings_getOptionFloatValue(self, var1);
        if (var1 == EnumOptions_SENSITIVITY) {
            if (var5 == 0.0f)
                snprintf(buf, sizeof(buf), "%s%s", var3,
                         StringTranslate_translateKey(var2, "options.sensitivity.min"));
            else if (var5 == 1.0f)
                snprintf(buf, sizeof(buf), "%s%s", var3,
                         StringTranslate_translateKey(var2, "options.sensitivity.max"));
            else
                snprintf(buf, sizeof(buf), "%s%d%%", var3, (int)(var5 * 200.0f));
        } else {
            if (var5 == 0.0f)
                snprintf(buf, sizeof(buf), "%s%s", var3,
                         StringTranslate_translateKey(var2, "options.off"));
            else
                snprintf(buf, sizeof(buf), "%s%d%%", var3, (int)(var5 * 100.0f));
        }
    } else if (EnumOptions_getEnumBoolean(var1)) {
        int var4 = GameSettings_getOptionOrdinalValue(self, var1);
        if (var4)
            snprintf(buf, sizeof(buf), "%s%s", var3,
                     StringTranslate_translateKey(var2, "options.on"));
        else
            snprintf(buf, sizeof(buf), "%s%s", var3,
                     StringTranslate_translateKey(var2, "options.off"));
    } else {
        if (var1 == EnumOptions_RENDER_DISTANCE)
            snprintf(buf, sizeof(buf), "%s%s", var3,
                     StringTranslate_translateKey(
                         var2, GameSettings_RENDER_DISTANCES[self->renderDistance]));
        else if (var1 == EnumOptions_DIFFICULTY)
            snprintf(
                buf, sizeof(buf), "%s%s", var3,
                StringTranslate_translateKey(var2, GameSettings_DIFFICULTIES[self->difficulty]));
        else if (var1 == EnumOptions_GUI_SCALE)
            snprintf(buf, sizeof(buf), "%s%s", var3,
                     StringTranslate_translateKey(var2, GameSettings_GUISCALES[self->guiScale]));
        else if (var1 == EnumOptions_FRAMERATE_LIMIT)
            snprintf(buf, sizeof(buf), "%s%s", var3,
                     StatCollector_translateToLocal(
                         GameSettings_LIMIT_FRAMERATES[self->limitFramerate]));
        else if (var1 == EnumOptions_GRAPHICS)
            snprintf(buf, sizeof(buf), "%s%s", var3,
                     self->fancyGraphics
                         ? StringTranslate_translateKey(var2, "options.graphics.fancy")
                         : StringTranslate_translateKey(var2, "options.graphics.fast"));
        else
            snprintf(buf, sizeof(buf), "%s", var3);
    }
    return buf;
}
