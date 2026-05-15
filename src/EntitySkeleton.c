#include "EntitySkeleton.h"
#include "EntityArrow.h"
#include "World.h"
#include "Item.h"
#include "ItemStack.h"
#include "MathHelper.h"
#include "NBTTagCompound.h"
#include <stdlib.h>
#include <math.h>

static ItemStack skeleton_defaultHeldItem;
static int skeleton_defaultHeldItem_init = 0;

static const char *skeleton_getLivingSound(EntityLiving *self) {
    (void)self;
    return "mob.skeleton";
}

static const char *skeleton_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.skeletonhurt";
}

static const char *skeleton_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.skeletonhurt";
}

static void skeleton_onLivingUpdate(EntityLiving *self) {
    Entity *e = &self->base;
    if (World_isDaytime(e->worldObj)) {
        float var1 = e->vtable->getEntityBrightness(e, 1.0f);
        if (var1 > 0.5f &&
            World_canBlockSeeTheSky(e->worldObj, MathHelper_floor_double(e->posX),
                                    MathHelper_floor_double(e->posY),
                                    MathHelper_floor_double(e->posZ)) &&
            JavaRandom_nextFloat(&e->rand) * 30.0f < (var1 - 0.4f) * 2.0f) {
            e->fire = 300;
        }
    }

    EntityMob_onLivingUpdate(self);
}

static void skeleton_attackEntity(EntityCreature *base, Entity *var1, float var2) {
    EntityLiving *el = &base->base;
    Entity *e = &el->base;
    if (var2 < 10.0f) {
        double var3 = var1->posX - e->posX;
        double var5 = var1->posZ - e->posZ;
        if (el->attackTime == 0) {
            EntityArrow *var7 = EntityArrow_create3(e->worldObj, el);
            var7->base.posY += (double)1.4f;
            double var8 = var1->posY + (double)var1->vtable->getEyeHeight(var1) - (double)0.2f -
                          var7->base.posY;
            float var10 = MathHelper_sqrt_double(var3 * var3 + var5 * var5) * 0.2f;
            World_playSoundAtEntity(e->worldObj, e, "random.bow", 1.0f,
                                    1.0f / (JavaRandom_nextFloat(&e->rand) * 0.4f + 0.8f));
            World_entityJoinedWorld(e->worldObj, &var7->base);
            EntityArrow_setArrowHeading(var7, var3, var8 + (double)var10, var5, 0.6f, 12.0f);
            el->attackTime = 30;
        }
        e->rotationYaw = (float)(atan2(var5, var3) * 180.0 / M_PI) - 90.0f;
        base->hasAttacked = 1;
    }
}

static void skeleton_writeEntityToNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.writeEntityToNBT(e, var1);
}

static void skeleton_readEntityFromNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.readEntityFromNBT(e, var1);
}

static int skeleton_getDropItemId(EntityLiving *self) {
    (void)self;
    return Item_arrow->shiftedIndex;
}

static void skeleton_dropFewItems(EntityLiving *base) {
    Entity *e = &base->base;
    int var1 = JavaRandom_nextInt(&e->rand, 3);
    for (int var2 = 0; var2 < var1; ++var2) {
        Entity_dropItem(e, Item_arrow->shiftedIndex, 1);
    }
    var1 = JavaRandom_nextInt(&e->rand, 3);
    for (int var2 = 0; var2 < var1; ++var2) {
        Entity_dropItem(e, Item_bone->shiftedIndex, 1);
    }
}

static ItemStack *skeleton_getHeldItem(EntityLiving *self) {
    (void)self;
    return &skeleton_defaultHeldItem;
}

static const char *skeleton_getEntityString(Entity *self) {
    (void)self;
    return "Skeleton";
}

EntitySkeleton *EntitySkeleton_create(World *world) {
    if (!skeleton_defaultHeldItem_init) {
        skeleton_defaultHeldItem = ItemStack_fromItemID(Item_bow->shiftedIndex, 1, 0);
        skeleton_defaultHeldItem_init = 1;
    }

    static EntityMobVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        EntityMob_initVtable(&vtable);
        vtable.base.base.base.getEntityString = skeleton_getEntityString;
        vtable.base.base.base.writeEntityToNBT = skeleton_writeEntityToNBT;
        vtable.base.base.base.readEntityFromNBT = skeleton_readEntityFromNBT;
        vtable.base.base.onLivingUpdate = skeleton_onLivingUpdate;
        vtable.base.base.getLivingSound = skeleton_getLivingSound;
        vtable.base.base.getHurtSound = skeleton_getHurtSound;
        vtable.base.base.getDeathSound = skeleton_getDeathSound;
        vtable.base.base.getDropItemId = skeleton_getDropItemId;
        vtable.base.base.dropFewItems = skeleton_dropFewItems;
        vtable.base.base.getHeldItem = skeleton_getHeldItem;
        vtable.base.attackEntity = skeleton_attackEntity;
        vtable_init = 1;
    }

    EntitySkeleton *self = (EntitySkeleton *)calloc(1, sizeof(EntitySkeleton));
    EntityMob_construct(&self->base, world);
    self->base.base.base.base.vtable = (const EntityVtable *)&vtable;
    self->base.base.base.texture = "/mob/skeleton.png";

    self->base.base.base.base.isSkeleton = 1;
    return self;
}
