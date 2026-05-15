#ifndef J_JSON_FIELD_BUILDER_H
#define J_JSON_FIELD_BUILDER_H

#include "J_JsonNodeBuilder.h"
#include "J_JsonStringNode.h"

typedef struct J_JsonFieldBuilder {
    J_JsonNodeBuilder *field_27306_a;
    J_JsonNodeBuilder *field_27305_b;
} J_JsonFieldBuilder;

J_JsonFieldBuilder *J_JsonFieldBuilder_func_27301_a(void);

J_JsonFieldBuilder *J_JsonFieldBuilder_func_27304_a(J_JsonFieldBuilder *self,
                                                    J_JsonNodeBuilder *var1);

J_JsonFieldBuilder *J_JsonFieldBuilder_func_27300_b(J_JsonFieldBuilder *self,
                                                    J_JsonNodeBuilder *var1);

J_JsonStringNode *J_JsonFieldBuilder_func_27303_b(J_JsonFieldBuilder *self);

J_JsonNode *J_JsonFieldBuilder_func_27302_c(J_JsonFieldBuilder *self);

#endif
