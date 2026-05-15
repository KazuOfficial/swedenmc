#include "EntityItem.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "World.h"
#include "Block.h"
#include "Item.h"
#include "Material.h"
#include "MathHelper.h"
#include "AchievementList.h"
#include "NBTTagCompound.h"
#include "ItemStack.h"
#include "JavaMath.h"
#include <stdlib.h>
#include <math.h>

static EntityVtable EntityItem_vt;
static int EntityItem_vt_init = 0;

static int item_canTriggerWalking(Entity *self) {
    (void)self;
    return 0;
}

static void item_entityInit(Entity *self) { (void)self; }

static void item_onUpdate(Entity *selfE) {
    EntityItem *self = (EntityItem *)selfE;

    Entity_defaultVtable.onUpdate(selfE);

    if (self->delayBeforeCanPickup > 0) {
        --self->delayBeforeCanPickup;
    }

    selfE->prevPosX = selfE->posX;
    selfE->prevPosY = selfE->posY;
    selfE->prevPosZ = selfE->posZ;
    selfE->motionY -= (double)0.04f;

    if (World_getBlockMaterial(selfE->worldObj, MathHelper_floor_double(selfE->posX),
                               MathHelper_floor_double(selfE->posY),
                               MathHelper_floor_double(selfE->posZ)) == Material_lava) {
        selfE->motionY = (double)0.2f;
        selfE->motionX =
            (double)((JavaRandom_nextFloat(&selfE->rand) - JavaRandom_nextFloat(&selfE->rand)) *
                     0.2f);
        selfE->motionZ =
            (double)((JavaRandom_nextFloat(&selfE->rand) - JavaRandom_nextFloat(&selfE->rand)) *
                     0.2f);
        World_playSoundAtEntity(selfE->worldObj, selfE, "random.fizz", 0.4f,
                                2.0f + JavaRandom_nextFloat(&selfE->rand) * 0.4f);
    }

    Entity_pushOutOfBlocks(selfE, selfE->posX,
                           (selfE->boundingBox.minY + selfE->boundingBox.maxY) / 2.0, selfE->posZ);
    selfE->vtable->moveEntity(selfE, selfE->motionX, selfE->motionY, selfE->motionZ);

    float var1 = 0.98f;
    if (selfE->onGround) {
        var1 = 0.1f * 0.1f * 58.8f;
        int var2 = World_getBlockId(selfE->worldObj, MathHelper_floor_double(selfE->posX),
                                    MathHelper_floor_double(selfE->boundingBox.minY) - 1,
                                    MathHelper_floor_double(selfE->posZ));
        if (var2 > 0 && Block_blocksList[var2]) {
            var1 = Block_blocksList[var2]->slipperiness * 0.98f;
        }
    }

    selfE->motionX *= (double)var1;
    selfE->motionY *= (double)0.98f;
    selfE->motionZ *= (double)var1;
    if (selfE->onGround) {
        selfE->motionY *= -0.5;
    }

    ++self->field_803_e;
    ++self->age;
    if (self->age >= 6000) {
        Entity_setEntityDead(selfE);
    }
}

static int item_handleWaterMovement(Entity *selfE) {
    return World_handleMaterialAcceleration(selfE->worldObj, &selfE->boundingBox, Material_water,
                                            selfE);
}

static void item_dealFireDamage(Entity *selfE, int var1) {
    selfE->vtable->attackEntityFrom(selfE, NULL, var1);
}

static int item_attackEntityFrom(Entity *selfE, Entity *var1, int var2) {
    (void)var1;
    EntityItem *self = (EntityItem *)selfE;
    Entity_setBeenAttacked(selfE);
    self->health -= var2;
    if (self->health <= 0) {
        Entity_setEntityDead(selfE);
    }
    return 0;
}

static void item_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityItem *self = (EntityItem *)selfE;
    NBTTagCompound_setShort(var1, "Health", (int16_t)((int8_t)self->health));
    NBTTagCompound_setShort(var1, "Age", (int16_t)self->age);
    NBTTagCompound *itemTag = NBTTagCompound_create();
    ItemStack_writeToNBT(&self->item, itemTag);
    NBTTagCompound_setCompoundTag(var1, "Item", itemTag);
}

