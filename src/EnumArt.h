#ifndef ENUM_ART_H
#define ENUM_ART_H

typedef struct EnumArt {
    const char *title;
    int sizeX;
    int sizeY;
    int offsetX;
    int offsetY;
} EnumArt;

#define ENUM_ART_MAX_TITLE_LENGTH 13

extern EnumArt EnumArt_Kebab;
extern EnumArt EnumArt_Aztec;
extern EnumArt EnumArt_Alban;
extern EnumArt EnumArt_Aztec2;
extern EnumArt EnumArt_Bomb;
extern EnumArt EnumArt_Plant;
extern EnumArt EnumArt_Wasteland;
extern EnumArt EnumArt_Pool;
extern EnumArt EnumArt_Courbet;
extern EnumArt EnumArt_Sea;
extern EnumArt EnumArt_Sunset;
extern EnumArt EnumArt_Creebet;
extern EnumArt EnumArt_Wanderer;
extern EnumArt EnumArt_Graham;
extern EnumArt EnumArt_Match;
extern EnumArt EnumArt_Bust;
extern EnumArt EnumArt_Stage;
extern EnumArt EnumArt_Void;
extern EnumArt EnumArt_SkullAndRoses;
extern EnumArt EnumArt_Fighters;
extern EnumArt EnumArt_Pointer;
extern EnumArt EnumArt_Pigscene;
extern EnumArt EnumArt_BurningSkull;
extern EnumArt EnumArt_Skeleton;
extern EnumArt EnumArt_DonkeyKong;

extern EnumArt *EnumArt_values[];
#define ENUM_ART_COUNT 25

#endif
