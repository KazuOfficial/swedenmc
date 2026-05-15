#ifndef J_JSON_ARRAY_NODE_SELECTOR_H
#define J_JSON_ARRAY_NODE_SELECTOR_H

#include "J_Functor.h"
#include "J_JsonNode.h"

typedef struct J_JsonArrayNodeSelector {
    J_Functor base;
} J_JsonArrayNodeSelector;

J_JsonArrayNodeSelector *J_JsonArrayNodeSelector_create(void);

#endif
