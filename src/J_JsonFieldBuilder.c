#include "J_JsonFieldBuilder.h"
#include <stdlib.h>

J_JsonFieldBuilder *J_JsonFieldBuilder_func_27301_a(void) {
    return (J_JsonFieldBuilder *)calloc(1, sizeof(J_JsonFieldBuilder));
}

J_JsonFieldBuilder *J_JsonFieldBuilder_func_27304_a(J_JsonFieldBuilder *self,
                                                    J_JsonNodeBuilder *var1) {
    self->field_27306_a = var1;
    return self;
}

J_JsonFieldBuilder *J_JsonFieldBuilder_func_27300_b(J_JsonFieldBuilder *self,
                                                    J_JsonNodeBuilder *var1) {
    self->field_27305_b = var1;
    return self;
}

J_JsonStringNode *J_JsonFieldBuilder_func_27303_b(J_JsonFieldBuilder *self) {
    return (J_JsonStringNode *)self->field_27306_a->func_27234_b(self->field_27306_a);
}

J_JsonNode *J_JsonFieldBuilder_func_27302_c(J_JsonFieldBuilder *self) {
    return self->field_27305_b->func_27234_b(self->field_27305_b);
}
