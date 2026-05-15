#ifndef J_JSON_NODE_SELECTOR_H
#define J_JSON_NODE_SELECTOR_H

#include "J_Functor.h"

typedef struct J_JsonNodeSelector {
    J_Functor *field_27359_a;
} J_JsonNodeSelector;

J_JsonNodeSelector *J_JsonNodeSelector_create(J_Functor *var1);

int J_JsonNodeSelector_func_27356_a(J_JsonNodeSelector *self, void *var1);

void *J_JsonNodeSelector_func_27357_b(J_JsonNodeSelector *self, void *var1);

J_JsonNodeSelector *J_JsonNodeSelector_func_27355_a(J_JsonNodeSelector *self,
                                                    J_JsonNodeSelector *var1);

const char *J_JsonNodeSelector_func_27358_a(J_JsonNodeSelector *self);

const char *J_JsonNodeSelector_toString(J_JsonNodeSelector *self);

#endif
