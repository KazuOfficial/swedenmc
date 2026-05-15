#ifndef ENUM_OPTIONS_H
#define ENUM_OPTIONS_H

typedef enum {
    EnumOptions_MUSIC = 0,
    EnumOptions_SOUND = 1,
    EnumOptions_INVERT_MOUSE = 2,
    EnumOptions_SENSITIVITY = 3,
    EnumOptions_RENDER_DISTANCE = 4,
    EnumOptions_VIEW_BOBBING = 5,
    EnumOptions_ANAGLYPH = 6,
    EnumOptions_ADVANCED_OPENGL = 7,
    EnumOptions_FRAMERATE_LIMIT = 8,
    EnumOptions_DIFFICULTY = 9,
    EnumOptions_GRAPHICS = 10,
    EnumOptions_AMBIENT_OCCLUSION = 11,
    EnumOptions_GUI_SCALE = 12
} EnumOptions;

#define ENUM_OPTIONS_COUNT 13

EnumOptions *EnumOptions_getEnumOptions(int var0);

int EnumOptions_getEnumFloat(EnumOptions self);

int EnumOptions_getEnumBoolean(EnumOptions self);

int EnumOptions_returnEnumOrdinal(EnumOptions self);

const char *EnumOptions_getEnumString(EnumOptions self);

#endif
