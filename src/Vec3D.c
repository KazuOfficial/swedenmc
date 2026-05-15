#include "Vec3D.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static __thread Vec3D **s_pool = NULL;
static __thread int s_poolSize = 0;
static __thread int s_next = 0;

static Vec3D *Vec3D_new(double x, double y, double z) {
    Vec3D *v = (Vec3D *)calloc(1, sizeof(Vec3D));
    v->xCoord = x == -0.0 ? 0.0 : x;
    v->yCoord = y == -0.0 ? 0.0 : y;
    v->zCoord = z == -0.0 ? 0.0 : z;
    return v;
}

static Vec3D *Vec3D_setComponents(Vec3D *v, double x, double y, double z) {
    v->xCoord = x;
    v->yCoord = y;
    v->zCoord = z;
    return v;
}

void Vec3D_clearPool(void) {
    int i;
    for (i = 0; i < s_poolSize; i++) {
        free(s_pool[i]);
        s_pool[i] = NULL;
    }
    free(s_pool);
    s_pool = NULL;
    s_poolSize = 0;
    s_next = 0;
}

void Vec3D_initialize(void) { s_next = 0; }

Vec3D *Vec3D_createVectorHelper(double x, double y, double z) { return Vec3D_new(x, y, z); }

Vec3D *Vec3D_createVector(double x, double y, double z) {
    if (s_next >= s_poolSize) {
        s_pool = (Vec3D **)realloc(s_pool, (s_poolSize + 1) * sizeof(Vec3D *));
        s_pool[s_poolSize++] = Vec3D_new(0.0, 0.0, 0.0);
    }
    return Vec3D_setComponents(s_pool[s_next++], x, y, z);
}

Vec3D *Vec3D_subtract(Vec3D *self, Vec3D *other) {
    return Vec3D_createVector(other->xCoord - self->xCoord, other->yCoord - self->yCoord,
                              other->zCoord - self->zCoord);
}

Vec3D *Vec3D_normalize(Vec3D *self) {
    double len = (double)MathHelper_sqrt_double(
        self->xCoord * self->xCoord + self->yCoord * self->yCoord + self->zCoord * self->zCoord);
    if (len < 1.0e-4)
        return Vec3D_createVector(0.0, 0.0, 0.0);
    return Vec3D_createVector(self->xCoord / len, self->yCoord / len, self->zCoord / len);
}

Vec3D *Vec3D_crossProduct(Vec3D *self, Vec3D *other) {
    return Vec3D_createVector(self->yCoord * other->zCoord - self->zCoord * other->yCoord,
                              self->zCoord * other->xCoord - self->xCoord * other->zCoord,
                              self->xCoord * other->yCoord - self->yCoord * other->xCoord);
}

Vec3D *Vec3D_addVector(Vec3D *self, double x, double y, double z) {
    return Vec3D_createVector(self->xCoord + x, self->yCoord + y, self->zCoord + z);
}

double Vec3D_distanceTo(Vec3D *self, Vec3D *other) {
    double dx = other->xCoord - self->xCoord;
    double dy = other->yCoord - self->yCoord;
    double dz = other->zCoord - self->zCoord;
    return (double)MathHelper_sqrt_double(dx * dx + dy * dy + dz * dz);
}

double Vec3D_squareDistanceTo(Vec3D *self, Vec3D *other) {
    double dx = other->xCoord - self->xCoord;
    double dy = other->yCoord - self->yCoord;
    double dz = other->zCoord - self->zCoord;
    return dx * dx + dy * dy + dz * dz;
}

double Vec3D_squareDistanceTo3(Vec3D *self, double x, double y, double z) {
    double dx = x - self->xCoord;
    double dy = y - self->yCoord;
    double dz = z - self->zCoord;
    return dx * dx + dy * dy + dz * dz;
}

double Vec3D_lengthVector(Vec3D *self) {
    return (double)MathHelper_sqrt_double(
        self->xCoord * self->xCoord + self->yCoord * self->yCoord + self->zCoord * self->zCoord);
}

Vec3D *Vec3D_getIntermediateWithXValue(Vec3D *self, Vec3D *other, double x) {
    double dx = other->xCoord - self->xCoord;
    double dy = other->yCoord - self->yCoord;
    double dz = other->zCoord - self->zCoord;
    if (dx * dx < 1.0e-7f)
        return NULL;
    double t = (x - self->xCoord) / dx;
    return (t >= 0.0 && t <= 1.0) ? Vec3D_createVector(self->xCoord + dx * t, self->yCoord + dy * t,
                                                       self->zCoord + dz * t)
                                  : NULL;
}

Vec3D *Vec3D_getIntermediateWithYValue(Vec3D *self, Vec3D *other, double y) {
    double dx = other->xCoord - self->xCoord;
    double dy = other->yCoord - self->yCoord;
    double dz = other->zCoord - self->zCoord;
    if (dy * dy < 1.0e-7f)
        return NULL;
    double t = (y - self->yCoord) / dy;
    return (t >= 0.0 && t <= 1.0) ? Vec3D_createVector(self->xCoord + dx * t, self->yCoord + dy * t,
                                                       self->zCoord + dz * t)
                                  : NULL;
}

Vec3D *Vec3D_getIntermediateWithZValue(Vec3D *self, Vec3D *other, double z) {
    double dx = other->xCoord - self->xCoord;
    double dy = other->yCoord - self->yCoord;
    double dz = other->zCoord - self->zCoord;
    if (dz * dz < 1.0e-7f)
        return NULL;
    double t = (z - self->zCoord) / dz;
    return (t >= 0.0 && t <= 1.0) ? Vec3D_createVector(self->xCoord + dx * t, self->yCoord + dy * t,
                                                       self->zCoord + dz * t)
                                  : NULL;
}

void Vec3D_rotateAroundX(Vec3D *self, float angle) {
    float c = MathHelper_cos(angle);
    float s = MathHelper_sin(angle);
    double x = self->xCoord;
    double y = self->yCoord * (double)c + self->zCoord * (double)s;
    double z = self->zCoord * (double)c - self->yCoord * (double)s;
    self->xCoord = x;
    self->yCoord = y;
    self->zCoord = z;
}

void Vec3D_rotateAroundY(Vec3D *self, float angle) {
    float c = MathHelper_cos(angle);
    float s = MathHelper_sin(angle);
    double x = self->xCoord * (double)c + self->zCoord * (double)s;
    double y = self->yCoord;
    double z = self->zCoord * (double)c - self->xCoord * (double)s;
    self->xCoord = x;
    self->yCoord = y;
    self->zCoord = z;
}

static void java_double_str(char *buf, int size, double v) {
    const char *p;
    snprintf(buf, size, "%g", v);
    for (p = buf; *p; p++) {
        if (*p == '.' || *p == 'e' || *p == 'E')
            return;
    }
    strncat(buf, ".0", size - (int)strlen(buf) - 1);
}

const char *Vec3D_toString(Vec3D *self) {
    static char buf[128];
    char sx[32], sy[32], sz[32];
    java_double_str(sx, sizeof(sx), self->xCoord);
    java_double_str(sy, sizeof(sy), self->yCoord);
    java_double_str(sz, sizeof(sz), self->zCoord);
    snprintf(buf, sizeof(buf), "(%s, %s, %s)", sx, sy, sz);
    return buf;
}
