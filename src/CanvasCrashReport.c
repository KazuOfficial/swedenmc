#include "CanvasCrashReport.h"

void CanvasCrashReport_construct(CanvasCrashReport *self, int var1) {

    self->preferredWidth = var1;
    self->preferredHeight = var1;
    self->minimumWidth = var1;
    self->minimumHeight = var1;
}
