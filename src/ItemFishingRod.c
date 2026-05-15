#include "Item.h"
#include "ItemStack.h"
#include "EntityPlayer.h"
#include "EntityFish.h"
#include "World.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int ItemFishingRod_isFull3D(Item *self) {
    (void)self;
    return 1;
}
static int ItemFishingRod_shouldRotate(Item *self) {
    (void)self;
    return 1;
}

static ItemStack *ItemFishingRod_onItemRightClick(Item *self, ItemStack *var1, World *var2,
                                                  EntityPlayer *var3) {
    (void)self;
    if (var3->fishEntity != NULL) {
        int var4 = EntityFish_catchFish(var3->fishEntity);
        ItemStack_damageItem(var1, var4, (Entity *)var3);
        EntityPlayer_swingItem(var3);
    } else {
        World_playSoundAtEntity(var2, (Entity *)var3, "random.bow", 0.5f,
                                0.4f / (JavaRandom_nextFloat(&Item_itemRand) * 0.4f + 0.8f));
        if (!var2->multiplayerWorld) {
            EntityFish *fish = (EntityFish *)calloc(1, sizeof(EntityFish));
            EntityFish_construct3(fish, var2, var3);
            World_entityJoinedWorld(var2, (Entity *)fish);
        }
        EntityPlayer_swingItem(var3);
    }
    return var1;
}

Item *ItemFishingRod_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.isFull3D = ItemFishingRod_isFull3D;
        vtable.shouldRotateAroundWhenRendering = ItemFishingRod_shouldRotate;
        vtable.onItemRightClick = ItemFishingRod_onItemRightClick;
        vi = 1;
    }
    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    Item_setMaxDamage(self, 64);
    self->maxStackSize = 1;
    return self;
}
