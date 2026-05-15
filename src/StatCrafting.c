#include "StatCrafting.h"
#include <stdlib.h>

StatCrafting *StatCrafting_create(int var1, const char *var2, int var3) {
    StatCrafting *self = (StatCrafting *)calloc(1, sizeof(StatCrafting));
    StatBase_init2(&self->base, var1, var2);
    self->field_25073_a = var3;
    return self;
}

int StatCrafting_func_25072_b(StatCrafting *self) { return self->field_25073_a; }
