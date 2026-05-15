#ifndef J_JSON_FALSE_NODE_BUILDER_H
#define J_JSON_FALSE_NODE_BUILDER_H

#include "J_JsonNodeBuilder.h"

typedef struct J_JsonFalseNodeBuilder {
    J_JsonNodeBuilder base;
} J_JsonFalseNodeBuilder;

J_JsonFalseNodeBuilder *J_JsonFalseNodeBuilder_create(void);

#endif
