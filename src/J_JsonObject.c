#include "J_JsonObject.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char *J_JsonObject_func_27216_b(J_JsonNode *s) {
    (void)s;
    fprintf(stderr, "Attempt to get text on a JsonNode without text.\n");
    exit(1);
    return NULL;
}
static J_JsonNodeArray *J_JsonObject_func_27215_d(J_JsonNode *s) {
    (void)s;
    fprintf(stderr, "Attempt to get elements on a JsonNode without elements.\n");
    exit(1);
    return NULL;
}

static const J_JsonNodeVtable J_JsonObject_vtable = {
    J_JsonObject_func_27218_a,
    J_JsonObject_func_27216_b,
    J_JsonObject_func_27214_c,
    J_JsonObject_func_27215_d,
};

J_JsonObject *J_JsonObject_create(J_JsonMap *var1) {
    J_JsonObject *self = (J_JsonObject *)calloc(1, sizeof(J_JsonObject));
    self->base.vtable = &J_JsonObject_vtable;

    self->field_27222_a = J_JsonMap_copy(var1);
    return self;
}

EnumJsonNodeType J_JsonObject_func_27218_a(J_JsonNode *self) {
    (void)self;
    return EnumJsonNodeType_OBJECT;
}

J_JsonMap *J_JsonObject_func_27214_c(J_JsonNode *self) {
    return J_JsonMap_copy(((J_JsonObject *)self)->field_27222_a);
}

int J_JsonObject_equals(const J_JsonObject *self, const J_JsonObject *var1) {
    if (self == var1)
        return 1;
    if (!var1)
        return 0;
    const J_JsonMap *a = self->field_27222_a;
    const J_JsonMap *b = var1->field_27222_a;
    if (a->count != b->count)
        return 0;
    for (int i = 0; i < a->count; i++) {
        if (a->entries[i].key != b->entries[i].key)
            return 0;
        if (a->entries[i].value != b->entries[i].value)
            return 0;
    }
    return 1;
}

int J_JsonObject_hashCode(const J_JsonObject *self) {
    int h = 0;
    for (int i = 0; i < self->field_27222_a->count; i++)
        h += (int)(size_t)self->field_27222_a->entries[i].key ^
             (int)(size_t)self->field_27222_a->entries[i].value;
    return h;
}

const char *J_JsonObject_toString(const J_JsonObject *self) {
    static char buf[256];
    snprintf(buf, sizeof(buf), "JsonObject fields:[count=%d]", self->field_27222_a->count);
    return buf;
}
