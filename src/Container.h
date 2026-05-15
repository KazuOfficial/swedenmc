#ifndef CONTAINER_H
#define CONTAINER_H

#include "ItemStack.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"

struct Slot;
struct ICrafting;
struct IInventory;
struct Container;

typedef struct {
    struct Slot **entries;
    int count;
    int capacity;
} SlotList;

typedef struct {
    ItemStack **entries;
    int count;
    int capacity;
} ItemStackPtrList;

typedef struct {
    struct ICrafting **entries;
    int count;
    int capacity;
} ICraftingList;

typedef struct ContainerVtable {

    int (*isUsableByPlayer)(struct Container *self, EntityPlayer *var1);

    ItemStack *(*getStackInSlot)(struct Container *self, int var1);

    void (*updateCraftingResults)(struct Container *self);

    void (*func_20112_a)(struct Container *self, int var1, int var2);

    void (*onCraftMatrixChanged)(struct Container *self, struct IInventory *var1);

    void (*onCraftGuiClosed)(struct Container *self, EntityPlayer *var1);
} ContainerVtable;

typedef struct Container {
    const ContainerVtable *vtable;

    ItemStackPtrList field_20123_d;

    SlotList slots;

    int windowId;

    short field_20917_a;

    ICraftingList field_20121_g;

    void *field_20918_b;
} Container;

void Container_construct(Container *self);

void Container_destroy(Container *self);

void Container_addSlot(Container *self, struct Slot *var1);

void Container_updateCraftingResults(Container *self);

struct Slot *Container_getSlot(Container *self, int var1);

ItemStack *Container_getStackInSlot(Container *self, int var1);

ItemStack *Container_func_27280_a(Container *self, int var1, int var2, int var3,
                                  EntityPlayer *var4);

void Container_onCraftGuiClosed(Container *self, EntityPlayer *var1);

void Container_onCraftMatrixChanged(Container *self, struct IInventory *var1);

void Container_putStackInSlot(Container *self, int var1, ItemStack *var2);

void Container_putStacksInSlots(Container *self, ItemStack **var1, int length);

void Container_func_20112_a(Container *self, int var1, int var2);

short Container_func_20111_a(Container *self, InventoryPlayer *var1);

void Container_func_20113_a(Container *self, short var1);

void Container_func_20110_b(Container *self, short var1);

static inline int Container_isUsableByPlayer(Container *self, EntityPlayer *var1) {
    return self->vtable->isUsableByPlayer(self, var1);
}

static inline ItemStack *Container_getStackInSlot_v(Container *self, int var1) {
    return self->vtable->getStackInSlot(self, var1);
}

static inline void Container_updateCraftingResults_v(Container *self) {
    self->vtable->updateCraftingResults(self);
}

static inline void Container_func_20112_a_v(Container *self, int var1, int var2) {
    self->vtable->func_20112_a(self, var1, var2);
}

static inline void Container_onCraftMatrixChanged_v(Container *self, struct IInventory *var1) {
    self->vtable->onCraftMatrixChanged(self, var1);
}

static inline void Container_onCraftGuiClosed_v(Container *self, EntityPlayer *var1) {
    self->vtable->onCraftGuiClosed(self, var1);
}

ItemStack *Container_vtable_base_getStackInSlot(Container *self, int var1);
void Container_vtable_base_updateCraftingResults(Container *self);
void Container_vtable_base_func_20112_a(Container *self, int var1, int var2);
void Container_vtable_base_onCraftMatrixChanged(Container *self, struct IInventory *var1);
void Container_vtable_base_onCraftGuiClosed(Container *self, EntityPlayer *var1);

void Container_func_28125_a(Container *self, ItemStack *var1, int var2, int var3, int var4);

#endif
