#include "MapColor.h"
#include <stdlib.h>

MapColor *MapColor_mapColorArray[MAP_COLOR_COUNT];
MapColor *MapColor_airColor;
MapColor *MapColor_grassColor;
MapColor *MapColor_sandColor;
MapColor *MapColor_clothColor;
MapColor *MapColor_tntColor;
MapColor *MapColor_iceColor;
MapColor *MapColor_ironColor;
MapColor *MapColor_foliageColor;
MapColor *MapColor_snowColor;
MapColor *MapColor_clayColor;
MapColor *MapColor_dirtColor;
MapColor *MapColor_stoneColor;
MapColor *MapColor_waterColor;
MapColor *MapColor_woodColor;

static MapColor *make(int index, int value) {
    MapColor *c = (MapColor *)calloc(1, sizeof(MapColor));
    c->colorIndex = index;
    c->colorValue = value;
    MapColor_mapColorArray[index] = c;
    return c;
}

void MapColor_init(void) {
    MapColor_airColor = make(0, 0);
    MapColor_grassColor = make(1, 8368696);
    MapColor_sandColor = make(2, 16247203);
    MapColor_clothColor = make(3, 10987431);
    MapColor_tntColor = make(4, 16711680);
    MapColor_iceColor = make(5, 10526975);
    MapColor_ironColor = make(6, 10987431);
    MapColor_foliageColor = make(7, 31744);
    MapColor_snowColor = make(8, 16777215);
    MapColor_clayColor = make(9, 10791096);
    MapColor_dirtColor = make(10, 12020271);
    MapColor_stoneColor = make(11, 7368816);
    MapColor_waterColor = make(12, 4210943);
    MapColor_woodColor = make(13, 6837042);
}
