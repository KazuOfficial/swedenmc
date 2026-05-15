#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include "J_JsonNodeSelector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char *J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27324_a(
    struct J_JsonNodeSelector **var0, int count) {
    static char buf[1024];
    int pos = 0;
    buf[0] = '\0';
    for (int var2 = count - 1; var2 >= 0; --var2) {
        const char *part = J_JsonNodeSelector_func_27358_a(var0[var2]);
        int plen = (int)strlen(part);
        if (pos + plen < (int)sizeof(buf) - 1) {
            memcpy(buf + pos, part, (size_t)plen);
            pos += plen;
        }
        if (var2 != 0 && pos < (int)sizeof(buf) - 2) {
            buf[pos++] = '.';
        }
    }
    buf[pos] = '\0';
    return buf;
}

static J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *
make_exc(J_Functor *functor, struct J_JsonNodeSelector **list, int count) {
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *e =
        (J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *)calloc(
            1, sizeof(J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException));

    char msg[1200];
    const char *path =
        J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27324_a(list, count);
    snprintf(msg, sizeof(msg), "Failed to match any JSON node at [%s]", path);
    J_JsonNodeDoesNotMatchJsonNodeSelectorException_init(&e->base, msg);
    e->field_27326_a = functor;

    e->field_27325_b_size = count;
    if (count > 0) {
        e->field_27325_b = (struct J_JsonNodeSelector **)malloc(
            (size_t)count * sizeof(struct J_JsonNodeSelector *));
        memcpy(e->field_27325_b, list, (size_t)count * sizeof(struct J_JsonNodeSelector *));
    } else {
        e->field_27325_b = NULL;
    }
    return e;
}

J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *
J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27322_a(J_Functor *var0) {
    return make_exc(var0, NULL, 0);
}

J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *
J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27323_a(
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *var0, struct J_JsonNodeSelector *var1) {
    int old = var0->field_27325_b_size;
    int newcount = old + 1;
    struct J_JsonNodeSelector **var2 = (struct J_JsonNodeSelector **)malloc(
        (size_t)newcount * sizeof(struct J_JsonNodeSelector *));
    if (old > 0)
        memcpy(var2, var0->field_27325_b, (size_t)old * sizeof(struct J_JsonNodeSelector *));
    var2[old] = var1;
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *result =
        make_exc(var0->field_27326_a, var2, newcount);
    free(var2);
    return result;
}

J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *
J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_func_27321_b(
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *var0, struct J_JsonNodeSelector *var1) {
    struct J_JsonNodeSelector *list[1];
    list[0] = var1;
    return make_exc(var0->field_27326_a, list, 1);
}

const char *J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_toString(
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *self) {
    static char buf[2048];
    snprintf(buf, sizeof(buf),
             "JsonNodeDoesNotMatchJsonNodeSelectorException{failedNode=%p, failPath=%p}",
             (void *)self->field_27326_a, (void *)self->field_27325_b);
    return buf;
}

void J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException_free(
    J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *e) {
    if (e) {
        free(e->field_27325_b);
        free(e);
    }
}
