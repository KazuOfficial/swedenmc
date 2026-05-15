#include "J_JsonStringNode.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static J_JsonMap *J_JsonStringNode_func_27214_c(J_JsonNode *self) {
    (void)self;
    fprintf(stderr, "IllegalStateException: Attempt to get fields on a JsonNode without fields.\n");
    abort();
}
static J_JsonNodeArray *J_JsonStringNode_func_27215_d(J_JsonNode *self) {
    (void)self;
    fprintf(stderr,
            "IllegalStateException: Attempt to get elements on a JsonNode without elements.\n");
    abort();
}

static const J_JsonNodeVtable J_JsonStringNode_vtable = {
    J_JsonStringNode_func_27218_a,
    J_JsonStringNode_func_27216_b,
    J_JsonStringNode_func_27214_c,
    J_JsonStringNode_func_27215_d,
};

J_JsonStringNode *J_JsonStringNode_create(const char *var1) {
    if (var1 == NULL) {

        fprintf(stderr,
                "NullPointerException: Attempt to construct a JsonString with a null value.\n");
        abort();
    }
    J_JsonStringNode *self = (J_JsonStringNode *)calloc(1, sizeof(J_JsonStringNode));
    self->base.vtable = &J_JsonStringNode_vtable;
    self->field_27224_a = strdup(var1);
    return self;
}

EnumJsonNodeType J_JsonStringNode_func_27218_a(J_JsonNode *self) {
    (void)self;
    return EnumJsonNodeType_STRING;
}

const char *J_JsonStringNode_func_27216_b(J_JsonNode *self) {
    return ((J_JsonStringNode *)self)->field_27224_a;
}

int J_JsonStringNode_func_27223_a(J_JsonStringNode *self, J_JsonStringNode *var1) {
    return strcmp(self->field_27224_a, var1->field_27224_a);
}

int J_JsonStringNode_compareTo(J_JsonStringNode *self, void *var1) {
    return J_JsonStringNode_func_27223_a(self, (J_JsonStringNode *)var1);
}
