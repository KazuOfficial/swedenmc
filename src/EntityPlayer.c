#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "Minecraft.h"
#include "EntityRenderer.h"
#include "ContainerPlayer.h"
#include "TileEntityDispenser.h"
#include "MathHelper.h"
#include "World.h"
#include "Item.h"
#include "Block.h"
#include "BlockBed.h"
#include "InventoryPlayer.h"
#include "EntityItem.h"
#include "AchievementList.h"
#include "StatList.h"
#include "Material.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "EntityWolf.h"
#include "EntityArrow.h"
#include "AxisAlignedBB.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

void EntityPlayer_addMovementStat(EntityPlayer *self, double var1, double var3, double var5);
static void EntityPlayer_addMountedMovementStat(EntityPlayer *self, double var1, double var3,
                                                double var5);
static void def_onItemPickup(EntityPlayer *self, Entity *var1, int var2);
static void def_addStat(EntityPlayer *self, struct StatBase *var1, int var2);

static int match_wolf(Entity *e) { return e->isWolf; }

static void alertWolves(EntityPlayer *self, EntityLiving *var1, int var2) {
    Entity *base = (Entity *)self;
    Entity *var1e = (Entity *)var1;
    if (var1e->isCreeper || var1e->isGhast)
        return;
    if (var1e->isWolf) {
        EntityWolf *w = (EntityWolf *)var1e;
        if (EntityWolf_isWolfTamed(w) && strcmp(self->username, EntityWolf_getWolfOwner(w)) == 0) {
            return;
        }
    }
    if (!var1e->isPlayer) {
        AxisAlignedBB *bb = AxisAlignedBB_getBoundingBoxFromPool(base->posX, base->posY, base->posZ,
                                                                 base->posX + 1.0, base->posY + 1.0,
                                                                 base->posZ + 1.0);
        AxisAlignedBB_expand(bb, 16.0, 4.0, 16.0);
        Entity *wolfList[64];
        int count = World_getEntitiesWithinAABB(base->worldObj, match_wolf, bb, wolfList, 64);
        for (int i = 0; i < count; ++i) {
            EntityWolf *var6 = (EntityWolf *)wolfList[i];
            if (!EntityWolf_isWolfTamed(var6))
                continue;
            if (EntityCreature_getTarget(&var6->base.base) != NULL)
                continue;
            if (strcmp(self->username, EntityWolf_getWolfOwner(var6)) != 0)
                continue;
            if (var2 && EntityWolf_isWolfSitting(var6))
                continue;
            EntityWolf_setWolfSitting(var6, 0);
            EntityCreature_setTarget(&var6->base.base, var1e);
        }
    }
}

void EntityPlayer_updatePlayerActionStateBase(EntityLiving *base) {
    if (base->isSwingInProgress) {
        ++base->swingProgressInt;
        if (base->swingProgressInt >= 8) {
            base->swingProgressInt = 0;
            base->isSwingInProgress = 0;
        }
    } else {
        base->swingProgressInt = 0;
    }
    base->swingProgress = (float)base->swingProgressInt / 8.0f;
}

static void def_updatePlayerActionState_player(EntityLiving *base) {
    EntityPlayer_updatePlayerActionStateBase(base);
}

void EntityPlayer_onLivingUpdateBase(EntityLiving *base) {
    EntityPlayer *self = (EntityPlayer *)base;
    Entity *entity = (Entity *)self;

    if (entity->worldObj->difficultySetting == 0 && base->health < 20 &&
        (base->ticksLived % 20) * 12 == 0) {
        ((EntityLivingVtable *)entity->vtable)->heal(base, 1);
    }

    InventoryPlayer_decrementAnimations(&self->inventory);

    self->field_775_e = self->field_774_f;

    EntityLiving_onLivingUpdateBase(base);

    float var1 = MathHelper_sqrt_double(entity->motionX * entity->motionX +
                                        entity->motionZ * entity->motionZ);
    float var2 = (float)atan(-entity->motionY * 0.2) * 15.0f;
    if (var1 > 0.1f)
        var1 = 0.1f;
    if (!entity->onGround || base->health <= 0)
        var1 = 0.0f;
    if (entity->onGround || base->health <= 0)
        var2 = 0.0f;
    self->field_774_f += (var1 - self->field_774_f) * 0.4f;
    base->field_9328_R += (var2 - base->field_9328_R) * 0.8f;

    if (base->health > 0) {
        AxisAlignedBB expanded = *AxisAlignedBB_expand(&entity->boundingBox, 1.0, 0.0, 1.0);
        Entity *entityBuf[64];
        int count = World_getEntitiesWithinAABBExcludingEntity(entity->worldObj, entity, &expanded,
                                                               entityBuf, 64);
        for (int var4 = 0; var4 < count; ++var4) {
            Entity *var5 = entityBuf[var4];
            if (!var5->isDead) {

                var5->vtable->onCollideWithPlayer(var5, self);
            }
        }
    }
}

static void def_onLivingUpdate_player(EntityLiving *base) { EntityPlayer_onLivingUpdateBase(base); }

