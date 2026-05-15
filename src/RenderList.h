#ifndef RENDER_LIST_H
#define RENDER_LIST_H

#include "Renderer.h"

#define RENDER_LIST_MAX 65536

typedef struct RenderList {
    int field_1242_a;
    int field_1241_b;
    int field_1240_c;
    float field_1239_d;
    float field_1238_e;
    float field_1237_f;
    RBuffer *buffers[RENDER_LIST_MAX];
    float translations[RENDER_LIST_MAX][3];
    int listCount;
    int field_1235_h;
    int field_1234_i;
} RenderList;

void RenderList_init(RenderList *self);

void RenderList_func_861_a(RenderList *self, int x, int y, int z, double cx, double cy, double cz);

int RenderList_func_862_a(RenderList *self, int x, int y, int z);

void RenderList_func_858_a(RenderList *self, RBuffer *buf, float tx, float ty, float tz);

void RenderList_func_860_a(RenderList *self);

void RenderList_func_859_b(RenderList *self);

#endif
