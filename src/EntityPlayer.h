#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H

#include "EntityLiving.h"
#include "InventoryPlayer.h"
#include "EnumStatus.h"
#include "ChunkCoordinates.h"

struct Block;
struct IInventory;
struct StatBase;

struct EntityPlayer;
typedef struct EntityPlayer EntityPlayer;

typedef struct EntityPlayerVtable {
    EntityLivingVtable base;

    void (*addItemStackToInventory)(EntityPlayer *self, ItemStack *stack);
    void (*dropPlayerItem)(EntityPlayer *self, ItemStack *stack);
    void (*addScore)(EntityPlayer *self, int score);
    void (*addExperience)(EntityPlayer *self, int xp);
    int (*isPlayerSleeping)(EntityPlayer *self);
    int (*isCreative)(EntityPlayer *self);
    void (*damageArmor)(EntityPlayer *self, int damage);
    void (*swingItem)(EntityPlayer *self);

    void (*resetHeight)(EntityPlayer *self);

    void (*onItemPickup)(EntityPlayer *self, Entity *var1, int var2);

    void (*addStat)(EntityPlayer *self, struct StatBase *var1, int var2);

    void (*respawnPlayer)(EntityPlayer *self);
} EntityPlayerVtable;

struct EntityPlayer {
    EntityLiving base;

    InventoryPlayer inventory;

    struct Container *inventorySlots;

    struct Container *craftingInventory;

    int field_9371_f;
    int score;
    int experienceLevel;
    int experienceTotal;
    float experience;

    float prevCameraYaw;
    float cameraYaw;

    int sleeping;
    int sleepTimer;

    ChunkCoordinates bedChunkCoordinates;

    float timeInPortal;
    float prevTimeInPortal;
    int timeUntilPortal;
    int inPortal;

    float field_775_e;
    float field_774_f;

    int isCreativeMode;

    int dimension;

    char username[64];

    char playerCloakUrl[128];

    double field_20066_r;
    double field_20065_s;
    double field_20064_t;

    double field_20063_u;
    double field_20062_v;
    double field_20061_w;

    float field_22062_y;

    float field_22063_x;
    float field_22061_z;

    struct EntityFish *fishEntity;

    int damageRemainder;

    int hasBedChunkCoordinates;

    int hasPlayerSpawnCoordinate;
    ChunkCoordinates playerSpawnCoordinate;

    int hasStartMinecartRidingCoordinate;
    ChunkCoordinates startMinecartRidingCoordinate;
};

void EntityPlayer_construct(EntityPlayer *self, World *world);
EntityPlayer *EntityPlayer_create(World *world);

void EntityPlayer_initVtable(EntityPlayerVtable *vt);
extern EntityPlayerVtable EntityPlayer_defaultVtable;

void EntityPlayer_onUpdate(Entity *self);

void EntityPlayer_onLivingUpdateBase(EntityLiving *self);

void EntityPlayer_updatePlayerActionStateBase(EntityLiving *self);

void EntityPlayer_wakeUpPlayer(EntityPlayer *self, int var1, int var2, int var3);

ChunkCoordinates *EntityPlayer_getPlayerSpawnCoordinate(EntityPlayer *self);

void EntityPlayer_setPlayerSpawnCoordinate(EntityPlayer *self, ChunkCoordinates *var1);

ChunkCoordinates *EntityPlayer_func_25060_a(struct World *var0, ChunkCoordinates *var1);

void EntityPlayer_triggerAchievement(EntityPlayer *self, struct StatBase *var1);

void EntityPlayer_closeScreen(EntityPlayer *self);

int EntityPlayer_isPlayerFullyAsleep(EntityPlayer *self);

void EntityPlayer_updateCloak(EntityPlayer *self);

void EntityPlayer_dropCurrentItem(EntityPlayer *self);

void EntityPlayer_dropPlayerItemWithRandomChoice(EntityPlayer *self, ItemStack *var1, int var2);

float EntityPlayer_getCurrentPlayerStrVsBlock(EntityPlayer *self, struct Block *var1);

void EntityPlayer_setInPortal(EntityPlayer *self);

void EntityPlayer_onKillEntity(EntityPlayer *self, EntityLiving *var1);

ItemStack *EntityPlayer_getHeldItem(EntityPlayer *self);
void EntityPlayer_addItemStackToInventory(EntityPlayer *self, ItemStack *stack);
void EntityPlayer_dropPlayerItem(EntityPlayer *self, ItemStack *stack);
void EntityPlayer_addScore(EntityPlayer *self, int score);
int EntityPlayer_getScore(EntityPlayer *self);
void EntityPlayer_respawnPlayer(EntityPlayer *self);
void EntityPlayer_addExperience(EntityPlayer *self, int xp);
int EntityPlayer_isPlayerSleeping(EntityPlayer *self);
int EntityPlayer_isCreative(EntityPlayer *self);
void EntityPlayer_damageArmor(EntityPlayer *self, int damage);
void EntityPlayer_swingItem(EntityPlayer *self);
ItemStack *EntityPlayer_getCurrentEquippedItem(EntityPlayer *self);
void EntityPlayer_destroyCurrentEquippedItem(EntityPlayer *self);

void EntityPlayer_addStat(EntityPlayer *self, struct StatBase *var1, int var2);
int EntityPlayer_canHarvestBlock(EntityPlayer *self, struct Block *block);
void EntityPlayer_attackTargetEntityWithCurrentItem(EntityPlayer *self, Entity *target);
void EntityPlayer_useCurrentItemOnEntity(EntityPlayer *self, Entity *entity);
float EntityPlayer_getArmorBonusPoints(EntityPlayer *self);

EnumStatus EntityPlayer_sleepInBedAt(EntityPlayer *self, int x, int y, int z);

void EntityPlayer_onItemPickup(EntityPlayer *self, Entity *var1, int var2);

void EntityPlayer_addChatMessage(EntityPlayer *self, const char *message);

void EntityPlayer_displayGUIChest(EntityPlayer *self, struct IInventory *inventory);
struct TileEntityDispenser;

void EntityPlayer_displayGUIDispenser(EntityPlayer *self, struct TileEntityDispenser *dispenser);
struct TileEntitySign;

void EntityPlayer_displayGUIEditSign(EntityPlayer *self, struct TileEntitySign *var1);
struct TileEntityFurnace;

void EntityPlayer_displayGUIFurnace(EntityPlayer *self, struct TileEntityFurnace *var1);

void EntityPlayer_displayWorkbenchGUI(EntityPlayer *self, int var1, int var2, int var3);

float EntityPlayer_getBedOrientationInDegrees(EntityPlayer *self);

int EntityPlayer_getPlayerArmorValue(EntityPlayer *self);

int EntityPlayer_func_22060_M(EntityPlayer *self);

void EntityPlayer_onItemStackChanged(EntityPlayer *self, ItemStack *var1);

#endif
