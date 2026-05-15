#include "J_JsonStringNodeBuilder.h"
#include "J_JsonNodeFactories.h"
#include <stdlib.h>
#include <string.h>

J_JsonStringNode *J_JsonStringNodeBuilder_func_27243_a(J_JsonStringNodeBuilder *self) {
    return J_JsonNodeFactories_func_27316_a(self->field_27244_a);
}

static J_JsonNode *J_JsonStringNodeBuilder_func_27234_b(J_JsonNodeBuilder *self) {
    return (J_JsonNode *)J_JsonStringNodeBuilder_func_27243_a((J_JsonStringNodeBuilder *)self);
}

J_JsonStringNodeBuilder *J_JsonStringNodeBuilder_create(const char *var1) {
    J_JsonStringNodeBuilder *self =
        (J_JsonStringNodeBuilder *)calloc(1, sizeof(J_JsonStringNodeBuilder));
    self->base.func_27234_b = J_JsonStringNodeBuilder_func_27234_b;
    self->field_27244_a = strdup(var1);
    return self;
}
