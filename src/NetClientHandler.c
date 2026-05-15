#include "NetClientHandler.h"
#include "NetworkManager.h"
#include "Minecraft.h"
#include "WorldClient.h"
#include "World.h"
#include "WorldInfo.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityClientPlayerMP.h"
#include "EntityOtherPlayerMP.h"
#include "EntityItem.h"
#include "EntityArrow.h"
#include "EntitySnowball.h"
#include "EntityFireball.h"
#include "EntityEgg.h"
#include "EntityBoat.h"
#include "EntityTNTPrimed.h"
#include "EntityFallingSand.h"
#include "EntityMinecart.h"
#include "EntityFish.h"
#include "EntityLightningBolt.h"
#include "EntityPainting.h"
#include "EntityPickupFX.h"
#include "EntityList.h"
#include "ItemStack.h"
#include "Item.h"
#include "Block.h"
#include "Chunk.h"
#include "Explosion.h"
#include "Container.h"
#include "InventoryBasic.h"
#include "TileEntityFurnace.h"
#include "TileEntityDispenser.h"
#include "TileEntitySign.h"
#include "TileEntity.h"
#include "GuiIngame.h"
#include "GuiDownloadTerrain.h"
#include "GuiConnectFailed.h"
#include "EffectRenderer.h"
#include "JavaRandom.h"
#include "MathHelper.h"
#include "InventoryPlayer.h"
#include "StatFileWriter.h"
#include "StatList.h"
#include "PlayerControllerMP.h"
#include "Packet.h"
#include "Packet1Login.h"
#include "Packet2Handshake.h"
#include "Packet3Chat.h"
#include "Packet4UpdateTime.h"
#include "Packet5PlayerInventory.h"
#include "Packet6SpawnPosition.h"
#include "Packet8UpdateHealth.h"
#include "Packet9Respawn.h"
#include "Packet10Flying.h"
#include "Packet13PlayerLookMove.h"
#include "Packet17Sleep.h"
#include "Packet18Animation.h"
#include "Packet20NamedEntitySpawn.h"
#include "Packet21PickupSpawn.h"
#include "Packet22Collect.h"
#include "Packet23VehicleSpawn.h"
#include "Packet24MobSpawn.h"
#include "Packet25EntityPainting.h"
#include "Packet28EntityVelocity.h"
#include "Packet29DestroyEntity.h"
#include "Packet30Entity.h"
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
#include "Packet103SetSlot.h"
#include "Packet104WindowItems.h"
#include "Packet105UpdateProgressbar.h"
#include "Packet106Transaction.h"
#include "Packet130UpdateSign.h"
#include "Packet131MapData.h"
#include "ItemMap.h"
#include "MapData.h"
#include "Packet200Statistic.h"
#include "Packet255KickDisconnect.h"
#include "ChunkCoordinates.h"
#include "DataWatcher.h"
#include "platform.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef __EMSCRIPTEN__
typedef struct {
    char *buf;
    size_t len;
} NCH_CurlBuf;
static size_t NCH_curl_write(void *ptr, size_t size, size_t nmemb, void *ud) {
    NCH_CurlBuf *b = (NCH_CurlBuf *)ud;
    size_t n = size * nmemb;
    b->buf = realloc(b->buf, b->len + n + 1);
    memcpy(b->buf + b->len, ptr, n);
    b->len += n;
    b->buf[b->len] = '\0';
    return n;
}
#endif

typedef struct {
    char username[64];
    char accessToken[2048];
    char uuid[64];
    char serverId[256];
    NetClientHandler *nch;
} JoinServerArgs;

static void *joinserver_thread(void *arg) {
    JoinServerArgs *args = (JoinServerArgs *)arg;

#ifdef __EMSCRIPTEN__

    Packet1Login *p = (Packet1Login *)Packet1Login_createWith(args->username, 14);
    NetClientHandler_addToSendQueue(args->nch, (Packet *)p);
#else
    CURL *curl = sw_curl_init();
    if (!curl) {
        NetworkManager_networkShutdown(args->nch->netManager, "disconnect.genericReason");
        free(args);
        return NULL;
    }

    char json[4096];
    snprintf(json, sizeof(json),
             "{\"accessToken\":\"%s\",\"selectedProfile\":\"%s\",\"serverId\":\"%s\"}",
             args->accessToken, args->uuid, args->serverId);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "https://sessionserver.mojang.com/session/minecraft/join");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NCH_curl_write);
    NCH_CurlBuf buf = {NULL, 0};
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

    CURLcode res = curl_easy_perform(curl);
    long code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(buf.buf);

    if (res != CURLE_OK) {
        char msg[512];
        snprintf(msg, sizeof(msg), "Internal client error: %s", curl_easy_strerror(res));
        NetworkManager_networkShutdown(args->nch->netManager, msg);
    } else if (code == 204) {
        Packet1Login *p = (Packet1Login *)Packet1Login_createWith(args->username, 14);
        NetClientHandler_addToSendQueue(args->nch, (Packet *)p);
    } else {
        NetworkManager_networkShutdownWithArg(args->nch->netManager, "disconnect.loginFailedInfo",
                                              "Invalid session (Try restarting your game)");
    }
#endif

    free(args);
    return NULL;
}

static NetHandler_vtable s_vtable;
static int s_vtableInit = 0;

static Entity *NCH_getEntityByID(NetClientHandler *self, int var1) {
    if (self->mc->thePlayer != NULL && var1 == self->mc->thePlayer->base.base.base.entityId)
        return (Entity *)self->mc->thePlayer;
    return WorldClient_func_709_b(self->worldClient, var1);
}

