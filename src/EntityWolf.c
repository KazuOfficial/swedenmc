#include "EntityWolf.h"
#include "EntityAnimal.h"
#include "EntityCreature.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "EntityArrow.h"
#include "EntitySheep.h"
#include "ItemFood.h"
#include "Item.h"
#include "ItemStack.h"
#include "InventoryPlayer.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "AxisAlignedBB.h"
#include "MathHelper.h"
#include "DataWatcher.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

static EntityAnimalVtable EntityWolf_vtable;
static int EntityWolf_vtable_init = 0;

static void showHeartsOrSmokeFX(EntityWolf *self, int var1);
static void getPathOrWalkableBlock(EntityWolf *self, Entity *var1, float var2);

static int match_EntitySheep(Entity *e) { return e->isSheep; }
static int match_EntityWolf(Entity *e) { return e->isWolf; }

static const char *wolf_getEntityString(Entity *self) {
    (void)self;
    return "Wolf";
}

void EntityWolf_initVtable(EntityAnimalVtable *vt) {
    EntityAnimal_initVtable(vt);

    vt->base.base.base.getEntityString = wolf_getEntityString;
    vt->base.base.base.entityInit = EntityWolf_entityInit;
    vt->base.base.base.canTriggerWalking = EntityWolf_canTriggerWalking;
    vt->base.base.base.writeEntityToNBT = EntityWolf_writeEntityToNBT;
    vt->base.base.base.readEntityFromNBT = EntityWolf_readEntityFromNBT;
    vt->base.base.base.attackEntityFrom = EntityWolf_attackEntityFrom;
    vt->base.base.base.interact = EntityWolf_interact;
    vt->base.base.base.getEyeHeight = EntityWolf_getEyeHeight;
    vt->base.base.base.handleHealthUpdate = EntityWolf_handleHealthUpdate;

    vt->base.base.getEntityTexture = EntityWolf_getEntityTexture;
    vt->base.base.canDespawn = EntityWolf_canDespawn;
    vt->base.base.getLivingSound = EntityWolf_getLivingSound;
    vt->base.base.getHurtSound = EntityWolf_getHurtSound;
    vt->base.base.getDeathSound = EntityWolf_getDeathSound;
    vt->base.base.getSoundVolume = EntityWolf_getSoundVolume;
    vt->base.base.getDropItemId = EntityWolf_getDropItemId;
    vt->base.base.updatePlayerActionState = EntityWolf_updatePlayerActionState;
    vt->base.base.onLivingUpdate = EntityWolf_onLivingUpdate;
    vt->base.base.func_25026_x = EntityWolf_func_25026_x;
    vt->base.base.getMaxSpawnedInChunk = EntityWolf_getMaxSpawnedInChunk;

    vt->base.findPlayerToAttack = EntityWolf_findPlayerToAttack;
    vt->base.attackEntity = EntityWolf_attackEntity;
    vt->base.isMovementCeased = EntityWolf_isMovementCeased;

    vt->base.base.base.onUpdate = EntityWolf_onUpdate;
}

void EntityWolf_construct(EntityWolf *self, World *var1) {
    if (!EntityWolf_vtable_init) {
        EntityWolf_initVtable(&EntityWolf_vtable);
        EntityWolf_vtable_init = 1;
    }
    EntityAnimal_construct(&self->base, var1);
    self->base.base.base.base.vtable = (EntityVtable *)&EntityWolf_vtable;
    self->base.base.base.base.isWolf = 1;

    self->base.base.base.texture = "/mob/wolf.png";
    Entity_setSize(&self->base.base.base.base, 0.8F, 0.8F);
    self->base.base.base.moveSpeed = 1.1F;
    self->base.base.base.health = 8;

    self->looksWithInterest = 0;
    self->field_25048_b = 0.0F;
    self->field_25054_c = 0.0F;
    self->isWolfShaking = 0;
    self->field_25052_g = 0;
    self->timeWolfIsShaking = 0.0F;
    self->prevTimeWolfIsShaking = 0.0F;
}

