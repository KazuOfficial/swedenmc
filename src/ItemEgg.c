#include "ItemEgg.h"
#include "ItemStack.h"
#include "World.h"
#include "EntityEgg.h"
#include <stdlib.h>

static ItemStack *ItemEgg_onItemRightClick(Item *self, ItemStack *var1, World *var2,
                                           EntityPlayer *var3) {
    (void)self;
    --var1->stackSize;
    World_playSoundAtEntity(var2, (struct Entity *)var3, "random.bow", 0.5F,
                            0.4F / (JavaRandom_nextFloat(&Item_itemRand) * 0.4F + 0.8F));
    if (!var2->multiplayerWorld) {

        EntityEgg *egg = (EntityEgg *)calloc(1, sizeof(EntityEgg));
        EntityEgg_construct2(egg, var2, (EntityLiving *)var3);
        World_entityJoinedWorld(var2, (struct Entity *)egg);
    }
    return var1;
}

Item *ItemEgg_create(int var1) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemRightClick = ItemEgg_onItemRightClick;
        vi = 1;
    }
    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, var1);
    self->vtable = &vtable;
    self->maxStackSize = 16;
    return self;
}
