#include "Packet.h"
#include "PacketCounter.h"
#include "NetHandler.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "Packet0KeepAlive.h"
#include "Packet1Login.h"
#include "Packet2Handshake.h"
#include "Packet3Chat.h"
#include "Packet4UpdateTime.h"
#include "Packet5PlayerInventory.h"
#include "Packet6SpawnPosition.h"
#include "Packet7UseEntity.h"
#include "Packet8UpdateHealth.h"
#include "Packet9Respawn.h"
#include "Packet10Flying.h"
#include "Packet11PlayerPosition.h"
#include "Packet12PlayerLook.h"
#include "Packet13PlayerLookMove.h"
#include "Packet14BlockDig.h"
#include "Packet15Place.h"
#include "Packet16BlockItemSwitch.h"
#include "Packet17Sleep.h"
#include "Packet18Animation.h"
#include "Packet19EntityAction.h"
#include "Packet20NamedEntitySpawn.h"
#include "Packet21PickupSpawn.h"
#include "Packet22Collect.h"
#include "Packet23VehicleSpawn.h"
#include "Packet24MobSpawn.h"
#include "Packet25EntityPainting.h"
#include "Packet27Position.h"
#include "Packet28EntityVelocity.h"
#include "Packet29DestroyEntity.h"
#include "Packet30Entity.h"
#include "Packet31RelEntityMove.h"
#include "Packet32EntityLook.h"
#include "Packet33RelEntityMoveLook.h"
#include "Packet34EntityTeleport.h"
#include "Packet38EntityStatus.h"
#include "Packet39AttachEntity.h"
#include "Packet40EntityMetadata.h"
#include "Packet50PreChunk.h"
#include "Packet51MapChunk.h"
#include "Packet52MultiBlockChange.h"
#include "Packet53BlockChange.h"
#include "Packet54PlayNoteBlock.h"
#include "Packet60Explosion.h"
#include "Packet61DoorChange.h"
#include "Packet70Bed.h"
#include "Packet71Weather.h"
#include "Packet100OpenWindow.h"
#include "Packet101CloseWindow.h"
#include "Packet102WindowClick.h"
#include "Packet103SetSlot.h"
#include "Packet104WindowItems.h"
#include "Packet105UpdateProgressbar.h"
#include "Packet106Transaction.h"
#include "Packet130UpdateSign.h"
#include "Packet131MapData.h"
#include "Packet200Statistic.h"
#include "Packet255KickDisconnect.h"

static PacketFactory s_factories[256];

static int s_clientIds[256];

static int s_serverIds[256];

static PacketCounter s_stats[256];
static int s_statsUsed[256];

static int s_totalPacketsCount;

static int s_registryInited = 0;

void Packet_addIdClassMapping(int id, int isClient, int isServer, PacketFactory factory) {
    s_factories[id] = factory;
    if (isClient)
        s_clientIds[id] = 1;
    if (isServer)
        s_serverIds[id] = 1;
}

Packet *Packet_getNewPacket(int id) {
    if (id < 0 || id > 255 || !s_factories[id])
        return NULL;
    Packet *p = s_factories[id]();
    if (p) {
        p->packetId = id;
        p->creationTimeMillis = (int64_t)time(NULL) * 1000LL;
    }
    return p;
}

int Packet_getPacketId(Packet *self) { return self->packetId; }

Packet *Packet_readPacket(DataStream *var0, int isServer) {

    uint8_t raw = DataStream_readByte(var0);
    if (var0->error)
        return NULL;
    int var6 = (int)raw;

    if (isServer && !s_serverIds[var6])
        return NULL;
    if (!isServer && !s_clientIds[var6])
        return NULL;

    Packet *var3 = Packet_getNewPacket(var6);
    if (!var3)
        return NULL;

    var3->vtable->readPacketData(var3, var0);

    if (!s_statsUsed[var6]) {
        PacketCounter_init(&s_stats[var6]);
        s_statsUsed[var6] = 1;
    }
    PacketCounter_addPacket(&s_stats[var6], var3->vtable->getPacketSize(var3));
    ++s_totalPacketsCount;

    return var3;
}

