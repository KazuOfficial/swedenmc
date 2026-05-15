#include "Frustrum.h"
#include <string.h>

static int ica_isBoundingBoxInFrustum(ICamera *self, AxisAlignedBB *bb) {
    return Frustrum_isBoundingBoxInFrustum((Frustrum *)self, bb);
}

static void ica_setPosition(ICamera *self, double var1, double var3, double var5) {
    Frustrum_setPosition((Frustrum *)self, var1, var3, var5);
}

void Frustrum_init(Frustrum *self) {
    memset(self, 0, sizeof(Frustrum));
    self->clippingHelper = ClippingHelperImpl_getInstance();
    self->base.isBoundingBoxInFrustum = ica_isBoundingBoxInFrustum;
    self->base.setPosition = ica_setPosition;
}

void Frustrum_setPosition(Frustrum *self, double x, double y, double z) {
    self->xPosition = x;
    self->yPosition = y;
    self->zPosition = z;
}

int Frustrum_isBoxInFrustum(Frustrum *self, double x1, double y1, double z1, double x2, double y2,
                            double z2) {
    return ClippingHelper_isBoxInFrustum(
        self->clippingHelper, x1 - self->xPosition, y1 - self->yPosition, z1 - self->zPosition,
        x2 - self->xPosition, y2 - self->yPosition, z2 - self->zPosition);
}

int Frustrum_isBoundingBoxInFrustum(Frustrum *self, AxisAlignedBB *bb) {
    return Frustrum_isBoxInFrustum(self, bb->minX, bb->minY, bb->minZ, bb->maxX, bb->maxY,
                                   bb->maxZ);
}
