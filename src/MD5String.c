#include "MD5String.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int md5_u32;
typedef unsigned char md5_u8;

typedef struct {
    md5_u32 state[4];
    md5_u32 count[2];
    md5_u8 buffer[64];
} MD5_CTX;

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define F(x, y, z) (((x) & (y)) | ((~(x)) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~(z))))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~(z))))
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define FF(a, b, c, d, x, s, ac)                                                                   \
    {                                                                                              \
        (a) += F((b), (c), (d)) + (x) + (md5_u32)(ac);                                             \
        (a) = ROTATE_LEFT((a), (s));                                                               \
        (a) += (b);                                                                                \
    }
#define GG(a, b, c, d, x, s, ac)                                                                   \
    {                                                                                              \
        (a) += G((b), (c), (d)) + (x) + (md5_u32)(ac);                                             \
        (a) = ROTATE_LEFT((a), (s));                                                               \
        (a) += (b);                                                                                \
    }
#define HH(a, b, c, d, x, s, ac)                                                                   \
    {                                                                                              \
        (a) += H((b), (c), (d)) + (x) + (md5_u32)(ac);                                             \
        (a) = ROTATE_LEFT((a), (s));                                                               \
        (a) += (b);                                                                                \
    }
#define II(a, b, c, d, x, s, ac)                                                                   \
    {                                                                                              \
        (a) += I((b), (c), (d)) + (x) + (md5_u32)(ac);                                             \
        (a) = ROTATE_LEFT((a), (s));                                                               \
        (a) += (b);                                                                                \
    }

