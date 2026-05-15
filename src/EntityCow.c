#include "EntityCow.h"
#include "World.h"
#include "Item.h"
#include "ItemStack.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "NBTTagCompound.h"
#include <stdlib.h>

static void cow_writeEntityToNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.writeEntityToNBT(e, var1);
}

static void cow_readEntityFromNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.readEntityFromNBT(e, var1);
}

static const char *cow_getLivingSound(EntityLiving *self) {
    (void)self;
    return "mob.cow";
}

static const char *cow_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.cowhurt";
}

static const char *cow_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.cowhurt";
}

static float cow_getSoundVolume(EntityLiving *self) {
    (void)self;
    return 0.4f;
}

static int cow_getDropItemId(EntityLiving *self) {
    (void)self;
    return Item_leather->shiftedIndex;
}

static int cow_interact(Entity *e, EntityPlayer *var1) {
    (void)e;
    ItemStack *var2 = InventoryPlayer_getCurrentItem(&var1->inventory);
    if (var2 != NULL && var2->itemID == Item_bucketEmpty->shiftedIndex) {
        ItemStack milk = ItemStack_fromItemID(Item_bucketMilk->shiftedIndex, 1, 0);
        ItemStack *milkPtr = (ItemStack *)calloc(1, sizeof(ItemStack));
        *milkPtr = milk;
        InventoryPlayer_setInventorySlotContents(&var1->inventory, var1->inventory.currentItem,
                                                 milkPtr);
        return 1;
    }
    return 0;
}

static const char *cow_getEntityString(Entity *self) {
    (void)self;
    return "Cow";
}

EntityCow *EntityCow_create(World *world) {
    static EntityAnimalVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        EntityAnimal_initVtable(&vtable);
        vtable.base.base.base.getEntityString = cow_getEntityString;
        vtable.base.base.base.writeEntityToNBT = cow_writeEntityToNBT;
        vtable.base.base.base.readEntityFromNBT = cow_readEntityFromNBT;
        vtable.base.base.base.interact = cow_interact;
        vtable.base.base.getLivingSound = cow_getLivingSound;
        vtable.base.base.getHurtSound = cow_getHurtSound;
        vtable.base.base.getDeathSound = cow_getDeathSound;
        vtable.base.base.getSoundVolume = cow_getSoundVolume;
        vtable.base.base.getDropItemId = cow_getDropItemId;
        vtable_init = 1;
    }

    EntityCow *self = (EntityCow *)calloc(1, sizeof(EntityCow));
    EntityAnimal_construct(&self->base, world);
    self->base.base.base.base.isCow = 1;
    self->base.base.base.base.vtable = (const EntityVtable *)&vtable;
    self->base.base.base.texture = "/mob/cow.png";
    Entity_setSize(&self->base.base.base.base, 0.9f, 1.3f);
    return self;
}
