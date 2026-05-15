#include "J_JsonFalseNodeBuilder.h"
#include "J_JsonNodeFactories.h"
#include <stdlib.h>

static J_JsonNode *J_JsonFalseNodeBuilder_func_27234_b(J_JsonNodeBuilder *self) {
    (void)self;
    return J_JsonNodeFactories_func_27314_c();
}

J_JsonFalseNodeBuilder *J_JsonFalseNodeBuilder_create(void) {
    J_JsonFalseNodeBuilder *self =
        (J_JsonFalseNodeBuilder *)calloc(1, sizeof(J_JsonFalseNodeBuilder));
    self->base.func_27234_b = J_JsonFalseNodeBuilder_func_27234_b;
    return self;
}
