#include "J_ObjectNodeContainer.h"
#include <stdlib.h>
#include <stdio.h>

static void J_ObjectNodeContainer_func_27290_a(J_NodeContainer *self, J_JsonNodeBuilder *var1) {
    (void)self;
    (void)var1;
    fprintf(stderr, "Coding failure in Argo:  Attempt to add a node to an object.\n");
    exit(1);
}

static void J_ObjectNodeContainer_func_27289_a(J_NodeContainer *self, J_JsonFieldBuilder *var1) {
    J_ObjectNodeContainer *c = (J_ObjectNodeContainer *)self;
    J_JsonObjectNodeBuilder_func_27237_a(c->field_27296_a, var1);
}

static const J_NodeContainer J_ObjectNodeContainer_vtable = {
    J_ObjectNodeContainer_func_27290_a,
    J_ObjectNodeContainer_func_27289_a,
};

J_ObjectNodeContainer *J_ObjectNodeContainer_create(struct J_JsonListenerToJdomAdapter *var1,
                                                    J_JsonObjectNodeBuilder *var2) {
    J_ObjectNodeContainer *self = (J_ObjectNodeContainer *)calloc(1, sizeof(J_ObjectNodeContainer));
    self->base = J_ObjectNodeContainer_vtable;
    self->field_27295_b = var1;
    self->field_27296_a = var2;
    return self;
}