void Packet_writePacket(Packet *var0, DataStream *var1) {

    DataStream_writeByte(var1, (uint8_t)Packet_getPacketId(var0));

    var0->vtable->writePacketData(var0, var1);
}

void Packet_writeString(const char *var0, DataStream *var1) {
    int len = (int)strlen(var0);

    if (len > 32767) {
        var1->error = 1;
        return;
    }

    DataStream_writeShort(var1, (int16_t)len);

    for (int i = 0; i < len; ++i) {
        DataStream_writeByte(var1, 0);
        DataStream_writeByte(var1, (uint8_t)var0[i]);
    }
}

char *Packet_readString(DataStream *var0, int maxLen) {

    int16_t var2 = DataStream_readShort(var0);
    if (var2 > maxLen || var2 < 0) {
        var0->error = 1;
        return NULL;
    }

    char *result = malloc((size_t)(var2 + 1));
    for (int var4 = 0; var4 < var2; ++var4) {

        int16_t ch = DataStream_readShort(var0);
        result[var4] = (char)(uint8_t)ch;
    }
    result[var2] = '\0';
    return result;
}

void Packet_initRegistry(void) {
    if (s_registryInited)
        return;
    s_registryInited = 1;
    memset(s_factories, 0, sizeof(s_factories));
    memset(s_clientIds, 0, sizeof(s_clientIds));
    memset(s_serverIds, 0, sizeof(s_serverIds));
    memset(s_statsUsed, 0, sizeof(s_statsUsed));
    s_totalPacketsCount = 0;

    Packet_addIdClassMapping(0, 1, 1, Packet0KeepAlive_create);
    Packet_addIdClassMapping(1, 1, 1, Packet1Login_create);
    Packet_addIdClassMapping(2, 1, 1, Packet2Handshake_create);
    Packet_addIdClassMapping(3, 1, 1, Packet3Chat_create);
    Packet_addIdClassMapping(4, 1, 0, Packet4UpdateTime_create);
    Packet_addIdClassMapping(5, 1, 0, Packet5PlayerInventory_create);
    Packet_addIdClassMapping(6, 1, 0, Packet6SpawnPosition_create);
    Packet_addIdClassMapping(7, 0, 1, Packet7UseEntity_create);
    Packet_addIdClassMapping(8, 1, 0, Packet8UpdateHealth_create);
    Packet_addIdClassMapping(9, 1, 1, Packet9Respawn_create);
    Packet_addIdClassMapping(10, 1, 1, Packet10Flying_create);
    Packet_addIdClassMapping(11, 1, 1, Packet11PlayerPosition_create);
    Packet_addIdClassMapping(12, 1, 1, Packet12PlayerLook_create);
    Packet_addIdClassMapping(13, 1, 1, Packet13PlayerLookMove_create);
    Packet_addIdClassMapping(14, 0, 1, Packet14BlockDig_create);
    Packet_addIdClassMapping(15, 0, 1, Packet15Place_create);
    Packet_addIdClassMapping(16, 0, 1, Packet16BlockItemSwitch_create);
    Packet_addIdClassMapping(17, 1, 0, Packet17Sleep_create);
    Packet_addIdClassMapping(18, 1, 1, Packet18Animation_create);
    Packet_addIdClassMapping(19, 0, 1, Packet19EntityAction_create);
    Packet_addIdClassMapping(20, 1, 0, Packet20NamedEntitySpawn_create);
    Packet_addIdClassMapping(21, 1, 0, Packet21PickupSpawn_create);
    Packet_addIdClassMapping(22, 1, 0, Packet22Collect_create);
    Packet_addIdClassMapping(23, 1, 0, Packet23VehicleSpawn_create);
    Packet_addIdClassMapping(24, 1, 0, Packet24MobSpawn_create);
    Packet_addIdClassMapping(25, 1, 0, Packet25EntityPainting_create);
    Packet_addIdClassMapping(27, 0, 1, Packet27Position_create);
    Packet_addIdClassMapping(28, 1, 0, Packet28EntityVelocity_create);
    Packet_addIdClassMapping(29, 1, 0, Packet29DestroyEntity_create);
    Packet_addIdClassMapping(30, 1, 0, Packet30Entity_create);
    Packet_addIdClassMapping(31, 1, 0, Packet31RelEntityMove_create);
    Packet_addIdClassMapping(32, 1, 0, Packet32EntityLook_create);
    Packet_addIdClassMapping(33, 1, 0, Packet33RelEntityMoveLook_create);
    Packet_addIdClassMapping(34, 1, 0, Packet34EntityTeleport_create);
    Packet_addIdClassMapping(38, 1, 0, Packet38EntityStatus_create);
    Packet_addIdClassMapping(39, 1, 0, Packet39AttachEntity_create);
    Packet_addIdClassMapping(40, 1, 0, Packet40EntityMetadata_create);
    Packet_addIdClassMapping(50, 1, 0, Packet50PreChunk_create);
    Packet_addIdClassMapping(51, 1, 0, Packet51MapChunk_create);
    Packet_addIdClassMapping(52, 1, 0, Packet52MultiBlockChange_create);
    Packet_addIdClassMapping(53, 1, 0, Packet53BlockChange_create);
    Packet_addIdClassMapping(54, 1, 0, Packet54PlayNoteBlock_create);
    Packet_addIdClassMapping(60, 1, 0, Packet60Explosion_create);
    Packet_addIdClassMapping(61, 1, 0, Packet61DoorChange_create);
    Packet_addIdClassMapping(70, 1, 0, Packet70Bed_create);
    Packet_addIdClassMapping(71, 1, 0, Packet71Weather_create);
    Packet_addIdClassMapping(100, 1, 0, Packet100OpenWindow_create);
    Packet_addIdClassMapping(101, 1, 1, Packet101CloseWindow_create);
    Packet_addIdClassMapping(102, 0, 1, Packet102WindowClick_create);
    Packet_addIdClassMapping(103, 1, 0, Packet103SetSlot_create);
    Packet_addIdClassMapping(104, 1, 0, Packet104WindowItems_create);
    Packet_addIdClassMapping(105, 1, 0, Packet105UpdateProgressbar_create);
    Packet_addIdClassMapping(106, 1, 1, Packet106Transaction_create);
    Packet_addIdClassMapping(130, 1, 1, Packet130UpdateSign_create);
    Packet_addIdClassMapping(131, 1, 0, Packet131MapData_create);
    Packet_addIdClassMapping(200, 1, 0, Packet200Statistic_create);
    Packet_addIdClassMapping(255, 1, 1, Packet255KickDisconnect_create);
}

