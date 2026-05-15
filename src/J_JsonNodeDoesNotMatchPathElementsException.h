#ifndef J_JSON_NODE_DOES_NOT_MATCH_PATH_ELEMENTS_EXCEPTION_H
#define J_JSON_NODE_DOES_NOT_MATCH_PATH_ELEMENTS_EXCEPTION_H

#include "J_JsonNodeDoesNotMatchJsonNodeSelectorException.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include "J_JsonRootNode.h"

typedef struct J_JsonNodeDoesNotMatchPathElementsException {
    J_JsonNodeDoesNotMatchJsonNodeSelectorException base;
} J_JsonNodeDoesNotMatchPathElementsException;

J_JsonNodeDoesNotMatchPathElementsException *
J_JsonNodeDoesNotMatchPathElementsException_func_27319_a(
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *var0, void **var1, int var1_count,
    J_JsonRootNode *var2);

#endif