void EntityPlayer_onUpdate(Entity *base) {
    EntityPlayer *self = (EntityPlayer *)base;

    if (EntityPlayer_isPlayerSleeping(self)) {
        ++self->sleepTimer;
        if (self->sleepTimer > 100)
            self->sleepTimer = 100;
        if (!base->worldObj->multiplayerWorld) {

            int inBed = World_getBlockId(base->worldObj, self->bedChunkCoordinates.x,
                                         self->bedChunkCoordinates.y,
                                         self->bedChunkCoordinates.z) == Block_blockBed->blockID;
            if (!inBed) {
                EntityPlayer_wakeUpPlayer(self, 1, 1, 0);
            } else if (World_isDaytime(base->worldObj)) {
                EntityPlayer_wakeUpPlayer(self, 0, 1, 1);
            }
        }
    } else if (self->sleepTimer > 0) {
        ++self->sleepTimer;
        if (self->sleepTimer >= 110)
            self->sleepTimer = 0;
    }

    EntityLiving_defaultVtable.base.onUpdate(base);

    if (!base->worldObj->multiplayerWorld && self->craftingInventory != NULL &&
        !Container_isUsableByPlayer(self->craftingInventory, self)) {
        EntityPlayer_closeScreen(self);
        self->craftingInventory = self->inventorySlots;
    }

    self->field_20066_r = self->field_20063_u;
    self->field_20065_s = self->field_20062_v;
    self->field_20064_t = self->field_20061_w;
    double var1 = base->posX - self->field_20063_u;
    double var3 = base->posY - self->field_20062_v;
    double var5 = base->posZ - self->field_20061_w;
    double var7 = 10.0;
    if (var1 > var7) {
        self->field_20066_r = self->field_20063_u = base->posX;
    }
    if (var5 > var7) {
        self->field_20064_t = self->field_20061_w = base->posZ;
    }
    if (var3 > var7) {
        self->field_20065_s = self->field_20062_v = base->posY;
    }
    if (var1 < -var7) {
        self->field_20066_r = self->field_20063_u = base->posX;
    }
    if (var5 < -var7) {
        self->field_20064_t = self->field_20061_w = base->posZ;
    }
    if (var3 < -var7) {
        self->field_20065_s = self->field_20062_v = base->posY;
    }
    self->field_20063_u += var1 * 0.25;
    self->field_20061_w += var5 * 0.25;
    self->field_20062_v += var3 * 0.25;

    EntityPlayer_addStat(self, StatList_minutesPlayedStat, 1);

    if (base->ridingEntity == NULL) {
        self->hasStartMinecartRidingCoordinate = 0;
    }
}

static int def_isMovementBlocked_player(EntityLiving *base) {
    EntityPlayer *self = (EntityPlayer *)base;
    return base->health <= 0 || EntityPlayer_isPlayerSleeping(self);
}

static void def_damageEntity_player(EntityLiving *base, Entity *source, int var1) {
    EntityPlayer *self = (EntityPlayer *)base;
    int var2 = 25 - InventoryPlayer_getTotalArmorValue(&self->inventory);
    int var3 = var1 * var2 + self->damageRemainder;
    InventoryPlayer_damageArmor(&self->inventory, var1);
    var1 = var3 / 25;
    self->damageRemainder = var3 % 25;

    EntityLiving_defaultVtable.damageEntity(base, source, var1);
}

static void def_preparePlayerToSpawn(Entity *base) {
    base->yOffset = 1.62f;
    Entity_setSize(base, 0.6f, 1.8f);
    Entity_preparePlayerToSpawn_base(base);
    ((EntityLiving *)base)->health = 20;
    ((EntityLiving *)base)->deathTime = 0;
}

static int def_attackEntityFrom_player(Entity *base, Entity *var1, int var2) {
    EntityPlayer *self = (EntityPlayer *)base;
    ((EntityLiving *)base)->entityAge = 0;
    if (((EntityLiving *)base)->health <= 0)
        return 0;
    if (EntityPlayer_isPlayerSleeping(self) && !base->worldObj->multiplayerWorld) {
        EntityPlayer_wakeUpPlayer(self, 1, 1, 0);
    }

    if (var1 != NULL && (var1->isMob || var1->isArrow)) {
        if (base->worldObj->difficultySetting == 0)
            var2 = 0;
        if (base->worldObj->difficultySetting == 1)
            var2 = var2 / 3 + 1;
        if (base->worldObj->difficultySetting == 3)
            var2 = var2 * 3 / 2;
    }
    if (var2 == 0)
        return 0;

    {
        Entity *var3 = var1;
        if (var3 != NULL && var3->isArrow && ((EntityArrow *)var3)->owner != NULL) {
            var3 = (Entity *)((EntityArrow *)var3)->owner;
        }
        if (var3 != NULL && var3->isLiving) {
            alertWolves(self, (EntityLiving *)var3, 0);
        }
    }
    EntityPlayer_addStat(self, StatList_damageTakenStat, var2);

    return EntityLiving_defaultVtable.base.attackEntityFrom(base, var1, var2);
}

static void def_onDeath_player(EntityLiving *base, Entity *var1) {
    EntityPlayer *self = (EntityPlayer *)base;
    Entity *entity = (Entity *)self;

    EntityLiving_defaultVtable.onDeath(base, var1);
    Entity_setSize(entity, 0.2f, 0.2f);
    entity->vtable->setPosition(entity, entity->posX, entity->posY, entity->posZ);
    entity->motionY = 0.1;

    if (strcmp(self->username, "Notch") == 0) {
        ItemStack appleStack = ItemStack_fromItemID(Item_appleRed->shiftedIndex, 1, 0);
        EntityPlayer_dropPlayerItemWithRandomChoice(self, &appleStack, 1);
    }
    InventoryPlayer_dropAllItems(&self->inventory);

    Minecraft *_mc = Minecraft_getMinecraft();
    if (_mc && _mc->entityRenderer)
        ((EntityRenderer *)_mc->entityRenderer)->itemRenderer.itemToRender = NULL;
    if (var1 != NULL) {
        entity->motionX = (double)(-MathHelper_cos((base->attackedAtYaw + entity->rotationYaw) *
                                                   (float)M_PI / 180.0f) *
                                   0.1f);
        entity->motionZ = (double)(-MathHelper_sin((base->attackedAtYaw + entity->rotationYaw) *
                                                   (float)M_PI / 180.0f) *
                                   0.1f);
    } else {
        entity->motionX = entity->motionZ = 0.0;
    }
    entity->yOffset = 0.1f;
    EntityPlayer_addStat(self, StatList_deathsStat, 1);
}

