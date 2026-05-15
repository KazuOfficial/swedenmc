#include "ClippingHelperImpl.h"
#include "Renderer.h"
#include <math.h>

static ClippingHelper _instance;

static void normalize(float f[6][4], int p) {
    float mag = sqrtf(f[p][0] * f[p][0] + f[p][1] * f[p][1] + f[p][2] * f[p][2]);
    f[p][0] /= mag;
    f[p][1] /= mag;
    f[p][2] /= mag;
    f[p][3] /= mag;
}

static void init(ClippingHelper *c) {

    R_getFloatv(R_PROJECTION_MATRIX, c->projectionMatrix);
    R_getFloatv(R_MODELVIEW_MATRIX, c->modelviewMatrix);

    float *p = c->projectionMatrix;
    float *m = c->modelviewMatrix;
    float *cl = c->clippingMatrix;

    cl[0] = m[0] * p[0] + m[1] * p[4] + m[2] * p[8] + m[3] * p[12];
    cl[1] = m[0] * p[1] + m[1] * p[5] + m[2] * p[9] + m[3] * p[13];
    cl[2] = m[0] * p[2] + m[1] * p[6] + m[2] * p[10] + m[3] * p[14];
    cl[3] = m[0] * p[3] + m[1] * p[7] + m[2] * p[11] + m[3] * p[15];
    cl[4] = m[4] * p[0] + m[5] * p[4] + m[6] * p[8] + m[7] * p[12];
    cl[5] = m[4] * p[1] + m[5] * p[5] + m[6] * p[9] + m[7] * p[13];
    cl[6] = m[4] * p[2] + m[5] * p[6] + m[6] * p[10] + m[7] * p[14];
    cl[7] = m[4] * p[3] + m[5] * p[7] + m[6] * p[11] + m[7] * p[15];
    cl[8] = m[8] * p[0] + m[9] * p[4] + m[10] * p[8] + m[11] * p[12];
    cl[9] = m[8] * p[1] + m[9] * p[5] + m[10] * p[9] + m[11] * p[13];
    cl[10] = m[8] * p[2] + m[9] * p[6] + m[10] * p[10] + m[11] * p[14];
    cl[11] = m[8] * p[3] + m[9] * p[7] + m[10] * p[11] + m[11] * p[15];
    cl[12] = m[12] * p[0] + m[13] * p[4] + m[14] * p[8] + m[15] * p[12];
    cl[13] = m[12] * p[1] + m[13] * p[5] + m[14] * p[9] + m[15] * p[13];
    cl[14] = m[12] * p[2] + m[13] * p[6] + m[14] * p[10] + m[15] * p[14];
    cl[15] = m[12] * p[3] + m[13] * p[7] + m[14] * p[11] + m[15] * p[15];

    float (*f)[4] = c->frustum;
    f[0][0] = cl[3] - cl[0];
    f[0][1] = cl[7] - cl[4];
    f[0][2] = cl[11] - cl[8];
    f[0][3] = cl[15] - cl[12];
    normalize(f, 0);
    f[1][0] = cl[3] + cl[0];
    f[1][1] = cl[7] + cl[4];
    f[1][2] = cl[11] + cl[8];
    f[1][3] = cl[15] + cl[12];
    normalize(f, 1);
    f[2][0] = cl[3] + cl[1];
    f[2][1] = cl[7] + cl[5];
    f[2][2] = cl[11] + cl[9];
    f[2][3] = cl[15] + cl[13];
    normalize(f, 2);
    f[3][0] = cl[3] - cl[1];
    f[3][1] = cl[7] - cl[5];
    f[3][2] = cl[11] - cl[9];
    f[3][3] = cl[15] - cl[13];
    normalize(f, 3);
    f[4][0] = cl[3] - cl[2];
    f[4][1] = cl[7] - cl[6];
    f[4][2] = cl[11] - cl[10];
    f[4][3] = cl[15] - cl[14];
    normalize(f, 4);
    f[5][0] = cl[3] + cl[2];
    f[5][1] = cl[7] + cl[6];
    f[5][2] = cl[11] + cl[10];
    f[5][3] = cl[15] + cl[14];
    normalize(f, 5);
}

ClippingHelper *ClippingHelperImpl_getInstance(void) {
    init(&_instance);
    return &_instance;
}
