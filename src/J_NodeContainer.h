#ifndef J_NODE_CONTAINER_H
#define J_NODE_CONTAINER_H

#include "J_JsonNodeBuilder.h"
#include "J_JsonFieldBuilder.h"

typedef struct J_NodeContainer J_NodeContainer;

struct J_NodeContainer {
    void (*func_27290_a)(J_NodeContainer *self, J_JsonNodeBuilder *var1);
    void (*func_27289_a)(J_NodeContainer *self, J_JsonFieldBuilder *var1);
};

#endif
