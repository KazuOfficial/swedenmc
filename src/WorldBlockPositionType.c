#include "WorldClient.h"
#include "WorldBlockPositionType.h"

void WorldBlockPositionType_init(WorldBlockPositionType *self, WorldClient *var1, int var2,
                                 int var3, int var4, int var5, int var6) {
    self->field_1203_g = var1;
    self->field_1202_a = var2;
    self->field_1201_b = var3;
    self->field_1207_c = var4;
    self->field_1206_d = 80;
    self->field_1205_e = var5;
    self->field_1204_f = var6;
}