static void NCH_handleLogin(NetHandler *base, Packet1Login *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    PlayerController_destroy(self->mc->playerController);
    self->mc->playerController = (PlayerController *)PlayerControllerMP_create(self->mc, self);
    StatFileWriter_readStat(self->mc->statFileWriter, StatList_joinMultiplayerStat, 1);
    self->worldClient = WorldClient_create(self, var1->mapSeed, var1->dimension);
    self->worldClient->base.multiplayerWorld = 1;
    Minecraft_changeWorld1(self->mc, (World *)self->worldClient);
    ((EntityPlayer *)self->mc->thePlayer)->dimension = var1->dimension;
    {
        GuiDownloadTerrain *gui = (GuiDownloadTerrain *)calloc(1, sizeof(GuiDownloadTerrain));
        GuiDownloadTerrain_construct(gui, self);
        Minecraft_displayGuiScreen(self->mc, (struct GuiScreen *)gui);
    }
    ((Entity *)self->mc->thePlayer)->entityId = var1->protocolVersion;
}

static void NCH_handlePickupSpawn(NetHandler *base, Packet21PickupSpawn *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    double var2 = (double)var1->xPosition / 32.0;
    double var4 = (double)var1->yPosition / 32.0;
    double var6 = (double)var1->zPosition / 32.0;
    ItemStack stack;
    stack.itemID = var1->itemID;
    stack.stackSize = var1->count;
    stack.itemDamage = var1->itemDamage;
    EntityItem *var8 = EntityItem_create((World *)self->worldClient, var2, var4, var6, stack);
    var8->base.motionX = (double)var1->rotation / 128.0;
    var8->base.motionY = (double)var1->pitch / 128.0;
    var8->base.motionZ = (double)var1->roll / 128.0;
    var8->base.serverPosX = var1->xPosition;
    var8->base.serverPosY = var1->yPosition;
    var8->base.serverPosZ = var1->zPosition;
    WorldClient_func_712_a(self->worldClient, var1->entityId, (Entity *)var8);
}

static void NCH_handleVehicleSpawn(NetHandler *base, Packet23VehicleSpawn *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    double var2 = (double)var1->xPosition / 32.0;
    double var4 = (double)var1->yPosition / 32.0;
    double var6 = (double)var1->zPosition / 32.0;
    Entity *var8 = NULL;

    if (var1->type == 10) {
        EntityMinecart *e = (EntityMinecart *)calloc(1, sizeof(EntityMinecart));
        EntityMinecart_construct2(e, (World *)self->worldClient, var2, var4, var6, 0);
        var8 = (Entity *)e;
    }
    if (var1->type == 11) {
        EntityMinecart *e = (EntityMinecart *)calloc(1, sizeof(EntityMinecart));
        EntityMinecart_construct2(e, (World *)self->worldClient, var2, var4, var6, 1);
        var8 = (Entity *)e;
    }
    if (var1->type == 12) {
        EntityMinecart *e = (EntityMinecart *)calloc(1, sizeof(EntityMinecart));
        EntityMinecart_construct2(e, (World *)self->worldClient, var2, var4, var6, 2);
        var8 = (Entity *)e;
    }
    if (var1->type == 90) {
        EntityFish *e = (EntityFish *)calloc(1, sizeof(EntityFish));
        EntityFish_construct2(e, (World *)self->worldClient, var2, var4, var6);
        var8 = (Entity *)e;
    }
    if (var1->type == 60) {
        EntityArrow *e = (EntityArrow *)calloc(1, sizeof(EntityArrow));
        EntityArrow_construct2(e, (World *)self->worldClient, var2, var4, var6);
        var8 = (Entity *)e;
    }
    if (var1->type == 61) {
        EntitySnowball *e = (EntitySnowball *)calloc(1, sizeof(EntitySnowball));
        EntitySnowball_construct3(e, (World *)self->worldClient, var2, var4, var6);
        var8 = (Entity *)e;
    }
    if (var1->type == 63) {
        EntityFireball *e = (EntityFireball *)calloc(1, sizeof(EntityFireball));
        EntityFireball_construct2(
            e, (World *)self->worldClient, var2, var4, var6, (double)var1->field_28047_e / 8000.0,
            (double)var1->field_28046_f / 8000.0, (double)var1->field_28045_g / 8000.0);
        var1->field_28044_i = 0;
        var8 = (Entity *)e;
    }
    if (var1->type == 62) {
        EntityEgg *e = (EntityEgg *)calloc(1, sizeof(EntityEgg));
        EntityEgg_construct3(e, (World *)self->worldClient, var2, var4, var6);
        var8 = (Entity *)e;
    }
    if (var1->type == 1) {
        EntityBoat *e = (EntityBoat *)calloc(1, sizeof(EntityBoat));
        EntityBoat_construct2(e, (World *)self->worldClient, var2, var4, var6);
        var8 = (Entity *)e;
    }
    if (var1->type == 50) {
        EntityTNTPrimed *e = (EntityTNTPrimed *)calloc(1, sizeof(EntityTNTPrimed));
        EntityTNTPrimed_construct2(e, (World *)self->worldClient, var2, var4, var6);
        var8 = (Entity *)e;
    }
    if (var1->type == 70) {
        EntityFallingSand *e = (EntityFallingSand *)calloc(1, sizeof(EntityFallingSand));
        EntityFallingSand_construct2(e, (World *)self->worldClient, var2, var4, var6,
                                     Block_sand->blockID);
        var8 = (Entity *)e;
    }
    if (var1->type == 71) {
        EntityFallingSand *e = (EntityFallingSand *)calloc(1, sizeof(EntityFallingSand));
        EntityFallingSand_construct2(e, (World *)self->worldClient, var2, var4, var6,
                                     Block_gravel->blockID);
        var8 = (Entity *)e;
    }

    if (var8 != NULL) {
        var8->serverPosX = var1->xPosition;
        var8->serverPosY = var1->yPosition;
        var8->serverPosZ = var1->zPosition;
        var8->rotationYaw = 0.0F;
        var8->rotationPitch = 0.0F;
        var8->entityId = var1->entityId;
        WorldClient_func_712_a(self->worldClient, var1->entityId, var8);
        if (var1->field_28044_i > 0) {
            if (var1->type == 60) {
                Entity *var9 = NCH_getEntityByID(self, var1->field_28044_i);
                if (var9 != NULL) {

                    ((EntityArrow *)var8)->owner = (EntityLiving *)var9;
                }
            }
            var8->vtable->setVelocity(var8, (double)var1->field_28047_e / 8000.0,
                                      (double)var1->field_28046_f / 8000.0,
                                      (double)var1->field_28045_g / 8000.0);
        }
    }
}

