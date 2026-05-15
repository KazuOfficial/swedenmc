#include "J_JsonNodeBuilders.h"
#include "J_JsonNullNodeBuilder.h"
#include "J_JsonTrueNodeBuilder.h"
#include "J_JsonFalseNodeBuilder.h"
#include "J_JsonNumberNodeBuilder.h"

J_JsonNodeBuilder *J_JsonNodeBuilders_func_27248_a(void) {
    return (J_JsonNodeBuilder *)J_JsonNullNodeBuilder_create();
}
J_JsonNodeBuilder *J_JsonNodeBuilders_func_27251_b(void) {
    return (J_JsonNodeBuilder *)J_JsonTrueNodeBuilder_create();
}
J_JsonNodeBuilder *J_JsonNodeBuilders_func_27252_c(void) {
    return (J_JsonNodeBuilder *)J_JsonFalseNodeBuilder_create();
}
J_JsonNodeBuilder *J_JsonNodeBuilders_func_27250_a(const char *var0) {
    return (J_JsonNodeBuilder *)J_JsonNumberNodeBuilder_create(var0);
}
J_JsonStringNodeBuilder *J_JsonNodeBuilders_func_27254_b(const char *var0) {
    return J_JsonStringNodeBuilder_create(var0);
}
J_JsonObjectNodeBuilder *J_JsonNodeBuilders_func_27253_d(void) {
    return J_JsonObjectNodeBuilder_create();
}
J_JsonArrayNodeBuilder *J_JsonNodeBuilders_func_27249_e(void) {
    return J_JsonArrayNodeBuilder_create();
}
