#ifndef J_JSON_OBJECT_H
#define J_JSON_OBJECT_H

#include "J_JsonRootNode.h"

typedef struct J_JsonObject {
    J_JsonNode base;
    J_JsonMap *field_27222_a;
} J_JsonObject;

J_JsonObject *J_JsonObject_create(J_JsonMap *var1);

EnumJsonNodeType J_JsonObject_func_27218_a(J_JsonNode *self);
J_JsonMap *J_JsonObject_func_27214_c(J_JsonNode *self);

int J_JsonObject_equals(const J_JsonObject *self, const J_JsonObject *var1);

int J_JsonObject_hashCode(const J_JsonObject *self);

const char *J_JsonObject_toString(const J_JsonObject *self);

#endif
