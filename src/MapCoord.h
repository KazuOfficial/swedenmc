#ifndef MAP_COORD_H
#define MAP_COORD_H

#include <stdint.h>

struct MapData;

typedef struct MapCoord {

    int8_t field_28217_a;

    int8_t field_28216_b;

    int8_t field_28220_c;

    int8_t field_28219_d;

    struct MapData *field_28218_e;
} MapCoord;

void MapCoord_init(MapCoord *self, struct MapData *var1, int8_t var2, int8_t var3, int8_t var4,
                   int8_t var5);

#endif
