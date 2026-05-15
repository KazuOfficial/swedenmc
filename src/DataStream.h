#ifndef DATA_STREAM_H
#define DATA_STREAM_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    FILE *file;
    uint8_t *buf;
    size_t pos;
    size_t len;
    int error;
    int growable;
} DataStream;

DataStream DataStream_fromFile(FILE *f);
DataStream DataStream_fromBuffer(uint8_t *buf, size_t len);

DataStream DataStream_growable(size_t initialCap);

void DataStream_writeByte(DataStream *s, uint8_t v);
uint8_t DataStream_readByte(DataStream *s);
void DataStream_writeBytes(DataStream *s, const uint8_t *data, size_t len);
void DataStream_readBytes(DataStream *s, uint8_t *out, size_t len);

void DataStream_writeShort(DataStream *s, int16_t v);
int16_t DataStream_readShort(DataStream *s);
void DataStream_writeInt(DataStream *s, int32_t v);
int32_t DataStream_readInt(DataStream *s);
void DataStream_writeLong(DataStream *s, int64_t v);
int64_t DataStream_readLong(DataStream *s);
void DataStream_writeFloat(DataStream *s, float v);
float DataStream_readFloat(DataStream *s);
void DataStream_writeDouble(DataStream *s, double v);
double DataStream_readDouble(DataStream *s);

void DataStream_writeUTF(DataStream *s, const char *str);
char *DataStream_readUTF(DataStream *s);

#endif
