#ifndef J_JSON_OBJECT_NODE_SELECTOR_H
#define J_JSON_OBJECT_NODE_SELECTOR_H

#include "J_Functor.h"
#include "J_JsonNode.h"

typedef struct J_JsonObjectNodeSelector {
    J_Functor base;
} J_JsonObjectNodeSelector;

J_JsonObjectNodeSelector *J_JsonObjectNodeSelector_create(void);

#endif
