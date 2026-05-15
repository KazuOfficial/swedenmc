#ifndef J_POSITION_TRACKING_PUSHBACK_READER_H
#define J_POSITION_TRACKING_PUSHBACK_READER_H

#include "J_ThingWithPosition.h"

typedef struct J_PositionTrackingPushbackReader {
    J_ThingWithPosition base;

    const char *buf;
    int pos;
    int len;

    char pushback_char;
    int has_pushback;

    int field_27337_b;

    int field_27340_c;

    int field_27339_d;
} J_PositionTrackingPushbackReader;

void J_PositionTrackingPushbackReader_init(J_PositionTrackingPushbackReader *self, const char *buf,
                                           int len);

void J_PositionTrackingPushbackReader_func_27334_a(J_PositionTrackingPushbackReader *self,
                                                   char var1);

void J_PositionTrackingPushbackReader_func_27335_a(J_PositionTrackingPushbackReader *self,
                                                   const char *var1, int len);

int J_PositionTrackingPushbackReader_func_27333_c(J_PositionTrackingPushbackReader *self);

int J_PositionTrackingPushbackReader_func_27336_b(J_PositionTrackingPushbackReader *self,
                                                  char *var1, int len);

int J_PositionTrackingPushbackReader_func_27331_a(J_ThingWithPosition *self);

int J_PositionTrackingPushbackReader_func_27330_b(J_ThingWithPosition *self);

#endif