EntityWolf *EntityWolf_create(World *var1) {
    EntityWolf *self = (EntityWolf *)calloc(1, sizeof(EntityWolf));
    EntityWolf_construct(self, var1);
    return self;
}

void EntityWolf_entityInit(Entity *self) {

    Entity_defaultVtable.entityInit(self);
    DataWatcher_addByte(&self->dataWatcher, 16, (int8_t)0);
    DataWatcher_addString(&self->dataWatcher, 17, "");
    DataWatcher_addInt(&self->dataWatcher, 18, ((EntityLiving *)self)->health);
}

int EntityWolf_canTriggerWalking(Entity *self) {
    (void)self;
    return 0;
}

const char *EntityWolf_getEntityTexture(EntityLiving *self) {
    EntityWolf *wolf = (EntityWolf *)self;
    if (EntityWolf_isWolfTamed(wolf))
        return "/mob/wolf_tame.png";
    if (EntityWolf_isWolfAngry(wolf))
        return "/mob/wolf_angry.png";

    return self->texture;
}

void EntityWolf_writeEntityToNBT(Entity *self, NBTTagCompound *var1) {
    EntityWolf *wolf = (EntityWolf *)self;

    EntityLiving_defaultVtable.base.writeEntityToNBT(self, var1);
    NBTTagCompound_setBoolean(var1, "Angry", EntityWolf_isWolfAngry(wolf));
    NBTTagCompound_setBoolean(var1, "Sitting", EntityWolf_isWolfSitting(wolf));
    const char *owner = EntityWolf_getWolfOwner(wolf);
    if (owner == NULL) {
        NBTTagCompound_setString(var1, "Owner", "");
    } else {
        NBTTagCompound_setString(var1, "Owner", owner);
    }
}

void EntityWolf_readEntityFromNBT(Entity *self, NBTTagCompound *var1) {
    EntityWolf *wolf = (EntityWolf *)self;

    EntityLiving_defaultVtable.base.readEntityFromNBT(self, var1);
    EntityWolf_setWolfAngry(wolf, NBTTagCompound_getBoolean(var1, "Angry"));
    EntityWolf_setWolfSitting(wolf, NBTTagCompound_getBoolean(var1, "Sitting"));
    const char *var2 = NBTTagCompound_getString(var1, "Owner");
    if (var2 != NULL && var2[0] != '\0') {
        EntityWolf_setWolfOwner(wolf, var2);
        EntityWolf_setWolfTamed(wolf, 1);
    }
}

int EntityWolf_canDespawn(EntityLiving *self) {
    return !EntityWolf_isWolfTamed((EntityWolf *)self);
}

const char *EntityWolf_getLivingSound(EntityLiving *self) {
    EntityWolf *wolf = (EntityWolf *)self;
    if (EntityWolf_isWolfAngry(wolf))
        return "mob.wolf.growl";
    if (JavaRandom_nextInt(&self->base.rand, 3) == 0) {
        if (EntityWolf_isWolfTamed(wolf) &&
            DataWatcher_getWatchableObjectInt(&self->base.dataWatcher, 18) < 10)
            return "mob.wolf.whine";
        return "mob.wolf.panting";
    }
    return "mob.wolf.bark";
}

const char *EntityWolf_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.wolf.hurt";
}

const char *EntityWolf_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.wolf.death";
}

float EntityWolf_getSoundVolume(EntityLiving *self) {
    (void)self;
    return 0.4F;
}

int EntityWolf_getDropItemId(EntityLiving *self) {
    (void)self;
    return -1;
}

