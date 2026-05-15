#include "EntitySpider.h"
#include "World.h"
#include "MathHelper.h"
#include "Item.h"
#include "NBTTagCompound.h"
#include <stdlib.h>

static double spider_getMountedYOffset(Entity *e) { return (double)e->height * 0.75 - 0.5; }

static int spider_canTriggerWalking(Entity *e) {
    (void)e;
    return 0;
}

static Entity *spider_findPlayerToAttack(EntityCreature *base) {
    Entity *e = &base->base.base;
    float var1 = e->vtable->getEntityBrightness(e, 1.0f);
    if (var1 < 0.5f) {
        double var2 = 16.0;
        return World_getClosestPlayerToEntity(e->worldObj, e, var2);
    }
    return NULL;
}

static const char *spider_getLivingSound(EntityLiving *self) {
    (void)self;
    return "mob.spider";
}

static const char *spider_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.spider";
}

static const char *spider_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.spiderdeath";
}

static void spider_attackEntity(EntityCreature *base, Entity *var1, float var2) {
    Entity *e = &base->base.base;
    float var3 = e->vtable->getEntityBrightness(e, 1.0f);
    if (var3 > 0.5f && JavaRandom_nextInt(&e->rand, 100) == 0) {
        base->playerToAttack = NULL;
    } else {
        if (var2 > 2.0f && var2 < 6.0f && JavaRandom_nextInt(&e->rand, 10) == 0) {
            if (e->onGround) {
                double var4 = var1->posX - e->posX;
                double var6 = var1->posZ - e->posZ;
                float var8 = MathHelper_sqrt_double(var4 * var4 + var6 * var6);
                e->motionX = var4 / (double)var8 * 0.5 * (double)0.8f + e->motionX * (double)0.2f;
                e->motionZ = var6 / (double)var8 * 0.5 * (double)0.8f + e->motionZ * (double)0.2f;
                e->motionY = (double)0.4f;
            }
        } else {

            EntityMob_attackEntity(base, var1, var2);
        }
    }
}

static void spider_writeEntityToNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.writeEntityToNBT(e, var1);
}

static void spider_readEntityFromNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.readEntityFromNBT(e, var1);
}

static int spider_getDropItemId(EntityLiving *self) {
    (void)self;
    return Item_silk->shiftedIndex;
}

static int spider_isOnLadder(EntityLiving *self) { return self->base.isCollidedHorizontally; }

static const char *spider_getEntityString(Entity *self) {
    (void)self;
    return "Spider";
}

EntitySpider *EntitySpider_create(World *world) {
    static EntityMobVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        EntityMob_initVtable(&vtable);
        vtable.base.base.base.getMountedYOffset = spider_getMountedYOffset;
        vtable.base.base.base.canTriggerWalking = spider_canTriggerWalking;
        vtable.base.base.base.getEntityString = spider_getEntityString;
        vtable.base.base.base.writeEntityToNBT = spider_writeEntityToNBT;
        vtable.base.base.base.readEntityFromNBT = spider_readEntityFromNBT;
        vtable.base.base.getLivingSound = spider_getLivingSound;
        vtable.base.base.getHurtSound = spider_getHurtSound;
        vtable.base.base.getDeathSound = spider_getDeathSound;
        vtable.base.base.isOnLadder = spider_isOnLadder;
        vtable.base.base.getDropItemId = spider_getDropItemId;
        vtable.base.findPlayerToAttack = spider_findPlayerToAttack;
        vtable.base.attackEntity = spider_attackEntity;
        vtable_init = 1;
    }

    EntitySpider *self = (EntitySpider *)calloc(1, sizeof(EntitySpider));
    EntityMob_construct(&self->base, world);
    self->base.base.base.base.vtable = (const EntityVtable *)&vtable;
    self->base.base.base.base.isSpider = 1;

    self->base.base.base.texture = "/mob/spider.png";

    Entity_setSize(&self->base.base.base.base, 1.4f, 0.9f);

    self->base.base.base.moveSpeed = 0.8f;
    return self;
}
