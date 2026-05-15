#ifndef J_JSON_ARRAY_H
#define J_JSON_ARRAY_H

#include "J_JsonRootNode.h"

typedef struct J_JsonArray {
    J_JsonNode base;
    J_JsonNodeArray *field_27221_a;
} J_JsonArray;

J_JsonArray *J_JsonArray_create(J_JsonNodeArray *var1);

EnumJsonNodeType J_JsonArray_func_27218_a(J_JsonNode *self);
J_JsonNodeArray *J_JsonArray_func_27215_d(J_JsonNode *self);

int J_JsonArray_equals(const J_JsonArray *self, const J_JsonArray *var1);

int J_JsonArray_hashCode(const J_JsonArray *self);

const char *J_JsonArray_toString(const J_JsonArray *self);

#endif
