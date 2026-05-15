#include "Item.h"
#include "ItemStack.h"
#include "World.h"
#include "EntityPlayer.h"
#include "EntityArrow.h"
#include "Entity.h"
#include "InventoryPlayer.h"
#include "JavaRandom.h"
#include <stdlib.h>

static ItemStack *ItemBow_onItemRightClick(Item *self, ItemStack *var1, World *var2,
                                           EntityPlayer *var3) {
    (void)self;
    if (InventoryPlayer_consumeInventoryItem(&var3->inventory, Item_arrow->shiftedIndex)) {
        World_playSoundAtEntity(var2, (Entity *)var3, "random.bow", 1.0f,
                                1.0f / (JavaRandom_nextFloat(&Item_itemRand) * 0.4f + 0.8f));
        if (!var2->multiplayerWorld) {
            EntityArrow *arrow = (EntityArrow *)calloc(1, sizeof(EntityArrow));
            EntityArrow_construct3(arrow, var2, (EntityLiving *)var3);
            World_entityJoinedWorld(var2, (Entity *)arrow);
        }
    }
    return var1;
}

Item *ItemBow_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemRightClick = ItemBow_onItemRightClick;
        vi = 1;
    }

    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    self->maxStackSize = 1;
    return self;
}
