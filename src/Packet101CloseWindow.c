#include "Packet101CloseWindow.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet101CloseWindow_readPacketData,
    Packet101CloseWindow_writePacketData,
    Packet101CloseWindow_processPacket,
    Packet101CloseWindow_getPacketSize,
};

Packet *Packet101CloseWindow_create(void) {
    Packet101CloseWindow *self = calloc(1, sizeof(Packet101CloseWindow));
    self->base.vtable = &vtable;
    self->base.packetId = 101;
    return (Packet *)self;
}

Packet *Packet101CloseWindow_createWith(int windowId) {
    Packet101CloseWindow *self = (Packet101CloseWindow *)Packet101CloseWindow_create();
    self->windowId = windowId;
    return (Packet *)self;
}

void Packet101CloseWindow_readPacketData(Packet *p, DataStream *var1) {

    ((Packet101CloseWindow *)p)->windowId = (int)(int8_t)DataStream_readByte(var1);
}

void Packet101CloseWindow_writePacketData(Packet *p, DataStream *var1) {
    DataStream_writeByte(var1, (uint8_t)((Packet101CloseWindow *)p)->windowId);
}

void Packet101CloseWindow_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_20092_a(var1, (Packet101CloseWindow *)p);
}

int Packet101CloseWindow_getPacketSize(Packet *p) {
    (void)p;
    return 1;
}
