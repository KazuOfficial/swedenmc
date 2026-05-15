#ifndef ICAMERA_H
#define ICAMERA_H

#include "AxisAlignedBB.h"

typedef struct ICamera {

    int (*isBoundingBoxInFrustum)(struct ICamera *self, AxisAlignedBB *var1);

    void (*setPosition)(struct ICamera *self, double var1, double var3, double var5);
} ICamera;

#endif
