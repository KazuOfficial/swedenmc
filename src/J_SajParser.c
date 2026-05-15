#include "J_SajParser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void J_SajParser_init(J_SajParser *self) { (void)self; }

static int func_27455_a(J_SajParser *self, J_PositionTrackingPushbackReader *var1,
                        J_JsonListener *var2, J_InvalidSyntaxException *err);
static int func_27453_b(J_SajParser *self, J_PositionTrackingPushbackReader *var1,
                        J_JsonListener *var2, J_InvalidSyntaxException *err);
static int func_27449_c(J_SajParser *self, J_PositionTrackingPushbackReader *var1,
                        J_JsonListener *var2, J_InvalidSyntaxException *err);
static int func_27464_d(J_SajParser *self, J_PositionTrackingPushbackReader *var1,
                        J_JsonListener *var2, J_InvalidSyntaxException *err);
static int func_27459_a(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err);
static int func_27451_b(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err);
static int func_27460_c(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out_ch,
                        J_InvalidSyntaxException *err);
static int func_27458_d(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out_ch,
                        J_InvalidSyntaxException *err);
static int func_27456_e(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err);
static int func_27462_f(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err);
static int func_27454_g(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err);
static int func_27461_h(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err);
static int func_27452_i(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err);
static int func_27457_j(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out_ch,
                        J_InvalidSyntaxException *err);
static int func_27450_k(J_SajParser *self, J_PositionTrackingPushbackReader *var1,
                        int *out_codepoint, J_InvalidSyntaxException *err);
static int func_27448_l(J_SajParser *self, J_PositionTrackingPushbackReader *var1, int *out_ch,
                        J_InvalidSyntaxException *err);

#define THROW(err, msg, reader)                                                                    \
    do {                                                                                           \
        if (err)                                                                                   \
            J_InvalidSyntaxException_init((err), (msg), (J_ThingWithPosition *)(reader));          \
        return -1;                                                                                 \
    } while (0)

