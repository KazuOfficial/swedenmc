#include "RenderSorter.h"
#include <stdlib.h>

RenderSorter *RenderSorter_create(EntityLiving *var1) {
    RenderSorter *self = (RenderSorter *)calloc(1, sizeof(RenderSorter));
    self->baseEntity = var1;
    return self;
}

int RenderSorter_doCompare(RenderSorter *self, WorldRenderer *var1, WorldRenderer *var2) {
    int var3 = var1->isInFrustum;
    int var4 = var2->isInFrustum;
    if (var3 && !var4) {
        return 1;
    } else if (var4 && !var3) {
        return -1;
    } else {
        double var5 =
            (double)WorldRenderer_distanceToEntitySquared(var1, (Entity *)self->baseEntity);
        double var7 =
            (double)WorldRenderer_distanceToEntitySquared(var2, (Entity *)self->baseEntity);
        return var5 < var7 ? 1
                           : (var5 > var7 ? -1 : (var1->chunkIndex < var2->chunkIndex ? 1 : -1));
    }
}

int RenderSorter_compare(RenderSorter *self, WorldRenderer *var1, WorldRenderer *var2) {
    return RenderSorter_doCompare(self, var1, var2);
}
