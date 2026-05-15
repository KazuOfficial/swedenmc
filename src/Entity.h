#ifndef ENTITY_H
#define ENTITY_H

#include "GameForward.h"
#include "Material.h"
#include "AxisAlignedBB.h"
#include "JavaRandom.h"
#include "DataWatcher.h"
#include "NBTTagCompound.h"
#include "Vec3D.h"

struct EntityItem;
struct Material;
struct EntityLightningBolt;
struct Block;

typedef struct EntityVtable EntityVtable;
typedef struct Entity Entity;

struct EntityVtable {

    void (*entityInit)(Entity *self);
    void (*onUpdate)(Entity *self);
    void (*onEntityUpdate)(Entity *self);

    void (*moveEntity)(Entity *self, double motX, double motY, double motZ);
    void (*setPosition)(Entity *self, double x, double y, double z);
    void (*setLocationAndAngles)(Entity *self, double x, double y, double z, float yaw,
                                 float pitch);
    void (*setPositionAndRotation2)(Entity *self, double x, double y, double z, float yaw,
                                    float pitch, int steps);
    void (*setVelocity)(Entity *self, double vx, double vy, double vz);
    void (*addVelocity)(Entity *self, double dx, double dy, double dz);

    AxisAlignedBB *(*getBoundingBox)(Entity *self);
    AxisAlignedBB *(*getCollisionBox)(Entity *self, Entity *other);
    void (*applyEntityCollision)(Entity *self, Entity *other);
    void (*onCollideWithPlayer)(Entity *self, EntityPlayer *player);

    int (*attackEntityFrom)(Entity *self, Entity *source, int damage);

    int (*interact)(Entity *self, EntityPlayer *player);

    void (*writeEntityToNBT)(Entity *self, NBTTagCompound *tag);
    void (*readEntityFromNBT)(Entity *self, NBTTagCompound *tag);

    float (*getShadowSize)(Entity *self);
    float (*getEyeHeight)(Entity *self);
    double (*getMountedYOffset)(Entity *self);
    double (*getYOffset)(Entity *self);
    int (*canBeCollidedWith)(Entity *self);
    float (*getCollisionBorderSize)(Entity *self);
    int (*canBePushed)(Entity *self);
    int (*canTriggerWalking)(Entity *self);
    int (*isEntityAlive)(Entity *self);
    void (*handleHealthUpdate)(Entity *self, int8_t data);
    void (*performHurtAnimation)(Entity *self);
    void (*onStruckByLightning)(Entity *self, struct EntityLightningBolt *bolt);
    void (*fall)(Entity *self, float distance);
    void (*setEntityDead)(Entity *self);
    void (*kill)(Entity *self);
    double (*getDistanceSq)(Entity *self, double x, double y, double z);
    int (*isInRangeToRenderDist)(Entity *self, double distSq);
    int (*isInRangeToRenderVec3D)(Entity *self, Vec3D *vec);
    Vec3D *(*getLookVec)(Entity *self);
    void (*setInPortal)(Entity *self);
    void (*updateRiderPosition)(Entity *self);
    void (*updateRidden)(Entity *self);
    const char *(*getEntityString)(Entity *self);

    float (*getEntityBrightness)(Entity *self, float partialTick);

    int (*handleWaterMovement)(Entity *self);

    void (*dealFireDamage)(Entity *self, int amount);

    int (*isInWater)(Entity *self);

    void (*preparePlayerToSpawn)(Entity *self);

    int (*isEntityInsideOpaqueBlock)(Entity *self);

    int (*isSneaking)(Entity *self);
};

struct Entity {
    const EntityVtable *vtable;

    int entityId;
    double renderDistanceWeight;
    int preventEntitySpawning;
    Entity *riddenByEntity;
    Entity *ridingEntity;
    World *worldObj;

    double prevPosX, prevPosY, prevPosZ;
    double posX, posY, posZ;
    double motionX, motionY, motionZ;

    float rotationYaw, rotationPitch;
    float prevRotationYaw, prevRotationPitch;

    AxisAlignedBB boundingBox;

    int onGround;
    int isCollidedHorizontally;
    int isCollidedVertically;
    int isCollided;
    int beenAttacked;
    int isInWeb;
    int field_9293_aM;
    int isDead;

    float yOffset;
    float width, height;
    float prevDistanceWalkedModified;
    float distanceWalkedModified;
    float fallDistance;
    int nextStepDistance;

    double lastTickPosX, lastTickPosY, lastTickPosZ;
    float ySize;
    float stepHeight;
    int noClip;
    float entityCollisionReduction;

    JavaRandom rand;

    int ticksExisted;
    int fireResistance;
    int fire;
    int maxAir;
    int inWater;
    int heartsLife;
    int air;
    int isFirstUpdate;
    int isImmuneToFire;

    DataWatcher dataWatcher;

