#include "J_JsonNumberNodeBuilder.h"
#include "J_JsonNodeFactories.h"
#include <stdlib.h>

static J_JsonNode *J_JsonNumberNodeBuilder_func_27234_b(J_JsonNodeBuilder *self) {
    return ((J_JsonNumberNodeBuilder *)self)->field_27239_a;
}

J_JsonNumberNodeBuilder *J_JsonNumberNodeBuilder_create(const char *var1) {
    J_JsonNumberNodeBuilder *self =
        (J_JsonNumberNodeBuilder *)calloc(1, sizeof(J_JsonNumberNodeBuilder));
    self->base.func_27234_b = J_JsonNumberNodeBuilder_func_27234_b;
    self->field_27239_a = J_JsonNodeFactories_func_27311_b(var1);
    return self;
}
