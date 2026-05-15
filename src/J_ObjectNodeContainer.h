#ifndef J_OBJECT_NODE_CONTAINER_H
#define J_OBJECT_NODE_CONTAINER_H

#include "J_NodeContainer.h"
#include "J_JsonObjectNodeBuilder.h"

struct J_JsonListenerToJdomAdapter;

typedef struct J_ObjectNodeContainer {
    J_NodeContainer base;
    J_JsonObjectNodeBuilder *field_27296_a;
    struct J_JsonListenerToJdomAdapter *field_27295_b;
} J_ObjectNodeContainer;

J_ObjectNodeContainer *J_ObjectNodeContainer_create(struct J_JsonListenerToJdomAdapter *var1,
                                                    J_JsonObjectNodeBuilder *var2);

#endif
