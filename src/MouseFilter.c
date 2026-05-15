#include "MouseFilter.h"
#include <string.h>

void MouseFilter_init(MouseFilter *self) { memset(self, 0, sizeof(MouseFilter)); }

float MouseFilter_func_22386_a(MouseFilter *self, float var1, float var2) {
    self->field_22388_a += var1;
    var1 = (self->field_22388_a - self->field_22387_b) * var2;
    self->field_22389_c += (var1 - self->field_22389_c) * 0.5F;
    if ((var1 > 0.0F && var1 > self->field_22389_c) ||
        (var1 < 0.0F && var1 < self->field_22389_c)) {
        var1 = self->field_22389_c;
    }
    self->field_22387_b += var1;
    return var1;
}
