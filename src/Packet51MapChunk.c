#include "Packet51MapChunk.h"
#include "NetHandler.h"
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static Packet_vtable vtable = {
    Packet51MapChunk_readPacketData,
    Packet51MapChunk_writePacketData,
    Packet51MapChunk_processPacket,
    Packet51MapChunk_getPacketSize,
};

Packet *Packet51MapChunk_create(void) {
    Packet51MapChunk *self = calloc(1, sizeof(Packet51MapChunk));
    self->base.vtable = &vtable;
    self->base.packetId = 51;

    self->base.isChunkDataPacket = 1;
    return (Packet *)self;
}

void Packet51MapChunk_readPacketData(Packet *p, DataStream *var1) {
    Packet51MapChunk *self = (Packet51MapChunk *)p;
    self->xPosition = DataStream_readInt(var1);

    self->yPosition = (int)DataStream_readShort(var1);
    self->zPosition = DataStream_readInt(var1);

    self->xSize = (int)DataStream_readByte(var1) + 1;
    self->ySize = (int)DataStream_readByte(var1) + 1;
    self->zSize = (int)DataStream_readByte(var1) + 1;

    self->chunkSize = DataStream_readInt(var1);

    uint8_t *var2 = malloc((size_t)self->chunkSize);
    DataStream_readBytes(var1, var2, (size_t)self->chunkSize);

    uLongf destLen = (uLongf)(self->xSize * self->ySize * self->zSize * 5 / 2);
    self->chunk = malloc(destLen);

    int ret = uncompress(self->chunk, &destLen, var2, (uLong)self->chunkSize);
    free(var2);
    if (ret != Z_OK) {

        var1->error = 1;
    }
}

void Packet51MapChunk_writePacketData(Packet *p, DataStream *var1) {
    Packet51MapChunk *self = (Packet51MapChunk *)p;
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeShort(var1, (int16_t)self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
    DataStream_writeByte(var1, (uint8_t)(self->xSize - 1));
    DataStream_writeByte(var1, (uint8_t)(self->ySize - 1));
    DataStream_writeByte(var1, (uint8_t)(self->zSize - 1));
    DataStream_writeInt(var1, self->chunkSize);
    DataStream_writeBytes(var1, self->chunk, (size_t)self->chunkSize);
}

void Packet51MapChunk_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleMapChunk(var1, (Packet51MapChunk *)p);
}

int Packet51MapChunk_getPacketSize(Packet *p) { return 17 + ((Packet51MapChunk *)p)->chunkSize; }
