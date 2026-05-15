#include "ChunkFolderPattern.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

static void ChunkFolderPattern_construct_private(ChunkFolderPattern *self) { (void)self; }

void ChunkFolderPattern_construct(ChunkFolderPattern *self, struct Empty2 *var1) {
    (void)var1;
    ChunkFolderPattern_construct_private(self);
}

int ChunkFolderPattern_accept(ChunkFolderPattern *self, const char *var1) {
    (void)self;

    struct stat st;
    if (stat(var1, &st) != 0 || !S_ISDIR(st.st_mode)) {
        return 0;
    }

    const char *name = strrchr(var1, '/');
    name = (name != NULL) ? name + 1 : var1;

    size_t len = strlen(name);
    if (len < 1 || len > 2)
        return 0;
    for (size_t i = 0; i < len; i++) {
        char c = name[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')))
            return 0;
    }
    return 1;
}

void ChunkFolderPattern_init(void) {}