void Packet_free(Packet *p) {
    if (!p)
        return;
    switch (p->packetId) {
    case 1:
        free(((Packet1Login *)p)->username);
        break;
    case 2:
        free(((Packet2Handshake *)p)->username);
        break;
    case 3:
        free(((Packet3Chat *)p)->message);
        break;
    case 15:
        free(((Packet15Place *)p)->itemStack);
        break;
    case 20:
        free(((Packet20NamedEntitySpawn *)p)->name);
        break;
    case 51:
        free(((Packet51MapChunk *)p)->chunk);
        break;
    case 52: {
        Packet52MultiBlockChange *q = (Packet52MultiBlockChange *)p;
        free(q->coordinateArray);
        free(q->typeArray);
        free(q->metadataArray);
        break;
    }
    case 60:
        free(((Packet60Explosion *)p)->destroyedBlocks);
        break;
    case 104: {
        Packet104WindowItems *q = (Packet104WindowItems *)p;

        free(q->itemStack);
        break;
    }
    case 102:
        free(((Packet102WindowClick *)p)->itemStack);
        break;
    case 131:
        free(((Packet131MapData *)p)->field_28056_c);
        break;
    case 25:
        free(((Packet25EntityPainting *)p)->title);
        break;
    case 130: {
        Packet130UpdateSign *q = (Packet130UpdateSign *)p;
        for (int i = 0; i < 4; i++)
            free(q->signLines[i]);
        break;
    }
    case 255:
        free(((Packet255KickDisconnect *)p)->reason);
        break;
    default:
        break;
    }
    free(p);
}
