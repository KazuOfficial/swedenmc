#ifndef J_FIELD_NODE_CONTAINER_H
#define J_FIELD_NODE_CONTAINER_H

#include "J_NodeContainer.h"
#include "J_JsonFieldBuilder.h"

struct J_JsonListenerToJdomAdapter;

typedef struct J_FieldNodeContainer {
    J_NodeContainer base;
    J_JsonFieldBuilder *field_27292_a;
    struct J_JsonListenerToJdomAdapter *field_27291_b;
} J_FieldNodeContainer;

J_FieldNodeContainer *J_FieldNodeContainer_create(struct J_JsonListenerToJdomAdapter *var1,
                                                  J_JsonFieldBuilder *var2);

#endif
