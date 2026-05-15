#include "ChunkBlockMap.h"
#include "Block.h"
#include <stddef.h>

uint8_t ChunkBlockMap_field_26003_a[256];

void ChunkBlockMap_init(void) {
    for (int var0 = 0; var0 < 256; ++var0) {
        uint8_t var1 = (uint8_t)var0;
        if (var1 != 0 && Block_blocksList[var1 & 255] == NULL) {
            var1 = 0;
        }
        ChunkBlockMap_field_26003_a[var0] = var1;
    }
}

void ChunkBlockMap_func_26002_a(uint8_t *var0, int length) {
    for (int var1 = 0; var1 < length; ++var1) {
        var0[var1] = ChunkBlockMap_field_26003_a[var0[var1] & 255];
    }
}
