#ifndef J_JSON_LISTENER_TO_JDOM_ADAPTER_H
#define J_JSON_LISTENER_TO_JDOM_ADAPTER_H

#include "J_JsonListener.h"
#include "J_JsonNodeBuilder.h"
#include "J_NodeContainer.h"
#include "J_JsonRootNode.h"

#define J_ADAPTER_STACK_SIZE 64

typedef struct J_JsonListenerToJdomAdapter {
    J_JsonListener base;

    J_NodeContainer *field_27210_a[J_ADAPTER_STACK_SIZE];
    int field_27210_a_top;

    J_JsonNodeBuilder *field_27209_b;
} J_JsonListenerToJdomAdapter;

void J_JsonListenerToJdomAdapter_init(J_JsonListenerToJdomAdapter *self);

J_JsonRootNode *J_JsonListenerToJdomAdapter_func_27208_a(J_JsonListenerToJdomAdapter *self);

#endif