int J_SajParser_func_27463_a(J_SajParser *self, const char *buf, int len, J_JsonListener *var2,
                             J_InvalidSyntaxException *err_out) {
    J_PositionTrackingPushbackReader var3;
    J_PositionTrackingPushbackReader_init(&var3, buf, len);

    int raw = J_PositionTrackingPushbackReader_func_27333_c(&var3);
    char var4 = (char)raw;
    switch (var4) {
    case '[':
        J_PositionTrackingPushbackReader_func_27334_a(&var3, var4);
        var2->vtable->func_27195_b(var2);
        if (func_27455_a(self, &var3, var2, err_out) != 0)
            return -1;
        break;
    case '{':
        J_PositionTrackingPushbackReader_func_27334_a(&var3, var4);
        var2->vtable->func_27195_b(var2);
        if (func_27453_b(self, &var3, var2, err_out) != 0)
            return -1;
        break;
    default: {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected either [ or { but got [%c].", var4);
        THROW(err_out, msg, &var3);
    }
    }

    int var5;
    if (func_27448_l(self, &var3, &var5, err_out) != 0)
        return -1;
    if (var5 != -1) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Got unexpected trailing character [%c].", (char)var5);
        THROW(err_out, msg, &var3);
    }
    var2->vtable->func_27204_c(var2);
    return 0;
}

static int func_27455_a(J_SajParser *self, J_PositionTrackingPushbackReader *var1,
                        J_JsonListener *var2, J_InvalidSyntaxException *err) {
    int raw;
    if (func_27448_l(self, var1, &raw, err) != 0)
        return -1;
    char var3 = (char)raw;
    if (var3 != 91) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected object to start with [ but got [%c].", var3);
        THROW(err, msg, var1);
    }
    var2->vtable->func_27200_d(var2);

    if (func_27448_l(self, var1, &raw, err) != 0)
        return -1;
    char var4 = (char)raw;
    J_PositionTrackingPushbackReader_func_27334_a(var1, var4);
    if (var4 != 93) {
        if (func_27464_d(self, var1, var2, err) != 0)
            return -1;
    }

    int var5 = 0;
    while (!var5) {
        if (func_27448_l(self, var1, &raw, err) != 0)
            return -1;
        char var6 = (char)raw;
        switch (var6) {
        case ',':
            if (func_27464_d(self, var1, var2, err) != 0)
                return -1;
            break;
        case ']':
            var5 = 1;
            break;
        default: {
            char msg[256];
            snprintf(msg, sizeof(msg), "Expected either , or ] but got [%c].", var6);
            THROW(err, msg, var1);
        }
        }
    }

    var2->vtable->func_27197_e(var2);
    return 0;
}

static int func_27453_b(J_SajParser *self, J_PositionTrackingPushbackReader *var1,
                        J_JsonListener *var2, J_InvalidSyntaxException *err) {
    int raw;
    if (func_27448_l(self, var1, &raw, err) != 0)
        return -1;
    char var3 = (char)raw;
    if (var3 != 123) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected object to start with { but got [%c].", var3);
        THROW(err, msg, var1);
    }
    var2->vtable->func_27194_f(var2);

    if (func_27448_l(self, var1, &raw, err) != 0)
        return -1;
    char var4 = (char)raw;
    J_PositionTrackingPushbackReader_func_27334_a(var1, var4);
    if (var4 != 125) {
        if (func_27449_c(self, var1, var2, err) != 0)
            return -1;
    }

    int var5 = 0;
    while (!var5) {
        if (func_27448_l(self, var1, &raw, err) != 0)
            return -1;
        char var6 = (char)raw;
        switch (var6) {
        case ',':
            if (func_27449_c(self, var1, var2, err) != 0)
                return -1;
            break;
        case '}':
            var5 = 1;
            break;
        default: {
            char msg[256];
            snprintf(msg, sizeof(msg), "Expected either , or } but got [%c].", var6);
            THROW(err, msg, var1);
        }
        }
    }

    var2->vtable->func_27203_g(var2);
    return 0;
}

static int func_27449_c(J_SajParser *self, J_PositionTrackingPushbackReader *var1,
                        J_JsonListener *var2, J_InvalidSyntaxException *err) {
    int raw;
    if (func_27448_l(self, var1, &raw, err) != 0)
        return -1;
    char var3 = (char)raw;
    if (34 != (int)var3) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected object identifier to begin with [\"] but got [%c].",
                 var3);
        THROW(err, msg, var1);
    }
    J_PositionTrackingPushbackReader_func_27334_a(var1, var3);

    char key_buf[4096];
    if (func_27452_i(self, var1, key_buf, (int)sizeof(key_buf), err) != 0)
        return -1;
    var2->vtable->func_27205_a(var2, key_buf);

    if (func_27448_l(self, var1, &raw, err) != 0)
        return -1;
    char var4 = (char)raw;
    if (var4 != 58) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected object identifier to be followed by : but got [%c].",
                 var4);
        THROW(err, msg, var1);
    }
    if (func_27464_d(self, var1, var2, err) != 0)
        return -1;
    var2->vtable->func_27199_h(var2);
    return 0;
}

