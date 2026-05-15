#include "J_JsonArrayNodeSelector.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include <stdlib.h>

static int func_27074_a(J_JsonNode *var1) {
    return EnumJsonNodeType_ARRAY == var1->vtable->func_27218_a(var1);
}

static const char *J_JsonArrayNodeSelector_func_27060_a(J_Functor *self) {
    (void)self;
    return "A short form array";
}

static J_JsonNodeArray *func_27075_b(J_JsonNode *var1) { return var1->vtable->func_27215_d(var1); }

static int J_JsonArrayNodeSelector_func_27058_a(J_Functor *self, void *var1) {
    (void)self;
    return func_27074_a((J_JsonNode *)var1);
}

static void *J_JsonArrayNodeSelector_func_27059_b(J_Functor *self, void *var1) {
    if (!J_JsonArrayNodeSelector_func_27058_a(self, var1)) {

        J_ChainedException_pending =
            J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27322_a(self);
        return NULL;
    }
    return (void *)func_27075_b((J_JsonNode *)var1);
}

static const char *J_JsonArrayNodeSelector_toString(J_Functor *self) {
    (void)self;
    return "an array";
}
static const J_Functor J_JsonArrayNodeSelector_vtable = {
    J_JsonArrayNodeSelector_func_27058_a,
    J_JsonArrayNodeSelector_func_27059_b,
    J_JsonArrayNodeSelector_func_27060_a,
    J_JsonArrayNodeSelector_toString,
};

J_JsonArrayNodeSelector *J_JsonArrayNodeSelector_create(void) {
    J_JsonArrayNodeSelector *self =
        (J_JsonArrayNodeSelector *)calloc(1, sizeof(J_JsonArrayNodeSelector));
    self->base = J_JsonArrayNodeSelector_vtable;
    return self;
}
