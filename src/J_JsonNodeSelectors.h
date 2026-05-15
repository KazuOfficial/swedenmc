#ifndef J_JSON_NODE_SELECTORS_H
#define J_JSON_NODE_SELECTORS_H

#include "J_JsonNodeSelector.h"
#include "J_JsonStringNode.h"

#ifndef J_PATH_ELEM_FORWARD
#define J_PATH_ELEM_FORWARD
typedef enum { J_PATH_ELEM_STRING = 0, J_PATH_ELEM_INT = 1 } J_PathElemType;
typedef struct J_PathElem {
    J_PathElemType type;
    union {
        const char *str;
        int i;
    } val;
} J_PathElem;
#endif

J_JsonNodeSelector *J_JsonNodeSelectors_func_27349_a(J_PathElem *var0, int count);

J_JsonNodeSelector *J_JsonNodeSelectors_func_27346_b(J_PathElem *var0, int count);

J_JsonNodeSelector *J_JsonNodeSelectors_func_27353_c(J_PathElem *var0, int count);

J_JsonNodeSelector *J_JsonNodeSelectors_func_27348_a(const char *var0);

J_JsonNodeSelector *J_JsonNodeSelectors_func_27350_a(J_JsonStringNode *var0);

J_JsonNodeSelector *J_JsonNodeSelectors_func_27351_b(const char *var0);

J_JsonNodeSelector *J_JsonNodeSelectors_func_27347_a(int var0);

J_JsonNodeSelector *J_JsonNodeSelectors_func_27354_b(int var0);

#endif
