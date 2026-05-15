#include "AxisAlignedBB.h"
#include "MovingObjectPosition.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static AxisAlignedBB **s_pool = NULL;
static int s_poolSize = 0;
static int s_poolCap = 0;

static int s_numBoundingBoxesInUse = 0;

static AxisAlignedBB *AxisAlignedBB_new(double var1, double var3, double var5, double var7,
                                        double var9, double var11) {
    AxisAlignedBB *bb = (AxisAlignedBB *)calloc(1, sizeof(AxisAlignedBB));
    bb->minX = var1;
    bb->minY = var3;
    bb->minZ = var5;
    bb->maxX = var7;
    bb->maxY = var9;
    bb->maxZ = var11;
    return bb;
}

AxisAlignedBB *AxisAlignedBB_getBoundingBox(double var0, double var2, double var4, double var6,
                                            double var8, double var10) {
    return AxisAlignedBB_new(var0, var2, var4, var6, var8, var10);
}

void AxisAlignedBB_func_28196_a(void) {

    int i;
    for (i = 0; i < s_poolSize; i++) {
        free(s_pool[i]);
        s_pool[i] = NULL;
    }
    s_poolSize = 0;
    s_numBoundingBoxesInUse = 0;
}

void AxisAlignedBB_clearBoundingBoxPool(void) { s_numBoundingBoxesInUse = 0; }

