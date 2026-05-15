#include "J_JsonNullNodeBuilder.h"
#include "J_JsonNodeFactories.h"
#include <stdlib.h>

static J_JsonNode *J_JsonNullNodeBuilder_func_27234_b(J_JsonNodeBuilder *self) {
    (void)self;
    return J_JsonNodeFactories_func_27310_a();
}

J_JsonNullNodeBuilder *J_JsonNullNodeBuilder_create(void) {
    J_JsonNullNodeBuilder *self = (J_JsonNullNodeBuilder *)calloc(1, sizeof(J_JsonNullNodeBuilder));
    self->base.func_27234_b = J_JsonNullNodeBuilder_func_27234_b;
    return self;
}
