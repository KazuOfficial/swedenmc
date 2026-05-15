#include "EntitySlime.h"
#include "World.h"
#include "Item.h"
#include "MathHelper.h"
#include "DataWatcher.h"
#include "Chunk.h"
#include "EntityPlayer.h"
#include <math.h>
#include <stdlib.h>

EntityLivingVtable EntitySlime_defaultVtable;
static int EntitySlime_vtable_init = 0;

static const char *slime_getEntityString(Entity *self) {
    (void)self;
    return "Slime";
}

void EntitySlime_initVtable(EntityLivingVtable *vt) {
    EntityLiving_initVtable(vt);
    vt->base.getEntityString = slime_getEntityString;
    vt->base.entityInit = EntitySlime_entityInit;
    vt->base.onUpdate = EntitySlime_onUpdate;
    vt->base.writeEntityToNBT = EntitySlime_writeEntityToNBT;
    vt->base.readEntityFromNBT = EntitySlime_readEntityFromNBT;
    vt->base.setEntityDead = EntitySlime_setEntityDead;
    vt->base.onCollideWithPlayer =
        (void (*)(Entity *, EntityPlayer *))EntitySlime_onCollideWithPlayer;
    vt->updatePlayerActionState = EntitySlime_updatePlayerActionState;
    vt->getHurtSound = EntitySlime_getHurtSound;
    vt->getDeathSound = EntitySlime_getDeathSound;
    vt->getDropItemId = EntitySlime_getDropItemId;
    vt->getCanSpawnHere = EntitySlime_getCanSpawnHere;
    vt->getSoundVolume = EntitySlime_getSoundVolume;
}

static void ensure_vtable(void) {
    if (EntitySlime_vtable_init)
        return;
    EntitySlime_initVtable(&EntitySlime_defaultVtable);
    EntitySlime_vtable_init = 1;
}

void EntitySlime_construct(EntitySlime *self, World *var1) {
    ensure_vtable();
    EntityLiving_construct(&self->base, var1);
    self->base.base.vtable = (const EntityVtable *)&EntitySlime_defaultVtable;
    self->base.texture = "/mob/slime.png";

    int var2 = 1 << JavaRandom_nextInt(&self->base.base.rand, 3);
    self->base.base.yOffset = 0.0F;
    self->slimeJumpDelay = JavaRandom_nextInt(&self->base.base.rand, 20) + 10;
    self->field_768_a = 0.0F;
    self->field_767_b = 0.0F;
    EntitySlime_setSlimeSize(self, var2);
    self->base.base.isSlime = 1;
}

EntitySlime *EntitySlime_create(World *var1) {
    EntitySlime *self = (EntitySlime *)calloc(1, sizeof(EntitySlime));
    EntitySlime_construct(self, var1);
    return self;
}

void EntitySlime_entityInit(Entity *selfE) {

    EntityLiving_defaultVtable.base.entityInit(selfE);

    DataWatcher_addByte(&selfE->dataWatcher, 16, (int8_t)1);
}

void EntitySlime_setSlimeSize(EntitySlime *self, int var1) {
    Entity *selfE = &self->base.base;

    DataWatcher_updateByte(&selfE->dataWatcher, 16, (int8_t)var1);
    Entity_setSize(selfE, 0.6F * (float)var1, 0.6F * (float)var1);
    self->base.health = var1 * var1;

    selfE->vtable->setPosition(selfE, selfE->posX, selfE->posY, selfE->posZ);
}

int EntitySlime_getSlimeSize(EntitySlime *self) {
    return (int)DataWatcher_getWatchableObjectByte(&self->base.base.dataWatcher, 16);
}

void EntitySlime_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {

    EntityLiving_defaultVtable.base.writeEntityToNBT(selfE, var1);
    EntitySlime *self = (EntitySlime *)selfE;
    NBTTagCompound_setInteger(var1, "Size", EntitySlime_getSlimeSize(self) - 1);
}

void EntitySlime_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {

    EntityLiving_defaultVtable.base.readEntityFromNBT(selfE, var1);
    EntitySlime *self = (EntitySlime *)selfE;
    EntitySlime_setSlimeSize(self, NBTTagCompound_getInteger(var1, "Size") + 1);
}

void EntitySlime_onUpdate(Entity *selfE) {
    EntitySlime *self = (EntitySlime *)selfE;
    self->field_767_b = self->field_768_a;
    int var1 = selfE->onGround;

    EntityLiving_defaultVtable.base.onUpdate(selfE);
    if (selfE->onGround && !var1) {
        int var2 = EntitySlime_getSlimeSize(self);
        for (int var3 = 0; var3 < var2 * 8; ++var3) {
            float var4 = JavaRandom_nextFloat(&selfE->rand) * (float)M_PI * 2.0F;
            float var5 = JavaRandom_nextFloat(&selfE->rand) * 0.5F + 0.5F;
            float var6 = MathHelper_sin(var4) * (float)var2 * 0.5F * var5;
            float var7 = MathHelper_cos(var4) * (float)var2 * 0.5F * var5;
            World_spawnParticle(selfE->worldObj, "slime", selfE->posX + (double)var6,
                                selfE->boundingBox.minY, selfE->posZ + (double)var7, 0.0, 0.0, 0.0);
        }
        if (var2 > 2) {
            EntityLivingVtable *lv = (EntityLivingVtable *)selfE->vtable;
            World_playSoundAtEntity(
                selfE->worldObj, selfE, "mob.slime", lv->getSoundVolume((EntityLiving *)selfE),
                ((JavaRandom_nextFloat(&selfE->rand) - JavaRandom_nextFloat(&selfE->rand)) * 0.2F +
                 1.0F) /
                    0.8F);
        }
        self->field_768_a = -0.5F;
    }
    self->field_768_a *= 0.6F;
}

