#include "Packet16BlockItemSwitch.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet16BlockItemSwitch_readPacketData,
    Packet16BlockItemSwitch_writePacketData,
    Packet16BlockItemSwitch_processPacket,
    Packet16BlockItemSwitch_getPacketSize,
};

Packet *Packet16BlockItemSwitch_create(void) {
    Packet16BlockItemSwitch *self = calloc(1, sizeof(Packet16BlockItemSwitch));
    self->base.vtable = &vtable;
    self->base.packetId = 16;
    return (Packet *)self;
}

Packet *Packet16BlockItemSwitch_createWith(int id) {
    Packet16BlockItemSwitch *self = (Packet16BlockItemSwitch *)Packet16BlockItemSwitch_create();
    self->id = id;
    return (Packet *)self;
}

void Packet16BlockItemSwitch_readPacketData(Packet *p, DataStream *var1) {
    ((Packet16BlockItemSwitch *)p)->id = (int)DataStream_readShort(var1);
}

void Packet16BlockItemSwitch_writePacketData(Packet *p, DataStream *var1) {
    DataStream_writeShort(var1, (int16_t)((Packet16BlockItemSwitch *)p)->id);
}

void Packet16BlockItemSwitch_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleBlockItemSwitch(var1, (Packet16BlockItemSwitch *)p);
}

int Packet16BlockItemSwitch_getPacketSize(Packet *p) {
    (void)p;
    return 2;
}