static void NCH_handleWeather(NetHandler *base, Packet71Weather *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    double var2 = (double)var1->field_27053_b / 32.0;
    double var4 = (double)var1->field_27057_c / 32.0;
    double var6 = (double)var1->field_27056_d / 32.0;
    EntityLightningBolt *var8 = NULL;
    if (var1->field_27055_e == 1) {
        var8 = (EntityLightningBolt *)calloc(1, sizeof(EntityLightningBolt));
        EntityLightningBolt_construct(var8, (World *)self->worldClient, var2, var4, var6);
    }
    if (var8 != NULL) {
        ((Entity *)var8)->serverPosX = var1->field_27053_b;
        ((Entity *)var8)->serverPosY = var1->field_27057_c;
        ((Entity *)var8)->serverPosZ = var1->field_27056_d;
        ((Entity *)var8)->rotationYaw = 0.0F;
        ((Entity *)var8)->rotationPitch = 0.0F;
        ((Entity *)var8)->entityId = var1->field_27054_a;
        World_addWeatherEffect((World *)self->worldClient, (Entity *)var8);
    }
}

static void NCH_func_21146_a(NetHandler *base, Packet25EntityPainting *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    EntityPainting *var2 =
        EntityPainting_create3((World *)self->worldClient, var1->xPosition, var1->yPosition,
                               var1->zPosition, var1->direction, var1->title);
    WorldClient_func_712_a(self->worldClient, var1->entityId, (Entity *)var2);
}

static void NCH_func_6498_a(NetHandler *base, Packet28EntityVelocity *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Entity *var2 = NCH_getEntityByID(self, var1->entityId);
    if (var2 != NULL) {
        var2->vtable->setVelocity(var2, (double)var1->motionX / 8000.0,
                                  (double)var1->motionY / 8000.0, (double)var1->motionZ / 8000.0);
    }
}

static void NCH_func_21148_a(NetHandler *base, Packet40EntityMetadata *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Entity *var2 = NCH_getEntityByID(self, var1->entityId);
    if (var2 != NULL && Packet40EntityMetadata_func_21047_b(var1) != NULL) {

        DataWatcher_updateWatchedObjectsFromList(Entity_getDataWatcher(var2),
                                                 Packet40EntityMetadata_func_21047_b(var1));
    }
}

static void NCH_handleNamedEntitySpawn(NetHandler *base, Packet20NamedEntitySpawn *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    double var2 = (double)var1->xPosition / 32.0;
    double var4 = (double)var1->yPosition / 32.0;
    double var6 = (double)var1->zPosition / 32.0;
    float var8 = (float)(var1->rotation * 360) / 256.0F;
    float var9 = (float)(var1->pitch * 360) / 256.0F;
    EntityOtherPlayerMP *var10 = EntityOtherPlayerMP_create(self->mc->theWorld, var1->name);
    ((Entity *)var10)->prevPosX = ((Entity *)var10)->lastTickPosX =
        (double)(((Entity *)var10)->serverPosX = var1->xPosition);
    ((Entity *)var10)->prevPosY = ((Entity *)var10)->lastTickPosY =
        (double)(((Entity *)var10)->serverPosY = var1->yPosition);
    ((Entity *)var10)->prevPosZ = ((Entity *)var10)->lastTickPosZ =
        (double)(((Entity *)var10)->serverPosZ = var1->zPosition);
    int var11 = var1->currentItem;
    if (var11 == 0) {
        var10->base.inventory.mainInventory[var10->base.inventory.currentItem] = NULL;
    } else {
        ItemStack *is = (ItemStack *)calloc(1, sizeof(ItemStack));
        is->itemID = var11;
        is->stackSize = 1;
        is->itemDamage = 0;
        var10->base.inventory.mainInventory[var10->base.inventory.currentItem] = is;
    }
    Entity_setPositionAndRotation((Entity *)var10, var2, var4, var6, var8, var9);
    WorldClient_func_712_a(self->worldClient, var1->entityId, (Entity *)var10);
}

static void NCH_handleEntityTeleport(NetHandler *base, Packet34EntityTeleport *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Entity *var2 = NCH_getEntityByID(self, var1->entityId);
    if (var2 != NULL) {
        var2->serverPosX = var1->xPosition;
        var2->serverPosY = var1->yPosition;
        var2->serverPosZ = var1->zPosition;
        double var3 = (double)var2->serverPosX / 32.0;
        double var5 = (double)var2->serverPosY / 32.0 + 1.0 / 64.0;
        double var7 = (double)var2->serverPosZ / 32.0;
        float var9 = (float)(var1->yaw * 360) / 256.0F;
        float var10 = (float)(var1->pitch * 360) / 256.0F;
        var2->vtable->setPositionAndRotation2(var2, var3, var5, var7, var9, var10, 3);
    }
}

