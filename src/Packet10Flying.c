#include "Packet10Flying.h"
#include "Packet11PlayerPosition.h"
#include "Packet12PlayerLook.h"
#include "Packet13PlayerLookMove.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable10 = {
    Packet10Flying_readPacketData,
    Packet10Flying_writePacketData,
    Packet10Flying_processPacket,
    Packet10Flying_getPacketSize,
};

Packet *Packet10Flying_create(void) {
    Packet10Flying *self = calloc(1, sizeof(Packet10Flying));
    self->base.vtable = &vtable10;
    self->base.packetId = 10;
    return (Packet *)self;
}

Packet *Packet10Flying_createWith(int onGround) {
    Packet10Flying *self = (Packet10Flying *)Packet10Flying_create();
    self->onGround = onGround;
    return (Packet *)self;
}

void Packet10Flying_readPacketData(Packet *p, DataStream *var1) {
    Packet10Flying *self = (Packet10Flying *)p;

    self->onGround = (int)DataStream_readByte(var1) != 0;
}

void Packet10Flying_writePacketData(Packet *p, DataStream *var1) {
    Packet10Flying *self = (Packet10Flying *)p;

    DataStream_writeByte(var1, (uint8_t)(self->onGround ? 1 : 0));
}

void Packet10Flying_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleFlying(var1, (Packet10Flying *)p);
}

int Packet10Flying_getPacketSize(Packet *p) {
    (void)p;
    return 1;
}

static Packet_vtable vtable11 = {
    Packet11PlayerPosition_readPacketData,
    Packet11PlayerPosition_writePacketData,
    Packet10Flying_processPacket,
    Packet11PlayerPosition_getPacketSize,
};

Packet *Packet11PlayerPosition_create(void) {
    Packet10Flying *self = calloc(1, sizeof(Packet10Flying));
    self->base.vtable = &vtable11;
    self->base.packetId = 11;

    self->moving = 1;
    return (Packet *)self;
}

Packet *Packet11PlayerPosition_createWith(double x, double y, double stance, double z,
                                          int onGround) {
    Packet10Flying *self = (Packet10Flying *)Packet11PlayerPosition_create();
    self->xPosition = x;
    self->yPosition = y;
    self->stance = stance;
    self->zPosition = z;
    self->onGround = onGround;
    return (Packet *)self;
}

void Packet11PlayerPosition_readPacketData(Packet *p, DataStream *var1) {
    Packet10Flying *self = (Packet10Flying *)p;
    self->xPosition = DataStream_readDouble(var1);
    self->yPosition = DataStream_readDouble(var1);
    self->stance = DataStream_readDouble(var1);
    self->zPosition = DataStream_readDouble(var1);

    Packet10Flying_readPacketData(p, var1);
}

void Packet11PlayerPosition_writePacketData(Packet *p, DataStream *var1) {
    Packet10Flying *self = (Packet10Flying *)p;
    DataStream_writeDouble(var1, self->xPosition);
    DataStream_writeDouble(var1, self->yPosition);
    DataStream_writeDouble(var1, self->stance);
    DataStream_writeDouble(var1, self->zPosition);

    Packet10Flying_writePacketData(p, var1);
}

int Packet11PlayerPosition_getPacketSize(Packet *p) {
    (void)p;
    return 33;
}

static Packet_vtable vtable12 = {
    Packet12PlayerLook_readPacketData,
    Packet12PlayerLook_writePacketData,
    Packet10Flying_processPacket,
    Packet12PlayerLook_getPacketSize,
};

Packet *Packet12PlayerLook_create(void) {
    Packet10Flying *self = calloc(1, sizeof(Packet10Flying));
    self->base.vtable = &vtable12;
    self->base.packetId = 12;

    self->rotating = 1;
    return (Packet *)self;
}

Packet *Packet12PlayerLook_createWith(float yaw, float pitch, int onGround) {
    Packet10Flying *self = (Packet10Flying *)Packet12PlayerLook_create();
    self->yaw = yaw;
    self->pitch = pitch;
    self->onGround = onGround;
    return (Packet *)self;
}

void Packet12PlayerLook_readPacketData(Packet *p, DataStream *var1) {
    Packet10Flying *self = (Packet10Flying *)p;
    self->yaw = DataStream_readFloat(var1);
    self->pitch = DataStream_readFloat(var1);
    Packet10Flying_readPacketData(p, var1);
}

void Packet12PlayerLook_writePacketData(Packet *p, DataStream *var1) {
    Packet10Flying *self = (Packet10Flying *)p;
    DataStream_writeFloat(var1, self->yaw);
    DataStream_writeFloat(var1, self->pitch);
    Packet10Flying_writePacketData(p, var1);
}

int Packet12PlayerLook_getPacketSize(Packet *p) {
    (void)p;
    return 9;
}

static Packet_vtable vtable13 = {
    Packet13PlayerLookMove_readPacketData,
    Packet13PlayerLookMove_writePacketData,
    Packet10Flying_processPacket,
    Packet13PlayerLookMove_getPacketSize,
};

Packet *Packet13PlayerLookMove_create(void) {
    Packet10Flying *self = calloc(1, sizeof(Packet10Flying));
    self->base.vtable = &vtable13;
    self->base.packetId = 13;
    self->rotating = 1;
    self->moving = 1;
    return (Packet *)self;
}

Packet *Packet13PlayerLookMove_createWith(double x, double y, double stance, double z, float yaw,
                                          float pitch, int onGround) {
    Packet10Flying *self = (Packet10Flying *)Packet13PlayerLookMove_create();
    self->xPosition = x;
    self->yPosition = y;
    self->stance = stance;
    self->zPosition = z;
    self->yaw = yaw;
    self->pitch = pitch;
    self->onGround = onGround;
    return (Packet *)self;
}

void Packet13PlayerLookMove_readPacketData(Packet *p, DataStream *var1) {
    Packet10Flying *self = (Packet10Flying *)p;
    self->xPosition = DataStream_readDouble(var1);
    self->yPosition = DataStream_readDouble(var1);
    self->stance = DataStream_readDouble(var1);
    self->zPosition = DataStream_readDouble(var1);
    self->yaw = DataStream_readFloat(var1);
    self->pitch = DataStream_readFloat(var1);
    Packet10Flying_readPacketData(p, var1);
}

void Packet13PlayerLookMove_writePacketData(Packet *p, DataStream *var1) {
    Packet10Flying *self = (Packet10Flying *)p;
    DataStream_writeDouble(var1, self->xPosition);
    DataStream_writeDouble(var1, self->yPosition);
    DataStream_writeDouble(var1, self->stance);
    DataStream_writeDouble(var1, self->zPosition);
    DataStream_writeFloat(var1, self->yaw);
    DataStream_writeFloat(var1, self->pitch);
    Packet10Flying_writePacketData(p, var1);
}

int Packet13PlayerLookMove_getPacketSize(Packet *p) {
    (void)p;
    return 41;
}
