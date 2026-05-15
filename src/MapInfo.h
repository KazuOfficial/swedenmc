#ifndef MAP_INFO_H
#define MAP_INFO_H

struct EntityPlayer;
struct MapData;

typedef struct MapInfo {

    struct EntityPlayer *entityplayerObj;

    int field_28119_b[128];

    int field_28124_c[128];

    int field_28122_e;

    int field_28121_f;

    struct MapData *mapDataObj;
} MapInfo;

void MapInfo_init(MapInfo *self, struct MapData *var1, struct EntityPlayer *var2);

#endif