void EntityWolf_updatePlayerActionState(EntityLiving *self) {
    EntityWolf *wolf = (EntityWolf *)self;
    EntityCreature *cr = &wolf->base.base;
    Entity *e = &self->base;

    EntityCreature_updatePlayerActionState(self);

    if (!cr->hasAttacked && !EntityCreature_hasPath(cr) && EntityWolf_isWolfTamed(wolf) &&
        e->ridingEntity == NULL) {
        Entity *var3 = World_getPlayerEntityByName(e->worldObj, EntityWolf_getWolfOwner(wolf));
        if (var3 != NULL) {
            float var2 = Entity_getDistanceToEntity(var3, e);
            if (var2 > 5.0F) {
                getPathOrWalkableBlock(wolf, var3, var2);
            }
        } else if (!e->vtable->isInWater(e)) {
            EntityWolf_setWolfSitting(wolf, 1);
        }
    } else if (cr->playerToAttack == NULL && !EntityCreature_hasPath(cr) &&
               !EntityWolf_isWolfTamed(wolf) && JavaRandom_nextInt(&e->worldObj->rand, 100) == 0) {

        AxisAlignedBB *bb = AxisAlignedBB_getBoundingBoxFromPool(
            e->posX, e->posY, e->posZ, e->posX + 1.0, e->posY + 1.0, e->posZ + 1.0);
        AxisAlignedBB_expand(bb, 16.0, 4.0, 16.0);
        Entity *sheepList[64];
        int count = World_getEntitiesWithinAABB(e->worldObj, match_EntitySheep, bb, sheepList, 64);
        if (count > 0) {
            EntityCreature_setTarget(cr, sheepList[JavaRandom_nextInt(&e->worldObj->rand, count)]);
        }
    }

    if (e->vtable->isInWater(e)) {
        EntityWolf_setWolfSitting(wolf, 0);
    }

    if (!e->worldObj->isRemote) {
        DataWatcher_updateInt(&self->base.dataWatcher, 18, self->health);
    }
}

void EntityWolf_onLivingUpdate(EntityLiving *self) {
    EntityWolf *wolf = (EntityWolf *)self;
    EntityCreature *cr = &wolf->base.base;
    Entity *e = &self->base;

    EntityLiving_onLivingUpdateBase(self);

    wolf->looksWithInterest = 0;
    if (EntityLiving_hasCurrentTarget(self) && !EntityCreature_hasPath(cr) &&
        !EntityWolf_isWolfAngry(wolf)) {
        Entity *var1 = EntityLiving_getCurrentTarget(self);
        if (var1->isPlayer) {
            EntityPlayer *var2 = (EntityPlayer *)var1;
            ItemStack *var3 = InventoryPlayer_getCurrentItem(&var2->inventory);
            if (var3 != NULL) {
                if (!EntityWolf_isWolfTamed(wolf) && var3->itemID == Item_bone->shiftedIndex) {
                    wolf->looksWithInterest = 1;
                } else if (EntityWolf_isWolfTamed(wolf) && Item_itemsList[var3->itemID] != NULL &&
                           Item_itemsList[var3->itemID]->isFood) {
                    wolf->looksWithInterest =
                        ItemFood_getIsWolfsFavoriteMeat((ItemFood *)Item_itemsList[var3->itemID]);
                }
            }
        }
    }

    if (!self->isMultiplayerEntity && wolf->isWolfShaking && !wolf->field_25052_g &&
        !EntityCreature_hasPath(cr) && e->onGround) {
        wolf->field_25052_g = 1;
        wolf->timeWolfIsShaking = 0.0F;
        wolf->prevTimeWolfIsShaking = 0.0F;
        World_func_9425_a(e->worldObj, e, (int8_t)8);
    }
}

