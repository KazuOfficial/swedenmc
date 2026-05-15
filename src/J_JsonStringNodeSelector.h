#ifndef J_JSON_STRING_NODE_SELECTOR_H
#define J_JSON_STRING_NODE_SELECTOR_H

#include "J_Functor.h"
#include "J_JsonNode.h"

typedef struct J_JsonStringNodeSelector {
    J_Functor base;
} J_JsonStringNodeSelector;

J_JsonStringNodeSelector *J_JsonStringNodeSelector_create(void);

#endif
