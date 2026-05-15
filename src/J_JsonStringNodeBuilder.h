#ifndef J_JSON_STRING_NODE_BUILDER_H
#define J_JSON_STRING_NODE_BUILDER_H

#include "J_JsonNodeBuilder.h"
#include "J_JsonStringNode.h"

typedef struct J_JsonStringNodeBuilder {
    J_JsonNodeBuilder base;
    const char *field_27244_a;
} J_JsonStringNodeBuilder;

J_JsonStringNodeBuilder *J_JsonStringNodeBuilder_create(const char *var1);

J_JsonStringNode *J_JsonStringNodeBuilder_func_27243_a(J_JsonStringNodeBuilder *self);

#endif
