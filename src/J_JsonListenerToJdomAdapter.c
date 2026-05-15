#include "J_JsonListenerToJdomAdapter.h"
#include "J_JsonNodeBuilders.h"
#include "J_ArrayNodeContainer.h"
#include "J_ObjectNodeContainer.h"
#include "J_FieldNodeContainer.h"
#include "J_JsonFieldBuilder.h"
#include <string.h>

static void stack_push(J_JsonListenerToJdomAdapter *self, J_NodeContainer *c) {
    self->field_27210_a[self->field_27210_a_top++] = c;
}
static void stack_pop(J_JsonListenerToJdomAdapter *self) { --self->field_27210_a_top; }
static J_NodeContainer *stack_peek(J_JsonListenerToJdomAdapter *self) {
    return self->field_27210_a[self->field_27210_a_top - 1];
}

static void func_27207_a(J_JsonListenerToJdomAdapter *self, J_JsonNodeBuilder *var1) {
    if (self->field_27209_b == NULL) {
        self->field_27209_b = var1;
    } else {
        stack_peek(self)->func_27290_a(stack_peek(self), var1);
    }
}

static void func_27206_b(J_JsonListenerToJdomAdapter *self, J_JsonNodeBuilder *var1) {
    stack_peek(self)->func_27290_a(stack_peek(self), var1);
}

static void adp_func_27195_b(J_JsonListener *self) { (void)self; }
static void adp_func_27204_c(J_JsonListener *self) { (void)self; }

static void adp_func_27200_d(J_JsonListener *self) {
    J_JsonListenerToJdomAdapter *a = (J_JsonListenerToJdomAdapter *)self;
    J_JsonArrayNodeBuilder *var1 = J_JsonNodeBuilders_func_27249_e();
    func_27207_a(a, (J_JsonNodeBuilder *)var1);
    stack_push(a, (J_NodeContainer *)J_ArrayNodeContainer_create(a, var1));
}
static void adp_func_27197_e(J_JsonListener *self) {
    stack_pop((J_JsonListenerToJdomAdapter *)self);
}
static void adp_func_27194_f(J_JsonListener *self) {
    J_JsonListenerToJdomAdapter *a = (J_JsonListenerToJdomAdapter *)self;
    J_JsonObjectNodeBuilder *var1 = J_JsonNodeBuilders_func_27253_d();
    func_27207_a(a, (J_JsonNodeBuilder *)var1);
    stack_push(a, (J_NodeContainer *)J_ObjectNodeContainer_create(a, var1));
}
static void adp_func_27203_g(J_JsonListener *self) {
    stack_pop((J_JsonListenerToJdomAdapter *)self);
}
static void adp_func_27205_a(J_JsonListener *self, const char *var1) {
    J_JsonListenerToJdomAdapter *a = (J_JsonListenerToJdomAdapter *)self;
    J_JsonFieldBuilder *var2 =
        J_JsonFieldBuilder_func_27304_a(J_JsonFieldBuilder_func_27301_a(),
                                        (J_JsonNodeBuilder *)J_JsonNodeBuilders_func_27254_b(var1));
    stack_peek(a)->func_27289_a(stack_peek(a), var2);
    stack_push(a, (J_NodeContainer *)J_FieldNodeContainer_create(a, var2));
}
static void adp_func_27199_h(J_JsonListener *self) {
    stack_pop((J_JsonListenerToJdomAdapter *)self);
}
static void adp_func_27201_b(J_JsonListener *self, const char *var1) {
    func_27206_b((J_JsonListenerToJdomAdapter *)self, J_JsonNodeBuilders_func_27250_a(var1));
}
static void adp_func_27196_i(J_JsonListener *self) {
    func_27206_b((J_JsonListenerToJdomAdapter *)self, J_JsonNodeBuilders_func_27251_b());
}
static void adp_func_27198_c(J_JsonListener *self, const char *var1) {
    func_27206_b((J_JsonListenerToJdomAdapter *)self,
                 (J_JsonNodeBuilder *)J_JsonNodeBuilders_func_27254_b(var1));
}
static void adp_func_27193_j(J_JsonListener *self) {
    func_27206_b((J_JsonListenerToJdomAdapter *)self, J_JsonNodeBuilders_func_27252_c());
}
static void adp_func_27202_k(J_JsonListener *self) {
    func_27206_b((J_JsonListenerToJdomAdapter *)self, J_JsonNodeBuilders_func_27248_a());
}

static J_JsonListenerVtable J_JsonListenerToJdomAdapter_vtable;
static int J_JsonListenerToJdomAdapter_vtable_init = 0;

void J_JsonListenerToJdomAdapter_init(J_JsonListenerToJdomAdapter *self) {
    if (!J_JsonListenerToJdomAdapter_vtable_init) {
        J_JsonListenerToJdomAdapter_vtable.func_27195_b = adp_func_27195_b;
        J_JsonListenerToJdomAdapter_vtable.func_27204_c = adp_func_27204_c;
        J_JsonListenerToJdomAdapter_vtable.func_27200_d = adp_func_27200_d;
        J_JsonListenerToJdomAdapter_vtable.func_27197_e = adp_func_27197_e;
        J_JsonListenerToJdomAdapter_vtable.func_27194_f = adp_func_27194_f;
        J_JsonListenerToJdomAdapter_vtable.func_27203_g = adp_func_27203_g;
        J_JsonListenerToJdomAdapter_vtable.func_27205_a = adp_func_27205_a;
        J_JsonListenerToJdomAdapter_vtable.func_27199_h = adp_func_27199_h;
        J_JsonListenerToJdomAdapter_vtable.func_27198_c = adp_func_27198_c;
        J_JsonListenerToJdomAdapter_vtable.func_27201_b = adp_func_27201_b;
        J_JsonListenerToJdomAdapter_vtable.func_27196_i = adp_func_27196_i;
        J_JsonListenerToJdomAdapter_vtable.func_27193_j = adp_func_27193_j;
        J_JsonListenerToJdomAdapter_vtable.func_27202_k = adp_func_27202_k;
        J_JsonListenerToJdomAdapter_vtable_init = 1;
    }
    memset(self, 0, sizeof(*self));
    self->base.vtable = &J_JsonListenerToJdomAdapter_vtable;
}

J_JsonRootNode *J_JsonListenerToJdomAdapter_func_27208_a(J_JsonListenerToJdomAdapter *self) {
    return (J_JsonRootNode *)self->field_27209_b->func_27234_b(self->field_27209_b);
}
