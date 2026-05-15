#ifndef J_JSON_STRING_NODE_H
#define J_JSON_STRING_NODE_H

#include "J_JsonNode.h"

typedef struct J_JsonStringNode {
    J_JsonNode base;
    const char *field_27224_a;
} J_JsonStringNode;

J_JsonStringNode *J_JsonStringNode_create(const char *var1);

EnumJsonNodeType J_JsonStringNode_func_27218_a(J_JsonNode *self);

const char *J_JsonStringNode_func_27216_b(J_JsonNode *self);

int J_JsonStringNode_func_27223_a(J_JsonStringNode *self, J_JsonStringNode *var1);

int J_JsonStringNode_compareTo(J_JsonStringNode *self, void *var1);

#endif
