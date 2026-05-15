#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "BlockJukeBox.h"
#include "World.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    Item base;
    char recordName[16];
} ItemRecord;

static int ItemRecord_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3,
                                int var4, int var5, int var6, int var7) {
    (void)var2;
    (void)var7;
    if (World_getBlockId(var3, var4, var5, var6) == Block_jukebox->blockID &&
        World_getBlockMetadata(var3, var4, var5, var6) == 0) {
        if (var3->multiplayerWorld) {
            return 1;
        } else {
            BlockJukeBox_ejectRecord(Block_jukebox, var3, var4, var5, var6, self->shiftedIndex);
            World_func_28107_a(var3, NULL, 1005, var4, var5, var6, self->shiftedIndex);
            --var1->stackSize;
            return 1;
        }
    } else {
        return 0;
    }
}

Item *ItemRecord_create(int id, const char *recordName) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemRecord_onItemUse;
        vi = 1;
    }

    ItemRecord *self = (ItemRecord *)calloc(1, sizeof(ItemRecord));
    Item_construct(&self->base, id);
    self->base.vtable = &vtable;
    self->base.maxStackSize = 1;
    strncpy(self->recordName, recordName, sizeof(self->recordName) - 1);
    return &self->base;
}
