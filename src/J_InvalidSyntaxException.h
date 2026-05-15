#ifndef J_INVALID_SYNTAX_EXCEPTION_H
#define J_INVALID_SYNTAX_EXCEPTION_H

#include "J_ThingWithPosition.h"

typedef struct J_InvalidSyntaxException {
    char message[512];

    int field_27191_a;

    int field_27190_b;
} J_InvalidSyntaxException;

void J_InvalidSyntaxException_init(J_InvalidSyntaxException *self, const char *var1,
                                   J_ThingWithPosition *var2);

void J_InvalidSyntaxException_init2(J_InvalidSyntaxException *self, const char *var1,
                                    J_ThingWithPosition *var3);

#endif