static int func_27464_d(J_SajParser *self, J_PositionTrackingPushbackReader *var1,
                        J_JsonListener *var2, J_InvalidSyntaxException *err) {
    int raw;
    if (func_27448_l(self, var1, &raw, err) != 0)
        return -1;
    char var3 = (char)raw;
    switch (var3) {
    case '"':
        J_PositionTrackingPushbackReader_func_27334_a(var1, var3);
        {
            char str_buf[4096];
            if (func_27452_i(self, var1, str_buf, (int)sizeof(str_buf), err) != 0)
                return -1;
            var2->vtable->func_27198_c(var2, str_buf);
        }
        break;
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        J_PositionTrackingPushbackReader_func_27334_a(var1, var3);
        {
            char num_buf[256];
            if (func_27459_a(self, var1, num_buf, (int)sizeof(num_buf), err) != 0)
                return -1;
            var2->vtable->func_27201_b(var2, num_buf);
        }
        break;
    case '[':
        J_PositionTrackingPushbackReader_func_27334_a(var1, var3);
        if (func_27455_a(self, var1, var2, err) != 0)
            return -1;
        break;
    case 'f': {
        char var6[4];
        int var7 = J_PositionTrackingPushbackReader_func_27336_b(var1, var6, 4);
        if (var7 != 4 || var6[0] != 97 || var6[1] != 108 || var6[2] != 115 || var6[3] != 101) {

            J_PositionTrackingPushbackReader_func_27335_a(var1, var6, var7);
            char msg[256];
            snprintf(msg, sizeof(msg),
                     "Expected 'f' to be followed by [[a, l, s, e]], but got [%c, %c, %c, %c].",
                     var6[0], var6[1], var6[2], var6[3]);
            THROW(err, msg, var1);
        }
        var2->vtable->func_27193_j(var2);
        break;
    }
    case 'n': {
        char var8[3];
        int var9 = J_PositionTrackingPushbackReader_func_27336_b(var1, var8, 3);
        if (var9 != 3 || var8[0] != 117 || var8[1] != 108 || var8[2] != 108) {
            J_PositionTrackingPushbackReader_func_27335_a(var1, var8, var9);
            char msg[256];
            snprintf(msg, sizeof(msg),
                     "Expected 'n' to be followed by [[u, l, l]], but got [%c, %c, %c].", var8[0],
                     var8[1], var8[2]);
            THROW(err, msg, var1);
        }
        var2->vtable->func_27202_k(var2);
        break;
    }
    case 't': {
        char var4[3];
        int var5 = J_PositionTrackingPushbackReader_func_27336_b(var1, var4, 3);
        if (var5 != 3 || var4[0] != 114 || var4[1] != 117 || var4[2] != 101) {
            J_PositionTrackingPushbackReader_func_27335_a(var1, var4, var5);
            char msg[256];
            snprintf(msg, sizeof(msg),
                     "Expected 't' to be followed by [[r, u, e]], but got [%c, %c, %c].", var4[0],
                     var4[1], var4[2]);
            THROW(err, msg, var1);
        }
        var2->vtable->func_27196_i(var2);
        break;
    }
    case '{':
        J_PositionTrackingPushbackReader_func_27334_a(var1, var3);
        if (func_27453_b(self, var1, var2, err) != 0)
            return -1;
        break;
    default: {
        char msg[256];
        snprintf(msg, sizeof(msg), "Invalid character at start of value [%c].", var3);
        THROW(err, msg, var1);
    }
    }
    return 0;
}

static int func_27459_a(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err) {
    out[0] = '\0';
    int pos = 0;

    int raw = J_PositionTrackingPushbackReader_func_27333_c(var1);
    char var3 = (char)raw;
    if (45 == (int)var3) {
        if (pos < out_cap - 1)
            out[pos++] = '-';
    } else {
        J_PositionTrackingPushbackReader_func_27334_a(var1, var3);
    }

    char tail[256];
    if (func_27451_b(self, var1, tail, (int)sizeof(tail), err) != 0)
        return -1;
    int tlen = (int)strlen(tail);
    if (pos + tlen < out_cap) {
        memcpy(out + pos, tail, (size_t)tlen);
        pos += tlen;
    }
    out[pos] = '\0';
    return 0;
}

static int func_27451_b(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err) {
    out[0] = '\0';
    int pos = 0;

    int raw = J_PositionTrackingPushbackReader_func_27333_c(var1);
    char var3 = (char)raw;

    char frac[64] = {0}, exp[64] = {0};

    if (48 == (int)var3) {
        if (pos < out_cap - 1)
            out[pos++] = '0';
        if (func_27462_f(self, var1, frac, (int)sizeof(frac), err) != 0)
            return -1;
        if (func_27454_g(self, var1, exp, (int)sizeof(exp), err) != 0)
            return -1;
    } else {
        J_PositionTrackingPushbackReader_func_27334_a(var1, var3);
        char digit1[4] = {0};
        if (func_27460_c(self, var1, &digit1[0], err) != 0)
            return -1;
        if (pos < out_cap - 1)
            out[pos++] = digit1[0];

        char more[64] = {0};
        if (func_27456_e(self, var1, more, (int)sizeof(more), err) != 0)
            return -1;
        if (func_27462_f(self, var1, frac, (int)sizeof(frac), err) != 0)
            return -1;
        if (func_27454_g(self, var1, exp, (int)sizeof(exp), err) != 0)
            return -1;

        int mlen = (int)strlen(more);
        if (pos + mlen < out_cap) {
            memcpy(out + pos, more, (size_t)mlen);
            pos += mlen;
        }
    }

    int flen = (int)strlen(frac), elen = (int)strlen(exp);
    if (pos + flen < out_cap) {
        memcpy(out + pos, frac, (size_t)flen);
        pos += flen;
    }
    if (pos + elen < out_cap) {
        memcpy(out + pos, exp, (size_t)elen);
        pos += elen;
    }
    out[pos] = '\0';
    return 0;
}

