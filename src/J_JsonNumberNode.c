#include "J_JsonNumberNode.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
static J_JsonMap *err_fields(J_JsonNode *s) {

    (void)s;
    fprintf(stderr, "IllegalStateException: Attempt to get fields on a JsonNode without fields.\n");
    abort();
}
static J_JsonNodeArray *err_elems(J_JsonNode *s) {

    (void)s;
    fprintf(stderr,
            "IllegalStateException: Attempt to get elements on a JsonNode without elements.\n");
    abort();
}

static int is_json_number(const char *s) {
    if (*s == '-')
        s++;
    if (*s == '0') {
        s++;
    } else if (*s >= '1' && *s <= '9') {
        s++;
        while (*s >= '0' && *s <= '9')
            s++;
    } else
        return 0;
    if (*s == '.') {
        s++;
        if (*s < '0' || *s > '9')
            return 0;
        while (*s >= '0' && *s <= '9')
            s++;
    }
    if (*s == 'e' || *s == 'E') {
        s++;
        if (*s == '+' || *s == '-')
            s++;
        if (*s < '0' || *s > '9')
            return 0;
        while (*s >= '0' && *s <= '9')
            s++;
    }
    return *s == '\0';
}

static const J_JsonNodeVtable J_JsonNumberNode_vtable = {
    J_JsonNumberNode_func_27218_a,
    J_JsonNumberNode_func_27216_b,
    err_fields,
    err_elems,
};

J_JsonNumberNode *J_JsonNumberNode_create(const char *var1) {
    if (var1 == NULL) {

        fprintf(stderr,
                "NullPointerException: Attempt to construct a JsonNumber with a null value.\n");
        abort();
    }
    if (!is_json_number(var1)) {

        fprintf(stderr,
                "IllegalArgumentException: Attempt to construct a JsonNumber with a String [%s] "
                "that does not match the JSON number specification.\n",
                var1);
        abort();
    }
    J_JsonNumberNode *self = (J_JsonNumberNode *)calloc(1, sizeof(J_JsonNumberNode));
    self->base.vtable = &J_JsonNumberNode_vtable;
    self->field_27225_b = strdup(var1);
    return self;
}

EnumJsonNodeType J_JsonNumberNode_func_27218_a(J_JsonNode *self) {
    (void)self;
    return EnumJsonNodeType_NUMBER;
}

const char *J_JsonNumberNode_func_27216_b(J_JsonNode *self) {
    return ((J_JsonNumberNode *)self)->field_27225_b;
}
