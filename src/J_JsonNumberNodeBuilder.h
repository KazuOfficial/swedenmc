#ifndef J_JSON_NUMBER_NODE_BUILDER_H
#define J_JSON_NUMBER_NODE_BUILDER_H

#include "J_JsonNodeBuilder.h"

typedef struct J_JsonNumberNodeBuilder {
    J_JsonNodeBuilder base;
    J_JsonNode *field_27239_a;
} J_JsonNumberNodeBuilder;

J_JsonNumberNodeBuilder *J_JsonNumberNodeBuilder_create(const char *var1);

#endif
