#include "DataStream.h"
#include <string.h>
#include <stdlib.h>

DataStream DataStream_fromFile(FILE *f) {
    DataStream s;
    s.file = f;
    s.buf = NULL;
    s.pos = 0;
    s.len = 0;
    s.error = 0;
    s.growable = 0;
    return s;
}

DataStream DataStream_fromBuffer(uint8_t *buf, size_t len) {
    DataStream s;
    s.file = NULL;
    s.buf = buf;
    s.pos = 0;
    s.len = len;
    s.error = 0;
    s.growable = 0;
    return s;
}

DataStream DataStream_growable(size_t initialCap) {
    DataStream s;
    s.file = NULL;
    s.buf = (uint8_t *)malloc(initialCap);
    s.pos = 0;
    s.len = initialCap;
    s.error = 0;
    s.growable = 1;
    return s;
}

void DataStream_writeByte(DataStream *s, uint8_t v) {
    if (s->error)
        return;
    if (s->file) {
        if (fputc(v, s->file) == EOF)
            s->error = 1;
    } else {
        if (s->pos >= s->len) {
            if (s->growable) {
                s->len = s->len * 2 + 64;
                s->buf = (uint8_t *)realloc(s->buf, s->len);
            } else {
                s->error = 1;
                return;
            }
        }
        s->buf[s->pos++] = v;
    }
}

uint8_t DataStream_readByte(DataStream *s) {
    if (s->error)
        return 0;
    if (s->file) {
        int c = fgetc(s->file);
        if (c == EOF) {
            s->error = 1;
            return 0;
        }
        return (uint8_t)c;
    } else {
        if (s->pos >= s->len) {
            s->error = 1;
            return 0;
        }
        return s->buf[s->pos++];
    }
}

void DataStream_writeBytes(DataStream *s, const uint8_t *data, size_t len) {
    if (s->error)
        return;
    if (s->file) {
        if (fwrite(data, 1, len, s->file) != len)
            s->error = 1;
    } else {
        if (s->pos + len > s->len) {
            if (s->growable) {
                size_t newlen = s->len * 2 + len + 64;
                s->buf = (uint8_t *)realloc(s->buf, newlen);
                s->len = newlen;
            } else {
                s->error = 1;
                return;
            }
        }
        memcpy(s->buf + s->pos, data, len);
        s->pos += len;
    }
}

void DataStream_readBytes(DataStream *s, uint8_t *out, size_t len) {
    if (s->error)
        return;
    if (s->file) {
        if (fread(out, 1, len, s->file) != len)
            s->error = 1;
    } else {
        if (s->pos + len <= s->len) {
            memcpy(out, s->buf + s->pos, len);
            s->pos += len;
        } else
            s->error = 1;
    }
}

void DataStream_writeShort(DataStream *s, int16_t v) {
    DataStream_writeByte(s, (uint8_t)((v >> 8) & 0xff));
    DataStream_writeByte(s, (uint8_t)(v & 0xff));
}

int16_t DataStream_readShort(DataStream *s) {
    uint8_t hi = DataStream_readByte(s);
    uint8_t lo = DataStream_readByte(s);
    return (int16_t)((hi << 8) | lo);
}

void DataStream_writeInt(DataStream *s, int32_t v) {
    DataStream_writeByte(s, (uint8_t)((v >> 24) & 0xff));
    DataStream_writeByte(s, (uint8_t)((v >> 16) & 0xff));
    DataStream_writeByte(s, (uint8_t)((v >> 8) & 0xff));
    DataStream_writeByte(s, (uint8_t)(v & 0xff));
}

int32_t DataStream_readInt(DataStream *s) {
    uint8_t b0 = DataStream_readByte(s);
    uint8_t b1 = DataStream_readByte(s);
    uint8_t b2 = DataStream_readByte(s);
    uint8_t b3 = DataStream_readByte(s);
    return (int32_t)(((uint32_t)b0 << 24) | ((uint32_t)b1 << 16) | ((uint32_t)b2 << 8) |
                     (uint32_t)b3);
}

void DataStream_writeLong(DataStream *s, int64_t v) {
    DataStream_writeInt(s, (int32_t)(v >> 32));
    DataStream_writeInt(s, (int32_t)(v & 0xffffffff));
}

int64_t DataStream_readLong(DataStream *s) {
    int32_t hi = DataStream_readInt(s);
    int32_t lo = DataStream_readInt(s);
    return (int64_t)(((uint64_t)(uint32_t)hi << 32) | (uint64_t)(uint32_t)lo);
}

void DataStream_writeFloat(DataStream *s, float v) {
    uint32_t bits;
    memcpy(&bits, &v, 4);
    DataStream_writeInt(s, (int32_t)bits);
}

float DataStream_readFloat(DataStream *s) {
    int32_t bits = DataStream_readInt(s);
    float v;
    memcpy(&v, &bits, 4);
    return v;
}

void DataStream_writeDouble(DataStream *s, double v) {
    uint64_t bits;
    memcpy(&bits, &v, 8);
    DataStream_writeLong(s, (int64_t)bits);
}

double DataStream_readDouble(DataStream *s) {
    int64_t bits = DataStream_readLong(s);
    double v;
    memcpy(&v, &bits, 8);
    return v;
}

void DataStream_writeUTF(DataStream *s, const char *str) {
    uint16_t len = (uint16_t)(str ? strlen(str) : 0);
    DataStream_writeShort(s, (int16_t)len);
    if (len > 0)
        DataStream_writeBytes(s, (const uint8_t *)str, len);
}

char *DataStream_readUTF(DataStream *s) {
    int16_t len = DataStream_readShort(s);
    if (s->error || len < 0)
        return NULL;
    char *str = (char *)malloc((size_t)len + 1);
    DataStream_readBytes(s, (uint8_t *)str, (size_t)len);
    str[len] = '\0';
    return str;
}
