#ifndef J_COMPACT_JSON_FORMATTER_H
#define J_COMPACT_JSON_FORMATTER_H

#include "J_JsonFormatter.h"
#include <stddef.h>

typedef struct {
    char *buf;
    size_t len;
    size_t cap;
} StrBuf;

typedef struct J_CompactJsonFormatter {
    J_JsonFormatter base;
} J_CompactJsonFormatter;

J_CompactJsonFormatter *J_CompactJsonFormatter_create(void);

const char *J_CompactJsonFormatter_func_27327_a(J_JsonFormatter *self, J_JsonRootNode *var1);

void J_CompactJsonFormatter_func_27329_a(J_JsonFormatter *self, J_JsonRootNode *var1, StrBuf *var2);

#endif
