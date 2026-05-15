#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include "EnumOptions.h"

typedef struct KeyBinding {
    const char *keyDescription;
    int keyCode;
    int pressed;
} KeyBinding;

#define NUM_KEY_BINDINGS 10

extern const char *GameSettings_RENDER_DISTANCES[4];
extern const char *GameSettings_DIFFICULTIES[4];
extern const char *GameSettings_GUISCALES[4];
extern const char *GameSettings_LIMIT_FRAMERATES[3];

struct Minecraft;

typedef struct GameSettings {

    struct Minecraft *mc;

    char optionsFile[512];

    float musicVolume;
    float soundVolume;
    float mouseSensitivity;
    int invertMouse;
    int renderDistance;
    int viewBobbing;
    int anaglyph;
    int advancedOpengl;
    int limitFramerate;
    int fancyGraphics;
    int ambientOcclusion;
    int difficulty;
    int hideGUI;
    int thirdPersonView;
    int showDebugInfo;
    int field_22275_C;
    int smoothCamera;
    int field_22273_E;
    float field_22272_F;
    float field_22271_G;
    int guiScale;

    KeyBinding keyBindForward;
    KeyBinding keyBindLeft;
    KeyBinding keyBindBack;
    KeyBinding keyBindRight;
    KeyBinding keyBindJump;
    KeyBinding keyBindSneak;
    KeyBinding keyBindDrop;
    KeyBinding keyBindInventory;
    KeyBinding keyBindChat;
    KeyBinding keyBindToggleFog;
    KeyBinding *keyBindings[NUM_KEY_BINDINGS];

    char lastServer[256];

    char skin[256];
} GameSettings;

void GameSettings_init(GameSettings *self, struct Minecraft *mc, const char *optionsDir);

void GameSettings_loadOptions(GameSettings *self);

void GameSettings_saveOptions(GameSettings *self);

const char *GameSettings_getKeyBindingDescription(GameSettings *self, int var1);

const char *GameSettings_getOptionDisplayString(GameSettings *self, int var1);

void GameSettings_setKeyBinding(GameSettings *self, int var1, int var2);

void GameSettings_setOptionFloatValue(GameSettings *self, EnumOptions var1, float var2);

void GameSettings_setOptionValue(GameSettings *self, EnumOptions var1, int var2);

float GameSettings_getOptionFloatValue(GameSettings *self, EnumOptions var1);

int GameSettings_getOptionOrdinalValue(GameSettings *self, EnumOptions var1);

const char *GameSettings_getKeyBinding(GameSettings *self, EnumOptions var1);

#endif
