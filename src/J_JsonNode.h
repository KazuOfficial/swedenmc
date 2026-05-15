#ifndef J_JSON_NODE_H
#define J_JSON_NODE_H

#include "EnumJsonNodeType.h"

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

typedef struct J_JsonNode J_JsonNode;

struct J_JsonStringNode;
typedef struct J_JsonKVPair {
    struct J_JsonStringNode *key;
    J_JsonNode *value;
} J_JsonKVPair;
typedef struct J_JsonMap {
    J_JsonKVPair *entries;
    int count;
    int capacity;
} J_JsonMap;

typedef struct J_JsonNodeArray {
    J_JsonNode **items;
    int count;
    int capacity;
} J_JsonNodeArray;

typedef struct J_JsonNodeVtable {

    EnumJsonNodeType (*func_27218_a)(J_JsonNode *self);

    const char *(*func_27216_b)(J_JsonNode *self);

    J_JsonMap *(*func_27214_c)(J_JsonNode *self);

    J_JsonNodeArray *(*func_27215_d)(J_JsonNode *self);
} J_JsonNodeVtable;

struct J_JsonNode {
    const J_JsonNodeVtable *vtable;
};

const char *J_JsonNode_func_27213_a(J_JsonNode *self, J_PathElem *var1, int count);

J_JsonNodeArray *J_JsonNode_func_27217_b(J_JsonNode *self, J_PathElem *var1, int count);

J_JsonMap *J_JsonMap_create(int capacity);
void J_JsonMap_put(J_JsonMap *m, struct J_JsonStringNode *key, J_JsonNode *value);
J_JsonNode *J_JsonMap_get(const J_JsonMap *m, const struct J_JsonStringNode *key);
int J_JsonMap_containsKey(const J_JsonMap *m, const struct J_JsonStringNode *key);
J_JsonMap *J_JsonMap_copy(const J_JsonMap *m);
void J_JsonMap_free(J_JsonMap *m);

J_JsonNodeArray *J_JsonNodeArray_create(int capacity);
void J_JsonNodeArray_add(J_JsonNodeArray *a, J_JsonNode *node);
J_JsonNodeArray *J_JsonNodeArray_copy(const J_JsonNodeArray *a);
void J_JsonNodeArray_free(J_JsonNodeArray *a);

#endif
