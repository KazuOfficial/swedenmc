#include "EnumOptions.h"
#include <stddef.h>

typedef struct {
    const char *enumString;
    int enumFloat;
    int enumBoolean;
} EnumOptionsData;

static const EnumOptionsData _data[ENUM_OPTIONS_COUNT] = {
    {"options.music", 1, 0},          {"options.sound", 1, 0},
    {"options.invertMouse", 0, 1},    {"options.sensitivity", 1, 0},
    {"options.renderDistance", 0, 0}, {"options.viewBobbing", 0, 1},
    {"options.anaglyph", 0, 1},       {"options.advancedOpengl", 0, 1},
    {"options.framerateLimit", 0, 0}, {"options.difficulty", 0, 0},
    {"options.graphics", 0, 0},       {"options.ao", 0, 1},
    {"options.guiScale", 0, 0},
};

EnumOptions *EnumOptions_getEnumOptions(int var0) {
    static EnumOptions result;
    int var2 = ENUM_OPTIONS_COUNT;
    for (int var3 = 0; var3 < var2; ++var3) {
        if (var3 == var0) {
            result = (EnumOptions)var3;
            return &result;
        }
    }
    return NULL;
}

int EnumOptions_getEnumFloat(EnumOptions self) { return _data[self].enumFloat; }

int EnumOptions_getEnumBoolean(EnumOptions self) { return _data[self].enumBoolean; }

int EnumOptions_returnEnumOrdinal(EnumOptions self) { return (int)self; }

const char *EnumOptions_getEnumString(EnumOptions self) { return _data[self].enumString; }
