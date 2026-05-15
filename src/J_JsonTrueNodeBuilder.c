#include "J_JsonTrueNodeBuilder.h"
#include "J_JsonNodeFactories.h"
#include <stdlib.h>

static J_JsonNode *J_JsonTrueNodeBuilder_func_27234_b(J_JsonNodeBuilder *self) {
    (void)self;
    return J_JsonNodeFactories_func_27313_b();
}

J_JsonTrueNodeBuilder *J_JsonTrueNodeBuilder_create(void) {
    J_JsonTrueNodeBuilder *self = (J_JsonTrueNodeBuilder *)calloc(1, sizeof(J_JsonTrueNodeBuilder));
    self->base.func_27234_b = J_JsonTrueNodeBuilder_func_27234_b;
    return self;
}