static void md5_transform(md5_u32 state[4], const md5_u8 block[64]) {
    md5_u32 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    for (int i = 0, j = 0; i < 16; i++, j += 4)
        x[i] = ((md5_u32)block[j]) | ((md5_u32)block[j + 1] << 8) | ((md5_u32)block[j + 2] << 16) |
               ((md5_u32)block[j + 3] << 24);
    FF(a, b, c, d, x[0], S11, 0xd76aa478);
    FF(d, a, b, c, x[1], S12, 0xe8c7b756);
    FF(c, d, a, b, x[2], S13, 0x242070db);
    FF(b, c, d, a, x[3], S14, 0xc1bdceee);
    FF(a, b, c, d, x[4], S11, 0xf57c0faf);
    FF(d, a, b, c, x[5], S12, 0x4787c62a);
    FF(c, d, a, b, x[6], S13, 0xa8304613);
    FF(b, c, d, a, x[7], S14, 0xfd469501);
    FF(a, b, c, d, x[8], S11, 0x698098d8);
    FF(d, a, b, c, x[9], S12, 0x8b44f7af);
    FF(c, d, a, b, x[10], S13, 0xffff5bb1);
    FF(b, c, d, a, x[11], S14, 0x895cd7be);
    FF(a, b, c, d, x[12], S11, 0x6b901122);
    FF(d, a, b, c, x[13], S12, 0xfd987193);
    FF(c, d, a, b, x[14], S13, 0xa679438e);
    FF(b, c, d, a, x[15], S14, 0x49b40821);
    GG(a, b, c, d, x[1], S21, 0xf61e2562);
    GG(d, a, b, c, x[6], S22, 0xc040b340);
    GG(c, d, a, b, x[11], S23, 0x265e5a51);
    GG(b, c, d, a, x[0], S24, 0xe9b6c7aa);
    GG(a, b, c, d, x[5], S21, 0xd62f105d);
    GG(d, a, b, c, x[10], S22, 0x02441453);
    GG(c, d, a, b, x[15], S23, 0xd8a1e681);
    GG(b, c, d, a, x[4], S24, 0xe7d3fbc8);
    GG(a, b, c, d, x[9], S21, 0x21e1cde6);
    GG(d, a, b, c, x[14], S22, 0xc33707d6);
    GG(c, d, a, b, x[3], S23, 0xf4d50d87);
    GG(b, c, d, a, x[8], S24, 0x455a14ed);
    GG(a, b, c, d, x[13], S21, 0xa9e3e905);
    GG(d, a, b, c, x[2], S22, 0xfcefa3f8);
    GG(c, d, a, b, x[7], S23, 0x676f02d9);
    GG(b, c, d, a, x[12], S24, 0x8d2a4c8a);
    HH(a, b, c, d, x[5], S31, 0xfffa3942);
    HH(d, a, b, c, x[8], S32, 0x8771f681);
    HH(c, d, a, b, x[11], S33, 0x6d9d6122);
    HH(b, c, d, a, x[14], S34, 0xfde5380c);
    HH(a, b, c, d, x[1], S31, 0xa4beea44);
    HH(d, a, b, c, x[4], S32, 0x4bdecfa9);
    HH(c, d, a, b, x[7], S33, 0xf6bb4b60);
    HH(b, c, d, a, x[10], S34, 0xbebfbc70);
    HH(a, b, c, d, x[13], S31, 0x289b7ec6);
    HH(d, a, b, c, x[0], S32, 0xeaa127fa);
    HH(c, d, a, b, x[3], S33, 0xd4ef3085);
    HH(b, c, d, a, x[6], S34, 0x04881d05);
    HH(a, b, c, d, x[9], S31, 0xd9d4d039);
    HH(d, a, b, c, x[12], S32, 0xe6db99e5);
    HH(c, d, a, b, x[15], S33, 0x1fa27cf8);
    HH(b, c, d, a, x[2], S34, 0xc4ac5665);
    II(a, b, c, d, x[0], S41, 0xf4292244);
    II(d, a, b, c, x[7], S42, 0x432aff97);
    II(c, d, a, b, x[14], S43, 0xab9423a7);
    II(b, c, d, a, x[5], S44, 0xfc93a039);
    II(a, b, c, d, x[12], S41, 0x655b59c3);
    II(d, a, b, c, x[3], S42, 0x8f0ccc92);
    II(c, d, a, b, x[10], S43, 0xffeff47d);
    II(b, c, d, a, x[1], S44, 0x85845dd1);
    II(a, b, c, d, x[8], S41, 0x6fa87e4f);
    II(d, a, b, c, x[15], S42, 0xfe2ce6e0);
    II(c, d, a, b, x[6], S43, 0xa3014314);
    II(b, c, d, a, x[13], S44, 0x4e0811a1);
    II(a, b, c, d, x[4], S41, 0xf7537e82);
    II(d, a, b, c, x[11], S42, 0xbd3af235);
    II(c, d, a, b, x[2], S43, 0x2ad7d2bb);
    II(b, c, d, a, x[9], S44, 0xeb86d391);
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}
static void md5_init(MD5_CTX *ctx) {
    ctx->count[0] = ctx->count[1] = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
}
static void md5_update(MD5_CTX *ctx, const md5_u8 *input, unsigned int len) {
    unsigned int i, index, partLen;
    index = (unsigned int)((ctx->count[0] >> 3) & 0x3F);
    if ((ctx->count[0] += ((md5_u32)len << 3)) < ((md5_u32)len << 3))
        ctx->count[1]++;
    ctx->count[1] += ((md5_u32)len >> 29);
    partLen = 64 - index;
    if (len >= partLen) {
        memcpy(&ctx->buffer[index], input, partLen);
        md5_transform(ctx->state, ctx->buffer);
        for (i = partLen; i + 63 < len; i += 64)
            md5_transform(ctx->state, &input[i]);
        index = 0;
    } else {
        i = 0;
    }
    memcpy(&ctx->buffer[index], &input[i], len - i);
}
static void md5_final(md5_u8 digest[16], MD5_CTX *ctx) {
    static const md5_u8 PADDING[64] = {0x80};
    md5_u8 bits[8];
    unsigned int index, padLen;
    for (int i = 0; i < 8; i++)
        bits[i] = (md5_u8)((ctx->count[i >> 2] >> ((i & 3) << 3)) & 0xff);
    index = (unsigned int)((ctx->count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    md5_update(ctx, PADDING, padLen);
    md5_update(ctx, bits, 8);
    for (int i = 0; i < 16; i++)
        digest[i] = (md5_u8)((ctx->state[i >> 2] >> ((i & 3) << 3)) & 0xff);
}

void MD5String_init(MD5String *self, const char *var1) { self->field_27370_a = var1; }

char *MD5String_func_27369_a(MD5String *self, const char *var1, char *out_buf) {

    size_t la = strlen(self->field_27370_a);
    size_t lb = strlen(var1);
    char *var2 = (char *)malloc(la + lb + 1);
    memcpy(var2, self->field_27370_a, la);
    memcpy(var2 + la, var1, lb);
    var2[la + lb] = '\0';

    MD5_CTX var3;
    md5_init(&var3);
    md5_update(&var3, (const md5_u8 *)var2, (unsigned int)(la + lb));
    free(var2);

    md5_u8 digest[16];
    md5_final(digest, &var3);

    int start = 0;
    while (start < 16 && digest[start] == 0)
        ++start;
    if (start == 16) {
        out_buf[0] = '0';
        out_buf[1] = '\0';
        return out_buf;
    }
    char *p = out_buf;
    for (int i = start; i < 16; i++) {
        if (i == start && (digest[i] >> 4) == 0) {
            p += sprintf(p, "%x", digest[i] & 0xF);
        } else {
            p += sprintf(p, "%02x", digest[i]);
        }
    }
    return out_buf;
}
