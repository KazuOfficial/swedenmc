#include "J_JsonObjectNodeSelector.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include <stdlib.h>

static int func_27070_a(J_JsonNode *var1) {
    return EnumJsonNodeType_OBJECT == var1->vtable->func_27218_a(var1);
}
static J_JsonMap *func_27071_b(J_JsonNode *var1) { return var1->vtable->func_27214_c(var1); }
static const char *J_JsonObjectNodeSelector_func_27060_a(J_Functor *self) {
    (void)self;
    return "A short form object";
}
static int J_JsonObjectNodeSelector_func_27058_a(J_Functor *self, void *var1) {
    (void)self;
    return func_27070_a((J_JsonNode *)var1);
}
static void *J_JsonObjectNodeSelector_func_27059_b(J_Functor *self, void *var1) {
    if (!J_JsonObjectNodeSelector_func_27058_a(self, var1)) {

        J_ChainedException_pending =
            J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27322_a(self);
        return NULL;
    }
    return (void *)func_27071_b((J_JsonNode *)var1);
}

static const char *J_JsonObjectNodeSelector_toString(J_Functor *self) {
    (void)self;
    return "an object";
}
static const J_Functor J_JsonObjectNodeSelector_vtable = {
    J_JsonObjectNodeSelector_func_27058_a,
    J_JsonObjectNodeSelector_func_27059_b,
    J_JsonObjectNodeSelector_func_27060_a,
    J_JsonObjectNodeSelector_toString,
};

J_JsonObjectNodeSelector *J_JsonObjectNodeSelector_create(void) {
    J_JsonObjectNodeSelector *self =
        (J_JsonObjectNodeSelector *)calloc(1, sizeof(J_JsonObjectNodeSelector));
    self->base = J_JsonObjectNodeSelector_vtable;
    return self;
}
