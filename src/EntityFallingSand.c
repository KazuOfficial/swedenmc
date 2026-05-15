#include "EntityFallingSand.h"
#include "Entity.h"
#include "World.h"
#include "Block.h"
#include "MathHelper.h"
#include "NBTTagCompound.h"

static EntityVtable EntityFallingSand_vtable;
static int EntityFallingSand_vtable_init = 0;

static const char *fallingsand_getEntityString(Entity *self) {
    (void)self;
    return "FallingSand";
}

static void ensure_vtable(void) {
    if (!EntityFallingSand_vtable_init) {
        Entity_initVtable(&EntityFallingSand_vtable);
        EntityFallingSand_vtable.getEntityString = fallingsand_getEntityString;
        EntityFallingSand_vtable.canTriggerWalking = EntityFallingSand_canTriggerWalking;
        EntityFallingSand_vtable.entityInit = EntityFallingSand_entityInit;
        EntityFallingSand_vtable.canBeCollidedWith = EntityFallingSand_canBeCollidedWith;
        EntityFallingSand_vtable.onUpdate = EntityFallingSand_onUpdate;
        EntityFallingSand_vtable.writeEntityToNBT = EntityFallingSand_writeEntityToNBT;
        EntityFallingSand_vtable.readEntityFromNBT = EntityFallingSand_readEntityFromNBT;
        EntityFallingSand_vtable.getShadowSize = EntityFallingSand_getShadowSize;
        EntityFallingSand_vtable_init = 1;
    }
}

void EntityFallingSand_construct1(EntityFallingSand *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityFallingSand_vtable;
    self->blockID = 0;
    self->fallTime = 0;
    self->base.isFallingSand = 1;
}

void EntityFallingSand_construct2(EntityFallingSand *self, World *var1, double var2, double var4,
                                  double var6, int var8) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityFallingSand_vtable;
    self->blockID = 0;
    self->fallTime = 0;

    self->blockID = var8;
    self->base.preventEntitySpawning = 1;
    Entity_setSize(&self->base, 0.98F, 0.98F);
    self->base.yOffset = self->base.height / 2.0F;
    self->base.vtable->setPosition(&self->base, var2, var4, var6);
    self->base.motionX = 0.0;
    self->base.motionY = 0.0;
    self->base.motionZ = 0.0;
    self->base.prevPosX = var2;
    self->base.prevPosY = var4;
    self->base.prevPosZ = var6;
    self->base.isFallingSand = 1;
}

int EntityFallingSand_canTriggerWalking(Entity *self) {
    (void)self;
    return 0;
}

void EntityFallingSand_entityInit(Entity *self) { (void)self; }

int EntityFallingSand_canBeCollidedWith(Entity *self) { return !self->isDead; }

void EntityFallingSand_onUpdate(Entity *selfE) {
    EntityFallingSand *self = (EntityFallingSand *)selfE;
    if (self->blockID == 0) {
        Entity_setEntityDead(selfE);
    } else {
        selfE->prevPosX = selfE->posX;
        selfE->prevPosY = selfE->posY;
        selfE->prevPosZ = selfE->posZ;
        ++self->fallTime;
        selfE->motionY -= (double)0.04F;
        selfE->vtable->moveEntity(selfE, selfE->motionX, selfE->motionY, selfE->motionZ);
        selfE->motionX *= (double)0.98F;
        selfE->motionY *= (double)0.98F;
        selfE->motionZ *= (double)0.98F;
        int var1 = MathHelper_floor_double(selfE->posX);
        int var2 = MathHelper_floor_double(selfE->posY);
        int var3 = MathHelper_floor_double(selfE->posZ);
        if (World_getBlockId(selfE->worldObj, var1, var2, var3) == self->blockID) {
            World_setBlockWithNotify(selfE->worldObj, var1, var2, var3, 0);
        }

        if (selfE->onGround) {
            selfE->motionX *= (double)0.7F;
            selfE->motionZ *= (double)0.7F;
            selfE->motionY *= -0.5;
            Entity_setEntityDead(selfE);
            if ((!World_canBlockBePlacedAt(selfE->worldObj, self->blockID, var1, var2, var3, 1,
                                           1) ||
                 BlockSand_canFallBelow(selfE->worldObj, var1, var2 - 1, var3) ||
                 !World_setBlockWithNotify(selfE->worldObj, var1, var2, var3, self->blockID)) &&
                !selfE->worldObj->isRemote) {
                Entity_dropItem(selfE, self->blockID, 1);
            }
        } else if (self->fallTime > 100 && !selfE->worldObj->isRemote) {
            Entity_dropItem(selfE, self->blockID, 1);
            Entity_setEntityDead(selfE);
        }
    }
}

void EntityFallingSand_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityFallingSand *self = (EntityFallingSand *)selfE;
    NBTTagCompound_setByte(var1, "Tile", (int8_t)self->blockID);
}

void EntityFallingSand_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityFallingSand *self = (EntityFallingSand *)selfE;
    self->blockID = NBTTagCompound_getByte(var1, "Tile") & 255;
}

float EntityFallingSand_getShadowSize(Entity *self) {
    (void)self;
    return 0.0F;
}

World *EntityFallingSand_getWorld(EntityFallingSand *self) { return self->base.worldObj; }