static void NCH_handleEntity(NetHandler *base, Packet30Entity *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Entity *var2 = NCH_getEntityByID(self, var1->entityId);
    if (var2 != NULL) {
        var2->serverPosX += var1->xPosition;
        var2->serverPosY += var1->yPosition;
        var2->serverPosZ += var1->zPosition;
        double var3 = (double)var2->serverPosX / 32.0;
        double var5 = (double)var2->serverPosY / 32.0;
        double var7 = (double)var2->serverPosZ / 32.0;
        float var9 = var1->rotating ? (float)(var1->yaw * 360) / 256.0F : var2->rotationYaw;
        float var10 = var1->rotating ? (float)(var1->pitch * 360) / 256.0F : var2->rotationPitch;
        var2->vtable->setPositionAndRotation2(var2, var3, var5, var7, var9, var10, 3);
    }
}

static void NCH_handleDestroyEntity(NetHandler *base, Packet29DestroyEntity *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    WorldClient_removeEntityFromWorld(self->worldClient, var1->entityId);
}

static void NCH_handleFlying(NetHandler *base, Packet10Flying *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    EntityPlayerSP *var2 = self->mc->thePlayer;
    double var3 = ((Entity *)var2)->posX;
    double var5 = ((Entity *)var2)->posY;
    double var7 = ((Entity *)var2)->posZ;
    float var9 = ((Entity *)var2)->rotationYaw;
    float var10 = ((Entity *)var2)->rotationPitch;
    if (var1->moving) {
        var3 = var1->xPosition;
        var5 = var1->yPosition;
        var7 = var1->zPosition;
    }
    if (var1->rotating) {
        var9 = var1->yaw;
        var10 = var1->pitch;
    }
    ((Entity *)var2)->ySize = 0.0F;
    ((Entity *)var2)->motionX = ((Entity *)var2)->motionY = ((Entity *)var2)->motionZ = 0.0;
    Entity_setPositionAndRotation((Entity *)var2, var3, var5, var7, var9, var10);
    Packet *echo = Packet13PlayerLookMove_createWith(
        ((Entity *)var2)->posX, ((Entity *)var2)->boundingBox.minY, ((Entity *)var2)->posY,
        ((Entity *)var2)->posZ, ((Entity *)var2)->rotationYaw, ((Entity *)var2)->rotationPitch,
        ((Entity *)var2)->onGround);
    NetworkManager_addToSendQueue(self->netManager, echo);
    if (!self->field_1210_g) {
        ((Entity *)self->mc->thePlayer)->prevPosX = ((Entity *)self->mc->thePlayer)->posX;
        ((Entity *)self->mc->thePlayer)->prevPosY = ((Entity *)self->mc->thePlayer)->posY;
        ((Entity *)self->mc->thePlayer)->prevPosZ = ((Entity *)self->mc->thePlayer)->posZ;
        self->field_1210_g = 1;
        Minecraft_displayGuiScreen(self->mc, NULL);
    }
}

static void NCH_handlePreChunk(NetHandler *base, Packet50PreChunk *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    WorldClient_doPreChunk(self->worldClient, var1->xPosition, var1->yPosition, var1->mode);
}

static void NCH_handleMultiBlockChange(NetHandler *base, Packet52MultiBlockChange *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Chunk *var2 = ((World *)self->worldClient)
                      ->vtable->getChunkFromChunkCoords((World *)self->worldClient, var1->xPosition,
                                                        var1->zPosition);
    int var3 = var1->xPosition * 16;
    int var4 = var1->zPosition * 16;
    for (int var5 = 0; var5 < var1->size; ++var5) {
        short var6 = var1->coordinateArray[var5];
        int var7 = var1->typeArray[var5] & 255;
        int8_t var8 = (int8_t)var1->metadataArray[var5];
        int var9 = (var6 >> 12) & 15;
        int var10 = (var6 >> 8) & 15;
        int var11 = var6 & 255;
        Chunk_setBlockIDWithMetadata(var2, var9, var11, var10, var7, var8);
        WorldClient_func_711_c(self->worldClient, var9 + var3, var11, var10 + var4, var9 + var3,
                               var11, var10 + var4);
        World_markBlocksDirty((World *)self->worldClient, var9 + var3, var11, var10 + var4,
                              var9 + var3, var11, var10 + var4);
    }
}

static void NCH_handleMapChunk(NetHandler *base, Packet51MapChunk *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    WorldClient_func_711_c(self->worldClient, var1->xPosition, var1->yPosition, var1->zPosition,
                           var1->xPosition + var1->xSize - 1, var1->yPosition + var1->ySize - 1,
                           var1->zPosition + var1->zSize - 1);
    World_setChunkData((World *)self->worldClient, var1->xPosition, var1->yPosition,
                       var1->zPosition, var1->xSize, var1->ySize, var1->zSize, var1->chunk);
}

static void NCH_handleBlockChange(NetHandler *base, Packet53BlockChange *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    WorldClient_func_714_c(self->worldClient, var1->xPosition, var1->yPosition, var1->zPosition,
                           var1->type, var1->metadata);
}

static void NCH_handleKickDisconnect(NetHandler *base, Packet255KickDisconnect *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    NetworkManager_networkShutdown(self->netManager, "disconnect.kicked");
    self->disconnected = 1;

    Minecraft *mc = self->mc;
    Minecraft_changeWorld1(mc, NULL);
    {
        GuiConnectFailed *gui = (GuiConnectFailed *)calloc(1, sizeof(GuiConnectFailed));
        GuiConnectFailed_construct(gui, "disconnect.disconnected", "disconnect.genericReason",
                                   var1->reason);
        Minecraft_displayGuiScreen(mc, (struct GuiScreen *)gui);
    }
}