static int func_27460_c(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out_ch,
                        J_InvalidSyntaxException *err) {
    (void)self;
    int raw = J_PositionTrackingPushbackReader_func_27333_c(var1);
    char var3 = (char)raw;
    switch (var3) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        *out_ch = var3;
        return 0;
    default: {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected a digit 1 - 9 but got [%c].", var3);
        THROW(err, msg, var1);
    }
    }
}

static int func_27458_d(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out_ch,
                        J_InvalidSyntaxException *err) {
    (void)self;
    int raw = J_PositionTrackingPushbackReader_func_27333_c(var1);
    char var3 = (char)raw;
    switch (var3) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        *out_ch = var3;
        return 0;
    default: {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected a digit 1 - 9 but got [%c].", var3);
        THROW(err, msg, var1);
    }
    }
}

static int func_27456_e(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err) {
    (void)self;
    (void)err;
    int pos = 0;
    int var3 = 0;
    while (!var3) {
        int raw = J_PositionTrackingPushbackReader_func_27333_c(var1);
        char var4 = (char)raw;
        switch (var4) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (pos < out_cap - 1)
                out[pos++] = var4;
            break;
        default:
            var3 = 1;
            J_PositionTrackingPushbackReader_func_27334_a(var1, var4);
        }
    }
    out[pos] = '\0';
    return 0;
}

static int func_27462_f(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err) {
    int pos = 0;
    int raw = J_PositionTrackingPushbackReader_func_27333_c(var1);
    char var3 = (char)raw;
    if (var3 == 46) {
        if (pos < out_cap - 1)
            out[pos++] = '.';
        char d;
        if (func_27458_d(self, var1, &d, err) != 0)
            return -1;
        if (pos < out_cap - 1)
            out[pos++] = d;
        char more[64] = {0};
        if (func_27456_e(self, var1, more, (int)sizeof(more), err) != 0)
            return -1;
        int mlen = (int)strlen(more);
        if (pos + mlen < out_cap) {
            memcpy(out + pos, more, (size_t)mlen);
            pos += mlen;
        }
    } else {
        J_PositionTrackingPushbackReader_func_27334_a(var1, var3);
    }
    out[pos] = '\0';
    return 0;
}

static int func_27454_g(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err) {
    int pos = 0;
    int raw = J_PositionTrackingPushbackReader_func_27333_c(var1);
    char var3 = (char)raw;
    if (var3 != 46 && var3 != 69) {
        J_PositionTrackingPushbackReader_func_27334_a(var1, var3);
    } else {
        if (pos < out_cap - 1)
            out[pos++] = 'E';
        char sign[4] = {0};
        if (func_27461_h(self, var1, sign, (int)sizeof(sign), err) != 0)
            return -1;
        int slen = (int)strlen(sign);
        if (pos + slen < out_cap) {
            memcpy(out + pos, sign, (size_t)slen);
            pos += slen;
        }
        char d;
        if (func_27458_d(self, var1, &d, err) != 0)
            return -1;
        if (pos < out_cap - 1)
            out[pos++] = d;
        char more[64] = {0};
        if (func_27456_e(self, var1, more, (int)sizeof(more), err) != 0)
            return -1;
        int mlen = (int)strlen(more);
        if (pos + mlen < out_cap) {
            memcpy(out + pos, more, (size_t)mlen);
            pos += mlen;
        }
    }
    out[pos] = '\0';
    return 0;
}

static int func_27461_h(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err) {
    (void)self;
    (void)err;
    int pos = 0;
    int raw = J_PositionTrackingPushbackReader_func_27333_c(var1);
    char var3 = (char)raw;
    if (var3 != 43 && var3 != 45) {
        J_PositionTrackingPushbackReader_func_27334_a(var1, var3);
    } else {
        if (pos < out_cap - 1)
            out[pos++] = var3;
    }
    out[pos] = '\0';
    return 0;
}

