#include "J_ArrayNodeContainer.h"
#include <stdlib.h>
#include <stdio.h>

static void J_ArrayNodeContainer_func_27290_a(J_NodeContainer *self, J_JsonNodeBuilder *var1) {
    J_ArrayNodeContainer *c = (J_ArrayNodeContainer *)self;
    J_JsonArrayNodeBuilder_func_27240_a(c->field_27294_a, var1);
}

static void J_ArrayNodeContainer_func_27289_a(J_NodeContainer *self, J_JsonFieldBuilder *var1) {
    (void)self;
    (void)var1;
    fprintf(stderr, "Coding failure in Argo:  Attempt to add a field to an array.\n");
    exit(1);
}

static const J_NodeContainer J_ArrayNodeContainer_vtable = {
    J_ArrayNodeContainer_func_27290_a,
    J_ArrayNodeContainer_func_27289_a,
};

J_ArrayNodeContainer *J_ArrayNodeContainer_create(struct J_JsonListenerToJdomAdapter *var1,
                                                  J_JsonArrayNodeBuilder *var2) {
    J_ArrayNodeContainer *self = (J_ArrayNodeContainer *)calloc(1, sizeof(J_ArrayNodeContainer));
    self->base = J_ArrayNodeContainer_vtable;
    self->field_27293_b = var1;
    self->field_27294_a = var2;
    return self;
}