static void NCH_handleErrorMessage(NetHandler *base, const char *var1, const char *var2) {
    NetClientHandler *self = (NetClientHandler *)base;
    if (!self->disconnected) {
        self->disconnected = 1;

        char var1_copy[256] = {0};
        char var2_copy[512] = {0};
        int has_var2 = var2 != NULL;
        if (var1)
            strncpy(var1_copy, var1, sizeof(var1_copy) - 1);
        if (var2)
            strncpy(var2_copy, var2, sizeof(var2_copy) - 1);
        Minecraft *mc = self->mc;
        Minecraft_changeWorld1(mc, NULL);
        {
            GuiConnectFailed *gui = (GuiConnectFailed *)calloc(1, sizeof(GuiConnectFailed));
            GuiConnectFailed_construct(gui, "disconnect.lost", var1_copy,
                                       has_var2 ? var2_copy : NULL);
            Minecraft_displayGuiScreen(mc, (struct GuiScreen *)gui);
        }
    }
}

static void NCH_handleCollect(NetHandler *base, Packet22Collect *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Entity *var2 = NCH_getEntityByID(self, var1->collectedEntityId);
    Entity *var3 = NCH_getEntityByID(self, var1->collectorEntityId);
    if (var3 == NULL) {
        var3 = (Entity *)self->mc->thePlayer;
    }
    if (var2 != NULL) {
        float pitch =
            ((JavaRandom_nextFloat(&self->rand) - JavaRandom_nextFloat(&self->rand)) * 0.7F +
             1.0F) *
            2.0F;
        World_playSoundAtEntity((World *)self->worldClient, var2, "random.pop", 0.2F, pitch);
        EntityPickupFX *fx = EntityPickupFX_create(self->mc->theWorld, var2, var3, -0.5F);
        EffectRenderer_addEffect(self->mc->effectRenderer, (struct EntityFX *)fx);
        WorldClient_removeEntityFromWorld(self->worldClient, var1->collectedEntityId);
    }
}

static void NCH_handleChat(NetHandler *base, Packet3Chat *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    GuiIngame_addChatMessage(self->mc->ingameGUI, var1->message);
}

static void NCH_handleArmAnimation(NetHandler *base, Packet18Animation *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Entity *var2 = NCH_getEntityByID(self, var1->entityId);
    if (var2 != NULL) {
        EntityPlayer *var3;
        if (var1->animate == 1) {
            var3 = (EntityPlayer *)var2;
            EntityPlayer_swingItem(var3);
        } else if (var1->animate == 2) {
            var2->vtable->performHurtAnimation(var2);
        } else if (var1->animate == 3) {
            var3 = (EntityPlayer *)var2;
            EntityPlayer_wakeUpPlayer(var3, 0, 0, 0);
        } else if (var1->animate == 4) {

            (void)var2;
        }
    }
}

static void NCH_func_22186_a(NetHandler *base, Packet17Sleep *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Entity *var2 = NCH_getEntityByID(self, var1->field_22045_a);
    if (var2 != NULL) {
        if (var1->field_22046_e == 0) {
            EntityPlayer *var3 = (EntityPlayer *)var2;
            EntityPlayer_sleepInBedAt(var3, var1->field_22044_b, var1->field_22048_c,
                                      var1->field_22047_d);
        }
    }
}

static void NCH_handleHandshake(NetHandler *base, Packet2Handshake *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    if (strcmp(var1->username, "-") == 0) {
        Packet1Login *p = (Packet1Login *)Packet1Login_createWith(self->mc->session->username, 14);
        NetClientHandler_addToSendQueue(self, (Packet *)p);
    } else {

        JoinServerArgs *args = (JoinServerArgs *)malloc(sizeof(JoinServerArgs));
        strncpy(args->username, self->mc->session->username, sizeof(args->username) - 1);
        args->username[sizeof(args->username) - 1] = '\0';

        const char *colon1 = strchr(self->mc->session->sessionId, ':');
        if (colon1) {
            colon1++;
            const char *colon2 = strchr(colon1, ':');
            size_t tlen = colon2 ? (size_t)(colon2 - colon1) : strlen(colon1);
            if (tlen >= sizeof(args->accessToken))
                tlen = sizeof(args->accessToken) - 1;
            memcpy(args->accessToken, colon1, tlen);
            args->accessToken[tlen] = '\0';
        } else {

            strncpy(args->accessToken, self->mc->session->sessionId, sizeof(args->accessToken) - 1);
            args->accessToken[sizeof(args->accessToken) - 1] = '\0';
        }
        strncpy(args->uuid, self->mc->session->uuid, sizeof(args->uuid) - 1);
        strncpy(args->serverId, var1->username, sizeof(args->serverId) - 1);
        args->uuid[sizeof(args->uuid) - 1] = '\0';
        args->serverId[sizeof(args->serverId) - 1] = '\0';
        args->nch = self;
        pthread_t t;
        pthread_create(&t, NULL, joinserver_thread, args);
        pthread_detach(t);
    }
}

