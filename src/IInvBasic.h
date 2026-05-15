#ifndef IINV_BASIC_H
#define IINV_BASIC_H

#include "GameForward.h"

struct IInvBasic;

typedef struct IInvBasicVtable {

    void (*func_20134_a)(struct IInvBasic *self, InventoryBasic *var1);
} IInvBasicVtable;

typedef struct IInvBasic {
    const IInvBasicVtable *vtable;
} IInvBasic;

#endif
