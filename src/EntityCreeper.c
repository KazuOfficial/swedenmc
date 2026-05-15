#include "EntityCreeper.h"
#include "World.h"
#include "Item.h"
#include "DataWatcher.h"
#include "NBTTagCompound.h"
#include <stdlib.h>

static int getCreeperState(EntityCreeper *self) {
    return DataWatcher_getWatchableObjectByte(&self->base.base.base.base.dataWatcher, 16);
}

static void setCreeperState(EntityCreeper *self, int var1) {
    DataWatcher_updateByte(&self->base.base.base.base.dataWatcher, 16, (int8_t)var1);
}

static void creeper_entityInit(Entity *e) {
    extern EntityVtable Entity_defaultVtable;
    Entity_defaultVtable.entityInit(e);
    DataWatcher_addByte(&e->dataWatcher, 16, (int8_t)-1);
    DataWatcher_addByte(&e->dataWatcher, 17, (int8_t)0);
}

static void creeper_writeEntityToNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.writeEntityToNBT(e, var1);
    if (DataWatcher_getWatchableObjectByte(&e->dataWatcher, 17) == 1) {
        NBTTagCompound_setBoolean(var1, "powered", 1);
    }
}

static void creeper_readEntityFromNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.readEntityFromNBT(e, var1);
    DataWatcher_updateByte(&e->dataWatcher, 17,
                           (int8_t)(NBTTagCompound_getBoolean(var1, "powered") ? 1 : 0));
}

static void creeper_attackBlockedEntity(EntityCreature *base, Entity *var1, float var2) {
    EntityCreeper *self = (EntityCreeper *)base;
    (void)var1;
    (void)var2;
    Entity *e = &base->base.base;
    if (!e->worldObj->multiplayerWorld) {
        if (self->timeSinceIgnited > 0) {
            setCreeperState(self, -1);
            --self->timeSinceIgnited;
            if (self->timeSinceIgnited < 0) {
                self->timeSinceIgnited = 0;
            }
        }
    }
}

static void creeper_onUpdate(Entity *e) {
    EntityCreeper *self = (EntityCreeper *)e;
    EntityCreature *cr = (EntityCreature *)e;

    self->lastActiveTime = self->timeSinceIgnited;
    if (e->worldObj->multiplayerWorld) {
        int var1 = getCreeperState(self);
        if (var1 > 0 && self->timeSinceIgnited == 0) {
            World_playSoundAtEntity(e->worldObj, e, "random.fuse", 1.0f, 0.5f);
        }
        self->timeSinceIgnited += var1;
        if (self->timeSinceIgnited < 0) {
            self->timeSinceIgnited = 0;
        }
        if (self->timeSinceIgnited >= 30) {
            self->timeSinceIgnited = 30;
        }
    }

    extern EntityLivingVtable EntityLiving_defaultVtable;
    EntityLiving_defaultVtable.base.onUpdate(e);

    if (cr->playerToAttack == NULL && self->timeSinceIgnited > 0) {
        setCreeperState(self, -1);
        --self->timeSinceIgnited;
        if (self->timeSinceIgnited < 0) {
            self->timeSinceIgnited = 0;
        }
    }
}

static const char *creeper_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.creeper";
}

static const char *creeper_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.creeperdeath";
}

static void creeper_onDeath(EntityLiving *base, Entity *var1) {
    EntityCreeper *self = (EntityCreeper *)base;
    extern EntityLivingVtable EntityLiving_defaultVtable;
    EntityLiving_defaultVtable.onDeath(base, var1);
    if (var1 != NULL && var1->isSkeleton) {
        Entity_dropItem(&base->base,
                        Item_record13->shiftedIndex + JavaRandom_nextInt(&base->base.rand, 2), 1);
    }
    (void)self;
}

static void creeper_attackEntity(EntityCreature *base, Entity *var1, float var2) {
    EntityCreeper *self = (EntityCreeper *)base;
    Entity *e = &base->base.base;
    (void)var1;
    if (!e->worldObj->multiplayerWorld) {
        int var3 = getCreeperState(self);
        if ((var3 <= 0 && var2 < 3.0f) || (var3 > 0 && var2 < 7.0f)) {
            if (self->timeSinceIgnited == 0) {
                World_playSoundAtEntity(e->worldObj, e, "random.fuse", 1.0f, 0.5f);
            }
            setCreeperState(self, 1);
            ++self->timeSinceIgnited;
            if (self->timeSinceIgnited >= 30) {
                float power = EntityCreeper_getPowered(self) ? 6.0f : 3.0f;
                World_createExplosion(e->worldObj, e, e->posX, e->posY, e->posZ, power);
                Entity_setEntityDead(e);
            }
            base->hasAttacked = 1;
        } else {
            setCreeperState(self, -1);
            --self->timeSinceIgnited;
            if (self->timeSinceIgnited < 0) {
                self->timeSinceIgnited = 0;
            }
        }
    }
}

static int creeper_getDropItemId(EntityLiving *self) {
    (void)self;
    return Item_gunpowder->shiftedIndex;
}

static void creeper_onStruckByLightning(Entity *e, struct EntityLightningBolt *var1) {
    extern EntityVtable Entity_defaultVtable;
    Entity_defaultVtable.onStruckByLightning(e, var1);
    DataWatcher_updateByte(&e->dataWatcher, 17, (int8_t)1);
}

static const char *creeper_getEntityString(Entity *self) {
    (void)self;
    return "Creeper";
}

EntityCreeper *EntityCreeper_create(World *world) {
    static EntityMobVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        EntityMob_initVtable(&vtable);
        vtable.base.base.base.getEntityString = creeper_getEntityString;
        vtable.base.base.base.entityInit = creeper_entityInit;
        vtable.base.base.base.onUpdate = creeper_onUpdate;
        vtable.base.base.base.writeEntityToNBT = creeper_writeEntityToNBT;
        vtable.base.base.base.readEntityFromNBT = creeper_readEntityFromNBT;
        vtable.base.base.base.onStruckByLightning = creeper_onStruckByLightning;
        vtable.base.base.getHurtSound = creeper_getHurtSound;
        vtable.base.base.getDeathSound = creeper_getDeathSound;
        vtable.base.base.onDeath = creeper_onDeath;
        vtable.base.base.getDropItemId = creeper_getDropItemId;
        vtable.base.attackBlockedEntity = creeper_attackBlockedEntity;
        vtable.base.attackEntity = creeper_attackEntity;
        vtable_init = 1;
    }

    EntityCreeper *self = (EntityCreeper *)calloc(1, sizeof(EntityCreeper));
    EntityMob_construct(&self->base, world);
    self->base.base.base.base.vtable = (const EntityVtable *)&vtable;
    self->base.base.base.base.isCreeper = 1;
    self->base.base.base.texture = "/mob/creeper.png";
    self->base.base.base.base.vtable->entityInit(&self->base.base.base.base);
    self->timeSinceIgnited = 0;
    self->lastActiveTime = 0;
    return self;
}

int EntityCreeper_getPowered(EntityCreeper *self) {
    return DataWatcher_getWatchableObjectByte(&self->base.base.base.base.dataWatcher, 17) == 1;
}

float EntityCreeper_setCreeperFlashTime(EntityCreeper *self, float var1) {
    return ((float)self->lastActiveTime +
            (float)(self->timeSinceIgnited - self->lastActiveTime) * var1) /
           28.0f;
}
