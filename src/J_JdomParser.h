#ifndef J_JDOM_PARSER_H
#define J_JDOM_PARSER_H

#include "J_JsonRootNode.h"
#include "J_InvalidSyntaxException.h"

typedef struct J_JdomParser {
    int _dummy;
} J_JdomParser;

void J_JdomParser_init(J_JdomParser *self);

J_JsonRootNode *J_JdomParser_func_27366_a(J_JdomParser *self, const char *buf, int len,
                                          J_InvalidSyntaxException *err_out);

J_JsonRootNode *J_JdomParser_func_27367_a(J_JdomParser *self, const char *var1,
                                          J_InvalidSyntaxException *err_out);

#endif
