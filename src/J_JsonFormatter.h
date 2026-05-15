#ifndef J_JSON_FORMATTER_H
#define J_JSON_FORMATTER_H

#include "J_JsonRootNode.h"

typedef struct J_JsonFormatter J_JsonFormatter;

struct J_JsonFormatter {

    const char *(*func_27327_a)(J_JsonFormatter *self, J_JsonRootNode *var1);
};

#endif
