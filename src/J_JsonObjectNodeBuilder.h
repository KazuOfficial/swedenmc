#ifndef J_JSON_OBJECT_NODE_BUILDER_H
#define J_JSON_OBJECT_NODE_BUILDER_H

#include "J_JsonNodeBuilder.h"
#include "J_JsonFieldBuilder.h"
#include "J_JsonRootNode.h"

typedef struct J_JsonObjectNodeBuilder {
    J_JsonNodeBuilder base;
    J_JsonFieldBuilder **field_27238_a;
    int field_27238_a_count;
    int field_27238_a_cap;
} J_JsonObjectNodeBuilder;

J_JsonObjectNodeBuilder *J_JsonObjectNodeBuilder_create(void);

J_JsonObjectNodeBuilder *J_JsonObjectNodeBuilder_func_27237_a(J_JsonObjectNodeBuilder *self,
                                                              J_JsonFieldBuilder *var1);

J_JsonRootNode *J_JsonObjectNodeBuilder_func_27235_a(J_JsonObjectNodeBuilder *self);

J_JsonFieldBuilder **J_JsonObjectNodeBuilder_func_27236_a(J_JsonObjectNodeBuilder *var0,
                                                          int *out_count);

#endif
