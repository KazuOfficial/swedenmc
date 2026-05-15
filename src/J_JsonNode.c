#include "J_JsonNode.h"
#include "J_JsonStringNode.h"
#include "J_JsonNodeSelectors.h"
#include "J_JsonNodeSelector.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include "J_JsonNodeDoesNotMatchPathElementsException.h"
#include "J_JsonNodeFactories.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

J_JsonMap *J_JsonMap_create(int capacity) {
    J_JsonMap *m = (J_JsonMap *)calloc(1, sizeof(J_JsonMap));
    m->entries = (J_JsonKVPair *)calloc(capacity, sizeof(J_JsonKVPair));
    m->capacity = capacity;
    return m;
}

void J_JsonMap_put(J_JsonMap *m, struct J_JsonStringNode *key, J_JsonNode *value) {
    if (m->count >= m->capacity) {
        m->capacity = m->capacity * 2 + 4;
        m->entries = (J_JsonKVPair *)realloc(m->entries, m->capacity * sizeof(J_JsonKVPair));
    }
    m->entries[m->count].key = key;
    m->entries[m->count].value = value;
    m->count++;
}

J_JsonNode *J_JsonMap_get(const J_JsonMap *m, const struct J_JsonStringNode *key) {
    for (int i = 0; i < m->count; i++)
        if (strcmp(m->entries[i].key->field_27224_a, key->field_27224_a) == 0)
            return m->entries[i].value;
    return NULL;
}

int J_JsonMap_containsKey(const J_JsonMap *m, const struct J_JsonStringNode *key) {
    return J_JsonMap_get(m, key) != NULL;
}

J_JsonMap *J_JsonMap_copy(const J_JsonMap *m) {
    J_JsonMap *c = J_JsonMap_create(m->count);
    for (int i = 0; i < m->count; i++)
        J_JsonMap_put(c, m->entries[i].key, m->entries[i].value);
    return c;
}

void J_JsonMap_free(J_JsonMap *m) {
    if (m) {
        free(m->entries);
        free(m);
    }
}

J_JsonNodeArray *J_JsonNodeArray_create(int capacity) {
    J_JsonNodeArray *a = (J_JsonNodeArray *)calloc(1, sizeof(J_JsonNodeArray));
    a->items = (J_JsonNode **)calloc(capacity + 1, sizeof(J_JsonNode *));
    a->capacity = capacity;
    return a;
}

void J_JsonNodeArray_add(J_JsonNodeArray *a, J_JsonNode *node) {
    if (a->count >= a->capacity) {
        a->capacity = a->capacity * 2 + 4;
        a->items = (J_JsonNode **)realloc(a->items, (a->capacity + 1) * sizeof(J_JsonNode *));
    }
    a->items[a->count++] = node;
}

J_JsonNodeArray *J_JsonNodeArray_copy(const J_JsonNodeArray *a) {
    J_JsonNodeArray *c = J_JsonNodeArray_create(a->count);
    for (int i = 0; i < a->count; i++)
        J_JsonNodeArray_add(c, a->items[i]);
    return c;
}

void J_JsonNodeArray_free(J_JsonNodeArray *a) {
    if (a) {
        free(a->items);
        free(a);
    }
}

static void *func_27219_a(J_JsonNodeSelector *var1, J_JsonNode *var2, J_PathElem *var3, int count) {
    void *result = J_JsonNodeSelector_func_27357_b(var1, var2);
    if (J_ChainedException_pending) {
        J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *var5 = J_ChainedException_pending;
        J_ChainedException_pending = NULL;

        J_JsonRootNode *arr = J_JsonNodeFactories_func_27315_a(&var2, 1);

        void **ptrs = (void **)malloc((size_t)count * sizeof(void *));
        for (int i = 0; i < count; i++)
            ptrs[i] = &var3[i];
        J_JsonNodeDoesNotMatchPathElementsException *path_ex =
            J_JsonNodeDoesNotMatchPathElementsException_func_27319_a(var5, ptrs, count, arr);
        free(ptrs);
        fprintf(stderr, "J_JsonNodeDoesNotMatchPathElementsException: %s\n", path_ex->base.message);
        exit(1);
    }
    return result;
}

const char *J_JsonNode_func_27213_a(J_JsonNode *self, J_PathElem *var1, int count) {
    J_JsonNodeSelector *sel = J_JsonNodeSelectors_func_27349_a(var1, count);
    return (const char *)func_27219_a(sel, self, var1, count);
}

J_JsonNodeArray *J_JsonNode_func_27217_b(J_JsonNode *self, J_PathElem *var1, int count) {
    J_JsonNodeSelector *sel = J_JsonNodeSelectors_func_27346_b(var1, count);
    return (J_JsonNodeArray *)func_27219_a(sel, self, var1, count);
}
