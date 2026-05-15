#include "J_JsonStringNodeSelector.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include <stdlib.h>

static int func_27072_a(J_JsonNode *var1) {
    return EnumJsonNodeType_STRING == var1->vtable->func_27218_a(var1);
}
static const char *func_27073_b(J_JsonNode *var1) { return var1->vtable->func_27216_b(var1); }
static const char *J_JsonStringNodeSelector_func_27060_a(J_Functor *self) {
    (void)self;
    return "A short form string";
}
static int J_JsonStringNodeSelector_func_27058_a(J_Functor *self, void *var1) {
    (void)self;
    return func_27072_a((J_JsonNode *)var1);
}
static void *J_JsonStringNodeSelector_func_27059_b(J_Functor *self, void *var1) {
    if (!J_JsonStringNodeSelector_func_27058_a(self, var1)) {

        J_ChainedException_pending =
            J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27322_a(self);
        return NULL;
    }
    return (void *)func_27073_b((J_JsonNode *)var1);
}

static const char *J_JsonStringNodeSelector_toString(J_Functor *self) {
    (void)self;
    return "a value that is a string";
}
static const J_Functor J_JsonStringNodeSelector_vtable = {
    J_JsonStringNodeSelector_func_27058_a,
    J_JsonStringNodeSelector_func_27059_b,
    J_JsonStringNodeSelector_func_27060_a,
    J_JsonStringNodeSelector_toString,
};

J_JsonStringNodeSelector *J_JsonStringNodeSelector_create(void) {
    J_JsonStringNodeSelector *self =
        (J_JsonStringNodeSelector *)calloc(1, sizeof(J_JsonStringNodeSelector));
    self->base = J_JsonStringNodeSelector_vtable;
    return self;
}
