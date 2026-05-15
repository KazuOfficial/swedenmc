#include "Item.h"
#include "ItemStack.h"
#include "Entity.h"
#include "EntityPig.h"
#include <stdlib.h>

static void ItemSaddle_saddleEntity(Item *self, ItemStack *var1, EntityLiving *var2) {
    (void)self;
    if (((Entity *)var2)->isPig) {
        EntityPig *var3 = (EntityPig *)var2;
        if (!EntityPig_getSaddled(var3)) {
            EntityPig_setSaddled(var3, 1);
            --var1->stackSize;
        }
    }
}

static int ItemSaddle_hitEntity(Item *self, ItemStack *stack, EntityLiving *target,
                                EntityLiving *attacker) {
    (void)attacker;
    ItemSaddle_saddleEntity(self, stack, target);
    return 1;
}

Item *ItemSaddle_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.saddleEntity = ItemSaddle_saddleEntity;
        vtable.hitEntity = ItemSaddle_hitEntity;
        vi = 1;
    }
    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    self->maxStackSize = 1;
    return self;
}