    float entityBrightness;
    double entityRiderPitchDelta;
    double entityRiderYawDelta;
    int addedToChunk;
    int chunkCoordX, chunkCoordY, chunkCoordZ;
    int serverPosX, serverPosY, serverPosZ;
    int ignoreFrustumCheck;

    int isPlayer;
    int isLiving;
    int isMob;
    int isArrow;
    int isSkeleton;
    int isSheep;
    int isWolf;
    int isCreeper;
    int isGhast;
    int isAnimal;
    int isWaterMob;
    int isSpider;
    int isMinecart;
    int isPlayerSP;
    int isLightningBolt;
    int isPig;
    int isCow;
    int isZombie;
    int isChicken;
    int isSlime;
    int isSquid;
    int isGiantZombie;
    int isSnowball;
    int isEgg;
    int isFireball;
    int isItemEntity;
    int isTNTPrimed;
    int isFallingSand;
    int isPainting;
    int isBoat;
    int isFish;
    int isClientPlayerMP;

    char skinUrl[128];
    char cloakUrl[128];
};

extern int Entity_nextEntityID;

extern EntityVtable Entity_defaultVtable;

void Entity_construct(Entity *self, World *world);

void Entity_destroy(Entity *self);

void Entity_flush_destroy_queue(void);

void Entity_flush_destroy_queue_for_world(struct World *world);

void Entity_flush_destroy_queue_force(void);

void Entity_registerViewEntityRef(struct EntityLiving **ref);
void Entity_registerPlayerRef(struct EntityLiving **ref);

void Entity_initVtable(EntityVtable *vt);

void Entity_setEntityDead(Entity *self);
void Entity_setSize(Entity *self, float w, float h);
void Entity_setRotation(Entity *self, float yaw, float pitch);
void Entity_setPositionAndRotation(Entity *self, double x, double y, double z, float yaw,
                                   float pitch);
void Entity_setPositionAndRotation2(Entity *self, double x, double y, double z, float yaw,
                                    float pitch, int steps);
void Entity_addVelocity(Entity *self, double dx, double dy, double dz);
void Entity_setVelocity(Entity *self, double vx, double vy, double vz);
void Entity_mountEntity(Entity *self, Entity *mount);
void Entity_updateRidden(Entity *self);
int Entity_isWet(Entity *self);
int Entity_isInWater(Entity *self);
int Entity_handleWaterMovement(Entity *self);
int Entity_handleLavaMovement(Entity *self);
int Entity_isInsideOfMaterial(Entity *self, Material *mat);
void Entity_moveFlying(Entity *self, float strafe, float forward, float speed);
float Entity_getEntityBrightness(Entity *self, float partialTick);

Vec3D *Entity_getLook(Entity *self, float partialTick);
void Entity_preparePlayerToSpawn(Entity *self);
void Entity_preparePlayerToSpawn_base(Entity *self);
int Entity_isEntityInsideOpaqueBlock(Entity *self);
int Entity_isEntityInsideOpaqueBlock_base(Entity *self);
int Entity_isOffsetPositionInLiquid(Entity *self, double dx, double dy, double dz);
float Entity_getDistanceToEntity(Entity *self, Entity *other);
double Entity_getDistanceSqToEntity(Entity *self, Entity *other);
double Entity_getDistance(Entity *self, double x, double y, double z);
void Entity_writeToNBT(Entity *self, NBTTagCompound *tag);
void Entity_readFromNBT(Entity *self, NBTTagCompound *tag);
int Entity_addEntityID(Entity *self, NBTTagCompound *tag);
void Entity_addToPlayerScore(Entity *self, Entity *scorer, int amount);
int Entity_isBurning(Entity *self);
int Entity_isRiding(Entity *self);
int Entity_isSneaking(Entity *self);
int Entity_getEntityFlag(Entity *self, int flag);
void Entity_setEntityFlag(Entity *self, int flag, int val);
struct EntityItem *Entity_dropItem(Entity *self, int itemId, int count);
struct EntityItem *Entity_dropItemWithOffset(Entity *self, int itemId, int count, float yOff);
struct EntityItem *Entity_entityDropItem(Entity *self, ItemStack item, float yOff);
void Entity_setBeenAttacked(Entity *self);
void Entity_setOnFireFromLava(Entity *self);
void Entity_dealFireDamage(Entity *self, int amount);
void Entity_updateFallState(Entity *self, double motY, int onGround);
void Entity_onKillEntity(Entity *self, Entity *killed);

DataWatcher *Entity_getDataWatcher(Entity *self);

int Entity_isPlayerSleeping(Entity *self);

int Entity_pushOutOfBlocks(Entity *self, double var1, double var3, double var5);

MovingObjectPosition *Entity_rayTrace(Entity *self, double blockReachDistance, float partialTick);

void Entity_func_346_d(Entity *self, float var1, float var2);

void Entity_setWorld(Entity *self, World *world);

const char *Entity_getEntityTexture(Entity *self);

void Entity_updateCloak(Entity *self);

void Entity_outfitWithItem(Entity *self, int var1, int var2, int var3);

#endif
