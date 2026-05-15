#include "Packet130UpdateSign.h"
#include "NetHandler.h"
#include <stdlib.h>
#include <string.h>

static Packet_vtable vtable = {
    Packet130UpdateSign_readPacketData,
    Packet130UpdateSign_writePacketData,
    Packet130UpdateSign_processPacket,
    Packet130UpdateSign_getPacketSize,
};

Packet *Packet130UpdateSign_create(void) {
    Packet130UpdateSign *self = calloc(1, sizeof(Packet130UpdateSign));
    self->base.vtable = &vtable;
    self->base.packetId = 130;

    self->base.isChunkDataPacket = 1;
    return (Packet *)self;
}

Packet *Packet130UpdateSign_createWith(int x, int y, int z, char *lines[4]) {
    Packet130UpdateSign *self = (Packet130UpdateSign *)Packet130UpdateSign_create();
    self->xPosition = x;
    self->yPosition = y;
    self->zPosition = z;
    for (int i = 0; i < 4; ++i) {
        self->signLines[i] = lines[i];
    }
    return (Packet *)self;
}

void Packet130UpdateSign_readPacketData(Packet *p, DataStream *var1) {
    Packet130UpdateSign *self = (Packet130UpdateSign *)p;
    self->xPosition = DataStream_readInt(var1);

    self->yPosition = (int)DataStream_readShort(var1);
    self->zPosition = DataStream_readInt(var1);

    for (int var2 = 0; var2 < 4; ++var2) {
        self->signLines[var2] = Packet_readString(var1, 15);
    }
}

void Packet130UpdateSign_writePacketData(Packet *p, DataStream *var1) {
    Packet130UpdateSign *self = (Packet130UpdateSign *)p;
    DataStream_writeInt(var1, self->xPosition);
    DataStream_writeShort(var1, (int16_t)self->yPosition);
    DataStream_writeInt(var1, self->zPosition);
    for (int var2 = 0; var2 < 4; ++var2) {
        Packet_writeString(self->signLines[var2], var1);
    }
}

void Packet130UpdateSign_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleSignUpdate(var1, (Packet130UpdateSign *)p);
}

int Packet130UpdateSign_getPacketSize(Packet *p) {
    Packet130UpdateSign *self = (Packet130UpdateSign *)p;
    int var1 = 0;
    for (int var2 = 0; var2 < 4; ++var2) {
        var1 += (int)strlen(self->signLines[var2]);
    }
    return var1;
}
