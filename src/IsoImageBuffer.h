#ifndef ISO_IMAGE_BUFFER_H
#define ISO_IMAGE_BUFFER_H

#include "GameForward.h"

typedef struct IsoImageBuffer {

    int *field_1348_a;
    World *worldObj;
    int field_1354_c;
    int field_1353_d;
    int field_1352_e;
    int field_1351_f;
    int field_1350_g;
    int field_1349_h;
} IsoImageBuffer;

void IsoImageBuffer_init(IsoImageBuffer *self, World *var1, int var2, int var3);

void IsoImageBuffer_func_889_a(IsoImageBuffer *self, int var1, int var2);

void IsoImageBuffer_func_888_a(IsoImageBuffer *self, World *var1, int var2, int var3);

#endif
