#include "J_JsonEscapedString.h"
#include <stdlib.h>
#include <string.h>

void J_JsonEscapedString_init(J_JsonEscapedString *self, const char *var1) {

    size_t len = strlen(var1);
    char *buf = (char *)malloc(len * 2 + 1);
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)var1[i];
        switch (c) {
        case '\\':
            buf[j++] = '\\';
            buf[j++] = '\\';
            break;
        case '"':
            buf[j++] = '\\';
            buf[j++] = '"';
            break;
        case '\b':
            buf[j++] = '\\';
            buf[j++] = 'b';
            break;
        case '\f':
            buf[j++] = '\\';
            buf[j++] = 'f';
            break;
        case '\n':
            buf[j++] = '\\';
            buf[j++] = 'n';
            break;
        case '\r':
            buf[j++] = '\\';
            buf[j++] = 'r';
            break;
        case '\t':
            buf[j++] = '\\';
            buf[j++] = 't';
            break;
        default:
            buf[j++] = (char)c;
            break;
        }
    }
    buf[j] = '\0';
    self->field_27031_a = buf;
}

const char *J_JsonEscapedString_toString(const J_JsonEscapedString *self) {
    return self->field_27031_a;
}

void J_JsonEscapedString_free(J_JsonEscapedString *self) {
    free(self->field_27031_a);
    self->field_27031_a = NULL;
}