void EntitySlime_updatePlayerActionState(EntityLiving *selfL) {
    EntitySlime *self = (EntitySlime *)selfL;
    Entity *selfE = &selfL->base;
    EntityLiving_func_27021_X(selfL);

    EntityPlayer *var1 =
        (EntityPlayer *)World_getClosestPlayerToEntity(selfE->worldObj, selfE, 16.0);
    if (var1 != NULL) {
        EntityLiving_faceEntity(selfL, (Entity *)var1, 10.0F, 20.0F);
    }
    if (selfE->onGround && self->slimeJumpDelay-- <= 0) {
        self->slimeJumpDelay = JavaRandom_nextInt(&selfE->rand, 20) + 10;
        if (var1 != NULL) {
            self->slimeJumpDelay /= 3;
        }
        selfL->isJumping = 1;
        if (EntitySlime_getSlimeSize(self) > 1) {
            EntityLivingVtable *lv = (EntityLivingVtable *)selfE->vtable;
            World_playSoundAtEntity(
                selfE->worldObj, selfE, "mob.slime", lv->getSoundVolume(selfL),
                ((JavaRandom_nextFloat(&selfE->rand) - JavaRandom_nextFloat(&selfE->rand)) * 0.2F +
                 1.0F) *
                    0.8F);
        }
        self->field_768_a = 1.0F;
        selfL->moveStrafing = 1.0F - JavaRandom_nextFloat(&selfE->rand) * 2.0F;
        selfL->moveForward = (float)(1 * EntitySlime_getSlimeSize(self));
    } else {
        selfL->isJumping = 0;
        if (selfE->onGround) {
            selfL->moveStrafing = selfL->moveForward = 0.0F;
        }
    }
}

void EntitySlime_setEntityDead(Entity *selfE) {
    EntitySlime *self = (EntitySlime *)selfE;
    EntityLiving *selfL = (EntityLiving *)selfE;
    int var1 = EntitySlime_getSlimeSize(self);
    if (!selfE->worldObj->multiplayerWorld && var1 > 1 && selfL->health == 0) {
        for (int var2 = 0; var2 < 4; ++var2) {
            float var3 = ((float)(var2 % 2) - 0.5F) * (float)var1 / 4.0F;
            float var4 = ((float)(var2 / 2) - 0.5F) * (float)var1 / 4.0F;
            EntitySlime *var5 = EntitySlime_create(selfE->worldObj);
            EntitySlime_setSlimeSize(var5, var1 / 2);

            var5->base.base.vtable->setLocationAndAngles(
                &var5->base.base, selfE->posX + (double)var3, selfE->posY + 0.5,
                selfE->posZ + (double)var4, JavaRandom_nextFloat(&selfE->rand) * 360.0F, 0.0F);
            World_entityJoinedWorld(selfE->worldObj, &var5->base.base);
        }
    }

    EntityLiving_defaultVtable.base.setEntityDead(selfE);
}

void EntitySlime_onCollideWithPlayer(Entity *selfE, struct EntityPlayer *var1) {
    EntitySlime *self = (EntitySlime *)selfE;
    EntityLiving *selfL = (EntityLiving *)selfE;
    int var2 = EntitySlime_getSlimeSize(self);
    if (var2 > 1 && EntityLiving_canEntityBeSeen(selfL, (Entity *)var1) &&
        (double)Entity_getDistanceToEntity(selfE, (Entity *)var1) < 0.6 * (double)var2 &&
        ((Entity *)var1)->vtable->attackEntityFrom((Entity *)var1, selfE, var2)) {
        World_playSoundAtEntity(
            selfE->worldObj, selfE, "mob.slimeattack", 1.0F,
            (JavaRandom_nextFloat(&selfE->rand) - JavaRandom_nextFloat(&selfE->rand)) * 0.2F +
                1.0F);
    }
}

const char *EntitySlime_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.slime";
}

const char *EntitySlime_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.slime";
}

int EntitySlime_getDropItemId(EntityLiving *self) {
    EntitySlime *s = (EntitySlime *)self;
    return EntitySlime_getSlimeSize(s) == 1 ? Item_slimeBall->shiftedIndex : 0;
}

float EntitySlime_getSoundVolume(EntityLiving *self) {
    (void)self;
    return 0.6F;
}

int EntitySlime_getCanSpawnHere(EntityLiving *selfL) {
    EntitySlime *self = (EntitySlime *)selfL;
    Entity *selfE = &selfL->base;

    Chunk *var1 = selfE->worldObj->vtable->getChunkFromBlockCoords(
        selfE->worldObj, MathHelper_floor_double(selfE->posX),
        MathHelper_floor_double(selfE->posZ));

    JavaRandom chunkRand;
    Chunk_func_997_a(var1, 987234911L, &chunkRand);
    return (EntitySlime_getSlimeSize(self) == 1 || selfE->worldObj->difficultySetting > 0) &&
           JavaRandom_nextInt(&selfE->rand, 10) == 0 && JavaRandom_nextInt(&chunkRand, 10) == 0 &&
           selfE->posY < 16.0;
}