static void def_setEntityDead_player(Entity *base) {
    EntityPlayer *self = (EntityPlayer *)base;
    EntityLiving_defaultVtable.base.setEntityDead(base);
    if (self->inventorySlots)
        Container_onCraftGuiClosed_v(self->inventorySlots, self);
    if (self->craftingInventory)
        Container_onCraftGuiClosed_v(self->craftingInventory, self);
}

static int def_isEntityInsideOpaqueBlock_player(Entity *self) {
    EntityPlayer *player = (EntityPlayer *)self;
    if (player->sleeping)
        return 0;
    return Entity_isEntityInsideOpaqueBlock_base(self);
}

static void def_writeEntityToNBT_player(Entity *base, NBTTagCompound *var1) {
    EntityPlayer *self = (EntityPlayer *)base;
    EntityLiving_defaultVtable.base.writeEntityToNBT(base, var1);
    NBTTagList *invList = NBTTagList_create();
    InventoryPlayer_writeToNBT(&self->inventory, invList);
    NBTTagCompound_setTag(var1, "Inventory", (NBTBase *)invList);
    NBTTagCompound_setInteger(var1, "Dimension", self->dimension);
    NBTTagCompound_setBoolean(var1, "Sleeping", self->sleeping);
    NBTTagCompound_setShort(var1, "SleepTimer", (short)self->sleepTimer);
    if (self->hasPlayerSpawnCoordinate) {
        NBTTagCompound_setInteger(var1, "SpawnX", self->playerSpawnCoordinate.x);
        NBTTagCompound_setInteger(var1, "SpawnY", self->playerSpawnCoordinate.y);
        NBTTagCompound_setInteger(var1, "SpawnZ", self->playerSpawnCoordinate.z);
    }
}

static void def_readEntityFromNBT_player(Entity *base, NBTTagCompound *var1) {
    EntityPlayer *self = (EntityPlayer *)base;
    EntityLiving_defaultVtable.base.readEntityFromNBT(base, var1);
    NBTTagList *var2 = NBTTagCompound_getTagList(var1, "Inventory");
    if (var2)
        InventoryPlayer_readFromNBT(&self->inventory, var2);
    self->dimension = NBTTagCompound_getInteger(var1, "Dimension");
    self->sleeping = NBTTagCompound_getBoolean(var1, "Sleeping");
    self->sleepTimer = NBTTagCompound_getShort(var1, "SleepTimer");
    if (self->sleeping) {
        self->hasBedChunkCoordinates = 1;
        self->bedChunkCoordinates.x = MathHelper_floor_double(base->posX);
        self->bedChunkCoordinates.y = MathHelper_floor_double(base->posY);
        self->bedChunkCoordinates.z = MathHelper_floor_double(base->posZ);
        EntityPlayer_wakeUpPlayer(self, 1, 1, 0);
    }
    if (NBTTagCompound_hasKey(var1, "SpawnX") && NBTTagCompound_hasKey(var1, "SpawnY") &&
        NBTTagCompound_hasKey(var1, "SpawnZ")) {
        self->hasPlayerSpawnCoordinate = 1;
        self->playerSpawnCoordinate.x = NBTTagCompound_getInteger(var1, "SpawnX");
        self->playerSpawnCoordinate.y = NBTTagCompound_getInteger(var1, "SpawnY");
        self->playerSpawnCoordinate.z = NBTTagCompound_getInteger(var1, "SpawnZ");
    }
}

static void def_resetHeight(EntityPlayer *self) { ((Entity *)self)->yOffset = 1.62f; }

static float def_getEyeHeight_player(Entity *self) {
    (void)self;
    return 0.12f;
}

static double def_getYOffset_player(Entity *self) { return (double)(self->yOffset - 0.5f); }

static void def_setInPortal_player(Entity *base) {
    EntityPlayer *self = (EntityPlayer *)base;
    if (self->timeUntilPortal > 0) {
        self->timeUntilPortal = 10;
    } else {
        self->inPortal = 1;
    }
}

static void def_entityInit_player(Entity *base) {
    EntityLiving_defaultVtable.base.entityInit(base);
    DataWatcher_addByte(&base->dataWatcher, 16, 0);
}

static void def_fall_player(Entity *base, float var1) {
    EntityPlayer *self = (EntityPlayer *)base;
    if (var1 >= 2.0f) {
        EntityPlayer_addStat(self, StatList_distanceFallenStat, (int)round((double)var1 * 100.0));
    }
    EntityLiving_defaultVtable.base.fall(base, var1);
}

static void def_moveEntityWithHeading_player(EntityLiving *base, float var1, float var2) {
    EntityPlayer *self = (EntityPlayer *)base;
    Entity *entity = (Entity *)self;
    double var3 = entity->posX;
    double var5 = entity->posY;
    double var7 = entity->posZ;
    EntityLiving_defaultVtable.moveEntityWithHeading(base, var1, var2);
    EntityPlayer_addMovementStat(self, entity->posX - var3, entity->posY - var5,
                                 entity->posZ - var7);
}

static void def_jump_player(EntityLiving *base) {
    EntityPlayer *self = (EntityPlayer *)base;
    EntityLiving_defaultVtable.jump(base);
    EntityPlayer_addStat(self, StatList_jumpStat, 1);
}

