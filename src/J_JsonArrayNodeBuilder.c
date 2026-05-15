#include "J_JsonArrayNodeBuilder.h"
#include "J_JsonNodeFactories.h"
#include <stdlib.h>

static J_JsonNode *J_JsonArrayNodeBuilder_func_27234_b(J_JsonNodeBuilder *self) {
    return (J_JsonNode *)J_JsonArrayNodeBuilder_func_27241_a((J_JsonArrayNodeBuilder *)self);
}

J_JsonArrayNodeBuilder *J_JsonArrayNodeBuilder_create(void) {
    J_JsonArrayNodeBuilder *self =
        (J_JsonArrayNodeBuilder *)calloc(1, sizeof(J_JsonArrayNodeBuilder));
    self->base.func_27234_b = J_JsonArrayNodeBuilder_func_27234_b;
    return self;
}

J_JsonArrayNodeBuilder *J_JsonArrayNodeBuilder_func_27240_a(J_JsonArrayNodeBuilder *self,
                                                            J_JsonNodeBuilder *var1) {
    if (self->field_27242_a_count >= self->field_27242_a_cap) {
        self->field_27242_a_cap = self->field_27242_a_cap * 2 + 4;
        self->field_27242_a = (J_JsonNodeBuilder **)realloc(
            self->field_27242_a, self->field_27242_a_cap * sizeof(J_JsonNodeBuilder *));
    }
    self->field_27242_a[self->field_27242_a_count++] = var1;
    return self;
}

J_JsonRootNode *J_JsonArrayNodeBuilder_func_27241_a(J_JsonArrayNodeBuilder *self) {
    J_JsonNodeArray *arr = J_JsonNodeArray_create(self->field_27242_a_count);
    for (int i = 0; i < self->field_27242_a_count; i++) {
        J_JsonNodeBuilder *var3 = self->field_27242_a[i];
        J_JsonNodeArray_add(arr, var3->func_27234_b(var3));
    }
    J_JsonRootNode *result = J_JsonNodeFactories_func_27309_a(arr);
    J_JsonNodeArray_free(arr);
    return result;
}
