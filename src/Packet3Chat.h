#ifndef PACKET3_CHAT_H
#define PACKET3_CHAT_H

#include "Packet.h"

struct Packet3Chat {
    Packet base;

    char *message;
};
typedef struct Packet3Chat Packet3Chat;

Packet *Packet3Chat_create(void);
Packet *Packet3Chat_createWith(const char *message);
void Packet3Chat_readPacketData(Packet *self, DataStream *var1);
void Packet3Chat_writePacketData(Packet *self, DataStream *var1);
void Packet3Chat_processPacket(Packet *self, NetHandler *var1);
int Packet3Chat_getPacketSize(Packet *self);

#endif
