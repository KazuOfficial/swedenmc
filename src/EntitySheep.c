#include "EntitySheep.h"
#include "World.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "DataWatcher.h"
#include "NBTTagCompound.h"
#include "EntityItem.h"
#include <stdlib.h>

float EntitySheep_fleeceColorTable[16][3] = {
    {1.0f, 1.0f, 1.0f}, {0.95f, 0.7f, 0.2f}, {0.9f, 0.5f, 0.85f}, {0.6f, 0.7f, 0.95f},
    {0.9f, 0.9f, 0.2f}, {0.5f, 0.8f, 0.1f},  {0.95f, 0.7f, 0.8f}, {0.3f, 0.3f, 0.3f},
    {0.6f, 0.6f, 0.6f}, {0.3f, 0.6f, 0.7f},  {0.7f, 0.4f, 0.9f},  {0.2f, 0.4f, 0.8f},
    {0.5f, 0.4f, 0.3f}, {0.4f, 0.5f, 0.2f},  {0.8f, 0.3f, 0.3f},  {0.1f, 0.1f, 0.1f},
};

static void sheep_entityInit(Entity *e) {
    extern EntityVtable Entity_defaultVtable;
    Entity_defaultVtable.entityInit(e);
    DataWatcher_addByte(&e->dataWatcher, 16, (int8_t)0);
}

static int sheep_attackEntityFrom(Entity *e, Entity *var1, int var2) {
    return EntityLiving_defaultVtable.base.attackEntityFrom(e, var1, var2);
}

static void sheep_dropFewItems(EntityLiving *base) {
    EntitySheep *self = (EntitySheep *)base;
    if (!EntitySheep_getSheared(self)) {
        ItemStack stack =
            ItemStack_fromBlockID(Block_cloth->blockID, 1, EntitySheep_getFleeceColor(self));
        Entity_entityDropItem(&base->base, stack, 0.0f);
    }
}

static int sheep_getDropItemId(EntityLiving *self) {
    (void)self;
    return Block_cloth->blockID;
}

static int sheep_interact(Entity *e, EntityPlayer *var1) {
    EntitySheep *self = (EntitySheep *)e;
    ItemStack *var2 = InventoryPlayer_getCurrentItem(&var1->inventory);
    if (var2 != NULL && var2->itemID == Item_shears->shiftedIndex &&
        !EntitySheep_getSheared(self)) {
        if (!e->worldObj->multiplayerWorld) {
            EntitySheep_setSheared(self, 1);
            int var3 = 2 + JavaRandom_nextInt(&e->rand, 3);
            for (int var4 = 0; var4 < var3; ++var4) {
                ItemStack stack = ItemStack_fromBlockID(Block_cloth->blockID, 1,
                                                        EntitySheep_getFleeceColor(self));
                struct EntityItem *var5 = Entity_entityDropItem(e, stack, 1.0f);
                var5->base.motionY += (double)(JavaRandom_nextFloat(&e->rand) * 0.05f);
                var5->base.motionX +=
                    (double)((JavaRandom_nextFloat(&e->rand) - JavaRandom_nextFloat(&e->rand)) *
                             0.1f);
                var5->base.motionZ +=
                    (double)((JavaRandom_nextFloat(&e->rand) - JavaRandom_nextFloat(&e->rand)) *
                             0.1f);
            }
        }
        ItemStack_damageItem(var2, 1, (Entity *)var1);
    }
    return 0;
}

static void sheep_writeEntityToNBT(Entity *e, NBTTagCompound *var1) {
    EntityLiving_defaultVtable.base.writeEntityToNBT(e, var1);
    NBTTagCompound_setBoolean(var1, "Sheared", EntitySheep_getSheared((EntitySheep *)e));
    NBTTagCompound_setByte(var1, "Color", (int8_t)EntitySheep_getFleeceColor((EntitySheep *)e));
}

static void sheep_readEntityFromNBT(Entity *e, NBTTagCompound *var1) {
    EntityLiving_defaultVtable.base.readEntityFromNBT(e, var1);
    EntitySheep_setSheared((EntitySheep *)e, NBTTagCompound_getBoolean(var1, "Sheared"));
    EntitySheep_setFleeceColor((EntitySheep *)e, (int)NBTTagCompound_getByte(var1, "Color"));
}

static const char *sheep_getLivingSound(EntityLiving *self) {
    (void)self;
    return "mob.sheep";
}

static const char *sheep_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.sheep";
}

static const char *sheep_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.sheep";
}

static const char *sheep_getEntityString(Entity *self) {
    (void)self;
    return "Sheep";
}

EntitySheep *EntitySheep_create(World *world) {
    static EntityAnimalVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        EntityAnimal_initVtable(&vtable);
        vtable.base.base.base.getEntityString = sheep_getEntityString;
        vtable.base.base.base.entityInit = sheep_entityInit;
        vtable.base.base.base.attackEntityFrom = sheep_attackEntityFrom;
        vtable.base.base.base.interact = sheep_interact;
        vtable.base.base.base.writeEntityToNBT = sheep_writeEntityToNBT;
        vtable.base.base.base.readEntityFromNBT = sheep_readEntityFromNBT;
        vtable.base.base.getLivingSound = sheep_getLivingSound;
        vtable.base.base.getHurtSound = sheep_getHurtSound;
        vtable.base.base.getDeathSound = sheep_getDeathSound;
        vtable.base.base.dropFewItems = sheep_dropFewItems;
        vtable.base.base.getDropItemId = sheep_getDropItemId;
        vtable_init = 1;
    }

    EntitySheep *self = (EntitySheep *)calloc(1, sizeof(EntitySheep));
    EntityAnimal_construct(&self->base, world);
    self->base.base.base.base.vtable = (const EntityVtable *)&vtable;
    self->base.base.base.base.isSheep = 1;
    self->base.base.base.texture = "/mob/sheep.png";
    Entity_setSize(&self->base.base.base.base, 0.9f, 1.3f);

    self->base.base.base.base.vtable->entityInit(&self->base.base.base.base);
    return self;
}

int EntitySheep_getFleeceColor(EntitySheep *self) {
    return DataWatcher_getWatchableObjectByte(&self->base.base.base.base.dataWatcher, 16) & 15;
}

void EntitySheep_setFleeceColor(EntitySheep *self, int var1) {
    DataWatcher *dw = &self->base.base.base.base.dataWatcher;
    int8_t var2 = DataWatcher_getWatchableObjectByte(dw, 16);
    DataWatcher_updateByte(dw, 16, (int8_t)((var2 & 240) | (var1 & 15)));
}

int EntitySheep_getSheared(EntitySheep *self) {
    return (DataWatcher_getWatchableObjectByte(&self->base.base.base.base.dataWatcher, 16) & 16) !=
           0;
}

void EntitySheep_setSheared(EntitySheep *self, int var1) {
    DataWatcher *dw = &self->base.base.base.base.dataWatcher;
    int8_t var2 = DataWatcher_getWatchableObjectByte(dw, 16);
    if (var1)
        DataWatcher_updateByte(dw, 16, (int8_t)(var2 | 16));
    else
        DataWatcher_updateByte(dw, 16, (int8_t)(var2 & (int8_t)-17));
}

int EntitySheep_getRandomFleeceColor(JavaRandom *var0) {
    int var1 = JavaRandom_nextInt(var0, 100);
    return var1 < 5    ? 15
           : var1 < 10 ? 7
           : var1 < 15 ? 8
           : var1 < 18 ? 12
                       : (JavaRandom_nextInt(var0, 500) == 0 ? 6 : 0);
}
