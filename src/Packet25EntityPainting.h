#ifndef PACKET25_ENTITY_PAINTING_H
#define PACKET25_ENTITY_PAINTING_H

#include "Packet.h"

struct Packet25EntityPainting {
    Packet base;
    int entityId;
    int xPosition;
    int yPosition;
    int zPosition;
    int direction;
    char *title;
};
typedef struct Packet25EntityPainting Packet25EntityPainting;

Packet *Packet25EntityPainting_create(void);

typedef struct EntityPainting EntityPainting;
Packet *Packet25EntityPainting_createWith(EntityPainting *var1);
void Packet25EntityPainting_readPacketData(Packet *self, DataStream *var1);
void Packet25EntityPainting_writePacketData(Packet *self, DataStream *var1);
void Packet25EntityPainting_processPacket(Packet *self, NetHandler *var1);
int Packet25EntityPainting_getPacketSize(Packet *self);

#endif
