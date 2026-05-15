#include "J_ChainedFunctor.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int J_ChainedFunctor_func_27058_a(J_Functor *self, void *var1);
void *J_ChainedFunctor_func_27059_b(J_Functor *self, void *var1);
const char *J_ChainedFunctor_func_27060_a(J_Functor *self);
static const char *J_ChainedFunctor_toString(J_Functor *self);

static const J_Functor J_ChainedFunctor_vtable_tmpl = {
    J_ChainedFunctor_func_27058_a,
    J_ChainedFunctor_func_27059_b,
    J_ChainedFunctor_func_27060_a,
    J_ChainedFunctor_toString,
};

J_ChainedFunctor *J_ChainedFunctor_create(J_JsonNodeSelector *var1, J_JsonNodeSelector *var2) {
    J_ChainedFunctor *self = (J_ChainedFunctor *)calloc(1, sizeof(J_ChainedFunctor));
    self->base = J_ChainedFunctor_vtable_tmpl;
    self->field_27062_a = var1;
    self->field_27061_b = var2;
    return self;
}

int J_ChainedFunctor_func_27058_a(J_Functor *self, void *var1) {
    J_ChainedFunctor *cf = (J_ChainedFunctor *)self;
    if (!J_JsonNodeSelector_func_27356_a(cf->field_27062_a, var1))
        return 0;
    void *mid = J_JsonNodeSelector_func_27357_b(cf->field_27062_a, var1);
    return J_JsonNodeSelector_func_27356_a(cf->field_27061_b, mid);
}

void *J_ChainedFunctor_func_27059_b(J_Functor *self, void *var1) {
    J_ChainedFunctor *cf = (J_ChainedFunctor *)self;

    void *var2 = J_JsonNodeSelector_func_27357_b(cf->field_27062_a, var1);
    if (J_ChainedException_pending) {
        J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *var6 = J_ChainedException_pending;
        J_ChainedException_pending = NULL;
        J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *rethrown =
            J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27321_b(var6,
                                                                                cf->field_27062_a);
        J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_free(var6);
        J_ChainedException_pending = rethrown;
        return NULL;
    }

    void *var3 = J_JsonNodeSelector_func_27357_b(cf->field_27061_b, var2);
    if (J_ChainedException_pending) {
        J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *var5 = J_ChainedException_pending;
        J_ChainedException_pending = NULL;
        J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *rethrown =
            J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27323_a(var5,
                                                                                cf->field_27062_a);
        J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_free(var5);
        J_ChainedException_pending = rethrown;
        return NULL;
    }
    return var3;
}

const char *J_ChainedFunctor_func_27060_a(J_Functor *self) {
    J_ChainedFunctor *cf = (J_ChainedFunctor *)self;
    return J_JsonNodeSelector_func_27358_a(cf->field_27061_b);
}

static const char *J_ChainedFunctor_toString(J_Functor *self) {
    J_ChainedFunctor *cf = (J_ChainedFunctor *)self;
    static char buf[512];
    const char *a = J_JsonNodeSelector_toString(cf->field_27062_a);
    const char *b = J_JsonNodeSelector_toString(cf->field_27061_b);
    snprintf(buf, sizeof(buf), "%s, with %s", a, b);
    return buf;
}
