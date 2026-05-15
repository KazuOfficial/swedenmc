#include "J_JsonFieldNodeSelector.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include <stdlib.h>
#include <stdio.h>

static int func_27065_a(J_JsonFieldNodeSelector *self, J_JsonMap *var1) {
    return J_JsonMap_containsKey(var1, self->field_27066_a);
}

static J_JsonNode *func_27064_b(J_JsonFieldNodeSelector *self, J_JsonMap *var1) {
    return J_JsonMap_get(var1, self->field_27066_a);
}
static const char *J_JsonFieldNodeSelector_func_27060_a(J_Functor *self) {
    J_JsonFieldNodeSelector *s = (J_JsonFieldNodeSelector *)self;
    static char buf[256];
    snprintf(buf, sizeof(buf), "\"%s\"", s->field_27066_a->field_27224_a);
    return buf;
}
static int J_JsonFieldNodeSelector_func_27058_a(J_Functor *self, void *var1) {
    J_JsonFieldNodeSelector *s = (J_JsonFieldNodeSelector *)self;
    return func_27065_a(s, (J_JsonMap *)var1);
}
static void *J_JsonFieldNodeSelector_func_27059_b(J_Functor *self, void *var1) {
    if (!J_JsonFieldNodeSelector_func_27058_a(self, var1)) {

        J_ChainedException_pending =
            J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27322_a(self);
        return NULL;
    }
    J_JsonFieldNodeSelector *s = (J_JsonFieldNodeSelector *)self;
    return (void *)func_27064_b(s, (J_JsonMap *)var1);
}

static const char *J_JsonFieldNodeSelector_toString(J_Functor *self) {
    J_JsonFieldNodeSelector *s = (J_JsonFieldNodeSelector *)self;
    static char buf[320];
    snprintf(buf, sizeof(buf), "a field called [\"%s\"]", s->field_27066_a->field_27224_a);
    return buf;
}
static const J_Functor J_JsonFieldNodeSelector_vtable = {
    J_JsonFieldNodeSelector_func_27058_a,
    J_JsonFieldNodeSelector_func_27059_b,
    J_JsonFieldNodeSelector_func_27060_a,
    J_JsonFieldNodeSelector_toString,
};

J_JsonFieldNodeSelector *J_JsonFieldNodeSelector_create(J_JsonStringNode *var1) {
    J_JsonFieldNodeSelector *self =
        (J_JsonFieldNodeSelector *)calloc(1, sizeof(J_JsonFieldNodeSelector));
    self->base = J_JsonFieldNodeSelector_vtable;
    self->field_27066_a = var1;
    return self;
}