static void NCH_handleMobSpawn(NetHandler *base, Packet24MobSpawn *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    double var2 = (double)var1->xPosition / 32.0;
    double var4 = (double)var1->yPosition / 32.0;
    double var6 = (double)var1->zPosition / 32.0;
    float var8 = (float)(var1->yaw * 360) / 256.0F;
    float var9 = (float)(var1->pitch * 360) / 256.0F;
    EntityLiving *var10 = (EntityLiving *)EntityList_createEntity(var1->type, self->mc->theWorld);
    var10->base.serverPosX = var1->xPosition;
    var10->base.serverPosY = var1->yPosition;
    var10->base.serverPosZ = var1->zPosition;
    var10->base.entityId = var1->entityId;
    Entity_setPositionAndRotation((Entity *)var10, var2, var4, var6, var8, var9);
    var10->isMultiplayerEntity = 1;
    WorldClient_func_712_a(self->worldClient, var1->entityId, (Entity *)var10);
    DataWatcher *var11 = Packet24MobSpawn_getMetadata(var1);
    if (var11 != NULL) {

        DataWatcher_updateWatchedObjectsFromList(Entity_getDataWatcher((Entity *)var10), var11);
    }
}

static void NCH_handleUpdateTime(NetHandler *base, Packet4UpdateTime *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    World_setWorldTime(self->mc->theWorld, var1->time);
}

static void NCH_handleSpawnPosition(NetHandler *base, Packet6SpawnPosition *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    ChunkCoordinates cc;
    cc.x = var1->xPosition;
    cc.y = var1->yPosition;
    cc.z = var1->zPosition;
    EntityPlayer_setPlayerSpawnCoordinate((EntityPlayer *)self->mc->thePlayer, &cc);
    WorldInfo_setSpawn(World_getWorldInfo(self->mc->theWorld), var1->xPosition, var1->yPosition,
                       var1->zPosition);
}

static void NCH_func_6497_a(NetHandler *base, Packet39AttachEntity *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Entity *var2 = NCH_getEntityByID(self, var1->entityId);
    Entity *var3 = NCH_getEntityByID(self, var1->vehicleEntityId);
    if (var1->entityId == self->mc->thePlayer->base.base.base.entityId) {
        var2 = (Entity *)self->mc->thePlayer;
    }
    if (var2 != NULL) {
        Entity_mountEntity(var2, var3);
    }
}

static void NCH_func_9447_a(NetHandler *base, Packet38EntityStatus *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Entity *var2 = NCH_getEntityByID(self, var1->entityId);
    if (var2 != NULL) {
        var2->vtable->handleHealthUpdate(var2, var1->entityStatus);
    }
}

static void NCH_handleHealth(NetHandler *base, Packet8UpdateHealth *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    EntityClientPlayerMP_setHealth((EntityLiving *)self->mc->thePlayer, var1->healthMP);
}

static void NCH_func_9448_a(NetHandler *base, Packet9Respawn *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    if (var1->field_28048_a != ((EntityPlayer *)self->mc->thePlayer)->dimension) {
        self->field_1210_g = 0;
        self->worldClient = WorldClient_create(
            self, WorldInfo_getRandomSeed(World_getWorldInfo((World *)self->worldClient)),
            var1->field_28048_a);
        self->worldClient->base.multiplayerWorld = 1;
        Minecraft_changeWorld1(self->mc, (World *)self->worldClient);
        ((EntityPlayer *)self->mc->thePlayer)->dimension = var1->field_28048_a;
        {
            GuiDownloadTerrain *gui = (GuiDownloadTerrain *)calloc(1, sizeof(GuiDownloadTerrain));
            GuiDownloadTerrain_construct(gui, self);
            Minecraft_displayGuiScreen(self->mc, (struct GuiScreen *)gui);
        }
    }

    Minecraft_respawn(self->mc, 1, var1->field_28048_a);
}

static void NCH_func_12245_a(NetHandler *base, Packet60Explosion *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Explosion *var2 = Explosion_create(self->mc->theWorld, NULL, var1->explosionX, var1->explosionY,
                                       var1->explosionZ, var1->explosionSize);
    var2->destroyedBlockPositions = var1->destroyedBlocks;
    Explosion_doExplosionB(var2, 1);
}

static void NCH_func_20087_a(NetHandler *base, Packet100OpenWindow *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    if (var1->inventoryType == 0) {
        InventoryBasic *var2 = InventoryBasic_create(var1->windowTitle, var1->slotsCount);
        EntityPlayer_displayGUIChest((EntityPlayer *)self->mc->thePlayer,
                                     (struct IInventory *)var2);
        self->mc->thePlayer->base.craftingInventory->windowId = var1->windowId;
    } else if (var1->inventoryType == 2) {
        TileEntityFurnace *var3 = TileEntityFurnace_create();
        EntityPlayer_displayGUIFurnace((EntityPlayer *)self->mc->thePlayer, var3);
        self->mc->thePlayer->base.craftingInventory->windowId = var1->windowId;
    } else if (var1->inventoryType == 3) {
        TileEntityDispenser *var4 = TileEntityDispenser_create();
        EntityPlayer_displayGUIDispenser((EntityPlayer *)self->mc->thePlayer, var4);
        self->mc->thePlayer->base.craftingInventory->windowId = var1->windowId;
    } else if (var1->inventoryType == 1) {
        EntityPlayerSP *var5 = self->mc->thePlayer;
        EntityPlayer_displayWorkbenchGUI((EntityPlayer *)var5,
                                         MathHelper_floor_double(((Entity *)var5)->posX),
                                         MathHelper_floor_double(((Entity *)var5)->posY),
                                         MathHelper_floor_double(((Entity *)var5)->posZ));
        self->mc->thePlayer->base.craftingInventory->windowId = var1->windowId;
    }
}

