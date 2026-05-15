#include "J_JsonConstants.h"
#include <stdlib.h>
#include <stdio.h>

J_JsonConstants *J_JsonConstants_field_27228_a = NULL;
J_JsonConstants *J_JsonConstants_field_27227_b = NULL;
J_JsonConstants *J_JsonConstants_field_27230_c = NULL;

static const char *J_JsonConstants_func_27216_b(J_JsonNode *self) {
    (void)self;
    fprintf(stderr, "Attempt to get text on a JsonNode without text.\n");
    return NULL;
}
static J_JsonMap *J_JsonConstants_func_27214_c(J_JsonNode *self) {
    (void)self;
    fprintf(stderr, "Attempt to get fields on a JsonNode without fields.\n");
    return NULL;
}
static J_JsonNodeArray *J_JsonConstants_func_27215_d(J_JsonNode *self) {
    (void)self;
    fprintf(stderr, "Attempt to get elements on a JsonNode without elements.\n");
    return NULL;
}

static const J_JsonNodeVtable J_JsonConstants_vtable = {
    J_JsonConstants_func_27218_a,
    J_JsonConstants_func_27216_b,
    J_JsonConstants_func_27214_c,
    J_JsonConstants_func_27215_d,
};

static J_JsonConstants *J_JsonConstants_new(EnumJsonNodeType type) {
    J_JsonConstants *self = (J_JsonConstants *)calloc(1, sizeof(J_JsonConstants));
    self->base.vtable = &J_JsonConstants_vtable;
    self->field_27229_d = type;
    return self;
}

void J_JsonConstants_init(void) {
    if (!J_JsonConstants_field_27228_a) {

        J_JsonConstants_field_27228_a = J_JsonConstants_new(EnumJsonNodeType_NULL);

        J_JsonConstants_field_27227_b = J_JsonConstants_new(EnumJsonNodeType_TRUE);

        J_JsonConstants_field_27230_c = J_JsonConstants_new(EnumJsonNodeType_FALSE);
    }
}

EnumJsonNodeType J_JsonConstants_func_27218_a(J_JsonNode *self) {
    return ((J_JsonConstants *)self)->field_27229_d;
}
