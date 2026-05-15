#include "Packet100OpenWindow.h"
#include "NetHandler.h"
#include <stdlib.h>
#include <string.h>

static Packet_vtable vtable = {
    Packet100OpenWindow_readPacketData,
    Packet100OpenWindow_writePacketData,
    Packet100OpenWindow_processPacket,
    Packet100OpenWindow_getPacketSize,
};

Packet *Packet100OpenWindow_create(void) {
    Packet100OpenWindow *self = calloc(1, sizeof(Packet100OpenWindow));
    self->base.vtable = &vtable;
    self->base.packetId = 100;
    return (Packet *)self;
}

void Packet100OpenWindow_readPacketData(Packet *p, DataStream *var1) {
    Packet100OpenWindow *self = (Packet100OpenWindow *)p;

    self->windowId = (int)(int8_t)DataStream_readByte(var1);
    self->inventoryType = (int)(int8_t)DataStream_readByte(var1);

    self->windowTitle = DataStream_readUTF(var1);
    self->slotsCount = (int)(int8_t)DataStream_readByte(var1);
}

void Packet100OpenWindow_writePacketData(Packet *p, DataStream *var1) {
    Packet100OpenWindow *self = (Packet100OpenWindow *)p;
    DataStream_writeByte(var1, (uint8_t)self->windowId);
    DataStream_writeByte(var1, (uint8_t)self->inventoryType);
    DataStream_writeUTF(var1, self->windowTitle);
    DataStream_writeByte(var1, (uint8_t)self->slotsCount);
}

void Packet100OpenWindow_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_20087_a(var1, (Packet100OpenWindow *)p);
}

int Packet100OpenWindow_getPacketSize(Packet *p) {
    return 3 + (int)strlen(((Packet100OpenWindow *)p)->windowTitle);
}
