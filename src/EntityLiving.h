#ifndef ENTITY_LIVING_H
#define ENTITY_LIVING_H

#include "Entity.h"
#include "ItemStack.h"
#include "MovingObjectPosition.h"

#define EQUIPMENT_SLOT_MAINHAND 0
#define EQUIPMENT_SLOT_BOOTS 1
#define EQUIPMENT_SLOT_LEGGINGS 2
#define EQUIPMENT_SLOT_CHESTPLATE 3
#define EQUIPMENT_SLOT_HELMET 4
#define EQUIPMENT_NUM_SLOTS 5

struct EntityLiving;
typedef struct EntityLiving EntityLiving;

typedef struct EntityLivingVtable {
    EntityVtable base;

    int (*getMaxHealth)(EntityLiving *self);
    void (*heal)(EntityLiving *self, int amount);
    int (*getHealth)(EntityLiving *self);
    void (*setHealth)(EntityLiving *self, int health);
    void (*knockBack)(EntityLiving *self, Entity *source, int damage, double vx, double vz);
    void (*damageEntity)(EntityLiving *self, Entity *source, int amount);
    void (*onDeath)(EntityLiving *self, Entity *cause);
    void (*onLivingUpdate)(EntityLiving *self);
    void (*moveEntityWithHeading)(EntityLiving *self, float strafe, float forward);
    int (*isOnLadder)(EntityLiving *self);
    void (*updatePlayerActionState)(EntityLiving *self);
    const char *(*getLivingSound)(EntityLiving *self);
    const char *(*getHurtSound)(EntityLiving *self);
    const char *(*getDeathSound)(EntityLiving *self);
    int (*getDropItemId)(EntityLiving *self);
    float (*getSoundVolume)(EntityLiving *self);
    int (*getCanSpawnHere)(EntityLiving *self);
    int (*getMaxSpawnedInChunk)(EntityLiving *self);

    ItemStack *(*getHeldItem)(EntityLiving *self);
    ItemStack *(*getEquipmentInSlot)(EntityLiving *self, int slot);
    void (*setCurrentItemOrArmor)(EntityLiving *self, int slot, ItemStack *stack);
    void (*dropEquipment)(EntityLiving *self, int recentlyHit, int lootingLevel);
    int (*isEntityUndead)(EntityLiving *self);

    int (*canBreatheUnderwater)(EntityLiving *self);

    int (*getTalkInterval)(EntityLiving *self);

    void (*dropFewItems)(EntityLiving *self);

    const char *(*getEntityTexture)(EntityLiving *self);

    int (*canDespawn)(EntityLiving *self);

    int (*func_25026_x)(EntityLiving *self);

    void (*jump)(EntityLiving *self);

    int (*isMovementBlocked)(EntityLiving *self);

    void (*onEntityDeath)(EntityLiving *self);

    int (*isPlayerSleeping)(EntityLiving *self);

    int (*getItemIcon)(EntityLiving *self, ItemStack *var1);
} EntityLivingVtable;

struct EntityLiving {
    Entity base;

    int health;
    int prevHealth;

    int heartsHalvesLife;

    int field_9346_af;

    int unused_flag;

    int hurtTime;
    int maxHurtTime;
    int deathTime;
    float attackedAtYaw;

    float moveForward;
    float moveStrafing;
    float rotationYawHead;
    float prevRotationYawHead;
    float rotationYawHeadResting;

    float randomYawVelocity;

    float defaultPitch;

    float moveSpeed;

    int isSwingInProgress;
    int swingProgressInt;
    float swingProgress;

    ItemStack equipment[EQUIPMENT_NUM_SLOTS];
    int equipmentHas[EQUIPMENT_NUM_SLOTS];

    int attackTime;
    int ticksLived;
    int entityAge;

    int isJumping;
    float jumpMovementFactor;
    float field_35143_bh;

    float field_705_Q;
    float field_704_R;
    float field_703_S;

    const char *texture;

    float field_9365_p;

    float field_9363_r;

    float field_9362_u;

    float field_9361_v;

    float field_9360_w;

    float field_9359_x;

    int field_9358_y;

    int field_9355_A;

    float field_9353_B;

    const char *field_9351_C;

    float field_9349_D;

    int scoreValue;

    float field_9345_F;

    float field_9348_ae;

    int field_9326_T;

    float field_9325_U;

    float renderYawOffset;

    float prevRenderYawOffset;

    float prevSwingProgress;

    float cameraPitch;

    float field_9328_R;

    int isMultiplayerEntity;

    int livingSoundTime;

    int newPosRotationIncrements;

    double newPosX, newPosY, newPosZ;

    double newRotationYaw, newRotationPitch;

    Entity *currentTarget;

    int numTicksToChaseTarget;
};

void EntityLiving_construct(EntityLiving *self, World *world);

void EntityLiving_initVtable(EntityLivingVtable *vt);
extern EntityLivingVtable EntityLiving_defaultVtable;

void EntityLiving_onEntityUpdateBase(EntityLiving *self);

void EntityLiving_onLivingUpdateBase(EntityLiving *self);

void EntityLiving_jump(EntityLiving *self);

void EntityLiving_moveEntityWithHeading(EntityLiving *self, float strafe, float forward);
float EntityLiving_getAIMoveSpeed(EntityLiving *self);

int EntityLiving_canEntityBeSeen(EntityLiving *self, Entity *other);

void EntityLiving_func_27021_X(EntityLiving *self);

void EntityLiving_faceEntity(EntityLiving *self, Entity *entity, float yawSpeed, float pitchSpeed);

int EntityLiving_hasCurrentTarget(EntityLiving *self);

Entity *EntityLiving_getCurrentTarget(EntityLiving *self);

void EntityLiving_playLivingSound(EntityLiving *self);

float EntityLiving_getSwingProgress(EntityLiving *self, float var1);

int EntityLiving_getHealth(EntityLiving *self);
void EntityLiving_setHealth(EntityLiving *self, int health);
int EntityLiving_getMaxHealth(EntityLiving *self);
void EntityLiving_heal(EntityLiving *self, int amount);
void EntityLiving_knockBack(EntityLiving *self, Entity *source, int damage, double vx, double vz);
ItemStack *EntityLiving_getHeldItem(EntityLiving *self);
ItemStack *EntityLiving_getEquipmentInSlot(EntityLiving *self, int slot);
void EntityLiving_setCurrentItemOrArmor(EntityLiving *self, int slot, ItemStack *stack);

void EntityLiving_spawnExplosionParticle(EntityLiving *self);

Vec3D *EntityLiving_getLook(EntityLiving *self, float var1);

Vec3D *EntityLiving_getPosition(EntityLiving *self, float var1);

MovingObjectPosition *EntityLiving_rayTrace(EntityLiving *self, double var1, float var3);

void EntityLiving_kill(EntityLiving *self);

#endif
