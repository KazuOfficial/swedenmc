#ifndef J_SAJ_PARSER_H
#define J_SAJ_PARSER_H

#include "J_PositionTrackingPushbackReader.h"
#include "J_JsonListener.h"
#include "J_InvalidSyntaxException.h"

typedef struct J_SajParser {
    int _dummy;
} J_SajParser;

void J_SajParser_init(J_SajParser *self);

int J_SajParser_func_27463_a(J_SajParser *self, const char *buf, int len, J_JsonListener *var2,
                             J_InvalidSyntaxException *err_out);

#endif
