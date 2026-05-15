#include "J_JsonArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char *J_JsonArray_func_27216_b(J_JsonNode *s) {
    (void)s;
    fprintf(stderr, "Attempt to get text on a JsonNode without text.\n");
    exit(1);
    return NULL;
}
static J_JsonMap *J_JsonArray_func_27214_c(J_JsonNode *s) {
    (void)s;
    fprintf(stderr, "Attempt to get fields on a JsonNode without fields.\n");
    exit(1);
    return NULL;
}

static const J_JsonNodeVtable J_JsonArray_vtable = {
    J_JsonArray_func_27218_a,
    J_JsonArray_func_27216_b,
    J_JsonArray_func_27214_c,
    J_JsonArray_func_27215_d,
};

static J_JsonNodeArray *func_27220_a(J_JsonNodeArray *var0) { return J_JsonNodeArray_copy(var0); }

J_JsonArray *J_JsonArray_create(J_JsonNodeArray *var1) {
    J_JsonArray *self = (J_JsonArray *)calloc(1, sizeof(J_JsonArray));
    self->base.vtable = &J_JsonArray_vtable;
    self->field_27221_a = func_27220_a(var1);
    return self;
}

EnumJsonNodeType J_JsonArray_func_27218_a(J_JsonNode *self) {
    (void)self;
    return EnumJsonNodeType_ARRAY;
}

J_JsonNodeArray *J_JsonArray_func_27215_d(J_JsonNode *self) {
    return J_JsonNodeArray_copy(((J_JsonArray *)self)->field_27221_a);
}

int J_JsonArray_equals(const J_JsonArray *self, const J_JsonArray *var1) {
    if (self == var1)
        return 1;
    if (!var1)
        return 0;
    const J_JsonNodeArray *a = self->field_27221_a;
    const J_JsonNodeArray *b = var1->field_27221_a;
    if (a->count != b->count)
        return 0;
    for (int i = 0; i < a->count; i++)
        if (a->items[i] != b->items[i])
            return 0;
    return 1;
}

int J_JsonArray_hashCode(const J_JsonArray *self) {
    int h = 1;
    for (int i = 0; i < self->field_27221_a->count; i++)
        h = 31 * h + (int)(size_t)self->field_27221_a->items[i];
    return h;
}

const char *J_JsonArray_toString(const J_JsonArray *self) {
    static char buf[256];
    snprintf(buf, sizeof(buf), "JsonArray elements:[count=%d]", self->field_27221_a->count);
    return buf;
}