void EntityWolf_onUpdate(Entity *self) {
    EntityWolf *wolf = (EntityWolf *)self;
    EntityLiving *el = (EntityLiving *)self;

    self->vtable->onEntityUpdate(self);
    ((EntityLivingVtable *)self->vtable)->onLivingUpdate(el);

    wolf->field_25054_c = wolf->field_25048_b;
    if (wolf->looksWithInterest) {
        wolf->field_25048_b += (1.0F - wolf->field_25048_b) * 0.4F;
    } else {
        wolf->field_25048_b += (0.0F - wolf->field_25048_b) * 0.4F;
    }

    if (wolf->looksWithInterest) {
        el->numTicksToChaseTarget = 10;
    }

    if (Entity_isWet(self)) {
        wolf->isWolfShaking = 1;
        wolf->field_25052_g = 0;
        wolf->timeWolfIsShaking = 0.0F;
        wolf->prevTimeWolfIsShaking = 0.0F;
    } else if ((wolf->isWolfShaking || wolf->field_25052_g) && wolf->field_25052_g) {
        if (wolf->timeWolfIsShaking == 0.0F) {
            float vol = EntityWolf_getSoundVolume(el);
            float pitch =
                (JavaRandom_nextFloat(&self->rand) - JavaRandom_nextFloat(&self->rand)) * 0.2F +
                1.0F;
            World_playSoundAtEntity(self->worldObj, self, "mob.wolf.shake", vol, pitch);
        }

        wolf->prevTimeWolfIsShaking = wolf->timeWolfIsShaking;
        wolf->timeWolfIsShaking += 0.05F;
        if (wolf->prevTimeWolfIsShaking >= 2.0F) {
            wolf->isWolfShaking = 0;
            wolf->field_25052_g = 0;
            wolf->prevTimeWolfIsShaking = 0.0F;
            wolf->timeWolfIsShaking = 0.0F;
        }

        if (wolf->timeWolfIsShaking > 0.4F) {
            float var1 = (float)self->boundingBox.minY;
            int var2 = (int)(MathHelper_sin((wolf->timeWolfIsShaking - 0.4F) * (float)M_PI) * 7.0F);
            for (int var3 = 0; var3 < var2; ++var3) {
                float var4 = (JavaRandom_nextFloat(&self->rand) * 2.0F - 1.0F) * self->width * 0.5F;
                float var5 = (JavaRandom_nextFloat(&self->rand) * 2.0F - 1.0F) * self->width * 0.5F;
                World_spawnParticle(self->worldObj, "splash", self->posX + (double)var4,
                                    (double)(var1 + 0.8F), self->posZ + (double)var5, self->motionX,
                                    self->motionY, self->motionZ);
            }
        }
    }
}

int EntityWolf_attackEntityFrom(Entity *self, Entity *var1, int var2) {
    EntityWolf *wolf = (EntityWolf *)self;
    EntityCreature *cr = &wolf->base.base;

    EntityWolf_setWolfSitting(wolf, 0);
    if (var1 != NULL && !var1->isPlayer && !var1->isArrow) {
        var2 = (var2 + 1) / 2;
    }

    if (!EntityLiving_defaultVtable.base.attackEntityFrom(self, var1, var2)) {
        return 0;
    }

    if (!EntityWolf_isWolfTamed(wolf) && !EntityWolf_isWolfAngry(wolf)) {
        if (var1 != NULL && var1->isPlayer) {
            EntityWolf_setWolfAngry(wolf, 1);
            cr->playerToAttack = var1;
        }
        if (var1 != NULL && var1->isArrow && ((EntityArrow *)var1)->owner != NULL) {
            var1 = (Entity *)((EntityArrow *)var1)->owner;
        }
        if (var1 != NULL && var1->isLiving) {
            AxisAlignedBB *bb = AxisAlignedBB_getBoundingBoxFromPool(
                self->posX, self->posY, self->posZ, self->posX + 1.0, self->posY + 1.0,
                self->posZ + 1.0);
            AxisAlignedBB_expand(bb, 16.0, 4.0, 16.0);
            Entity *wolfList[64];
            int count =
                World_getEntitiesWithinAABB(self->worldObj, match_EntityWolf, bb, wolfList, 64);
            for (int i = 0; i < count; ++i) {
                EntityWolf *var6 = (EntityWolf *)wolfList[i];
                if (!EntityWolf_isWolfTamed(var6) && var6->base.base.playerToAttack == NULL) {
                    var6->base.base.playerToAttack = var1;
                    if (var1->isPlayer) {
                        EntityWolf_setWolfAngry(var6, 1);
                    }
                }
            }
        }
    } else if (var1 != self && var1 != NULL) {
        if (EntityWolf_isWolfTamed(wolf) && var1->isPlayer) {
            EntityPlayer *ep = (EntityPlayer *)var1;
            if (strcasecmp(ep->username, EntityWolf_getWolfOwner(wolf)) == 0) {
                return 1;
            }
        }
        cr->playerToAttack = var1;
    }
    return 1;
}

