#include "J_JsonObjectNodeBuilder.h"
#include "J_JsonObjectNodeList.h"
#include "J_JsonNodeFactories.h"
#include <stdlib.h>

static J_JsonNode *J_JsonObjectNodeBuilder_func_27234_b(J_JsonNodeBuilder *self) {
    return (J_JsonNode *)J_JsonObjectNodeBuilder_func_27235_a((J_JsonObjectNodeBuilder *)self);
}

J_JsonObjectNodeBuilder *J_JsonObjectNodeBuilder_create(void) {
    J_JsonObjectNodeBuilder *self =
        (J_JsonObjectNodeBuilder *)calloc(1, sizeof(J_JsonObjectNodeBuilder));
    self->base.func_27234_b = J_JsonObjectNodeBuilder_func_27234_b;
    return self;
}

J_JsonObjectNodeBuilder *J_JsonObjectNodeBuilder_func_27237_a(J_JsonObjectNodeBuilder *self,
                                                              J_JsonFieldBuilder *var1) {
    if (self->field_27238_a_count >= self->field_27238_a_cap) {
        self->field_27238_a_cap = self->field_27238_a_cap * 2 + 4;
        self->field_27238_a = (J_JsonFieldBuilder **)realloc(
            self->field_27238_a, self->field_27238_a_cap * sizeof(J_JsonFieldBuilder *));
    }
    self->field_27238_a[self->field_27238_a_count++] = var1;
    return self;
}

J_JsonRootNode *J_JsonObjectNodeBuilder_func_27235_a(J_JsonObjectNodeBuilder *self) {
    J_JsonMap *map = J_JsonObjectNodeList_create(self);
    J_JsonRootNode *result = J_JsonNodeFactories_func_27312_a(map);
    J_JsonMap_free(map);
    return result;
}

J_JsonFieldBuilder **J_JsonObjectNodeBuilder_func_27236_a(J_JsonObjectNodeBuilder *var0,
                                                          int *out_count) {
    if (out_count)
        *out_count = var0->field_27238_a_count;
    return var0->field_27238_a;
}
