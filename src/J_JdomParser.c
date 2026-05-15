#include "J_JdomParser.h"
#include "J_JsonListenerToJdomAdapter.h"
#include "J_SajParser.h"
#include <string.h>
#include <stdio.h>

void J_JdomParser_init(J_JdomParser *self) { (void)self; }

J_JsonRootNode *J_JdomParser_func_27366_a(J_JdomParser *self, const char *buf, int len,
                                          J_InvalidSyntaxException *err_out) {
    (void)self;
    J_JsonListenerToJdomAdapter var2;
    J_JsonListenerToJdomAdapter_init(&var2);

    J_SajParser parser;
    J_SajParser_init(&parser);

    if (J_SajParser_func_27463_a(&parser, buf, len, (J_JsonListener *)&var2, err_out) != 0) {
        return NULL;
    }
    return J_JsonListenerToJdomAdapter_func_27208_a(&var2);
}

J_JsonRootNode *J_JdomParser_func_27367_a(J_JdomParser *self, const char *var1,
                                          J_InvalidSyntaxException *err_out) {

    int len = (int)strlen(var1);
    J_JsonRootNode *var2 = J_JdomParser_func_27366_a(self, var1, len, err_out);

    return var2;
}
