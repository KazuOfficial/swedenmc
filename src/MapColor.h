#ifndef MAP_COLOR_H
#define MAP_COLOR_H

#define MAP_COLOR_COUNT 16

typedef struct {
    int colorIndex;
    int colorValue;
} MapColor;

extern MapColor *MapColor_mapColorArray[MAP_COLOR_COUNT];

extern MapColor *MapColor_airColor;
extern MapColor *MapColor_grassColor;
extern MapColor *MapColor_sandColor;
extern MapColor *MapColor_clothColor;
extern MapColor *MapColor_tntColor;
extern MapColor *MapColor_iceColor;
extern MapColor *MapColor_ironColor;
extern MapColor *MapColor_foliageColor;
extern MapColor *MapColor_snowColor;
extern MapColor *MapColor_clayColor;
extern MapColor *MapColor_dirtColor;
extern MapColor *MapColor_stoneColor;
extern MapColor *MapColor_waterColor;
extern MapColor *MapColor_woodColor;

void MapColor_init(void);

#endif
