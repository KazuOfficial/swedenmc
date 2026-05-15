#include "Item.h"
#include "ItemStack.h"
#include "World.h"
#include "EntitySnowball.h"
#include "Entity.h"
#include "JavaRandom.h"
#include <stdlib.h>

static ItemStack *ItemSnowball_onItemRightClick(Item *self, ItemStack *var1, World *var2,
                                                EntityPlayer *var3) {
    (void)self;
    --var1->stackSize;
    World_playSoundAtEntity(var2, (Entity *)var3, "random.bow", 0.5f,
                            0.4f / (JavaRandom_nextFloat(&Item_itemRand) * 0.4f + 0.8f));
    if (!var2->multiplayerWorld) {
        EntitySnowball *sb = (EntitySnowball *)calloc(1, sizeof(EntitySnowball));
        EntitySnowball_construct2(sb, var2, (EntityLiving *)var3);
        World_entityJoinedWorld(var2, (Entity *)sb);
    }
    return var1;
}

Item *ItemSnowball_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemRightClick = ItemSnowball_onItemRightClick;
        vi = 1;
    }

    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    self->maxStackSize = 16;
    return self;
}
