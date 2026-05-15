#include "PathPoint.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int PathPoint_func_22329_a(int var0, int var1, int var2) {

    return (int)((unsigned int)(var1 & 255) | ((unsigned int)(var0 & 32767) << 8) |
                 ((unsigned int)(var2 & 32767) << 24) |
                 (unsigned int)(var0 < 0 ? (int)0x80000000 : 0) |
                 (unsigned int)(var2 < 0 ? 32768 : 0));
}

PathPoint *PathPoint_create(int var1, int var2, int var3) {
    PathPoint *self = (PathPoint *)calloc(1, sizeof(PathPoint));
    self->xCoord = var1;
    self->yCoord = var2;
    self->zCoord = var3;
    self->hash = PathPoint_func_22329_a(var1, var2, var3);
    self->index = -1;
    return self;
}

float PathPoint_distanceTo(PathPoint *self, PathPoint *var1) {
    float var2 = (float)(var1->xCoord - self->xCoord);
    float var3 = (float)(var1->yCoord - self->yCoord);
    float var4 = (float)(var1->zCoord - self->zCoord);
    return MathHelper_sqrt_float(var2 * var2 + var3 * var3 + var4 * var4);
}

int PathPoint_equals(PathPoint *self, PathPoint *var1) {
    if (var1 == NULL)
        return 0;
    return self->hash == var1->hash && self->xCoord == var1->xCoord &&
           self->yCoord == var1->yCoord && self->zCoord == var1->zCoord;
}

int PathPoint_hashCode(PathPoint *self) { return self->hash; }

int PathPoint_isAssigned(PathPoint *self) { return self->index >= 0; }

char *PathPoint_toString(PathPoint *self) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%d, %d, %d", self->xCoord, self->yCoord, self->zCoord);
    return strdup(buf);
}
