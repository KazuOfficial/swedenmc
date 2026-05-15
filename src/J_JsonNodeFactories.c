#include "J_JsonNodeFactories.h"
#include "J_JsonConstants.h"
#include "J_JsonNumberNode.h"
#include "J_JsonArray.h"
#include "J_JsonObject.h"

J_JsonNode *J_JsonNodeFactories_func_27310_a(void) {
    J_JsonConstants_init();
    return (J_JsonNode *)J_JsonConstants_field_27228_a;
}

J_JsonNode *J_JsonNodeFactories_func_27313_b(void) {
    J_JsonConstants_init();
    return (J_JsonNode *)J_JsonConstants_field_27227_b;
}

J_JsonNode *J_JsonNodeFactories_func_27314_c(void) {
    J_JsonConstants_init();
    return (J_JsonNode *)J_JsonConstants_field_27230_c;
}

J_JsonStringNode *J_JsonNodeFactories_func_27316_a(const char *var0) {
    return J_JsonStringNode_create(var0);
}

J_JsonNode *J_JsonNodeFactories_func_27311_b(const char *var0) {
    return (J_JsonNode *)J_JsonNumberNode_create(var0);
}

J_JsonRootNode *J_JsonNodeFactories_func_27309_a(J_JsonNodeArray *var0) {
    return (J_JsonRootNode *)J_JsonArray_create(var0);
}

J_JsonRootNode *J_JsonNodeFactories_func_27315_a(J_JsonNode **var0, int count) {

    J_JsonNodeArray *arr = J_JsonNodeArray_create(count);
    for (int i = 0; i < count; i++)
        J_JsonNodeArray_add(arr, var0[i]);
    J_JsonRootNode *result = J_JsonNodeFactories_func_27309_a(arr);
    J_JsonNodeArray_free(arr);
    return result;
}

J_JsonRootNode *J_JsonNodeFactories_func_27312_a(J_JsonMap *var0) {
    return (J_JsonRootNode *)J_JsonObject_create(var0);
}
