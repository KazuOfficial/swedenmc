#include "Packet71Weather.h"
#include "NetHandler.h"
#include "Entity.h"
#include "EntityLightningBolt.h"
#include "MathHelper.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet71Weather_readPacketData,
    Packet71Weather_writePacketData,
    Packet71Weather_processPacket,
    Packet71Weather_getPacketSize,
};

Packet *Packet71Weather_create(void) {
    Packet71Weather *self = calloc(1, sizeof(Packet71Weather));
    self->base.vtable = &vtable;
    self->base.packetId = 71;
    return (Packet *)self;
}

Packet *Packet71Weather_createFromEntity(Entity *var1) {
    Packet71Weather *self = (Packet71Weather *)Packet71Weather_create();
    self->field_27054_a = var1->entityId;
    self->field_27053_b = MathHelper_floor_double(var1->posX * 32.0);
    self->field_27057_c = MathHelper_floor_double(var1->posY * 32.0);
    self->field_27056_d = MathHelper_floor_double(var1->posZ * 32.0);

    if (var1->isLightningBolt) {
        self->field_27055_e = 1;
    }
    return (Packet *)self;
}

void Packet71Weather_readPacketData(Packet *p, DataStream *var1) {
    Packet71Weather *self = (Packet71Weather *)p;
    self->field_27054_a = DataStream_readInt(var1);

    self->field_27055_e = (int)(int8_t)DataStream_readByte(var1);
    self->field_27053_b = DataStream_readInt(var1);
    self->field_27057_c = DataStream_readInt(var1);
    self->field_27056_d = DataStream_readInt(var1);
}

void Packet71Weather_writePacketData(Packet *p, DataStream *var1) {
    Packet71Weather *self = (Packet71Weather *)p;
    DataStream_writeInt(var1, self->field_27054_a);
    DataStream_writeByte(var1, (uint8_t)self->field_27055_e);
    DataStream_writeInt(var1, self->field_27053_b);
    DataStream_writeInt(var1, self->field_27057_c);
    DataStream_writeInt(var1, self->field_27056_d);
}

void Packet71Weather_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->handleWeather(var1, (Packet71Weather *)p);
}

int Packet71Weather_getPacketSize(Packet *p) {
    (void)p;
    return 17;
}
