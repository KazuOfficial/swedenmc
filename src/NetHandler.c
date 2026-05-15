#include "NetHandler.h"
#include "Packet.h"

int NetHandler_isServerHandler(NetHandler *self) {
    (void)self;
    return 0;
}

void NetHandler_handleMapChunk(NetHandler *self, Packet51MapChunk *var1) {
    (void)self;
    (void)var1;
}

void NetHandler_registerPacket(NetHandler *self, Packet *var1) {
    (void)self;
    (void)var1;
}

void NetHandler_handleErrorMessage(NetHandler *self, const char *var1, const char *var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

void NetHandler_handleKickDisconnect(NetHandler *self, Packet255KickDisconnect *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleLogin(NetHandler *self, Packet1Login *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleFlying(NetHandler *self, Packet10Flying *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleMultiBlockChange(NetHandler *self, Packet52MultiBlockChange *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleBlockDig(NetHandler *self, Packet14BlockDig *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleBlockChange(NetHandler *self, Packet53BlockChange *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handlePreChunk(NetHandler *self, Packet50PreChunk *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleNamedEntitySpawn(NetHandler *self, Packet20NamedEntitySpawn *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleEntity(NetHandler *self, Packet30Entity *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleEntityTeleport(NetHandler *self, Packet34EntityTeleport *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handlePlace(NetHandler *self, Packet15Place *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleBlockItemSwitch(NetHandler *self, Packet16BlockItemSwitch *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleDestroyEntity(NetHandler *self, Packet29DestroyEntity *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handlePickupSpawn(NetHandler *self, Packet21PickupSpawn *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleCollect(NetHandler *self, Packet22Collect *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleChat(NetHandler *self, Packet3Chat *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleVehicleSpawn(NetHandler *self, Packet23VehicleSpawn *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleArmAnimation(NetHandler *self, Packet18Animation *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_21147_a(NetHandler *self, Packet19EntityAction *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleHandshake(NetHandler *self, Packet2Handshake *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleMobSpawn(NetHandler *self, Packet24MobSpawn *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleUpdateTime(NetHandler *self, Packet4UpdateTime *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleSpawnPosition(NetHandler *self, Packet6SpawnPosition *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_6498_a(NetHandler *self, Packet28EntityVelocity *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_21148_a(NetHandler *self, Packet40EntityMetadata *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_6497_a(NetHandler *self, Packet39AttachEntity *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleUseEntity(NetHandler *self, Packet7UseEntity *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_9447_a(NetHandler *self, Packet38EntityStatus *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleHealth(NetHandler *self, Packet8UpdateHealth *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_9448_a(NetHandler *self, Packet9Respawn *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_12245_a(NetHandler *self, Packet60Explosion *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_20087_a(NetHandler *self, Packet100OpenWindow *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_20092_a(NetHandler *self, Packet101CloseWindow *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_20091_a(NetHandler *self, Packet102WindowClick *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_20088_a(NetHandler *self, Packet103SetSlot *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_20094_a(NetHandler *self, Packet104WindowItems *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleSignUpdate(NetHandler *self, Packet130UpdateSign *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_20090_a(NetHandler *self, Packet105UpdateProgressbar *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handlePlayerInventory(NetHandler *self, Packet5PlayerInventory *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_20089_a(NetHandler *self, Packet106Transaction *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_21146_a(NetHandler *self, Packet25EntityPainting *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleNotePlay(NetHandler *self, Packet54PlayNoteBlock *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_27245_a(NetHandler *self, Packet200Statistic *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_22186_a(NetHandler *self, Packet17Sleep *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_22185_a(NetHandler *self, Packet27Position *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_25118_a(NetHandler *self, Packet70Bed *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_handleWeather(NetHandler *self, Packet71Weather *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_28116_a(NetHandler *self, Packet131MapData *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}
void NetHandler_func_28115_a(NetHandler *self, Packet61DoorChange *var1) {
    self->vtable->registerPacket(self, (Packet *)var1);
}

NetHandler_vtable NetHandler_defaultVtable = {
    NetHandler_isServerHandler,       NetHandler_handleMapChunk,
    NetHandler_registerPacket,        NetHandler_handleErrorMessage,
    NetHandler_handleKickDisconnect,  NetHandler_handleLogin,
    NetHandler_handleFlying,          NetHandler_handleMultiBlockChange,
    NetHandler_handleBlockDig,        NetHandler_handleBlockChange,
    NetHandler_handlePreChunk,        NetHandler_handleNamedEntitySpawn,
    NetHandler_handleEntity,          NetHandler_handleEntityTeleport,
    NetHandler_handlePlace,           NetHandler_handleBlockItemSwitch,
    NetHandler_handleDestroyEntity,   NetHandler_handlePickupSpawn,
    NetHandler_handleCollect,         NetHandler_handleChat,
    NetHandler_handleVehicleSpawn,    NetHandler_handleArmAnimation,
    NetHandler_func_21147_a,          NetHandler_handleHandshake,
    NetHandler_handleMobSpawn,        NetHandler_handleUpdateTime,
    NetHandler_handleSpawnPosition,   NetHandler_func_6498_a,
    NetHandler_func_21148_a,          NetHandler_func_6497_a,
    NetHandler_handleUseEntity,       NetHandler_func_9447_a,
    NetHandler_handleHealth,          NetHandler_func_9448_a,
    NetHandler_func_12245_a,          NetHandler_func_20087_a,
    NetHandler_func_20092_a,          NetHandler_func_20091_a,
    NetHandler_func_20088_a,          NetHandler_func_20094_a,
    NetHandler_handleSignUpdate,      NetHandler_func_20090_a,
    NetHandler_handlePlayerInventory, NetHandler_func_20089_a,
    NetHandler_func_21146_a,          NetHandler_handleNotePlay,
    NetHandler_func_27245_a,          NetHandler_func_22186_a,
    NetHandler_func_22185_a,          NetHandler_func_25118_a,
    NetHandler_handleWeather,         NetHandler_func_28116_a,
    NetHandler_func_28115_a,
};
