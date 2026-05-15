#include "EntityChicken.h"
#include "World.h"
#include "Item.h"
#include "NBTTagCompound.h"
#include <stdlib.h>

static void chicken_onLivingUpdate(EntityLiving *base) {
    EntityChicken *self = (EntityChicken *)base;
    Entity *e = &base->base;

    EntityLiving_onLivingUpdateBase(base);

    self->field_756_e = self->field_752_b;
    self->field_757_d = self->destPos;
    self->destPos = (float)((double)self->destPos + (double)(e->onGround ? -1 : 4) * 0.3);
    if (self->destPos < 0.0f) {
        self->destPos = 0.0f;
    }
    if (self->destPos > 1.0f) {
        self->destPos = 1.0f;
    }
    if (!e->onGround && self->field_755_h < 1.0f) {
        self->field_755_h = 1.0f;
    }
    self->field_755_h = (float)((double)self->field_755_h * 0.9);
    if (!e->onGround && e->motionY < 0.0) {
        e->motionY *= 0.6;
    }
    self->field_752_b += self->field_755_h * 2.0f;
    if (!e->worldObj->multiplayerWorld && --self->timeUntilNextEgg <= 0) {
        World_playSoundAtEntity(
            e->worldObj, e, "mob.chickenplop", 1.0f,
            (JavaRandom_nextFloat(&e->rand) - JavaRandom_nextFloat(&e->rand)) * 0.2f + 1.0f);
        Entity_dropItem(e, Item_egg->shiftedIndex, 1);
        self->timeUntilNextEgg = JavaRandom_nextInt(&e->rand, 6000) + 6000;
    }
}

static void chicken_fall(Entity *e, float var1) {
    (void)e;
    (void)var1;
}

static void chicken_writeEntityToNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.writeEntityToNBT(e, var1);
}

static void chicken_readEntityFromNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.readEntityFromNBT(e, var1);
}

static const char *chicken_getLivingSound(EntityLiving *self) {
    (void)self;
    return "mob.chicken";
}

static const char *chicken_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.chickenhurt";
}

static const char *chicken_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.chickenhurt";
}

static int chicken_getDropItemId(EntityLiving *self) {
    (void)self;
    return Item_feather->shiftedIndex;
}

static const char *chicken_getEntityString(Entity *self) {
    (void)self;
    return "Chicken";
}

EntityChicken *EntityChicken_create(World *world) {
    static EntityAnimalVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        EntityAnimal_initVtable(&vtable);
        vtable.base.base.base.getEntityString = chicken_getEntityString;
        vtable.base.base.base.fall = chicken_fall;
        vtable.base.base.base.writeEntityToNBT = chicken_writeEntityToNBT;
        vtable.base.base.base.readEntityFromNBT = chicken_readEntityFromNBT;
        vtable.base.base.onLivingUpdate = chicken_onLivingUpdate;
        vtable.base.base.getLivingSound = chicken_getLivingSound;
        vtable.base.base.getHurtSound = chicken_getHurtSound;
        vtable.base.base.getDeathSound = chicken_getDeathSound;
        vtable.base.base.getDropItemId = chicken_getDropItemId;
        vtable_init = 1;
    }

    EntityChicken *self = (EntityChicken *)calloc(1, sizeof(EntityChicken));
    EntityAnimal_construct(&self->base, world);
    self->base.base.base.base.vtable = (const EntityVtable *)&vtable;
    self->base.base.base.texture = "/mob/chicken.png";
    Entity_setSize(&self->base.base.base.base, 0.3f, 0.4f);
    self->base.base.base.health = 4;
    self->field_755_h = 1.0f;
    self->timeUntilNextEgg = JavaRandom_nextInt(&self->base.base.base.base.rand, 6000) + 6000;
    self->base.base.base.base.isChicken = 1;
    return self;
}
