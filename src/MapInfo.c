#include "MapInfo.h"

void MapInfo_init(MapInfo *self, struct MapData *var1, struct EntityPlayer *var2) {
    self->mapDataObj = var1;

    self->field_28122_e = 0;
    self->field_28121_f = 0;
    self->entityplayerObj = var2;

    for (int var3 = 0; var3 < 128; ++var3) {
        self->field_28119_b[var3] = 0;
        self->field_28124_c[var3] = 127;
    }
}
