#include "EntityPig.h"
#include "EntityPigZombie.h"
#include "World.h"
#include "Item.h"
#include "ItemStack.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "AchievementList.h"
#include "DataWatcher.h"
#include "NBTTagCompound.h"
#include <stdlib.h>

static void pig_entityInit(Entity *e) {
    extern EntityVtable Entity_defaultVtable;
    Entity_defaultVtable.entityInit(e);
    DataWatcher_addByte(&e->dataWatcher, 16, (int8_t)0);
}

static void pig_writeEntityToNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.writeEntityToNBT(e, var1);
    NBTTagCompound_setBoolean(var1, "Saddle", EntityPig_getSaddled((EntityPig *)e));
}

static void pig_readEntityFromNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.readEntityFromNBT(e, var1);
    EntityPig_setSaddled((EntityPig *)e, NBTTagCompound_getBoolean(var1, "Saddle"));
}

static const char *pig_getLivingSound(EntityLiving *self) {
    (void)self;
    return "mob.pig";
}

static const char *pig_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.pig";
}

static const char *pig_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.pigdeath";
}

static int pig_interact(Entity *e, EntityPlayer *var1) {
    if (!EntityPig_getSaddled((EntityPig *)e) || e->worldObj->multiplayerWorld ||
        (e->riddenByEntity != NULL && e->riddenByEntity != (Entity *)var1)) {
        return 0;
    }
    Entity_mountEntity((Entity *)var1, e);
    return 1;
}

static int pig_getDropItemId(EntityLiving *base) {
    return base->base.fire > 0 ? Item_porkCooked->shiftedIndex : Item_porkRaw->shiftedIndex;
}

static void pig_fall(Entity *e, float var1) {
    extern EntityVtable Entity_defaultVtable;
    Entity_defaultVtable.fall(e, var1);
    if (var1 > 5.0f && e->riddenByEntity != NULL && e->riddenByEntity->isPlayer) {
        EntityPlayer_triggerAchievement((EntityPlayer *)e->riddenByEntity,
                                        (struct StatBase *)AchievementList_flyPig);
    }
}

static void pig_onStruckByLightning(Entity *e, struct EntityLightningBolt *var1) {
    (void)var1;
    if (!e->worldObj->multiplayerWorld) {
        EntityPigZombie *var2 = EntityPigZombie_create(e->worldObj);
        Entity *pze = &var2->base.base.base.base.base;
        pze->vtable->setLocationAndAngles(pze, e->posX, e->posY, e->posZ, e->rotationYaw,
                                          e->rotationPitch);
        World_entityJoinedWorld(e->worldObj, pze);
        Entity_setEntityDead(e);
    }
}

static const char *pig_getEntityString(Entity *self) {
    (void)self;
    return "Pig";
}

EntityPig *EntityPig_create(World *world) {
    static EntityAnimalVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        EntityAnimal_initVtable(&vtable);
        vtable.base.base.base.getEntityString = pig_getEntityString;
        vtable.base.base.base.entityInit = pig_entityInit;
        vtable.base.base.base.writeEntityToNBT = pig_writeEntityToNBT;
        vtable.base.base.base.readEntityFromNBT = pig_readEntityFromNBT;
        vtable.base.base.base.interact = pig_interact;
        vtable.base.base.base.fall = pig_fall;
        vtable.base.base.base.onStruckByLightning = pig_onStruckByLightning;
        vtable.base.base.getLivingSound = pig_getLivingSound;
        vtable.base.base.getHurtSound = pig_getHurtSound;
        vtable.base.base.getDeathSound = pig_getDeathSound;
        vtable.base.base.getDropItemId = pig_getDropItemId;
        vtable_init = 1;
    }

    EntityPig *self = (EntityPig *)calloc(1, sizeof(EntityPig));
    EntityAnimal_construct(&self->base, world);
    self->base.base.base.base.isPig = 1;
    self->base.base.base.base.vtable = (const EntityVtable *)&vtable;
    self->base.base.base.texture = "/mob/pig.png";
    Entity_setSize(&self->base.base.base.base, 0.9f, 0.9f);
    self->base.base.base.base.vtable->entityInit(&self->base.base.base.base);
    return self;
}

int EntityPig_getSaddled(EntityPig *self) {
    return (DataWatcher_getWatchableObjectByte(&self->base.base.base.base.dataWatcher, 16) & 1) !=
           0;
}

void EntityPig_setSaddled(EntityPig *self, int var1) {
    DataWatcher_updateByte(&self->base.base.base.base.dataWatcher, 16, (int8_t)(var1 ? 1 : 0));
}
