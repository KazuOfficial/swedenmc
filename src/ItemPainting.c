#include "ItemPainting.h"
#include "ItemStack.h"
#include "EntityPainting.h"
#include "World.h"
#include <stdlib.h>

static int ItemPainting_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3,
                                  int var4, int var5, int var6, int var7) {
    (void)self;
    (void)var2;
    if (var7 == 0) {
        return 0;
    }
    if (var7 == 1) {
        return 0;
    }

    int var8 = 0;
    if (var7 == 4) {
        var8 = 1;
    }
    if (var7 == 3) {
        var8 = 2;
    }
    if (var7 == 5) {
        var8 = 3;
    }
    EntityPainting *var9 = EntityPainting_create2(var3, var4, var5, var6, var8);
    if (EntityPainting_func_410_i(var9)) {
        if (!var3->multiplayerWorld) {
            World_entityJoinedWorld(var3, (Entity *)var9);
        }
        --var1->stackSize;
    }
    return 1;
}

Item *ItemPainting_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemPainting_onItemUse;
        vi = 1;
    }

    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    return self;
}