static void NCH_func_20088_a(NetHandler *base, Packet103SetSlot *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    if (var1->windowId == -1) {
        InventoryPlayer_setItemStack(&self->mc->thePlayer->base.inventory, var1->myItemStack);
    } else if (var1->windowId == 0 && var1->itemSlot >= 36 && var1->itemSlot < 45) {
        ItemStack *var2 =
            Container_getStackInSlot(self->mc->thePlayer->base.inventorySlots, var1->itemSlot);
        if (var1->myItemStack != NULL &&
            (var2 == NULL || var2->stackSize < var1->myItemStack->stackSize)) {
            var1->myItemStack->animationsToGo = 5;
        }
        Container_putStackInSlot(self->mc->thePlayer->base.inventorySlots, var1->itemSlot,
                                 var1->myItemStack);
    } else if (var1->windowId == self->mc->thePlayer->base.craftingInventory->windowId) {
        Container_putStackInSlot(self->mc->thePlayer->base.craftingInventory, var1->itemSlot,
                                 var1->myItemStack);
    }
}

static void NCH_func_20089_a(NetHandler *base, Packet106Transaction *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Container *var2 = NULL;
    if (var1->windowId == 0) {
        var2 = self->mc->thePlayer->base.inventorySlots;
    } else if (var1->windowId == self->mc->thePlayer->base.craftingInventory->windowId) {
        var2 = self->mc->thePlayer->base.craftingInventory;
    }
    if (var2 != NULL) {
        if (var1->field_20030_c) {
            Container_func_20113_a(var2, var1->field_20028_b);
        } else {
            Container_func_20110_b(var2, var1->field_20028_b);
            NetClientHandler_addToSendQueue(
                self, Packet106Transaction_createWith(var1->windowId, var1->field_20028_b, 1));
        }
    }
}

static void NCH_func_20094_a(NetHandler *base, Packet104WindowItems *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    if (var1->windowId == 0) {
        Container_putStacksInSlots(self->mc->thePlayer->base.inventorySlots, var1->itemStack,
                                   var1->itemStackLen);
    } else if (var1->windowId == self->mc->thePlayer->base.craftingInventory->windowId) {
        Container_putStacksInSlots(self->mc->thePlayer->base.craftingInventory, var1->itemStack,
                                   var1->itemStackLen);
    }
}

static void NCH_handleSignUpdate(NetHandler *base, Packet130UpdateSign *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    if (World_blockExists(self->mc->theWorld, var1->xPosition, var1->yPosition, var1->zPosition)) {
        TileEntity *var2 = World_getBlockTileEntity(self->mc->theWorld, var1->xPosition,
                                                    var1->yPosition, var1->zPosition);
        if (var2 != NULL && var2->tileEntityTypeId == TE_TYPE_SIGN) {
            TileEntitySign *var3 = (TileEntitySign *)var2;
            for (int var4 = 0; var4 < 4; ++var4) {
                strncpy(var3->signText[var4], var1->signLines[var4] ? var1->signLines[var4] : "",
                        SIGN_LINE_LEN - 1);
                var3->signText[var4][SIGN_LINE_LEN - 1] = '\0';
            }
            TileEntity_onInventoryChanged(&var3->base);
        }
    }
}

static void NCH_func_20090_a(NetHandler *base, Packet105UpdateProgressbar *var1) {
    NetClientHandler *self = (NetClientHandler *)base;

    self->base.vtable->registerPacket(&self->base, (Packet *)var1);
    if (self->mc->thePlayer->base.craftingInventory != NULL &&
        self->mc->thePlayer->base.craftingInventory->windowId == var1->windowId) {
        self->mc->thePlayer->base.craftingInventory->vtable->func_20112_a(
            self->mc->thePlayer->base.craftingInventory, var1->progressBar, var1->progressBarValue);
    }
}

static void NCH_handlePlayerInventory(NetHandler *base, Packet5PlayerInventory *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    Entity *var2 = NCH_getEntityByID(self, var1->entityID);
    if (var2 != NULL) {

        EntityOtherPlayerMP_outfitWithItem((EntityOtherPlayerMP *)var2, var1->slot, var1->itemID,
                                           var1->itemDamage);
    }
}

static void NCH_func_20092_a(NetHandler *base, Packet101CloseWindow *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    (void)var1;
    EntityPlayer_closeScreen((EntityPlayer *)self->mc->thePlayer);
}

static void NCH_handleNotePlay(NetHandler *base, Packet54PlayNoteBlock *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    World_playNoteAt(self->mc->theWorld, var1->xLocation, var1->yLocation, var1->zLocation,
                     var1->instrumentType, var1->pitch);
}

static void NCH_func_25118_a(NetHandler *base, Packet70Bed *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    int var2 = var1->field_25019_b;
    if (var2 >= 0 && var2 < 3 && Packet70Bed_field_25020_a[var2] != NULL) {
        EntityPlayer_addChatMessage((EntityPlayer *)self->mc->thePlayer,
                                    Packet70Bed_field_25020_a[var2]);
    }
    if (var2 == 1) {
        WorldInfo_setRaining(World_getWorldInfo((World *)self->worldClient), 1);
        World_func_27158_h((World *)self->worldClient, 1.0F);
    } else if (var2 == 2) {
        WorldInfo_setRaining(World_getWorldInfo((World *)self->worldClient), 0);
        World_func_27158_h((World *)self->worldClient, 0.0F);
    }
}

static void NCH_func_28116_a(NetHandler *base, Packet131MapData *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    if (var1->field_28055_a == Item_mapItem->shiftedIndex) {

        MapData_func_28171_a(ItemMap_func_28013_a(var1->field_28054_b, self->mc->theWorld),
                             (int8_t *)var1->field_28056_c, var1->field_28056_c_len);
    } else {
        printf("Unknown itemid: %d\n", var1->field_28054_b);
    }
}