static void def_addItemStackToInventory(EntityPlayer *self, ItemStack *stack) {
    InventoryPlayer_addItemStackToInventory(&self->inventory, stack);
}

static void def_dropPlayerItem(EntityPlayer *self, ItemStack *stack) {
    EntityPlayer_dropPlayerItemWithRandomChoice(self, stack, 0);
}

static void def_addScore(EntityPlayer *self, int score) { self->score += score; }

static void def_addExperience(EntityPlayer *self, int xp) { self->experienceTotal += xp; }

static int def_isPlayerSleeping(EntityPlayer *self) { return self->sleeping; }

static int def_isCreative(EntityPlayer *self) { return self->isCreativeMode; }

static void def_damageArmor(EntityPlayer *self, int damage) {
    InventoryPlayer_damageArmor(&self->inventory, damage);
}

static void def_swingItem(EntityPlayer *self) {
    self->base.swingProgressInt = -1;
    self->base.isSwingInProgress = 1;
}

static int def_getMaxHealth_player(EntityLiving *self) {
    (void)self;
    return 20;
}

static const char *def_getEntityString(Entity *self) {
    (void)self;
    return "Player";
}

static void def_updateRidden_player(Entity *base) {
    EntityPlayer *self = (EntityPlayer *)base;
    double var1 = base->posX;
    double var3 = base->posY;
    double var5 = base->posZ;
    EntityLiving_defaultVtable.base.updateRidden(base);
    self->field_775_e = self->field_774_f;
    self->field_774_f = 0.0f;
    EntityPlayer_addMountedMovementStat(self, base->posX - var1, base->posY - var3,
                                        base->posZ - var5);
}

static int def_getItemIcon_player(EntityLiving *base, ItemStack *var1) {
    int var2 = EntityLiving_defaultVtable.getItemIcon(base, var1);
    EntityPlayer *self = (EntityPlayer *)base;
    if (var1->itemID == Item_fishingRod->shiftedIndex && self->fishEntity != NULL) {
        var2 = ItemStack_getIconIndex(var1) + 16;
    }
    return var2;
}

EntityPlayerVtable EntityPlayer_defaultVtable;

void EntityPlayer_initVtable(EntityPlayerVtable *vt) {
    EntityLiving_initVtable(&vt->base);
    vt->base.base.entityInit = def_entityInit_player;
    vt->base.base.onUpdate = EntityPlayer_onUpdate;
    vt->base.base.preparePlayerToSpawn = def_preparePlayerToSpawn;
    vt->base.base.attackEntityFrom = def_attackEntityFrom_player;
    vt->base.base.writeEntityToNBT = def_writeEntityToNBT_player;
    vt->base.base.readEntityFromNBT = def_readEntityFromNBT_player;
    vt->base.base.setEntityDead = def_setEntityDead_player;
    vt->base.base.setInPortal = def_setInPortal_player;
    vt->base.base.fall = def_fall_player;
    vt->base.base.getEyeHeight = def_getEyeHeight_player;
    vt->base.base.getYOffset = def_getYOffset_player;
    vt->base.base.getEntityString = def_getEntityString;
    vt->base.getMaxHealth = def_getMaxHealth_player;
    vt->base.onLivingUpdate = def_onLivingUpdate_player;
    vt->base.damageEntity = def_damageEntity_player;
    vt->base.onDeath = def_onDeath_player;
    vt->base.updatePlayerActionState = def_updatePlayerActionState_player;
    vt->base.moveEntityWithHeading = def_moveEntityWithHeading_player;
    vt->base.jump = def_jump_player;
    vt->base.isMovementBlocked = def_isMovementBlocked_player;
    vt->base.base.isEntityInsideOpaqueBlock = def_isEntityInsideOpaqueBlock_player;
    vt->base.base.updateRidden = def_updateRidden_player;
    vt->base.getItemIcon = def_getItemIcon_player;

    vt->addItemStackToInventory = def_addItemStackToInventory;
    vt->dropPlayerItem = def_dropPlayerItem;
    vt->addScore = def_addScore;
    vt->addExperience = def_addExperience;
    vt->isPlayerSleeping = def_isPlayerSleeping;
    vt->isCreative = def_isCreative;
    vt->damageArmor = def_damageArmor;
    vt->swingItem = def_swingItem;
    vt->resetHeight = def_resetHeight;
    vt->onItemPickup = def_onItemPickup;
    vt->addStat = def_addStat;
    vt->respawnPlayer = EntityPlayer_respawnPlayer;
}

void EntityPlayer_construct(EntityPlayer *self, World *world) {
    EntityLiving_construct(&self->base, world);
    static int once = 0;
    if (!once) {
        EntityPlayer_initVtable(&EntityPlayer_defaultVtable);
        once = 1;
    }
    self->base.base.vtable = &EntityPlayer_defaultVtable.base.base;
    self->base.base.isPlayer = 1;

    InventoryPlayer_init(&self->inventory, self);

    ContainerPlayer *cp = (ContainerPlayer *)calloc(1, sizeof(ContainerPlayer));
    ContainerPlayer_construct(cp, &self->inventory, !world->multiplayerWorld);
    self->inventorySlots = (struct Container *)cp;
    self->craftingInventory = self->inventorySlots;

    Entity_setSize(&self->base.base, 0.6f, 1.8f);
    self->base.base.yOffset = 1.62f;

    ChunkCoordinates var2 = World_getSpawnPoint(world);
    self->base.base.vtable->setLocationAndAngles(&self->base.base, (double)var2.x + 0.5,
                                                 (double)(var2.y + 1), (double)var2.z + 0.5, 0.0f,
                                                 0.0f);

    self->base.health = 20;
    self->base.field_9351_C = "humanoid";
    self->base.field_9353_B = 180.0f;
    self->base.base.fireResistance = 20;
    self->base.texture = "/mob/char.png";
    self->timeUntilPortal = 20;

    DataWatcher_addShort(&self->base.base.dataWatcher, 1, 300);
    DataWatcher_addString(&self->base.base.dataWatcher, 2, "");
    DataWatcher_addByte(&self->base.base.dataWatcher, 3, 0);
}

