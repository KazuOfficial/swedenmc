#include "J_JsonNodeDoesNotMatchPathElementsException.h"
#include "J_CompactJsonFormatter.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include "J_JsonNodeSelectors.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static J_CompactJsonFormatter field_27320_a;
static int field_27320_a_init = 0;
static void ensure_formatter(void) {
    if (!field_27320_a_init) {
        field_27320_a_init = 1;
        field_27320_a.base.func_27327_a = J_CompactJsonFormatter_func_27327_a;
    }
}

static void func_27317_a(void **var0, int count, char *out, int cap) {
    int pos = 0;
    int var2 = 1;
    for (int var5 = 0; var5 < count; ++var5) {
        J_PathElem *var6 = (J_PathElem *)var0[var5];
        if (!var2 && pos < cap - 1)
            out[pos++] = '.';
        var2 = 0;
        if (var6->type == J_PATH_ELEM_STRING) {

            int n = snprintf(out + pos, (size_t)(cap - pos), "\"%s\"", var6->val.str);
            if (n > 0)
                pos += n;
        } else {

            int n = snprintf(out + pos, (size_t)(cap - pos), "%d", var6->val.i);
            if (n > 0)
                pos += n;
        }
    }
    if (pos < cap)
        out[pos] = '\0';
}

static void func_27318_b(J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *var0, void **var1,
                         int var1_count, J_JsonRootNode *var2, char *out, int cap) {
    ensure_formatter();

    const char *functor_str =
        var0->field_27326_a ? var0->field_27326_a->toString(var0->field_27326_a) : "null";

    const char *path_str = J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27324_a(
        var0->field_27325_b, var0->field_27325_b_size);

    char resolving[512];
    func_27317_a(var1, var1_count, resolving, (int)sizeof(resolving));

    const char *json_str =
        J_CompactJsonFormatter_func_27327_a((J_JsonFormatter *)&field_27320_a, var2);
    snprintf(out, (size_t)cap, "Failed to find %s at [%s] while resolving [%s] in %s.", functor_str,
             path_str, resolving, json_str ? json_str : "null");
    free((void *)json_str);
}

J_JsonNodeDoesNotMatchPathElementsException *
J_JsonNodeDoesNotMatchPathElementsException_func_27319_a(
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *var0, void **var1, int var1_count,
    J_JsonRootNode *var2) {
    J_JsonNodeDoesNotMatchPathElementsException *e =
        (J_JsonNodeDoesNotMatchPathElementsException *)calloc(
            1, sizeof(J_JsonNodeDoesNotMatchPathElementsException));
    char msg[2048];
    func_27318_b(var0, var1, var1_count, var2, msg, (int)sizeof(msg));
    J_JsonNodeDoesNotMatchJsonNodeSelectorException_init(&e->base, msg);
    return e;
}