AxisAlignedBB *AxisAlignedBB_getBoundingBoxFromPool(double var0, double var2, double var4,
                                                    double var6, double var8, double var10) {
    if (s_numBoundingBoxesInUse >= s_poolSize) {

        if (s_poolSize >= s_poolCap) {
            int newcap = s_poolCap == 0 ? 16 : s_poolCap * 2;
            s_pool = (AxisAlignedBB **)realloc(s_pool, newcap * sizeof(AxisAlignedBB *));
            s_poolCap = newcap;
        }
        s_pool[s_poolSize++] = AxisAlignedBB_getBoundingBox(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }
    return AxisAlignedBB_setBounds(s_pool[s_numBoundingBoxesInUse++], var0, var2, var4, var6, var8,
                                   var10);
}

AxisAlignedBB *AxisAlignedBB_setBounds(AxisAlignedBB *self, double var1, double var3, double var5,
                                       double var7, double var9, double var11) {
    self->minX = var1;
    self->minY = var3;
    self->minZ = var5;
    self->maxX = var7;
    self->maxY = var9;
    self->maxZ = var11;
    return self;
}

AxisAlignedBB *AxisAlignedBB_addCoord(AxisAlignedBB *self, double var1, double var3, double var5) {
    double var7 = self->minX;
    double var9 = self->minY;
    double var11 = self->minZ;
    double var13 = self->maxX;
    double var15 = self->maxY;
    double var17 = self->maxZ;
    if (var1 < 0.0) {
        var7 += var1;
    }
    if (var1 > 0.0) {
        var13 += var1;
    }
    if (var3 < 0.0) {
        var9 += var3;
    }
    if (var3 > 0.0) {
        var15 += var3;
    }
    if (var5 < 0.0) {
        var11 += var5;
    }
    if (var5 > 0.0) {
        var17 += var5;
    }
    return AxisAlignedBB_getBoundingBoxFromPool(var7, var9, var11, var13, var15, var17);
}

AxisAlignedBB *AxisAlignedBB_expand(AxisAlignedBB *self, double var1, double var3, double var5) {
    double var7 = self->minX - var1;
    double var9 = self->minY - var3;
    double var11 = self->minZ - var5;
    double var13 = self->maxX + var1;
    double var15 = self->maxY + var3;
    double var17 = self->maxZ + var5;
    return AxisAlignedBB_getBoundingBoxFromPool(var7, var9, var11, var13, var15, var17);
}

AxisAlignedBB *AxisAlignedBB_getOffsetBoundingBox(AxisAlignedBB *self, double var1, double var3,
                                                  double var5) {
    return AxisAlignedBB_getBoundingBoxFromPool(self->minX + var1, self->minY + var3,
                                                self->minZ + var5, self->maxX + var1,
                                                self->maxY + var3, self->maxZ + var5);
}

double AxisAlignedBB_calculateXOffset(AxisAlignedBB *self, AxisAlignedBB *var1, double var2) {
    if (var1->maxY > self->minY && var1->minY < self->maxY) {
        if (var1->maxZ > self->minZ && var1->minZ < self->maxZ) {
            double var4;
            if (var2 > 0.0 && var1->maxX <= self->minX) {
                var4 = self->minX - var1->maxX;
                if (var4 < var2) {
                    var2 = var4;
                }
            }
            if (var2 < 0.0 && var1->minX >= self->maxX) {
                var4 = self->maxX - var1->minX;
                if (var4 > var2) {
                    var2 = var4;
                }
            }
            return var2;
        } else {
            return var2;
        }
    } else {
        return var2;
    }
}

double AxisAlignedBB_calculateYOffset(AxisAlignedBB *self, AxisAlignedBB *var1, double var2) {
    if (var1->maxX > self->minX && var1->minX < self->maxX) {
        if (var1->maxZ > self->minZ && var1->minZ < self->maxZ) {
            double var4;
            if (var2 > 0.0 && var1->maxY <= self->minY) {
                var4 = self->minY - var1->maxY;
                if (var4 < var2) {
                    var2 = var4;
                }
            }
            if (var2 < 0.0 && var1->minY >= self->maxY) {
                var4 = self->maxY - var1->minY;
                if (var4 > var2) {
                    var2 = var4;
                }
            }
            return var2;
        } else {
            return var2;
        }
    } else {
        return var2;
    }
}

double AxisAlignedBB_calculateZOffset(AxisAlignedBB *self, AxisAlignedBB *var1, double var2) {
    if (var1->maxX > self->minX && var1->minX < self->maxX) {
        if (var1->maxY > self->minY && var1->minY < self->maxY) {
            double var4;
            if (var2 > 0.0 && var1->maxZ <= self->minZ) {
                var4 = self->minZ - var1->maxZ;
                if (var4 < var2) {
                    var2 = var4;
                }
            }
            if (var2 < 0.0 && var1->minZ >= self->maxZ) {
                var4 = self->maxZ - var1->minZ;
                if (var4 > var2) {
                    var2 = var4;
                }
            }
            return var2;
        } else {
            return var2;
        }
    } else {
        return var2;
    }
}

int AxisAlignedBB_intersectsWith(AxisAlignedBB *self, AxisAlignedBB *var1) {
    return var1->maxX > self->minX && var1->minX < self->maxX
               ? (var1->maxY > self->minY && var1->minY < self->maxY
                      ? var1->maxZ > self->minZ && var1->minZ < self->maxZ
                      : 0)
               : 0;
}

AxisAlignedBB *AxisAlignedBB_offset(AxisAlignedBB *self, double var1, double var3, double var5) {
    self->minX += var1;
    self->minY += var3;
    self->minZ += var5;
    self->maxX += var1;
    self->maxY += var3;
    self->maxZ += var5;
    return self;
}

int AxisAlignedBB_isVecInside(AxisAlignedBB *self, Vec3D *var1) {
    return var1->xCoord > self->minX && var1->xCoord < self->maxX
               ? (var1->yCoord > self->minY && var1->yCoord < self->maxY
                      ? var1->zCoord > self->minZ && var1->zCoord < self->maxZ
                      : 0)
               : 0;
}

double AxisAlignedBB_getAverageEdgeLength(AxisAlignedBB *self) {
    double var1 = self->maxX - self->minX;
    double var3 = self->maxY - self->minY;
    double var5 = self->maxZ - self->minZ;
    return (var1 + var3 + var5) / 3.0;
}

AxisAlignedBB *AxisAlignedBB_func_28195_e(AxisAlignedBB *self, double var1, double var3,
                                          double var5) {
    double var7 = self->minX + var1;
    double var9 = self->minY + var3;
    double var11 = self->minZ + var5;
    double var13 = self->maxX - var1;
    double var15 = self->maxY - var3;
    double var17 = self->maxZ - var5;
    return AxisAlignedBB_getBoundingBoxFromPool(var7, var9, var11, var13, var15, var17);
}

AxisAlignedBB *AxisAlignedBB_copy(AxisAlignedBB *self) {
    return AxisAlignedBB_getBoundingBoxFromPool(self->minX, self->minY, self->minZ, self->maxX,
                                                self->maxY, self->maxZ);
}

static int isVecInYZ(AxisAlignedBB *self, Vec3D *var1) {
    return var1 == NULL ? 0
                        : var1->yCoord >= self->minY && var1->yCoord <= self->maxY &&
                              var1->zCoord >= self->minZ && var1->zCoord <= self->maxZ;
}

static int isVecInXZ(AxisAlignedBB *self, Vec3D *var1) {
    return var1 == NULL ? 0
                        : var1->xCoord >= self->minX && var1->xCoord <= self->maxX &&
                              var1->zCoord >= self->minZ && var1->zCoord <= self->maxZ;
}

static int isVecInXY(AxisAlignedBB *self, Vec3D *var1) {
    return var1 == NULL ? 0
                        : var1->xCoord >= self->minX && var1->xCoord <= self->maxX &&
                              var1->yCoord >= self->minY && var1->yCoord <= self->maxY;
}

MovingObjectPosition *AxisAlignedBB_func_1169_a(AxisAlignedBB *self, Vec3D *var1, Vec3D *var2) {
    Vec3D *var3 = Vec3D_getIntermediateWithXValue(var1, var2, self->minX);
    Vec3D *var4 = Vec3D_getIntermediateWithXValue(var1, var2, self->maxX);
    Vec3D *var5 = Vec3D_getIntermediateWithYValue(var1, var2, self->minY);
    Vec3D *var6 = Vec3D_getIntermediateWithYValue(var1, var2, self->maxY);
    Vec3D *var7 = Vec3D_getIntermediateWithZValue(var1, var2, self->minZ);
    Vec3D *var8 = Vec3D_getIntermediateWithZValue(var1, var2, self->maxZ);
    if (!isVecInYZ(self, var3)) {
        var3 = NULL;
    }
    if (!isVecInYZ(self, var4)) {
        var4 = NULL;
    }
    if (!isVecInXZ(self, var5)) {
        var5 = NULL;
    }
    if (!isVecInXZ(self, var6)) {
        var6 = NULL;
    }
    if (!isVecInXY(self, var7)) {
        var7 = NULL;
    }
    if (!isVecInXY(self, var8)) {
        var8 = NULL;
    }
    Vec3D *var9 = NULL;
    if (var3 != NULL &&
        (var9 == NULL || Vec3D_squareDistanceTo(var1, var3) < Vec3D_squareDistanceTo(var1, var9))) {
        var9 = var3;
    }
    if (var4 != NULL &&
        (var9 == NULL || Vec3D_squareDistanceTo(var1, var4) < Vec3D_squareDistanceTo(var1, var9))) {
        var9 = var4;
    }
    if (var5 != NULL &&
        (var9 == NULL || Vec3D_squareDistanceTo(var1, var5) < Vec3D_squareDistanceTo(var1, var9))) {
        var9 = var5;
    }
    if (var6 != NULL &&
        (var9 == NULL || Vec3D_squareDistanceTo(var1, var6) < Vec3D_squareDistanceTo(var1, var9))) {
        var9 = var6;
    }
    if (var7 != NULL &&
        (var9 == NULL || Vec3D_squareDistanceTo(var1, var7) < Vec3D_squareDistanceTo(var1, var9))) {
        var9 = var7;
    }
    if (var8 != NULL &&
        (var9 == NULL || Vec3D_squareDistanceTo(var1, var8) < Vec3D_squareDistanceTo(var1, var9))) {
        var9 = var8;
    }
    if (var9 == NULL) {
        return NULL;
    } else {

        int8_t var10 = -1;
        if (var9 == var3) {
            var10 = 4;
        }
        if (var9 == var4) {
            var10 = 5;
        }
        if (var9 == var5) {
            var10 = 0;
        }
        if (var9 == var6) {
            var10 = 1;
        }
        if (var9 == var7) {
            var10 = 2;
        }
        if (var9 == var8) {
            var10 = 3;
        }
        return MovingObjectPosition_create(0, 0, 0, var10, var9);
    }
}

void AxisAlignedBB_setBB(AxisAlignedBB *self, AxisAlignedBB *var1) {
    self->minX = var1->minX;
    self->minY = var1->minY;
    self->minZ = var1->minZ;
    self->maxX = var1->maxX;
    self->maxY = var1->maxY;
    self->maxZ = var1->maxZ;
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

const char *AxisAlignedBB_toString(AxisAlignedBB *self) {
    static char buf[256];
    char s0[32], s1[32], s2[32], s3[32], s4[32], s5[32];
    java_double_str(s0, sizeof(s0), self->minX);
    java_double_str(s1, sizeof(s1), self->minY);
    java_double_str(s2, sizeof(s2), self->minZ);
    java_double_str(s3, sizeof(s3), self->maxX);
    java_double_str(s4, sizeof(s4), self->maxY);
    java_double_str(s5, sizeof(s5), self->maxZ);
    snprintf(buf, sizeof(buf), "box[%s, %s, %s -> %s, %s, %s]", s0, s1, s2, s3, s4, s5);
    return buf;
}
