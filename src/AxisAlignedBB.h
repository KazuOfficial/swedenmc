#ifndef AXIS_ALIGNED_BB_H
#define AXIS_ALIGNED_BB_H

#include "Vec3D.h"
#include "MovingObjectPosition.h"

typedef struct {

    double minX, minY, minZ;
    double maxX, maxY, maxZ;
} AxisAlignedBB;

AxisAlignedBB *AxisAlignedBB_getBoundingBox(double var0, double var2, double var4, double var6,
                                            double var8, double var10);

void AxisAlignedBB_func_28196_a(void);

void AxisAlignedBB_clearBoundingBoxPool(void);

AxisAlignedBB *AxisAlignedBB_getBoundingBoxFromPool(double var0, double var2, double var4,
                                                    double var6, double var8, double var10);

AxisAlignedBB *AxisAlignedBB_setBounds(AxisAlignedBB *self, double var1, double var3, double var5,
                                       double var7, double var9, double var11);

AxisAlignedBB *AxisAlignedBB_addCoord(AxisAlignedBB *self, double var1, double var3, double var5);

AxisAlignedBB *AxisAlignedBB_expand(AxisAlignedBB *self, double var1, double var3, double var5);

AxisAlignedBB *AxisAlignedBB_getOffsetBoundingBox(AxisAlignedBB *self, double var1, double var3,
                                                  double var5);

double AxisAlignedBB_calculateXOffset(AxisAlignedBB *self, AxisAlignedBB *var1, double var2);

double AxisAlignedBB_calculateYOffset(AxisAlignedBB *self, AxisAlignedBB *var1, double var2);

double AxisAlignedBB_calculateZOffset(AxisAlignedBB *self, AxisAlignedBB *var1, double var2);

int AxisAlignedBB_intersectsWith(AxisAlignedBB *self, AxisAlignedBB *var1);

AxisAlignedBB *AxisAlignedBB_offset(AxisAlignedBB *self, double var1, double var3, double var5);

int AxisAlignedBB_isVecInside(AxisAlignedBB *self, Vec3D *var1);

double AxisAlignedBB_getAverageEdgeLength(AxisAlignedBB *self);

AxisAlignedBB *AxisAlignedBB_func_28195_e(AxisAlignedBB *self, double var1, double var3,
                                          double var5);

AxisAlignedBB *AxisAlignedBB_copy(AxisAlignedBB *self);

MovingObjectPosition *AxisAlignedBB_func_1169_a(AxisAlignedBB *self, Vec3D *var1, Vec3D *var2);

void AxisAlignedBB_setBB(AxisAlignedBB *self, AxisAlignedBB *var1);

const char *AxisAlignedBB_toString(AxisAlignedBB *self);

#endif
