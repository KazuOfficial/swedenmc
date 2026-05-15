#include "J_InvalidSyntaxException.h"
#include <stdio.h>

void J_InvalidSyntaxException_init(J_InvalidSyntaxException *self, const char *var1,
                                   J_ThingWithPosition *var2) {
    self->field_27191_a = var2->func_27331_a(var2);
    self->field_27190_b = var2->func_27330_b(var2);
    snprintf(self->message, sizeof(self->message), "At line %d, column %d:  %s",
             self->field_27190_b, self->field_27191_a, var1);
}

void J_InvalidSyntaxException_init2(J_InvalidSyntaxException *self, const char *var1,
                                    J_ThingWithPosition *var3) {

    J_InvalidSyntaxException_init(self, var1, var3);
}
