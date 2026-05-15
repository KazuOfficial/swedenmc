#include "EntitySorter.h"
#include "WorldRenderer.h"

void EntitySorter_construct(EntitySorter *self, Entity *var1) {
    self->field_30008_a = -var1->posX;
    self->field_30007_b = -var1->posY;
    self->field_30009_c = -var1->posZ;
}

int EntitySorter_sortByDistanceToEntity(EntitySorter *self, WorldRenderer *var1,
                                        WorldRenderer *var2) {
    double var3 = (double)var1->posXPlus + self->field_30008_a;
    double var5 = (double)var1->posYPlus + self->field_30007_b;
    double var7 = (double)var1->posZPlus + self->field_30009_c;
    double var9 = (double)var2->posXPlus + self->field_30008_a;
    double var11 = (double)var2->posYPlus + self->field_30007_b;
    double var13 = (double)var2->posZPlus + self->field_30009_c;
    return (int)((var3 * var3 + var5 * var5 + var7 * var7 -
                  (var9 * var9 + var11 * var11 + var13 * var13)) *
                 1024.0);
}

int EntitySorter_compare(EntitySorter *self, void *var1, void *var2) {
    return EntitySorter_sortByDistanceToEntity(self, (WorldRenderer *)var1, (WorldRenderer *)var2);
}
