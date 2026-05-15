#include "Packet131MapData.h"
#include "NetHandler.h"
#include <stdlib.h>

static Packet_vtable vtable = {
    Packet131MapData_readPacketData,
    Packet131MapData_writePacketData,
    Packet131MapData_processPacket,
    Packet131MapData_getPacketSize,
};

Packet *Packet131MapData_create(void) {
    Packet131MapData *self = calloc(1, sizeof(Packet131MapData));
    self->base.vtable = &vtable;
    self->base.packetId = 131;

    self->base.isChunkDataPacket = 1;
    return (Packet *)self;
}

void Packet131MapData_readPacketData(Packet *p, DataStream *var1) {
    Packet131MapData *self = (Packet131MapData *)p;
    self->field_28055_a = DataStream_readShort(var1);
    self->field_28054_b = DataStream_readShort(var1);

    self->field_28056_c_len = (int)DataStream_readByte(var1) & 255;
    self->field_28056_c = malloc((size_t)self->field_28056_c_len);
    DataStream_readBytes(var1, self->field_28056_c, (size_t)self->field_28056_c_len);
}

void Packet131MapData_writePacketData(Packet *p, DataStream *var1) {
    Packet131MapData *self = (Packet131MapData *)p;
    DataStream_writeShort(var1, self->field_28055_a);
    DataStream_writeShort(var1, self->field_28054_b);
    DataStream_writeByte(var1, (uint8_t)self->field_28056_c_len);
    DataStream_writeBytes(var1, self->field_28056_c, (size_t)self->field_28056_c_len);
}

void Packet131MapData_processPacket(Packet *p, NetHandler *var1) {
    var1->vtable->func_28116_a(var1, (Packet131MapData *)p);
}

int Packet131MapData_getPacketSize(Packet *p) {
    return 4 + ((Packet131MapData *)p)->field_28056_c_len;
}
