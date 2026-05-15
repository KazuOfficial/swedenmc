#ifndef J_JSON_NULL_NODE_BUILDER_H
#define J_JSON_NULL_NODE_BUILDER_H

#include "J_JsonNodeBuilder.h"

typedef struct J_JsonNullNodeBuilder {
    J_JsonNodeBuilder base;
} J_JsonNullNodeBuilder;

J_JsonNullNodeBuilder *J_JsonNullNodeBuilder_create(void);

#endif