Entity *EntityWolf_findPlayerToAttack(EntityCreature *self) {
    EntityWolf *wolf = (EntityWolf *)self;
    if (EntityWolf_isWolfAngry(wolf)) {
        return World_getClosestPlayerToEntity(self->base.base.worldObj, &self->base.base, 16.0);
    }
    return NULL;
}

void EntityWolf_attackEntity(EntityCreature *self, Entity *var1, float var2) {
    EntityWolf *wolf = (EntityWolf *)self;
    Entity *e = &self->base.base;

    if (var2 > 2.0F && var2 < 6.0F && JavaRandom_nextInt(&e->rand, 10) == 0) {
        if (e->onGround) {
            double var8 = var1->posX - e->posX;
            double var5 = var1->posZ - e->posZ;
            float var7 = MathHelper_sqrt_double(var8 * var8 + var5 * var5);
            e->motionX = var8 / (double)var7 * 0.5 * (double)0.8F + e->motionX * (double)0.2F;
            e->motionZ = var5 / (double)var7 * 0.5 * (double)0.8F + e->motionZ * (double)0.2F;
            e->motionY = (double)0.4F;
        }
    } else if ((double)var2 < 1.5 && var1->boundingBox.maxY > e->boundingBox.minY &&
               var1->boundingBox.minY < e->boundingBox.maxY) {
        self->base.attackTime = 20;
        int8_t var3 = 2;
        if (EntityWolf_isWolfTamed(wolf)) {
            var3 = 4;
        }
        var1->vtable->attackEntityFrom(var1, e, var3);
    }
}

int EntityWolf_interact(Entity *self, EntityPlayer *var1) {
    EntityWolf *wolf = (EntityWolf *)self;

    ItemStack *var2 = InventoryPlayer_getCurrentItem(&var1->inventory);
    if (!EntityWolf_isWolfTamed(wolf)) {
        if (var2 != NULL && var2->itemID == Item_bone->shiftedIndex &&
            !EntityWolf_isWolfAngry(wolf)) {
            --var2->stackSize;
            if (var2->stackSize <= 0) {
                InventoryPlayer_setInventorySlotContents(&var1->inventory,
                                                         var1->inventory.currentItem, NULL);
            }
            if (!self->worldObj->isRemote) {
                if (JavaRandom_nextInt(&self->rand, 3) == 0) {
                    EntityWolf_setWolfTamed(wolf, 1);
                    EntityCreature_setPathToEntity(&wolf->base.base, NULL);
                    EntityWolf_setWolfSitting(wolf, 1);
                    ((EntityLiving *)self)->health = 20;
                    EntityWolf_setWolfOwner(wolf, var1->username);
                    showHeartsOrSmokeFX(wolf, 1);
                    World_func_9425_a(self->worldObj, self, (int8_t)7);
                } else {
                    showHeartsOrSmokeFX(wolf, 0);
                    World_func_9425_a(self->worldObj, self, (int8_t)6);
                }
            }
            return 1;
        }
    } else {
        if (var2 != NULL && Item_itemsList[var2->itemID] != NULL &&
            Item_itemsList[var2->itemID]->isFood) {
            ItemFood *var3 = (ItemFood *)Item_itemsList[var2->itemID];
            if (ItemFood_getIsWolfsFavoriteMeat(var3) &&
                DataWatcher_getWatchableObjectInt(&self->dataWatcher, 18) < 20) {
                --var2->stackSize;
                if (var2->stackSize <= 0) {
                    InventoryPlayer_setInventorySlotContents(&var1->inventory,
                                                             var1->inventory.currentItem, NULL);
                }
                EntityLiving_heal((EntityLiving *)self,
                                  ItemFood_getHealAmount((ItemFood *)Item_porkRaw));
                return 1;
            }
        }

        if (strcasecmp(var1->username, EntityWolf_getWolfOwner(wolf)) == 0) {
            if (!self->worldObj->isRemote) {
                EntityWolf_setWolfSitting(wolf, !EntityWolf_isWolfSitting(wolf));
                ((EntityLiving *)self)->isJumping = 0;
                EntityCreature_setPathToEntity(&wolf->base.base, NULL);
            }
            return 1;
        }
    }
    return 0;
}

