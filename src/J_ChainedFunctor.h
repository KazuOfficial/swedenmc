#ifndef J_CHAINED_FUNCTOR_H
#define J_CHAINED_FUNCTOR_H

#include "J_Functor.h"
#include "J_JsonNodeSelector.h"

typedef struct J_ChainedFunctor {
    J_Functor base;
    J_JsonNodeSelector *field_27062_a;
    J_JsonNodeSelector *field_27061_b;
} J_ChainedFunctor;

J_ChainedFunctor *J_ChainedFunctor_create(J_JsonNodeSelector *var1, J_JsonNodeSelector *var2);

int J_ChainedFunctor_func_27058_a(J_Functor *self, void *var1);
void *J_ChainedFunctor_func_27059_b(J_Functor *self, void *var1);
const char *J_ChainedFunctor_func_27060_a(J_Functor *self);

#endif
