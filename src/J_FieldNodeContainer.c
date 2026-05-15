#include "J_FieldNodeContainer.h"
#include <stdlib.h>
#include <stdio.h>

static void J_FieldNodeContainer_func_27290_a(J_NodeContainer *self, J_JsonNodeBuilder *var1) {
    J_FieldNodeContainer *c = (J_FieldNodeContainer *)self;
    J_JsonFieldBuilder_func_27300_b(c->field_27292_a, var1);
}

static void J_FieldNodeContainer_func_27289_a(J_NodeContainer *self, J_JsonFieldBuilder *var1) {
    (void)self;
    (void)var1;
    fprintf(stderr, "Coding failure in Argo:  Attempt to add a field to a field.\n");
    exit(1);
}

static const J_NodeContainer J_FieldNodeContainer_vtable = {
    J_FieldNodeContainer_func_27290_a,
    J_FieldNodeContainer_func_27289_a,
};

J_FieldNodeContainer *J_FieldNodeContainer_create(struct J_JsonListenerToJdomAdapter *var1,
                                                  J_JsonFieldBuilder *var2) {
    J_FieldNodeContainer *self = (J_FieldNodeContainer *)calloc(1, sizeof(J_FieldNodeContainer));
    self->base = J_FieldNodeContainer_vtable;
    self->field_27291_b = var1;
    self->field_27292_a = var2;
    return self;
}