EntityPlayer *EntityPlayer_create(World *world) {
    EntityPlayer *self = (EntityPlayer *)calloc(1, sizeof(EntityPlayer));
    EntityPlayer_construct(self, world);
    return self;
}

void EntityPlayer_closeScreen(EntityPlayer *self) {
    self->craftingInventory = self->inventorySlots;
}

void EntityPlayer_updateCloak(EntityPlayer *self) {
    snprintf(self->playerCloakUrl, sizeof(self->playerCloakUrl),
             "http://s3.amazonaws.com/MinecraftCloaks/%s.png", self->username);
    strncpy(((Entity *)self)->cloakUrl, self->playerCloakUrl,
            sizeof(((Entity *)self)->cloakUrl) - 1);
}

ItemStack *EntityPlayer_getHeldItem(EntityPlayer *self) {
    return EntityLiving_getHeldItem(&self->base);
}
void EntityPlayer_addItemStackToInventory(EntityPlayer *self, ItemStack *stack) {
    ((EntityPlayerVtable *)self->base.base.vtable)->addItemStackToInventory(self, stack);
}
void EntityPlayer_dropPlayerItem(EntityPlayer *self, ItemStack *stack) {
    ((EntityPlayerVtable *)self->base.base.vtable)->dropPlayerItem(self, stack);
}
void EntityPlayer_addScore(EntityPlayer *self, int score) {
    ((EntityPlayerVtable *)self->base.base.vtable)->addScore(self, score);
}
int EntityPlayer_getScore(EntityPlayer *self) { return self->score; }

void EntityPlayer_respawnPlayer(EntityPlayer *self) { (void)self; }

void EntityPlayer_addExperience(EntityPlayer *self, int xp) {
    ((EntityPlayerVtable *)self->base.base.vtable)->addExperience(self, xp);
}
int EntityPlayer_isPlayerSleeping(EntityPlayer *self) {
    return ((EntityPlayerVtable *)self->base.base.vtable)->isPlayerSleeping(self);
}
int EntityPlayer_isCreative(EntityPlayer *self) {
    return ((EntityPlayerVtable *)self->base.base.vtable)->isCreative(self);
}
void EntityPlayer_damageArmor(EntityPlayer *self, int damage) {
    ((EntityPlayerVtable *)self->base.base.vtable)->damageArmor(self, damage);
}
void EntityPlayer_swingItem(EntityPlayer *self) {
    ((EntityPlayerVtable *)self->base.base.vtable)->swingItem(self);
}

ItemStack *EntityPlayer_getCurrentEquippedItem(EntityPlayer *self) {
    return InventoryPlayer_getCurrentItem(&self->inventory);
}

void EntityPlayer_destroyCurrentEquippedItem(EntityPlayer *self) {
    InventoryPlayer_setInventorySlotContents(&self->inventory, self->inventory.currentItem, NULL);
}

int EntityPlayer_canHarvestBlock(EntityPlayer *self, struct Block *block) {
    return InventoryPlayer_canHarvestBlock(&self->inventory, block);
}

void EntityPlayer_dropCurrentItem(EntityPlayer *self) {
    ItemStack *dropped =
        InventoryPlayer_decrStackSize(&self->inventory, self->inventory.currentItem, 1);
    if (dropped) {
        EntityPlayer_dropPlayerItemWithRandomChoice(self, dropped, 0);
        Minecraft_invalidateItemToRender(dropped);
        ItemStack_free(dropped);
    }
}

void EntityPlayer_dropPlayerItemWithRandomChoice(EntityPlayer *self, ItemStack *var1, int var2) {
    if (var1 == NULL)
        return;
    Entity *entity = (Entity *)self;
    EntityItem *var3 = EntityItem_create(
        entity->worldObj, entity->posX,
        entity->posY - 0.3 + (double)entity->vtable->getEyeHeight(entity), entity->posZ, *var1);
    var3->delayBeforeCanPickup = 40;
    float var4 = 0.1f;
    float var5;
    if (var2) {
        var5 = JavaRandom_nextFloat(&entity->rand) * 0.5f;
        float var6 = JavaRandom_nextFloat(&entity->rand) * (float)M_PI * 2.0f;
        var3->base.motionX = (double)(-MathHelper_sin(var6) * var5);
        var3->base.motionZ = (double)(MathHelper_cos(var6) * var5);
        var3->base.motionY = 0.2;
    } else {
        var4 = 0.3f;
        var3->base.motionX =
            (double)(-MathHelper_sin(entity->rotationYaw / 180.0f * (float)M_PI) *
                     MathHelper_cos(entity->rotationPitch / 180.0f * (float)M_PI) * var4);
        var3->base.motionZ =
            (double)(MathHelper_cos(entity->rotationYaw / 180.0f * (float)M_PI) *
                     MathHelper_cos(entity->rotationPitch / 180.0f * (float)M_PI) * var4);
        var3->base.motionY =
            (double)(-MathHelper_sin(entity->rotationPitch / 180.0f * (float)M_PI) * var4 + 0.1f);
        var4 = 0.02f;
        var5 = JavaRandom_nextFloat(&entity->rand) * (float)M_PI * 2.0f;
        var4 *= JavaRandom_nextFloat(&entity->rand);
        var3->base.motionX += cos((double)var5) * (double)var4;
        var3->base.motionY +=
            (double)((JavaRandom_nextFloat(&entity->rand) - JavaRandom_nextFloat(&entity->rand)) *
                     0.1f);
        var3->base.motionZ += sin((double)var5) * (double)var4;
    }
    World_entityJoinedWorld(entity->worldObj, (Entity *)var3);
    EntityPlayer_addStat(self, StatList_dropStat, 1);
}

