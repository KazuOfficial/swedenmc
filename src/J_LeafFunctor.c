#include "J_LeafFunctor.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include <stdlib.h>

void *J_LeafFunctor_func_27059_b(J_Functor *self, void *var1) {
    if (!self->func_27058_a(self, var1)) {

        J_ChainedException_pending =
            J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27322_a(self);
        return NULL;
    }

    return NULL;
}
