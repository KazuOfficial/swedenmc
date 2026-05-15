#ifndef J_JSON_CONSTANTS_H
#define J_JSON_CONSTANTS_H

#include "J_JsonNode.h"

typedef struct J_JsonConstants {
    J_JsonNode base;
    EnumJsonNodeType field_27229_d;
} J_JsonConstants;

extern J_JsonConstants *J_JsonConstants_field_27228_a;

extern J_JsonConstants *J_JsonConstants_field_27227_b;

extern J_JsonConstants *J_JsonConstants_field_27230_c;

void J_JsonConstants_init(void);

EnumJsonNodeType J_JsonConstants_func_27218_a(J_JsonNode *self);

#endif