float EntityPlayer_getCurrentPlayerStrVsBlock(EntityPlayer *self, struct Block *var1) {
    float var2 = InventoryPlayer_getStrVsBlock(&self->inventory, var1);
    if (Entity_isInsideOfMaterial((Entity *)self, Material_water))
        var2 /= 5.0f;
    if (!((Entity *)self)->onGround)
        var2 /= 5.0f;
    return var2;
}

void EntityPlayer_attackTargetEntityWithCurrentItem(EntityPlayer *self, Entity *target) {
    int var2 = InventoryPlayer_getDamageVsEntity(&self->inventory, target);
    if (var2 > 0) {
        if (self->base.base.motionY < 0.0)
            ++var2;
        target->vtable->attackEntityFrom(target, &self->base.base, var2);
        ItemStack *var3 = EntityPlayer_getCurrentEquippedItem(self);
        if (var3 != NULL && target->isLiving) {
            ItemStack_hitEntity(var3, (EntityLiving *)target, self);
            if (var3->stackSize <= 0) {
                ItemStack_func_1097_a(var3, self);
                EntityPlayer_destroyCurrentEquippedItem(self);
            }
        }
        if (target->isLiving) {
            if (target->vtable->isEntityAlive(target)) {
                alertWolves(self, (EntityLiving *)target, 1);
            }
            EntityPlayer_addStat(self, StatList_damageDealtStat, var2);
        }
    }
}

void EntityPlayer_useCurrentItemOnEntity(EntityPlayer *self, Entity *entity) {
    if (!entity->vtable->interact(entity, self)) {
        ItemStack *var2 = EntityPlayer_getCurrentEquippedItem(self);
        if (var2 != NULL && entity->isLiving) {
            ItemStack_useItemOnEntity(var2, (EntityLiving *)entity);
            if (var2->stackSize <= 0) {
                ItemStack_func_1097_a(var2, self);
                EntityPlayer_destroyCurrentEquippedItem(self);
            }
        }
    }
}

float EntityPlayer_getArmorBonusPoints(EntityPlayer *self) {
    float total = 0.0f;
    for (int i = EQUIPMENT_SLOT_BOOTS; i <= EQUIPMENT_SLOT_HELMET; i++) {
        if (!self->base.equipmentHas[i])
            continue;
        ItemStack *s = &self->base.equipment[i];
        if (s->itemID < ITEM_LIST_SIZE && Item_itemsList[s->itemID])
            total += (float)Item_itemsList[s->itemID]->vtable->getDamageVsEntity(
                Item_itemsList[s->itemID], NULL);
    }
    return total;
}

EnumStatus EntityPlayer_sleepInBedAt(EntityPlayer *self, int var1, int var2, int var3) {
    Entity *base = (Entity *)self;
    if (!base->worldObj->multiplayerWorld) {
        if (EntityPlayer_isPlayerSleeping(self) || !base->vtable->isEntityAlive(base)) {
            return ENUMSTATUS_OTHER_PROBLEM;
        }
        if (base->worldObj->worldProvider->isNether) {
            return ENUMSTATUS_NOT_POSSIBLE_HERE;
        }
        if (World_isDaytime(base->worldObj)) {
            return ENUMSTATUS_NOT_POSSIBLE_NOW;
        }
        if (fabs(base->posX - (double)var1) > 3.0 || fabs(base->posY - (double)var2) > 2.0 ||
            fabs(base->posZ - (double)var3) > 3.0) {
            return ENUMSTATUS_TOO_FAR_AWAY;
        }
    }

    Entity_setSize(base, 0.2f, 0.2f);
    base->yOffset = 0.2f;
    if (World_blockExists(base->worldObj, var1, var2, var3)) {
        int var4 = World_getBlockMetadata(base->worldObj, var1, var2, var3);
        int var5 = BlockBed_getDirectionFromMetadata(var4);
        float var6 = 0.5f;
        float var7 = 0.5f;
        switch (var5) {
        case 0:
            var7 = 0.9f;
            break;
        case 1:
            var6 = 0.1f;
            break;
        case 2:
            var7 = 0.1f;
            break;
        case 3:
            var6 = 0.9f;
            break;
        }

        self->field_22063_x = 0.0f;
        self->field_22061_z = 0.0f;
        switch (var5) {
        case 0:
            self->field_22061_z = -1.8f;
            break;
        case 1:
            self->field_22063_x = 1.8f;
            break;
        case 2:
            self->field_22061_z = 1.8f;
            break;
        case 3:
            self->field_22063_x = -1.8f;
            break;
        }
        base->vtable->setPosition(base, (double)((float)var1 + var6),
                                  (double)((float)var2 + 15.0f / 16.0f),
                                  (double)((float)var3 + var7));
    } else {
        base->vtable->setPosition(base, (double)((float)var1 + 0.5f),
                                  (double)((float)var2 + 15.0f / 16.0f),
                                  (double)((float)var3 + 0.5f));
    }

    self->sleeping = 1;
    self->sleepTimer = 0;
    self->hasBedChunkCoordinates = 1;
    self->bedChunkCoordinates.x = var1;
    self->bedChunkCoordinates.y = var2;
    self->bedChunkCoordinates.z = var3;
    base->motionX = base->motionZ = base->motionY = 0.0;
    if (!base->worldObj->multiplayerWorld) {
        World_updateAllPlayersSleepingFlag(base->worldObj);
    }
    return ENUMSTATUS_OK;
}

