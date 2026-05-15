#ifndef J_JSON_ELEMENT_NODE_SELECTOR_H
#define J_JSON_ELEMENT_NODE_SELECTOR_H

#include "J_Functor.h"
#include "J_JsonNode.h"

typedef struct J_JsonElementNodeSelector {
    J_Functor base;
    int field_27069_a;
} J_JsonElementNodeSelector;

J_JsonElementNodeSelector *J_JsonElementNodeSelector_create(int var1);

#endif
