#ifndef J_ARRAY_NODE_CONTAINER_H
#define J_ARRAY_NODE_CONTAINER_H

#include "J_NodeContainer.h"
#include "J_JsonArrayNodeBuilder.h"

struct J_JsonListenerToJdomAdapter;

typedef struct J_ArrayNodeContainer {
    J_NodeContainer base;
    J_JsonArrayNodeBuilder *field_27294_a;
    struct J_JsonListenerToJdomAdapter *field_27293_b;
} J_ArrayNodeContainer;

J_ArrayNodeContainer *J_ArrayNodeContainer_create(struct J_JsonListenerToJdomAdapter *var1,
                                                  J_JsonArrayNodeBuilder *var2);

#endif
