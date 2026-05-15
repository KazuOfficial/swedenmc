#include "ItemMapBase.h"
#include "Item.h"
#include <stdlib.h>

Item *ItemMapBase_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vi = 1;
    }
    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    return self;
}
