#ifndef J_JSON_ARRAY_NODE_BUILDER_H
#define J_JSON_ARRAY_NODE_BUILDER_H

#include "J_JsonNodeBuilder.h"
#include "J_JsonRootNode.h"

typedef struct J_JsonArrayNodeBuilder {
    J_JsonNodeBuilder base;
    J_JsonNodeBuilder **field_27242_a;
    int field_27242_a_count;
    int field_27242_a_cap;
} J_JsonArrayNodeBuilder;

J_JsonArrayNodeBuilder *J_JsonArrayNodeBuilder_create(void);

J_JsonArrayNodeBuilder *J_JsonArrayNodeBuilder_func_27240_a(J_JsonArrayNodeBuilder *self,
                                                            J_JsonNodeBuilder *var1);

J_JsonRootNode *J_JsonArrayNodeBuilder_func_27241_a(J_JsonArrayNodeBuilder *self);

#endif
