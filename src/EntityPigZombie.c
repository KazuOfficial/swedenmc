#include "EntityPigZombie.h"
#include "World.h"
#include "Item.h"
#include "ItemStack.h"
#include "AxisAlignedBB.h"
#include <stdlib.h>

ItemStack EntityPigZombie_defaultHeldItem;
static int defaultHeldItem_init = 0;

static EntityMobVtable EntityPigZombie_vtable;
static int EntityPigZombie_vtable_init = 0;

static const char *pigzombie_getEntityString(Entity *self) {
    (void)self;
    return "PigZombie";
}

void EntityPigZombie_initVtable(EntityMobVtable *vt) {
    EntityZombie_initVtable(vt);
    vt->base.base.base.getEntityString = pigzombie_getEntityString;
    vt->base.base.base.onUpdate = EntityPigZombie_onUpdate;
    vt->base.base.base.writeEntityToNBT = EntityPigZombie_writeEntityToNBT;
    vt->base.base.base.readEntityFromNBT = EntityPigZombie_readEntityFromNBT;
    vt->base.base.base.attackEntityFrom = EntityPigZombie_attackEntityFrom;
    vt->base.base.onLivingUpdate = EntityPigZombie_onLivingUpdate;
    vt->base.base.getLivingSound = EntityPigZombie_getLivingSound;
    vt->base.base.getHurtSound = EntityPigZombie_getHurtSound;
    vt->base.base.getDeathSound = EntityPigZombie_getDeathSound;
    vt->base.base.getDropItemId = EntityPigZombie_getDropItemId;
    vt->base.base.getCanSpawnHere = EntityPigZombie_getCanSpawnHere;
    vt->base.base.getHeldItem = EntityPigZombie_getHeldItem;
    vt->base.findPlayerToAttack = EntityPigZombie_findPlayerToAttack;
}

static void ensure_vtable(void) {
    if (EntityPigZombie_vtable_init)
        return;
    EntityPigZombie_initVtable(&EntityPigZombie_vtable);
    EntityPigZombie_vtable_init = 1;
}

void EntityPigZombie_construct(EntityPigZombie *self, World *var1) {
    ensure_vtable();

    EntityMob_construct(&self->base.base, var1);
    self->base.base.base.base.base.vtable = (const EntityVtable *)&EntityPigZombie_vtable;

    self->base.base.base.base.texture = "/mob/pigzombie.png";

    self->base.base.base.base.moveSpeed = 0.5F;

    self->base.base.attackStrength = 5;

    self->base.base.base.base.base.isImmuneToFire = 1;

    self->angerLevel = 0;
    self->randomSoundDelay = 0;

    if (!defaultHeldItem_init) {
        EntityPigZombie_defaultHeldItem = ItemStack_fromItemID(Item_swordGold->shiftedIndex, 1, 0);
        defaultHeldItem_init = 1;
    }
}

EntityPigZombie *EntityPigZombie_create(World *var1) {
    EntityPigZombie *self = (EntityPigZombie *)calloc(1, sizeof(EntityPigZombie));
    EntityPigZombie_construct(self, var1);
    return self;
}

void EntityPigZombie_onUpdate(Entity *selfE) {
    EntityPigZombie *self = (EntityPigZombie *)selfE;
    EntityCreature *cr = (EntityCreature *)selfE;

    cr->base.moveSpeed = cr->playerToAttack != NULL ? 0.95F : 0.5F;
    if (self->randomSoundDelay > 0 && --self->randomSoundDelay == 0) {
        EntityLivingVtable *lv = (EntityLivingVtable *)selfE->vtable;
        World_playSoundAtEntity(
            selfE->worldObj, selfE, "mob.zombiepig.zpigangry",
            lv->getSoundVolume((EntityLiving *)selfE) * 2.0F,
            ((JavaRandom_nextFloat(&selfE->rand) - JavaRandom_nextFloat(&selfE->rand)) * 0.2F +
             1.0F) *
                1.8F);
    }

    EntityZombie_defaultVtable.base.base.base.onUpdate(selfE);
}

int EntityPigZombie_getCanSpawnHere(EntityLiving *selfL) {
    Entity *selfE = &selfL->base;
    return selfE->worldObj->difficultySetting > 0 &&
           World_checkIfAABBIsClear(selfE->worldObj, &selfE->boundingBox) &&
           World_getCollidingBoundingBoxes(selfE->worldObj, selfE, &selfE->boundingBox, NULL, 0) ==
               0 &&
           !World_getIsAnyLiquid(selfE->worldObj, &selfE->boundingBox);
}

void EntityPigZombie_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityPigZombie *self = (EntityPigZombie *)selfE;

    EntityZombie_defaultVtable.base.base.base.writeEntityToNBT(selfE, var1);
    NBTTagCompound_setShort(var1, "Anger", (int16_t)self->angerLevel);
}

void EntityPigZombie_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityPigZombie *self = (EntityPigZombie *)selfE;

    EntityZombie_defaultVtable.base.base.base.readEntityFromNBT(selfE, var1);
    self->angerLevel = NBTTagCompound_getShort(var1, "Anger");
}

Entity *EntityPigZombie_findPlayerToAttack(EntityCreature *selfC) {
    EntityPigZombie *self = (EntityPigZombie *)selfC;
    if (self->angerLevel == 0) {
        return NULL;
    }

    return EntityZombie_defaultVtable.base.findPlayerToAttack(selfC);
}

void EntityPigZombie_onLivingUpdate(EntityLiving *selfL) {

    EntityZombie_defaultVtable.base.base.onLivingUpdate(selfL);
}

int EntityPigZombie_attackEntityFrom(Entity *selfE, Entity *var1, int var2) {
    EntityPigZombie *self = (EntityPigZombie *)selfE;
    (void)self;

    if (var1 != NULL && var1->isPlayer) {
        AxisAlignedBB *expanded = AxisAlignedBB_expand(&selfE->boundingBox, 32.0, 32.0, 32.0);
        Entity *nearby[256];
        int count = World_getEntitiesWithinAABBExcludingEntity(selfE->worldObj, selfE, expanded,
                                                               nearby, 256);
        for (int var4 = 0; var4 < count; ++var4) {
            Entity *var5 = nearby[var4];

            if (var5->vtable == (const EntityVtable *)&EntityPigZombie_vtable) {
                EntityPigZombie *var6 = (EntityPigZombie *)var5;
                EntityPigZombie_becomeAngryAt(var6, var1);
            }
        }
        EntityPigZombie_becomeAngryAt(self, var1);
    }

    return EntityZombie_defaultVtable.base.base.base.attackEntityFrom(selfE, var1, var2);
}

void EntityPigZombie_becomeAngryAt(EntityPigZombie *self, Entity *var1) {
    EntityCreature *cr = (EntityCreature *)self;
    cr->playerToAttack = var1;
    self->angerLevel = 400 + JavaRandom_nextInt(&cr->base.base.rand, 400);
    self->randomSoundDelay = JavaRandom_nextInt(&cr->base.base.rand, 40);
}

const char *EntityPigZombie_getLivingSound(EntityLiving *self) {
    (void)self;
    return "mob.zombiepig.zpig";
}

const char *EntityPigZombie_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.zombiepig.zpighurt";
}

const char *EntityPigZombie_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.zombiepig.zpigdeath";
}

int EntityPigZombie_getDropItemId(EntityLiving *self) {
    (void)self;
    return Item_porkCooked->shiftedIndex;
}

ItemStack *EntityPigZombie_getHeldItem(EntityLiving *self) {
    (void)self;
    return &EntityPigZombie_defaultHeldItem;
}