float EntityWolf_getEyeHeight(Entity *self) { return self->height * 0.8F; }

int EntityWolf_func_25026_x(EntityLiving *self) {
    EntityWolf *wolf = (EntityWolf *)self;
    if (EntityWolf_isWolfSitting(wolf))
        return 20;

    return 40;
}

int EntityWolf_isMovementCeased(EntityCreature *self) {
    EntityWolf *wolf = (EntityWolf *)self;
    return EntityWolf_isWolfSitting(wolf) || wolf->field_25052_g;
}

void EntityWolf_handleHealthUpdate(Entity *self, int8_t var1) {
    EntityWolf *wolf = (EntityWolf *)self;
    if (var1 == 7) {
        showHeartsOrSmokeFX(wolf, 1);
    } else if (var1 == 6) {
        showHeartsOrSmokeFX(wolf, 0);
    } else if (var1 == 8) {
        wolf->field_25052_g = 1;
        wolf->timeWolfIsShaking = 0.0F;
        wolf->prevTimeWolfIsShaking = 0.0F;
    } else {

        EntityLiving_defaultVtable.base.handleHealthUpdate(self, var1);
    }
}

int EntityWolf_getMaxSpawnedInChunk(EntityLiving *self) {
    (void)self;
    return 8;
}

static void showHeartsOrSmokeFX(EntityWolf *self, int var1) {
    Entity *e = &self->base.base.base.base;
    EntityLiving *el = (EntityLiving *)e;
    const char *var2 = var1 ? "heart" : "smoke";
    for (int var3 = 0; var3 < 7; ++var3) {
        double var4 = JavaRandom_nextGaussian(&e->rand) * 0.02;
        double var6 = JavaRandom_nextGaussian(&e->rand) * 0.02;
        double var8 = JavaRandom_nextGaussian(&e->rand) * 0.02;
        World_spawnParticle(
            e->worldObj, var2,
            e->posX + (double)(JavaRandom_nextFloat(&e->rand) * el->base.width * 2.0F) -
                (double)el->base.width,
            e->posY + 0.5 + (double)(JavaRandom_nextFloat(&e->rand) * el->base.height),
            e->posZ + (double)(JavaRandom_nextFloat(&e->rand) * el->base.width * 2.0F) -
                (double)el->base.width,
            var4, var6, var8);
    }
}

static void getPathOrWalkableBlock(EntityWolf *self, Entity *var1, float var2) {
    Entity *e = &self->base.base.base.base;
    EntityCreature *cr = &self->base.base;

    struct PathEntity *var3 = World_getPathToEntity(e->worldObj, e, var1, 16.0F);
    if (var3 == NULL && var2 > 12.0F) {
        int var4 = MathHelper_floor_double(var1->posX) - 2;
        int var5 = MathHelper_floor_double(var1->posZ) - 2;
        int var6 = MathHelper_floor_double(var1->boundingBox.minY);
        for (int var7 = 0; var7 <= 4; ++var7) {
            for (int var8 = 0; var8 <= 4; ++var8) {
                if ((var7 < 1 || var8 < 1 || var7 > 3 || var8 > 3) &&
                    World_isBlockNormalCube(e->worldObj, var4 + var7, var6 - 1, var5 + var8) &&
                    !World_isBlockNormalCube(e->worldObj, var4 + var7, var6, var5 + var8) &&
                    !World_isBlockNormalCube(e->worldObj, var4 + var7, var6 + 1, var5 + var8)) {
                    e->vtable->setLocationAndAngles(e, (double)((float)(var4 + var7) + 0.5F),
                                                    (double)var6,
                                                    (double)((float)(var5 + var8) + 0.5F),
                                                    ((EntityLiving *)e)->base.rotationYaw,
                                                    ((EntityLiving *)e)->base.rotationPitch);
                    return;
                }
            }
        }
    } else {
        EntityCreature_setPathToEntity(cr, var3);
    }
}