static void NCH_func_28115_a(NetHandler *base, Packet61DoorChange *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    World_func_28106_e(self->mc->theWorld, var1->field_28050_a, var1->field_28053_c,
                       var1->field_28052_d, var1->field_28051_e, var1->field_28049_b);
}

static void NCH_func_27245_a(NetHandler *base, Packet200Statistic *var1) {
    NetClientHandler *self = (NetClientHandler *)base;
    EntityClientPlayerMP_func_27027_b((EntityClientPlayerMP *)self->mc->thePlayer,
                                      StatList_func_27361_a(var1->field_27052_a),
                                      var1->field_27051_b);
}

static int NCH_isServerHandler(NetHandler *base) {
    (void)base;
    return 0;
}

static void NCH_initVtable(void) {
    s_vtable = NetHandler_defaultVtable;
    s_vtable.handleLogin = NCH_handleLogin;
    s_vtable.handlePickupSpawn = NCH_handlePickupSpawn;
    s_vtable.handleVehicleSpawn = NCH_handleVehicleSpawn;
    s_vtable.handleWeather = NCH_handleWeather;
    s_vtable.func_21146_a = NCH_func_21146_a;
    s_vtable.func_6498_a = NCH_func_6498_a;
    s_vtable.func_21148_a = NCH_func_21148_a;
    s_vtable.handleNamedEntitySpawn = NCH_handleNamedEntitySpawn;
    s_vtable.handleEntityTeleport = NCH_handleEntityTeleport;
    s_vtable.handleEntity = NCH_handleEntity;
    s_vtable.handleDestroyEntity = NCH_handleDestroyEntity;
    s_vtable.handleFlying = NCH_handleFlying;
    s_vtable.handlePreChunk = NCH_handlePreChunk;
    s_vtable.handleMultiBlockChange = NCH_handleMultiBlockChange;
    s_vtable.handleMapChunk = NCH_handleMapChunk;
    s_vtable.handleBlockChange = NCH_handleBlockChange;
    s_vtable.handleKickDisconnect = NCH_handleKickDisconnect;
    s_vtable.handleErrorMessage = NCH_handleErrorMessage;
    s_vtable.handleCollect = NCH_handleCollect;
    s_vtable.handleChat = NCH_handleChat;
    s_vtable.handleArmAnimation = NCH_handleArmAnimation;
    s_vtable.func_22186_a = NCH_func_22186_a;
    s_vtable.handleHandshake = NCH_handleHandshake;
    s_vtable.handleMobSpawn = NCH_handleMobSpawn;
    s_vtable.handleUpdateTime = NCH_handleUpdateTime;
    s_vtable.handleSpawnPosition = NCH_handleSpawnPosition;
    s_vtable.func_6497_a = NCH_func_6497_a;
    s_vtable.func_9447_a = NCH_func_9447_a;
    s_vtable.handleHealth = NCH_handleHealth;
    s_vtable.func_9448_a = NCH_func_9448_a;
    s_vtable.func_12245_a = NCH_func_12245_a;
    s_vtable.func_20087_a = NCH_func_20087_a;
    s_vtable.func_20088_a = NCH_func_20088_a;
    s_vtable.func_20089_a = NCH_func_20089_a;
    s_vtable.func_20094_a = NCH_func_20094_a;
    s_vtable.handleSignUpdate = NCH_handleSignUpdate;
    s_vtable.func_20090_a = NCH_func_20090_a;
    s_vtable.handlePlayerInventory = NCH_handlePlayerInventory;
    s_vtable.func_20092_a = NCH_func_20092_a;
    s_vtable.handleNotePlay = NCH_handleNotePlay;
    s_vtable.func_25118_a = NCH_func_25118_a;
    s_vtable.func_28116_a = NCH_func_28116_a;
    s_vtable.func_28115_a = NCH_func_28115_a;
    s_vtable.func_27245_a = NCH_func_27245_a;
    s_vtable.isServerHandler = NCH_isServerHandler;
}

NetClientHandler *NetClientHandler_create(Minecraft *var1, const char *var2, int var3) {
    if (!s_vtableInit) {
        NCH_initVtable();
        s_vtableInit = 1;
    }
    NetClientHandler *self = (NetClientHandler *)calloc(1, sizeof(NetClientHandler));
    self->base.vtable = &s_vtable;
    self->mc = var1;

    self->netManager = NetworkManager_create(var2, var3, &self->base);
    MapStorage_init(&self->field_28118_b, NULL);
    JavaRandom_setSeed(&self->rand, 0);
    return self;
}

void NetClientHandler_processReadPackets(NetClientHandler *self) {
    if (!self->disconnected) {
        NetworkManager_processReadPackets(self->netManager);
    }
    NetworkManager_wakeThreads(self->netManager);
}

void NetClientHandler_func_28117_a(NetClientHandler *self, Packet *var1) {
    if (!self->disconnected) {
        NetworkManager_addToSendQueue(self->netManager, var1);
        NetworkManager_func_28142_c(self->netManager);
    }
}

void NetClientHandler_addToSendQueue(NetClientHandler *self, Packet *var1) {
    if (!self->disconnected) {
        NetworkManager_addToSendQueue(self->netManager, var1);
    }
}

void NetClientHandler_disconnect(NetClientHandler *self) {
    self->disconnected = 1;
    NetworkManager_wakeThreads(self->netManager);
    NetworkManager_networkShutdown(self->netManager, "disconnect.closed");
}

int NetClientHandler_isServerHandler(NetClientHandler *self) {
    (void)self;
    return 0;
}

void NetClientHandler_destroy(NetClientHandler *self) {
    if (self->netManager) {
        NetworkManager_destroy(self->netManager);
        self->netManager = NULL;
    }
    MapStorage_destroy(&self->field_28118_b);
    free(self);
}
