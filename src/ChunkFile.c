#include "ChunkFile.h"
#include "ChunkFilePattern.h"
#include <stdlib.h>
#include <string.h>

ChunkFile *ChunkFile_create(const char *path) {
    ChunkFile *self = (ChunkFile *)calloc(1, sizeof(ChunkFile));
    self->field_22326_a = strdup(path);

    const char *name = strrchr(path, '/');
    name = (name != NULL) ? name + 1 : path;
    int cx = 0, cz = 0;
    ChunkFilePattern_parse(name, &cx, &cz);
    self->field_22325_b = cx;
    self->field_22327_c = cz;
    return self;
}

void ChunkFile_destroy(ChunkFile *self) {
    free(self->field_22326_a);
    free(self);
}

int ChunkFile_func_22322_a(const ChunkFile *self, const ChunkFile *var1) {
    int var2 = self->field_22325_b >> 5;
    int var3 = var1->field_22325_b >> 5;
    if (var2 == var3) {
        int var4 = self->field_22327_c >> 5;
        int var5 = var1->field_22327_c >> 5;
        return var4 - var5;
    } else {
        return var2 - var3;
    }
}

char *ChunkFile_func_22324_a(const ChunkFile *self) { return self->field_22326_a; }

int ChunkFile_func_22323_b(const ChunkFile *self) { return self->field_22325_b; }

int ChunkFile_func_22321_c(const ChunkFile *self) { return self->field_22327_c; }

int ChunkFile_compareTo(const ChunkFile *self, void *var1) {
    return ChunkFile_func_22322_a(self, (const ChunkFile *)var1);
}
