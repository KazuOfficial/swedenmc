#include "J_JsonElementNodeSelector.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include <stdlib.h>
#include <stdio.h>

static int func_27067_a(J_JsonElementNodeSelector *self, J_JsonNodeArray *var1) {
    return var1->count > self->field_27069_a;
}

static J_JsonNode *func_27068_b(J_JsonElementNodeSelector *self, J_JsonNodeArray *var1) {
    return var1->items[self->field_27069_a];
}
static const char *J_JsonElementNodeSelector_func_27060_a(J_Functor *self) {
    J_JsonElementNodeSelector *s = (J_JsonElementNodeSelector *)self;
    static char buf[32];
    snprintf(buf, sizeof(buf), "%d", s->field_27069_a);
    return buf;
}
static int J_JsonElementNodeSelector_func_27058_a(J_Functor *self, void *var1) {
    J_JsonElementNodeSelector *s = (J_JsonElementNodeSelector *)self;
    return func_27067_a(s, (J_JsonNodeArray *)var1);
}
static void *J_JsonElementNodeSelector_func_27059_b(J_Functor *self, void *var1) {
    if (!J_JsonElementNodeSelector_func_27058_a(self, var1)) {

        J_ChainedException_pending =
            J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27322_a(self);
        return NULL;
    }
    J_JsonElementNodeSelector *s = (J_JsonElementNodeSelector *)self;
    return (void *)func_27068_b(s, (J_JsonNodeArray *)var1);
}

static const char *J_JsonElementNodeSelector_toString(J_Functor *self) {
    J_JsonElementNodeSelector *s = (J_JsonElementNodeSelector *)self;
    static char buf[64];
    snprintf(buf, sizeof(buf), "an element at index [%d]", s->field_27069_a);
    return buf;
}
static const J_Functor J_JsonElementNodeSelector_vtable = {
    J_JsonElementNodeSelector_func_27058_a,
    J_JsonElementNodeSelector_func_27059_b,
    J_JsonElementNodeSelector_func_27060_a,
    J_JsonElementNodeSelector_toString,
};

J_JsonElementNodeSelector *J_JsonElementNodeSelector_create(int var1) {
    J_JsonElementNodeSelector *self =
        (J_JsonElementNodeSelector *)calloc(1, sizeof(J_JsonElementNodeSelector));
    self->base = J_JsonElementNodeSelector_vtable;
    self->field_27069_a = var1;
    return self;
}
