#ifndef J_JSON_NUMBER_NODE_H
#define J_JSON_NUMBER_NODE_H

#include "J_JsonNode.h"

typedef struct J_JsonNumberNode {
    J_JsonNode base;
    char *field_27225_b;
} J_JsonNumberNode;

J_JsonNumberNode *J_JsonNumberNode_create(const char *var1);

EnumJsonNodeType J_JsonNumberNode_func_27218_a(J_JsonNode *self);
const char *J_JsonNumberNode_func_27216_b(J_JsonNode *self);

#endif
