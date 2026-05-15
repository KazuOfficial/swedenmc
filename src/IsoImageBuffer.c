#include "IsoImageBuffer.h"
#include <string.h>

void IsoImageBuffer_func_889_a(IsoImageBuffer *self, int var1, int var2) {
    self->field_1352_e = 0;
    self->field_1354_c = var1;
    self->field_1353_d = var2;
    self->field_1350_g = 0;
    self->field_1349_h = 0;
}

void IsoImageBuffer_init(IsoImageBuffer *self, World *var1, int var2, int var3) {
    memset(self, 0, sizeof(IsoImageBuffer));
    self->worldObj = var1;
    IsoImageBuffer_func_889_a(self, var2, var3);
}

void IsoImageBuffer_func_888_a(IsoImageBuffer *self, World *var1, int var2, int var3) {
    self->worldObj = var1;
    IsoImageBuffer_func_889_a(self, var2, var3);
}
