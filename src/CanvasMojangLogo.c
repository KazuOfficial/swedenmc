#include "CanvasMojangLogo.h"
#include <stddef.h>
#include <stdint.h>

void CanvasMojangLogo_construct(CanvasMojangLogo *self) {

    self->logo = NULL;
    {
        int8_t var1 = 100;

        self->preferredWidth = var1;
        self->preferredHeight = var1;
        self->minimumWidth = var1;
        self->minimumHeight = var1;
    }
}

void CanvasMojangLogo_paint(CanvasMojangLogo *self, struct Graphics *var1) {

    (void)self;
    (void)var1;
}
