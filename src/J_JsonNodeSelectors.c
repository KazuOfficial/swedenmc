#include "J_JsonNodeSelectors.h"
#include "J_JsonNodeFactories.h"
#include "J_JsonArrayNodeSelector.h"
#include "J_JsonObjectNodeSelector.h"
#include "J_JsonStringNodeSelector.h"
#include "J_JsonFieldNodeSelector.h"
#include "J_JsonElementNodeSelector.h"
#include "J_ChainedFunctor.h"
#include <stdlib.h>
#include <string.h>

static J_JsonNodeSelector *func_27345_a(J_JsonNodeSelector *var0, J_JsonNodeSelector *var1) {
    J_ChainedFunctor *cf = J_ChainedFunctor_create(var0, var1);
    return J_JsonNodeSelector_create((J_Functor *)cf);
}

static J_JsonNodeSelector *func_27352_a(J_PathElem *var0, int count, J_JsonNodeSelector *var1) {
    J_JsonNodeSelector *var2 = var1;
    for (int var3 = count - 1; var3 >= 0; --var3) {
        if (var0[var3].type == J_PATH_ELEM_INT)
            var2 = func_27345_a(J_JsonNodeSelectors_func_27354_b(var0[var3].val.i), var2);
        else
            var2 = func_27345_a(J_JsonNodeSelectors_func_27351_b(var0[var3].val.str), var2);
    }
    return var2;
}

J_JsonNodeSelector *J_JsonNodeSelectors_func_27349_a(J_PathElem *var0, int count) {
    return func_27352_a(var0, count,
                        J_JsonNodeSelector_create((J_Functor *)J_JsonStringNodeSelector_create()));
}

J_JsonNodeSelector *J_JsonNodeSelectors_func_27346_b(J_PathElem *var0, int count) {
    return func_27352_a(var0, count,
                        J_JsonNodeSelector_create((J_Functor *)J_JsonArrayNodeSelector_create()));
}

J_JsonNodeSelector *J_JsonNodeSelectors_func_27353_c(J_PathElem *var0, int count) {
    return func_27352_a(var0, count,
                        J_JsonNodeSelector_create((J_Functor *)J_JsonObjectNodeSelector_create()));
}

J_JsonNodeSelector *J_JsonNodeSelectors_func_27348_a(const char *var0) {
    return J_JsonNodeSelectors_func_27350_a(J_JsonNodeFactories_func_27316_a(var0));
}

J_JsonNodeSelector *J_JsonNodeSelectors_func_27350_a(J_JsonStringNode *var0) {
    return J_JsonNodeSelector_create((J_Functor *)J_JsonFieldNodeSelector_create(var0));
}

J_JsonNodeSelector *J_JsonNodeSelectors_func_27351_b(const char *var0) {

    J_JsonNodeSelector *obj = J_JsonNodeSelectors_func_27353_c(NULL, 0);
    return J_JsonNodeSelector_func_27355_a(obj, J_JsonNodeSelectors_func_27348_a(var0));
}

J_JsonNodeSelector *J_JsonNodeSelectors_func_27347_a(int var0) {
    return J_JsonNodeSelector_create((J_Functor *)J_JsonElementNodeSelector_create(var0));
}

J_JsonNodeSelector *J_JsonNodeSelectors_func_27354_b(int var0) {

    J_JsonNodeSelector *arr = J_JsonNodeSelectors_func_27346_b(NULL, 0);
    return J_JsonNodeSelector_func_27355_a(arr, J_JsonNodeSelectors_func_27347_a(var0));
}
