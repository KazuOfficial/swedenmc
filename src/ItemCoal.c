#include "ItemCoal.h"
#include <stdlib.h>
#include <string.h>

static const char *ItemCoal_getItemNameIS(Item *self, ItemStack *stack) {
    (void)self;
    return stack->itemDamage == 1 ? "item.charcoal" : "item.coal";
}

Item *ItemCoal_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.getItemNameIS = ItemCoal_getItemNameIS;
        vi = 1;
    }

    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    Item_setHasSubtypes(self, 1);
    Item_setMaxDamage(self, 0);
    return self;
}
