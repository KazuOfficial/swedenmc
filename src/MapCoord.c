#include "MapCoord.h"

void MapCoord_init(MapCoord *self, struct MapData *var1, int8_t var2, int8_t var3, int8_t var4,
                   int8_t var5) {
    self->field_28218_e = var1;
    self->field_28217_a = var2;
    self->field_28216_b = var3;
    self->field_28220_c = var4;
    self->field_28219_d = var5;
}