void EntityPlayer_wakeUpPlayer(EntityPlayer *self, int var1, int var2, int var3) {
    Entity_setSize((Entity *)self, 0.6f, 1.8f);
    ((EntityPlayerVtable *)((Entity *)self)->vtable)->resetHeight(self);
    ChunkCoordinates var4 = self->bedChunkCoordinates;
    ChunkCoordinates var5 = self->bedChunkCoordinates;
    if (self->hasBedChunkCoordinates && World_getBlockId(((Entity *)self)->worldObj, var4.x, var4.y,
                                                         var4.z) == Block_blockBed->blockID) {
        BlockBed_setBedOccupied(((Entity *)self)->worldObj, var4.x, var4.y, var4.z, 0);
        ChunkCoordinates *near = BlockBed_getNearestEmptyChunkCoordinates(
            ((Entity *)self)->worldObj, var4.x, var4.y, var4.z, 0);
        if (near == NULL) {
            var5.x = var4.x;
            var5.y = var4.y + 1;
            var5.z = var4.z;
        } else {
            var5 = *near;
        }
        ((Entity *)self)
            ->vtable->setPosition((Entity *)self, (double)((float)var5.x + 0.5f),
                                  (double)((float)var5.y + ((Entity *)self)->yOffset + 0.1f),
                                  (double)((float)var5.z + 0.5f));
    }
    self->sleeping = 0;
    if (!((Entity *)self)->worldObj->multiplayerWorld && var2) {
        World_updateAllPlayersSleepingFlag(((Entity *)self)->worldObj);
    }
    if (var1) {
        self->sleepTimer = 0;
    } else {
        self->sleepTimer = 100;
    }
    if (var3) {
        EntityPlayer_setPlayerSpawnCoordinate(self, &self->bedChunkCoordinates);
    }
}

ChunkCoordinates *EntityPlayer_getPlayerSpawnCoordinate(EntityPlayer *self) {
    return self->hasPlayerSpawnCoordinate ? &self->playerSpawnCoordinate : NULL;
}

void EntityPlayer_setPlayerSpawnCoordinate(EntityPlayer *self, ChunkCoordinates *var1) {
    if (var1 != NULL) {
        self->hasPlayerSpawnCoordinate = 1;
        self->playerSpawnCoordinate = *var1;
    } else {
        self->hasPlayerSpawnCoordinate = 0;
    }
}

void EntityPlayer_triggerAchievement(EntityPlayer *self, struct StatBase *var1) {
    EntityPlayer_addStat(self, var1, 1);
}

void EntityPlayer_addStat(EntityPlayer *self, struct StatBase *var1, int var2) {
    ((EntityPlayerVtable *)((Entity *)self)->vtable)->addStat(self, var1, var2);
}

