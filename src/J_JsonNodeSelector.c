#include "J_JsonNodeSelector.h"
#include "J_ChainedFunctor.h"
#include <stdlib.h>

J_JsonNodeSelector *J_JsonNodeSelector_create(J_Functor *var1) {
    J_JsonNodeSelector *self = (J_JsonNodeSelector *)calloc(1, sizeof(J_JsonNodeSelector));
    self->field_27359_a = var1;
    return self;
}

int J_JsonNodeSelector_func_27356_a(J_JsonNodeSelector *self, void *var1) {
    return self->field_27359_a->func_27058_a(self->field_27359_a, var1);
}

void *J_JsonNodeSelector_func_27357_b(J_JsonNodeSelector *self, void *var1) {
    return self->field_27359_a->func_27059_b(self->field_27359_a, var1);
}

J_JsonNodeSelector *J_JsonNodeSelector_func_27355_a(J_JsonNodeSelector *self,
                                                    J_JsonNodeSelector *var1) {
    J_ChainedFunctor *cf = J_ChainedFunctor_create(self, var1);
    return J_JsonNodeSelector_create((J_Functor *)cf);
}

const char *J_JsonNodeSelector_func_27358_a(J_JsonNodeSelector *self) {
    return self->field_27359_a->func_27060_a(self->field_27359_a);
}

const char *J_JsonNodeSelector_toString(J_JsonNodeSelector *self) {
    return self->field_27359_a->toString(self->field_27359_a);
}
