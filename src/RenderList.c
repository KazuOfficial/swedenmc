#include "RenderList.h"
#include <string.h>

void RenderList_init(RenderList *self) { memset(self, 0, sizeof(RenderList)); }

void RenderList_func_861_a(RenderList *self, int x, int y, int z, double cx, double cy, double cz) {
    self->field_1235_h = 1;
    self->listCount = 0;
    self->field_1234_i = 0;
    self->field_1242_a = x;
    self->field_1241_b = y;
    self->field_1240_c = z;
    self->field_1239_d = (float)cx;
    self->field_1238_e = (float)cy;
    self->field_1237_f = (float)cz;
}

int RenderList_func_862_a(RenderList *self, int x, int y, int z) {
    if (!self->field_1235_h)
        return 0;
    return x == self->field_1242_a && y == self->field_1241_b && z == self->field_1240_c;
}

void RenderList_func_858_a(RenderList *self, RBuffer *buf, float tx, float ty, float tz) {
    if (!buf)
        return;
    int i = self->listCount++;
    self->buffers[i] = buf;
    self->translations[i][0] = tx;
    self->translations[i][1] = ty;
    self->translations[i][2] = tz;
    if (self->listCount >= RENDER_LIST_MAX)
        RenderList_func_860_a(self);
}

void RenderList_func_860_a(RenderList *self) {
    if (!self->field_1235_h)
        return;
    if (!self->field_1234_i)
        self->field_1234_i = 1;

    float reg_tx = (float)self->field_1242_a - self->field_1239_d;
    float reg_ty = (float)self->field_1241_b - self->field_1238_e;
    float reg_tz = (float)self->field_1240_c - self->field_1237_f;

    for (int i = 0; i < self->listCount; i++) {
        R_bufferDraw(self->buffers[i], self->translations[i][0] + reg_tx,
                     self->translations[i][1] + reg_ty, self->translations[i][2] + reg_tz);
    }
}

void RenderList_func_859_b(RenderList *self) {
    self->field_1235_h = 0;
    self->field_1234_i = 0;
    self->listCount = 0;
}