static void item_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityItem *self = (EntityItem *)selfE;
    self->health = NBTTagCompound_getShort(var1, "Health") & 255;
    self->age = NBTTagCompound_getShort(var1, "Age");
    NBTTagCompound *var2 = NBTTagCompound_getCompoundTag(var1, "Item");
    self->item = ItemStack_fromNBT(var2);
}

static void item_onCollideWithPlayer(Entity *selfE, EntityPlayer *var1) {
    EntityItem *self = (EntityItem *)selfE;
    if (!selfE->worldObj->multiplayerWorld) {
        int var2 = self->item.stackSize;
        if (self->delayBeforeCanPickup == 0 &&
            InventoryPlayer_addItemStackToInventory(&var1->inventory, &self->item)) {
            if (self->item.itemID == Block_wood->blockID) {
                EntityPlayer_triggerAchievement(var1, (struct StatBase *)AchievementList_mineWood);
            }
            if (self->item.itemID == Item_leather->shiftedIndex) {
                EntityPlayer_triggerAchievement(var1, (struct StatBase *)AchievementList_killCow);
            }
            World_playSoundAtEntity(
                selfE->worldObj, selfE, "random.pop", 0.2f,
                ((JavaRandom_nextFloat(&selfE->rand) - JavaRandom_nextFloat(&selfE->rand)) * 0.7f +
                 1.0f) *
                    2.0f);
            ((EntityPlayerVtable *)var1->base.base.vtable)->onItemPickup(var1, selfE, var2);
            if (self->item.stackSize <= 0) {
                Entity_setEntityDead(selfE);
            }
        }
    }
}

static const char *item_getEntityString(Entity *self) {
    (void)self;
    return "Item";
}

static void ensure_vtable(void) {
    if (EntityItem_vt_init)
        return;
    Entity_initVtable(&EntityItem_vt);
    EntityItem_vt.canTriggerWalking = item_canTriggerWalking;
    EntityItem_vt.entityInit = item_entityInit;
    EntityItem_vt.onUpdate = item_onUpdate;
    EntityItem_vt.handleWaterMovement = item_handleWaterMovement;
    EntityItem_vt.dealFireDamage = item_dealFireDamage;
    EntityItem_vt.attackEntityFrom = item_attackEntityFrom;
    EntityItem_vt.writeEntityToNBT = item_writeEntityToNBT;
    EntityItem_vt.readEntityFromNBT = item_readEntityFromNBT;
    EntityItem_vt.onCollideWithPlayer = item_onCollideWithPlayer;
    EntityItem_vt.getEntityString = item_getEntityString;
    EntityItem_vt_init = 1;
}

void EntityItem_construct(EntityItem *self, World *world, double x, double y, double z,
                          ItemStack item) {
    ensure_vtable();
    Entity_construct(&self->base, world);
    self->base.vtable = &EntityItem_vt;

    Entity_setSize(&self->base, 0.25f, 0.25f);
    self->base.yOffset = self->base.height / 2.0f;
    self->base.vtable->setPosition(&self->base, x, y, z);

    self->item = item;
    self->health = 5;
    self->field_804_d = (float)(Math_random() * M_PI * 2.0);

    self->base.rotationYaw = (float)(Math_random() * 360.0);
    self->base.motionX = (double)((float)(Math_random() * (double)0.2f - (double)0.1f));
    self->base.motionY = (double)0.2f;
    self->base.motionZ = (double)((float)(Math_random() * (double)0.2f - (double)0.1f));
    self->base.isItemEntity = 1;
}

void EntityItem_constructDefault(EntityItem *self, World *world) {
    ensure_vtable();
    Entity_construct(&self->base, world);
    self->base.vtable = &EntityItem_vt;

    Entity_setSize(&self->base, 0.25f, 0.25f);
    self->base.yOffset = self->base.height / 2.0f;
    self->health = 5;
    self->field_804_d = (float)(Math_random() * M_PI * 2.0);
    self->base.isItemEntity = 1;
}

EntityItem *EntityItem_create(World *world, double x, double y, double z, ItemStack item) {
    EntityItem *self = (EntityItem *)calloc(1, sizeof(EntityItem));
    EntityItem_construct(self, world, x, y, z, item);
    return self;
}

EntityItem *EntityItem_createDefault(World *world) {
    EntityItem *self = (EntityItem *)calloc(1, sizeof(EntityItem));
    EntityItem_constructDefault(self, world);
    return self;
}
