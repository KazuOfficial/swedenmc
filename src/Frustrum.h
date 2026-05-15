#ifndef FRUSTRUM_H
#define FRUSTRUM_H

#include "ICamera.h"
#include "ClippingHelperImpl.h"
#include "AxisAlignedBB.h"

typedef struct Frustrum {
    ICamera base;
    ClippingHelper *clippingHelper;
    double xPosition;
    double yPosition;
    double zPosition;
} Frustrum;

void Frustrum_init(Frustrum *self);
void Frustrum_setPosition(Frustrum *self, double x, double y, double z);
int Frustrum_isBoxInFrustum(Frustrum *self, double x1, double y1, double z1, double x2, double y2,
                            double z2);
int Frustrum_isBoundingBoxInFrustum(Frustrum *self, AxisAlignedBB *bb);

#endif
