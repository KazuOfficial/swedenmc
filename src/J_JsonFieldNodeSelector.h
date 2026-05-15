#ifndef J_JSON_FIELD_NODE_SELECTOR_H
#define J_JSON_FIELD_NODE_SELECTOR_H

#include "J_Functor.h"
#include "J_JsonNode.h"
#include "J_JsonStringNode.h"

typedef struct J_JsonFieldNodeSelector {
    J_Functor base;
    J_JsonStringNode *field_27066_a;
} J_JsonFieldNodeSelector;

J_JsonFieldNodeSelector *J_JsonFieldNodeSelector_create(J_JsonStringNode *var1);

#endif
