#include "Packet30Entity.h"
#include "Packet31RelEntityMove.h"
#include "Packet32EntityLook.h"
#include "Packet33RelEntityMoveLook.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable30 = {
    Packet30Entity_readPacketData,
    Packet30Entity_writePacketData,
    Packet30Entity_processPacket,
    Packet30Entity_getPacketSize,
};

Packet *Packet30Entity_create(void) {
    Packet30Entity *self = calloc(1, sizeof(Packet30Entity));
    self->base.vtable = &vtable30;
    self->base.packetId = 30;

    self->rotating = 0;
    return (Packet *)self;
}

void Packet30Entity_readPacketData(Packet *p, DataStream *var1) {
    ((Packet30Entity *)p)->entityId = DataStream_readInt(var1);
}

void Packet30Entity_writePacketData(Packet *p, DataStream *var1) {
    DataStream_writeInt(var1, ((Packet30Entity *)p)->entityId);
}

void Packet30Entity_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleEntity(var1, (Packet30Entity *)p);
}

int Packet30Entity_getPacketSize(Packet *p) {
    (void)p;
    return 4;
}

static Packet_vtable vtable31 = {
    Packet31RelEntityMove_readPacketData,
    Packet31RelEntityMove_writePacketData,
    Packet30Entity_processPacket,
    Packet31RelEntityMove_getPacketSize,
};

Packet *Packet31RelEntityMove_create(void) {
    Packet30Entity *self = calloc(1, sizeof(Packet30Entity));
    self->base.vtable = &vtable31;
    self->base.packetId = 31;
    return (Packet *)self;
}

void Packet31RelEntityMove_readPacketData(Packet *p, DataStream *var1) {
    Packet30Entity *self = (Packet30Entity *)p;

    Packet30Entity_readPacketData(p, var1);
    self->xPosition = (int8_t)DataStream_readByte(var1);
    self->yPosition = (int8_t)DataStream_readByte(var1);
    self->zPosition = (int8_t)DataStream_readByte(var1);
}

void Packet31RelEntityMove_writePacketData(Packet *p, DataStream *var1) {
    Packet30Entity *self = (Packet30Entity *)p;
    Packet30Entity_writePacketData(p, var1);
    DataStream_writeByte(var1, (uint8_t)self->xPosition);
    DataStream_writeByte(var1, (uint8_t)self->yPosition);
    DataStream_writeByte(var1, (uint8_t)self->zPosition);
}

int Packet31RelEntityMove_getPacketSize(Packet *p) {
    (void)p;
    return 7;
}

static Packet_vtable vtable32 = {
    Packet32EntityLook_readPacketData,
    Packet32EntityLook_writePacketData,
    Packet30Entity_processPacket,
    Packet32EntityLook_getPacketSize,
};

Packet *Packet32EntityLook_create(void) {
    Packet30Entity *self = calloc(1, sizeof(Packet30Entity));
    self->base.vtable = &vtable32;
    self->base.packetId = 32;

    self->rotating = 1;
    return (Packet *)self;
}

void Packet32EntityLook_readPacketData(Packet *p, DataStream *var1) {
    Packet30Entity *self = (Packet30Entity *)p;
    Packet30Entity_readPacketData(p, var1);
    self->yaw = (int8_t)DataStream_readByte(var1);
    self->pitch = (int8_t)DataStream_readByte(var1);
}

void Packet32EntityLook_writePacketData(Packet *p, DataStream *var1) {
    Packet30Entity *self = (Packet30Entity *)p;
    Packet30Entity_writePacketData(p, var1);
    DataStream_writeByte(var1, (uint8_t)self->yaw);
    DataStream_writeByte(var1, (uint8_t)self->pitch);
}

int Packet32EntityLook_getPacketSize(Packet *p) {
    (void)p;
    return 6;
}

static Packet_vtable vtable33 = {
    Packet33RelEntityMoveLook_readPacketData,
    Packet33RelEntityMoveLook_writePacketData,
    Packet30Entity_processPacket,
    Packet33RelEntityMoveLook_getPacketSize,
};

Packet *Packet33RelEntityMoveLook_create(void) {
    Packet30Entity *self = calloc(1, sizeof(Packet30Entity));
    self->base.vtable = &vtable33;
    self->base.packetId = 33;

    self->rotating = 1;
    return (Packet *)self;
}

void Packet33RelEntityMoveLook_readPacketData(Packet *p, DataStream *var1) {
    Packet30Entity *self = (Packet30Entity *)p;
    Packet30Entity_readPacketData(p, var1);
    self->xPosition = (int8_t)DataStream_readByte(var1);
    self->yPosition = (int8_t)DataStream_readByte(var1);
    self->zPosition = (int8_t)DataStream_readByte(var1);
    self->yaw = (int8_t)DataStream_readByte(var1);
    self->pitch = (int8_t)DataStream_readByte(var1);
}

void Packet33RelEntityMoveLook_writePacketData(Packet *p, DataStream *var1) {
    Packet30Entity *self = (Packet30Entity *)p;
    Packet30Entity_writePacketData(p, var1);
    DataStream_writeByte(var1, (uint8_t)self->xPosition);
    DataStream_writeByte(var1, (uint8_t)self->yPosition);
    DataStream_writeByte(var1, (uint8_t)self->zPosition);
    DataStream_writeByte(var1, (uint8_t)self->yaw);
    DataStream_writeByte(var1, (uint8_t)self->pitch);
}

int Packet33RelEntityMoveLook_getPacketSize(Packet *p) {
    (void)p;
    return 9;
}
