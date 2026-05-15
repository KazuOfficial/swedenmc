#ifndef ICRAFTING_H
#define ICRAFTING_H

#include "GameForward.h"

struct ICrafting;

typedef struct ICraftingVtable {

    void (*func_20159_a)(struct ICrafting *self, Container *var1, int var2, ItemStack *var3);

    void (*func_20158_a)(struct ICrafting *self, Container *var1, int var2, int var3);
} ICraftingVtable;

typedef struct ICrafting {
    const ICraftingVtable *vtable;
} ICrafting;

#endif