static int func_27452_i(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out,
                        int out_cap, J_InvalidSyntaxException *err) {
    int raw = J_PositionTrackingPushbackReader_func_27333_c(var1);
    char var3 = (char)raw;
    if (34 != (int)var3) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected [\"] but got [%c].", var3);
        THROW(err, msg, var1);
    }

    int pos = 0;
    int var4 = 0;
    while (!var4) {
        int raw2 = J_PositionTrackingPushbackReader_func_27333_c(var1);
        char var5 = (char)raw2;
        switch (var5) {
        case '"':
            var4 = 1;
            break;
        case '\\': {
            char var6[4] = {0};
            if (func_27457_j(self, var1, var6, err) != 0)
                return -1;
            for (int bi = 0; var6[bi] && pos < out_cap - 1; bi++)
                out[pos++] = var6[bi];
            break;
        }
        default:
            if (pos < out_cap - 1)
                out[pos++] = var5;
        }
    }
    out[pos] = '\0';
    return 0;
}

static int func_27457_j(J_SajParser *self, J_PositionTrackingPushbackReader *var1, char *out_ch,
                        J_InvalidSyntaxException *err) {
    (void)self;
    int raw = J_PositionTrackingPushbackReader_func_27333_c(var1);
    char var3 = (char)raw;
    char var2;
    switch (var3) {
    case '"':
        var2 = 34;
        break;
    case '/':
        var2 = 47;
        break;
    case '\\':
        var2 = 92;
        break;
    case 'b':
        var2 = 8;
        break;
    case 'f':
        var2 = 12;
        break;
    case 'n':
        var2 = 10;
        break;
    case 'r':
        var2 = 13;
        break;
    case 't':
        var2 = 9;
        break;
    case 'u': {

        int codepoint;
        if (func_27450_k(self, var1, &codepoint, err) != 0)
            return -1;

        if (codepoint < 0x80) {
            var2 = (char)codepoint;
        } else if (codepoint < 0x800) {
            out_ch[0] = (char)(0xC0 | (codepoint >> 6));
            out_ch[1] = (char)(0x80 | (codepoint & 0x3F));
            out_ch[2] = '\0';
            return 0;
        } else {
            out_ch[0] = (char)(0xE0 | (codepoint >> 12));
            out_ch[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
            out_ch[2] = (char)(0x80 | (codepoint & 0x3F));
            out_ch[3] = '\0';
            return 0;
        }
        break;
    }
    default: {
        char msg[256];
        snprintf(msg, sizeof(msg), "Unrecognised escape character [%c].", var3);
        THROW(err, msg, var1);
    }
    }
    *out_ch = var2;
    return 0;
}

static int func_27450_k(J_SajParser *self, J_PositionTrackingPushbackReader *var1,
                        int *out_codepoint, J_InvalidSyntaxException *err) {
    (void)self;
    char var2[4];
    int var3 = J_PositionTrackingPushbackReader_func_27336_b(var1, var2, 4);
    if (var3 != 4) {
        char msg[256];
        snprintf(msg, sizeof(msg),
                 "Expected a 4 digit hexidecimal number but got only [%d], namely [%.*s].", var3,
                 var3, var2);
        THROW(err, msg, var1);
    }

    char hex[5];
    memcpy(hex, var2, 4);
    hex[4] = '\0';
    char *end;
    long val = strtol(hex, &end, 16);
    if (end != hex + 4) {

        J_PositionTrackingPushbackReader_func_27335_a(var1, var2, 4);
        char msg[256];
        snprintf(msg, sizeof(msg), "Unable to parse [%s] as a hexidecimal number.", hex);
        THROW(err, msg, var1);
    }
    *out_codepoint = (int)val;
    return 0;
}

static int func_27448_l(J_SajParser *self, J_PositionTrackingPushbackReader *var1, int *out_ch,
                        J_InvalidSyntaxException *err) {
    (void)self;
    (void)err;
    int var3 = 0;
    int var2 = -1;
    do {
        var2 = J_PositionTrackingPushbackReader_func_27333_c(var1);
        switch (var2) {
        case 9:
        case 10:
        case 13:
        case 32:
            break;
        default:
            var3 = 1;
        }
    } while (!var3);
    *out_ch = var2;
    return 0;
}
