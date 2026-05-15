#ifndef J_JSON_ESCAPED_STRING_H
#define J_JSON_ESCAPED_STRING_H

typedef struct J_JsonEscapedString {
    char *field_27031_a;
} J_JsonEscapedString;

void J_JsonEscapedString_init(J_JsonEscapedString *self, const char *var1);

const char *J_JsonEscapedString_toString(const J_JsonEscapedString *self);
void J_JsonEscapedString_free(J_JsonEscapedString *self);

#endif
