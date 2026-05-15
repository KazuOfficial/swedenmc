#ifndef J_JSON_NODE_DOES_NOT_MATCH_CHAINED_H
#define J_JSON_NODE_DOES_NOT_MATCH_CHAINED_H

#include "J_JsonNodeDoesNotMatchJsonNodeSelectorException.h"
#include "J_Functor.h"

struct J_JsonNodeSelector;

typedef struct J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException {
    J_JsonNodeDoesNotMatchJsonNodeSelectorException base;
    J_Functor *field_27326_a;

    struct J_JsonNodeSelector **field_27325_b;
    int field_27325_b_size;
} J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException;

J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *
J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27322_a(J_Functor *var0);

J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *
J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27323_a(
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *var0, struct J_JsonNodeSelector *var1);

J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *
J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27321_b(
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *var0, struct J_JsonNodeSelector *var1);

const char *J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27324_a(
    struct J_JsonNodeSelector **var0, int count);

const char *J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_toString(
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *self);

void J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_free(
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *e);

#endif
