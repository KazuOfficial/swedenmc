#include "ChunkFilePattern.h"
#include <string.h>
#include <stdlib.h>

void ChunkFilePattern_init(void) {}

static void ChunkFilePattern_construct_private(ChunkFilePattern *self) { (void)self; }

void ChunkFilePattern_construct(ChunkFilePattern *self, Empty2 *var1) {
    (void)var1;
    ChunkFilePattern_construct_private(self);
}

static int parse_base36(const char *s, const char *end, int *out) {
    if (s >= end)
        return 0;
    int negative = 0;
    if (*s == '-') {
        negative = 1;
        ++s;
    }
    if (s >= end)
        return 0;
    long val = 0;
    while (s < end) {
        char c = *s++;
        int digit;
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'a' && c <= 'z')
            digit = c - 'a' + 10;
        else
            return 0;
        val = val * 36 + digit;
    }
    *out = (int)(negative ? -val : val);
    return 1;
}

int ChunkFilePattern_parse(const char *name, int *chunkX, int *chunkZ) {
    if (name[0] != 'c' || name[1] != '.')
        return 0;
    const char *p = name + 2;

    const char *xStart = p;
    if (*p == '-')
        ++p;
    if (!((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'z')))
        return 0;
    while ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'z'))
        ++p;
    const char *xEnd = p;

    if (*p != '.')
        return 0;
    ++p;

    const char *zStart = p;
    if (*p == '-')
        ++p;
    if (!((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'z')))
        return 0;
    while ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'z'))
        ++p;
    const char *zEnd = p;

    if (strcmp(p, ".dat") != 0)
        return 0;

    if (!parse_base36(xStart, xEnd, chunkX))
        return 0;
    if (!parse_base36(zStart, zEnd, chunkZ))
        return 0;
    return 1;
}

int ChunkFilePattern_accept(const char *name) {
    int cx, cz;
    return ChunkFilePattern_parse(name, &cx, &cz);
}
