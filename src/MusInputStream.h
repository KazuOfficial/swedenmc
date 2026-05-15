#ifndef MUS_INPUT_STREAM_H
#define MUS_INPUT_STREAM_H

#include "CodecMus.h"
#include <stdint.h>
#include <stdio.h>

typedef struct MusInputStream {

    int hash;

    FILE *inputStream;

    uint8_t buffer[1];

    CodecMus *codec;
} MusInputStream;

void MusInputStream_init(MusInputStream *self, CodecMus *var1, const char *url_path, FILE *var3);

int MusInputStream_read(MusInputStream *self);

int MusInputStream_readBuf(MusInputStream *self, uint8_t *var1, int var2, int var3);

#endif
