#ifndef J_JSON_TRUE_NODE_BUILDER_H
#define J_JSON_TRUE_NODE_BUILDER_H

#include "J_JsonNodeBuilder.h"

typedef struct J_JsonTrueNodeBuilder {
    J_JsonNodeBuilder base;
} J_JsonTrueNodeBuilder;

J_JsonTrueNodeBuilder *J_JsonTrueNodeBuilder_create(void);

#endif