int EntityWolf_getWolfShaking(EntityWolf *self) { return self->isWolfShaking; }

float EntityWolf_getShadingWhileShaking(EntityWolf *self, float var1) {
    return 12.0F / 16.0F + (self->prevTimeWolfIsShaking +
                            (self->timeWolfIsShaking - self->prevTimeWolfIsShaking) * var1) /
                               2.0F * 0.25F;
}

float EntityWolf_getShakeAngle(EntityWolf *self, float var1, float var2) {
    float var3 = (self->prevTimeWolfIsShaking +
                  (self->timeWolfIsShaking - self->prevTimeWolfIsShaking) * var1 + var2) /
                 1.8F;
    if (var3 < 0.0F)
        var3 = 0.0F;
    else if (var3 > 1.0F)
        var3 = 1.0F;
    return MathHelper_sin(var3 * (float)M_PI) * MathHelper_sin(var3 * (float)M_PI * 11.0F) * 0.15F *
           (float)M_PI;
}

float EntityWolf_getInterestedAngle(EntityWolf *self, float var1) {
    return (self->field_25054_c + (self->field_25048_b - self->field_25054_c) * var1) * 0.15F *
           (float)M_PI;
}

float EntityWolf_setTailRotation(EntityWolf *self) {
    EntityLiving *el = (EntityLiving *)self;
    if (EntityWolf_isWolfAngry(self))
        return (float)M_PI * 0.49F;
    if (EntityWolf_isWolfTamed(self)) {
        return (0.55F - (float)(20 - DataWatcher_getWatchableObjectInt(&el->base.dataWatcher, 18)) *
                            0.02F) *
               (float)M_PI;
    }
    return (float)M_PI * 0.2F;
}

const char *EntityWolf_getWolfOwner(EntityWolf *self) {
    return DataWatcher_getWatchableObjectString(&self->base.base.base.base.dataWatcher, 17);
}

void EntityWolf_setWolfOwner(EntityWolf *self, const char *var1) {
    DataWatcher_updateString(&self->base.base.base.base.dataWatcher, 17, var1);
}

int EntityWolf_isWolfSitting(EntityWolf *self) {
    return (DataWatcher_getWatchableObjectByte(&self->base.base.base.base.dataWatcher, 16) & 1) !=
           0;
}

void EntityWolf_setWolfSitting(EntityWolf *self, int var1) {
    DataWatcher *dw = &self->base.base.base.base.dataWatcher;
    int8_t var2 = DataWatcher_getWatchableObjectByte(dw, 16);
    if (var1)
        DataWatcher_updateByte(dw, 16, (int8_t)(var2 | 1));
    else
        DataWatcher_updateByte(dw, 16, (int8_t)(var2 & (int8_t)-2));
}

int EntityWolf_isWolfAngry(EntityWolf *self) {
    return (DataWatcher_getWatchableObjectByte(&self->base.base.base.base.dataWatcher, 16) & 2) !=
           0;
}

void EntityWolf_setWolfAngry(EntityWolf *self, int var1) {
    DataWatcher *dw = &self->base.base.base.base.dataWatcher;
    int8_t var2 = DataWatcher_getWatchableObjectByte(dw, 16);
    if (var1)
        DataWatcher_updateByte(dw, 16, (int8_t)(var2 | 2));
    else
        DataWatcher_updateByte(dw, 16, (int8_t)(var2 & (int8_t)-3));
}

int EntityWolf_isWolfTamed(EntityWolf *self) {
    return (DataWatcher_getWatchableObjectByte(&self->base.base.base.base.dataWatcher, 16) & 4) !=
           0;
}

void EntityWolf_setWolfTamed(EntityWolf *self, int var1) {
    DataWatcher *dw = &self->base.base.base.base.dataWatcher;
    int8_t var2 = DataWatcher_getWatchableObjectByte(dw, 16);
    if (var1)
        DataWatcher_updateByte(dw, 16, (int8_t)(var2 | 4));
    else
        DataWatcher_updateByte(dw, 16, (int8_t)(var2 & (int8_t)-5));
}
