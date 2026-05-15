#include "WorldProviderSurface.h"
#include <stdlib.h>

WorldProvider *WorldProviderSurface_create(void) {
    WorldProvider *self = (WorldProvider *)calloc(1, sizeof(WorldProvider));
    self->vtable = &WorldProvider_defaultVtable;
    return self;
}
