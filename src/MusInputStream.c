#include "MusInputStream.h"
#include <string.h>

void MusInputStream_init(MusInputStream *self, CodecMus *var1, const char *url_path, FILE *var3) {
    self->codec = var1;
    self->buffer[0] = 0;
    self->inputStream = var3;

    const char *var4 = url_path;

    const char *slash = strrchr(var4, '/');
    if (slash != NULL) {
        var4 = slash + 1;
    }

    int hash = 0;
    for (const char *p = var4; *p != '\0'; ++p) {
        hash = hash * 31 + (unsigned char)*p;
    }
    self->hash = hash;
}

int MusInputStream_read(MusInputStream *self) {
    int var1 = MusInputStream_readBuf(self, self->buffer, 0, 1);
    return var1 < 0 ? var1 : (int)self->buffer[0];
}

int MusInputStream_readBuf(MusInputStream *self, uint8_t *var1, int var2, int var3) {

    int read = (int)fread(var1 + var2, 1, (size_t)var3, self->inputStream);
    if (read == 0 && feof(self->inputStream)) {
        return -1;
    }
    var3 = read;

    for (int var4 = 0; var4 < var3; ++var4) {

        uint8_t var5 = var1[var2 + var4] = (uint8_t)(var1[var2 + var4] ^ (self->hash >> 8));

        self->hash = self->hash * 498729871 + 85731 * (int8_t)var5;
    }

    return var3;
}
