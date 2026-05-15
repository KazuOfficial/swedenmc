#ifndef CLIPPING_HELPER_H
#define CLIPPING_HELPER_H

typedef struct ClippingHelper {
    float frustum[6][4];
    float projectionMatrix[16];
    float modelviewMatrix[16];
    float clippingMatrix[16];
} ClippingHelper;

int ClippingHelper_isBoxInFrustum(ClippingHelper *self, double x1, double y1, double z1, double x2,
                                  double y2, double z2);

#endif
