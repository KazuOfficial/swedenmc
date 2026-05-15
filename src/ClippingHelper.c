#include "ClippingHelper.h"

int ClippingHelper_isBoxInFrustum(ClippingHelper *self, double x1, double y1, double z1, double x2,
                                  double y2, double z2) {
    for (int i = 0; i < 6; i++) {
        float *p = self->frustum[i];

        if ((double)p[0] * x1 + (double)p[1] * y1 + (double)p[2] * z1 + (double)p[3] <= 0.0 &&
            (double)p[0] * x2 + (double)p[1] * y1 + (double)p[2] * z1 + (double)p[3] <= 0.0 &&
            (double)p[0] * x1 + (double)p[1] * y2 + (double)p[2] * z1 + (double)p[3] <= 0.0 &&
            (double)p[0] * x2 + (double)p[1] * y2 + (double)p[2] * z1 + (double)p[3] <= 0.0 &&
            (double)p[0] * x1 + (double)p[1] * y1 + (double)p[2] * z2 + (double)p[3] <= 0.0 &&
            (double)p[0] * x2 + (double)p[1] * y1 + (double)p[2] * z2 + (double)p[3] <= 0.0 &&
            (double)p[0] * x1 + (double)p[1] * y2 + (double)p[2] * z2 + (double)p[3] <= 0.0 &&
            (double)p[0] * x2 + (double)p[1] * y2 + (double)p[2] * z2 + (double)p[3] <= 0.0)
            return 0;
    }
    return 1;
}
