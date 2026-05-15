#include "Packet60Explosion.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet60Explosion_readPacketData,
    Packet60Explosion_writePacketData,
    Packet60Explosion_processPacket,
    Packet60Explosion_getPacketSize,
};

Packet *Packet60Explosion_create(void) {
    Packet60Explosion *self = calloc(1, sizeof(Packet60Explosion));
    self->base.vtable = &vtable;
    self->base.packetId = 60;
    return (Packet *)self;
}

void Packet60Explosion_readPacketData(Packet *p, DataStream *var1) {
    Packet60Explosion *self = (Packet60Explosion *)p;
    self->explosionX = DataStream_readDouble(var1);
    self->explosionY = DataStream_readDouble(var1);
    self->explosionZ = DataStream_readDouble(var1);
    self->explosionSize = DataStream_readFloat(var1);

    int var2 = DataStream_readInt(var1);

    self->destroyedBlocks = malloc((size_t)var2 * sizeof(ChunkPosition));
    self->destroyedBlockCount = var2;
    int var3 = (int)self->explosionX;
    int var4 = (int)self->explosionY;
    int var5 = (int)self->explosionZ;
    for (int var6 = 0; var6 < var2; ++var6) {

        int var7 = (int)(int8_t)DataStream_readByte(var1) + var3;
        int var8 = (int)(int8_t)DataStream_readByte(var1) + var4;
        int var9 = (int)(int8_t)DataStream_readByte(var1) + var5;

        self->destroyedBlocks[var6].x = var7;
        self->destroyedBlocks[var6].y = var8;
        self->destroyedBlocks[var6].z = var9;
    }
}

void Packet60Explosion_writePacketData(Packet *p, DataStream *var1) {
    Packet60Explosion *self = (Packet60Explosion *)p;
    DataStream_writeDouble(var1, self->explosionX);
    DataStream_writeDouble(var1, self->explosionY);
    DataStream_writeDouble(var1, self->explosionZ);
    DataStream_writeFloat(var1, self->explosionSize);
    DataStream_writeInt(var1, self->destroyedBlockCount);
    int var2 = (int)self->explosionX;
    int var3 = (int)self->explosionY;
    int var4 = (int)self->explosionZ;
    for (int i = 0; i < self->destroyedBlockCount; ++i) {
        int var7 = self->destroyedBlocks[i].x - var2;
        int var8 = self->destroyedBlocks[i].y - var3;
        int var9 = self->destroyedBlocks[i].z - var4;
        DataStream_writeByte(var1, (uint8_t)var7);
        DataStream_writeByte(var1, (uint8_t)var8);
        DataStream_writeByte(var1, (uint8_t)var9);
    }
}

void Packet60Explosion_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_12245_a(var1, (Packet60Explosion *)p);
}

int Packet60Explosion_getPacketSize(Packet *p) {
    return 32 + ((Packet60Explosion *)p)->destroyedBlockCount * 3;
}