static void def_addStat(EntityPlayer *self, struct StatBase *var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

static void def_onItemPickup(EntityPlayer *self, Entity *var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}
void EntityPlayer_onItemPickup(EntityPlayer *self, Entity *var1, int var2) {
    ((EntityPlayerVtable *)((Entity *)self)->vtable)->onItemPickup(self, var1, var2);
}

void EntityPlayer_addChatMessage(EntityPlayer *self, const char *message) {
    (void)self;
    (void)message;
}

void EntityPlayer_displayGUIChest(EntityPlayer *self, struct IInventory *inventory) {
    if (((Entity *)self)->isPlayerSP) {
        EntityPlayerSP_displayGUIChest((EntityPlayerSP *)self, inventory);
    }
}

void EntityPlayer_displayGUIDispenser(EntityPlayer *self, struct TileEntityDispenser *dispenser) {
    if (((Entity *)self)->isPlayerSP) {
        EntityPlayerSP_displayGUIDispenser((EntityPlayerSP *)self, dispenser);
    }
}

void EntityPlayer_displayGUIEditSign(EntityPlayer *self, struct TileEntitySign *var1) {
    if (((Entity *)self)->isPlayerSP) {
        EntityPlayerSP_displayGUIEditSign((EntityPlayerSP *)self, var1);
    }
}

void EntityPlayer_displayGUIFurnace(EntityPlayer *self, struct TileEntityFurnace *var1) {
    if (((Entity *)self)->isPlayerSP) {
        EntityPlayerSP_displayGUIFurnace((EntityPlayerSP *)self, var1);
    }
}

void EntityPlayer_displayWorkbenchGUI(EntityPlayer *self, int var1, int var2, int var3) {
    if (((Entity *)self)->isPlayerSP) {
        EntityPlayerSP_displayWorkbenchGUI((EntityPlayerSP *)self, var1, var2, var3);
    }
}

float EntityPlayer_getBedOrientationInDegrees(EntityPlayer *self) {
    if (self->hasBedChunkCoordinates) {
        int var1 = World_getBlockMetadata(((Entity *)self)->worldObj, self->bedChunkCoordinates.x,
                                          self->bedChunkCoordinates.y, self->bedChunkCoordinates.z);
        int var2 = BlockBed_getDirectionFromMetadata(var1);
        switch (var2) {
        case 0:
            return 90.0f;
        case 1:
            return 0.0f;
        case 2:
            return 270.0f;
        case 3:
            return 180.0f;
        }
    }
    return 0.0f;
}

int EntityPlayer_getPlayerArmorValue(EntityPlayer *self) {
    return InventoryPlayer_getTotalArmorValue(&self->inventory);
}

int EntityPlayer_func_22060_M(EntityPlayer *self) { return self->sleepTimer; }

int EntityPlayer_isPlayerFullyAsleep(EntityPlayer *self) {
    return self->sleeping && self->sleepTimer >= 100;
}

void EntityPlayer_setInPortal(EntityPlayer *self) {
    ((Entity *)self)->vtable->setInPortal((Entity *)self);
}

void EntityPlayer_onKillEntity(EntityPlayer *self, EntityLiving *var1) {
    if (((Entity *)var1)->isMob) {
        EntityPlayer_triggerAchievement(self, (struct StatBase *)AchievementList_killEnemy);
    }
}

void EntityPlayer_addToPlayerScore_impl(EntityPlayer *self, Entity *var1, int var2) {
    self->score += var2;
    if (var1->isPlayer) {
        EntityPlayer_addStat(self, StatList_playerKillsStat, 1);
    } else {
        EntityPlayer_addStat(self, StatList_mobKillsStat, 1);
    }
}

static void EntityPlayer_addMountedMovementStat(EntityPlayer *self, double var1, double var3,
                                                double var5) {
    Entity *entity = (Entity *)self;
    if (entity->ridingEntity != NULL) {
        int var7 =
            (int)roundf(MathHelper_sqrt_double(var1 * var1 + var3 * var3 + var5 * var5) * 100.0f);
        if (var7 > 0) {
            if (entity->ridingEntity->isMinecart) {
                EntityPlayer_addStat(self, StatList_distanceByMinecartStat, var7);
                if (!self->hasStartMinecartRidingCoordinate) {
                    self->hasStartMinecartRidingCoordinate = 1;
                    self->startMinecartRidingCoordinate.x = MathHelper_floor_double(entity->posX);
                    self->startMinecartRidingCoordinate.y = MathHelper_floor_double(entity->posY);
                    self->startMinecartRidingCoordinate.z = MathHelper_floor_double(entity->posZ);
                } else if (ChunkCoordinates_getSqDistanceTo(
                               &self->startMinecartRidingCoordinate,
                               MathHelper_floor_double(entity->posX),
                               MathHelper_floor_double(entity->posY),
                               MathHelper_floor_double(entity->posZ)) >= 1000.0) {
                    EntityPlayer_addStat(self, (struct StatBase *)AchievementList_onARail, 1);
                }
            } else if (entity->ridingEntity->isBoat) {
                EntityPlayer_addStat(self, StatList_distanceByBoatStat, var7);
            } else if (entity->ridingEntity->isPig) {
                EntityPlayer_addStat(self, StatList_distanceByPigStat, var7);
            }
        }
    }
}

void EntityPlayer_addMovementStat(EntityPlayer *self, double var1, double var3, double var5) {
    if (((Entity *)self)->ridingEntity == NULL) {
        int var7;
        if (Entity_isInsideOfMaterial((Entity *)self, Material_water)) {
            var7 = (int)roundf(MathHelper_sqrt_double(var1 * var1 + var3 * var3 + var5 * var5) *
                               100.0f);
            if (var7 > 0)
                EntityPlayer_addStat(self, StatList_distanceDoveStat, var7);
        } else if (Entity_isInWater((Entity *)self)) {
            var7 = (int)roundf(MathHelper_sqrt_double(var1 * var1 + var5 * var5) * 100.0f);
            if (var7 > 0)
                EntityPlayer_addStat(self, StatList_distanceSwumStat, var7);
        } else if (((EntityLivingVtable *)((Entity *)self)->vtable)->isOnLadder != NULL &&
                   ((EntityLivingVtable *)((Entity *)self)->vtable)
                       ->isOnLadder((EntityLiving *)self)) {
            if (var3 > 0.0)
                EntityPlayer_addStat(self, StatList_distanceClimbedStat, (int)round(var3 * 100.0));
        } else if (((Entity *)self)->onGround) {
            var7 = (int)roundf(MathHelper_sqrt_double(var1 * var1 + var5 * var5) * 100.0f);
            if (var7 > 0)
                EntityPlayer_addStat(self, StatList_distanceWalkedStat, var7);
        } else {
            var7 = (int)roundf(MathHelper_sqrt_double(var1 * var1 + var5 * var5) * 100.0f);
            if (var7 > 25)
                EntityPlayer_addStat(self, StatList_distanceFlownStat, var7);
        }
    }
}

void EntityPlayer_onItemStackChanged(EntityPlayer *self, ItemStack *var1) {
    (void)self;
    (void)var1;
}

ChunkCoordinates *EntityPlayer_func_25060_a(World *var0, ChunkCoordinates *var1) {
    IChunkProvider *var2 = World_getIChunkProvider(var0);
    if (var2) {
        IChunkProvider_prepareChunk(var2, (var1->x - 3) >> 4, (var1->z - 3) >> 4);
        IChunkProvider_prepareChunk(var2, (var1->x + 3) >> 4, (var1->z - 3) >> 4);
        IChunkProvider_prepareChunk(var2, (var1->x - 3) >> 4, (var1->z + 3) >> 4);
        IChunkProvider_prepareChunk(var2, (var1->x + 3) >> 4, (var1->z + 3) >> 4);
    }
    if (World_getBlockId(var0, var1->x, var1->y, var1->z) != Block_blockBed->blockID) {
        return NULL;
    } else {
        ChunkCoordinates *var3 =
            BlockBed_getNearestEmptyChunkCoordinates(var0, var1->x, var1->y, var1->z, 0);
        return var3;
    }
}
