#include "EntityMob.h"
#include "World.h"
#include "MathHelper.h"
#include "EnumSkyBlock.h"

static Entity *def_findPlayerToAttack(EntityCreature *base) {
    EntityMob *self = (EntityMob *)base;
    (void)self;
    Entity *e = &base->base.base;
    Entity *var1 = World_getClosestPlayerToEntity(e->worldObj, e, 16.0);
    if (var1 != NULL && EntityLiving_canEntityBeSeen(&base->base, var1)) {
        return var1;
    }
    return NULL;
}

void EntityMob_attackEntity(EntityCreature *base, Entity *var1, float var2) {
    EntityMob *self = (EntityMob *)base;
    EntityLiving *el = &base->base;
    Entity *e = &el->base;
    if (el->attackTime <= 0 && var2 < 2.0f && var1->boundingBox.maxY > e->boundingBox.minY &&
        var1->boundingBox.minY < e->boundingBox.maxY) {
        el->attackTime = 20;
        var1->vtable->attackEntityFrom(var1, e, self->attackStrength);
    }
}

static float def_getBlockPathWeight(EntityCreature *base, int var1, int var2, int var3) {
    Entity *e = &base->base.base;
    return 0.5f - (float)e->worldObj->vtable->getLightBrightness(e->worldObj, var1, var2, var3);
}

void EntityMob_onLivingUpdate(EntityLiving *base) {
    EntityMob *self = (EntityMob *)base;
    (void)self;
    Entity *e = &base->base;

    float var1 = e->vtable->getEntityBrightness(e, 1.0f);
    if (var1 > 0.5f) {
        base->entityAge += 2;
    }

    EntityLiving_onLivingUpdateBase(base);
}

static void def_onLivingUpdate(EntityLiving *base) { EntityMob_onLivingUpdate(base); }

static void def_onUpdate(Entity *e) {

    extern EntityLivingVtable EntityLiving_defaultVtable;
    EntityLiving_defaultVtable.base.onUpdate(e);

    if (e->worldObj && !e->worldObj->multiplayerWorld && e->worldObj->difficultySetting == 0) {
        Entity_setEntityDead(e);
    }
}

static int def_attackEntityFrom(Entity *e, Entity *var1, int var2) {
    EntityCreature *self = (EntityCreature *)e;
    extern EntityLivingVtable EntityLiving_defaultVtable;
    if (EntityLiving_defaultVtable.base.attackEntityFrom(e, var1, var2)) {
        if (e->riddenByEntity != var1 && e->ridingEntity != var1) {
            if (var1 != e) {
                self->playerToAttack = var1;
            }
        }
        return 1;
    }
    return 0;
}

static int def_getCanSpawnHere(EntityLiving *base) {
    Entity *e = &base->base;
    int var1 = MathHelper_floor_double(e->posX);
    int var2 = MathHelper_floor_double(e->boundingBox.minY);
    int var3 = MathHelper_floor_double(e->posZ);
    if (World_getSavedLightValue(e->worldObj, EnumSkyBlock_Sky, var1, var2, var3) >
        JavaRandom_nextInt(&e->rand, 32)) {
        return 0;
    }
    int var4 = World_getBlockLightValue(e->worldObj, var1, var2, var3);
    if (World_func_27160_B(e->worldObj)) {
        int var5 = e->worldObj->skylightSubtracted;
        e->worldObj->skylightSubtracted = 10;
        var4 = World_getBlockLightValue(e->worldObj, var1, var2, var3);
        e->worldObj->skylightSubtracted = var5;
    }
    return var4 <= JavaRandom_nextInt(&e->rand, 8) &&
           EntityCreature_getCanSpawnHere((EntityCreature *)base);
}

void EntityMob_initVtable(EntityMobVtable *vt) {
    EntityCreature_initVtable(&vt->base);
    vt->base.base.base.onUpdate = def_onUpdate;
    vt->base.base.base.attackEntityFrom = def_attackEntityFrom;
    vt->base.base.onLivingUpdate = def_onLivingUpdate;
    vt->base.base.getCanSpawnHere = def_getCanSpawnHere;
    vt->base.findPlayerToAttack = def_findPlayerToAttack;
    vt->base.attackEntity = EntityMob_attackEntity;
    vt->base.getBlockPathWeight = def_getBlockPathWeight;
}

void EntityMob_construct(EntityMob *self, World *world) {
    EntityCreature_construct(&self->base, world);
    self->attackStrength = 2;
    self->base.base.health = 20;
    self->base.base.base.isMob = 1;
}
