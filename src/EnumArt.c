#include "EnumArt.h"
#include <stddef.h>

EnumArt EnumArt_Kebab = {"Kebab", 16, 16, 0, 0};
EnumArt EnumArt_Aztec = {"Aztec", 16, 16, 16, 0};
EnumArt EnumArt_Alban = {"Alban", 16, 16, 32, 0};
EnumArt EnumArt_Aztec2 = {"Aztec2", 16, 16, 48, 0};
EnumArt EnumArt_Bomb = {"Bomb", 16, 16, 64, 0};
EnumArt EnumArt_Plant = {"Plant", 16, 16, 80, 0};
EnumArt EnumArt_Wasteland = {"Wasteland", 16, 16, 96, 0};
EnumArt EnumArt_Pool = {"Pool", 32, 16, 0, 32};
EnumArt EnumArt_Courbet = {"Courbet", 32, 16, 32, 32};
EnumArt EnumArt_Sea = {"Sea", 32, 16, 64, 32};
EnumArt EnumArt_Sunset = {"Sunset", 32, 16, 96, 32};
EnumArt EnumArt_Creebet = {"Creebet", 32, 16, 128, 32};
EnumArt EnumArt_Wanderer = {"Wanderer", 16, 32, 0, 64};
EnumArt EnumArt_Graham = {"Graham", 16, 32, 16, 64};
EnumArt EnumArt_Match = {"Match", 32, 32, 0, 128};
EnumArt EnumArt_Bust = {"Bust", 32, 32, 32, 128};
EnumArt EnumArt_Stage = {"Stage", 32, 32, 64, 128};
EnumArt EnumArt_Void = {"Void", 32, 32, 96, 128};
EnumArt EnumArt_SkullAndRoses = {"SkullAndRoses", 32, 32, 128, 128};
EnumArt EnumArt_Fighters = {"Fighters", 64, 32, 0, 96};
EnumArt EnumArt_Pointer = {"Pointer", 64, 64, 0, 192};
EnumArt EnumArt_Pigscene = {"Pigscene", 64, 64, 64, 192};
EnumArt EnumArt_BurningSkull = {"BurningSkull", 64, 64, 128, 192};
EnumArt EnumArt_Skeleton = {"Skeleton", 64, 48, 192, 64};
EnumArt EnumArt_DonkeyKong = {"DonkeyKong", 64, 48, 192, 112};

EnumArt *EnumArt_values[] = {&EnumArt_Kebab,         &EnumArt_Aztec,
                             &EnumArt_Alban,         &EnumArt_Aztec2,
                             &EnumArt_Bomb,          &EnumArt_Plant,
                             &EnumArt_Wasteland,     &EnumArt_Pool,
                             &EnumArt_Courbet,       &EnumArt_Sea,
                             &EnumArt_Sunset,        &EnumArt_Creebet,
                             &EnumArt_Wanderer,      &EnumArt_Graham,
                             &EnumArt_Match,         &EnumArt_Bust,
                             &EnumArt_Stage,         &EnumArt_Void,
                             &EnumArt_SkullAndRoses, &EnumArt_Fighters,
                             &EnumArt_Pointer,       &EnumArt_Pigscene,
                             &EnumArt_BurningSkull,  &EnumArt_Skeleton,
                             &EnumArt_DonkeyKong,    NULL};
