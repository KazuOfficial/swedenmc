#include "J_PositionTrackingPushbackReader.h"
#include <string.h>

static const J_ThingWithPosition J_PositionTrackingPushbackReader_twp = {
    J_PositionTrackingPushbackReader_func_27331_a,
    J_PositionTrackingPushbackReader_func_27330_b,
};

void J_PositionTrackingPushbackReader_init(J_PositionTrackingPushbackReader *self, const char *buf,
                                           int len) {
    memset(self, 0, sizeof(*self));
    self->base = J_PositionTrackingPushbackReader_twp;
    self->buf = buf;
    self->pos = 0;
    self->len = len;
    self->field_27337_b = 0;
    self->field_27340_c = 1;
    self->field_27339_d = 0;
}

static void func_27332_a(J_PositionTrackingPushbackReader *self, int var1) {
    if (13 == var1) {
        self->field_27337_b = 0;
        ++self->field_27340_c;
        self->field_27339_d = 1;
    } else {
        if (10 == var1 && !self->field_27339_d) {
            self->field_27337_b = 0;
            ++self->field_27340_c;
        } else {
            ++self->field_27337_b;
        }
        self->field_27339_d = 0;
    }
}

void J_PositionTrackingPushbackReader_func_27334_a(J_PositionTrackingPushbackReader *self,
                                                   char var1) {
    --self->field_27337_b;
    if (self->field_27337_b < 0)
        self->field_27337_b = 0;
    self->pushback_char = var1;
    self->has_pushback = 1;
}

void J_PositionTrackingPushbackReader_func_27335_a(J_PositionTrackingPushbackReader *self,
                                                   const char *var1, int len) {
    (void)var1;

    self->field_27337_b -= len;
    if (self->field_27337_b < 0)
        self->field_27337_b = 0;
}

int J_PositionTrackingPushbackReader_func_27333_c(J_PositionTrackingPushbackReader *self) {
    int var1;
    if (self->has_pushback) {
        var1 = (unsigned char)self->pushback_char;
        self->has_pushback = 0;
    } else if (self->pos < self->len) {
        var1 = (unsigned char)self->buf[self->pos++];
    } else {
        var1 = -1;
    }
    func_27332_a(self, var1);
    return var1;
}

int J_PositionTrackingPushbackReader_func_27336_b(J_PositionTrackingPushbackReader *self,
                                                  char *var1, int len) {
    int i;
    for (i = 0; i < len; i++) {
        int c = J_PositionTrackingPushbackReader_func_27333_c(self);
        if (c == -1)
            break;
        var1[i] = (char)c;
    }
    return i;
}

int J_PositionTrackingPushbackReader_func_27331_a(J_ThingWithPosition *self) {
    return ((J_PositionTrackingPushbackReader *)self)->field_27337_b;
}

int J_PositionTrackingPushbackReader_func_27330_b(J_ThingWithPosition *self) {
    return ((J_PositionTrackingPushbackReader *)self)->field_27340_c;
}
