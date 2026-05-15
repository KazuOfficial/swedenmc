#include "PlayerControllerTest.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "Block.h"
#include "Minecraft.h"
#include "Session.h"
#include "World.h"
#include <stdlib.h>

static void vt_func_6473_b(PlayerController *base, EntityPlayer *var1) {
    PlayerControllerTest *self = (PlayerControllerTest *)base;
    for (int var2 = 0; var2 < 9; ++var2) {
        if (var1->inventory.mainInventory[var2] == NULL) {

            ItemStack *stack = (ItemStack *)calloc(1, sizeof(ItemStack));
            *stack = ItemStack_fromBlockID(Session_registeredBlocksList[var2]->blockID, 1, 0);
            ((EntityPlayer *)self->base.mc->thePlayer)->inventory.mainInventory[var2] = stack;
        } else {
            ((EntityPlayer *)self->base.mc->thePlayer)->inventory.mainInventory[var2]->stackSize =
                1;
        }
    }
}

static int vt_shouldDrawHUD(PlayerController *base) {
    (void)base;
    return 0;
}

static void vt_func_717_a(PlayerController *base, World *var1) {

    PlayerController_defaultVtable.func_717_a(base, var1);
}

static void vt_updateController(PlayerController *base) { (void)base; }

static PlayerControllerVtable s_vtable;
static int s_vtable_init = 0;

static void initVtable(void) {
    s_vtable = PlayerController_defaultVtable;
    s_vtable.func_6473_b = vt_func_6473_b;
    s_vtable.shouldDrawHUD = vt_shouldDrawHUD;
    s_vtable.func_717_a = vt_func_717_a;
    s_vtable.updateController = vt_updateController;
}

int PlayerControllerTest_isInstance(PlayerController *self) {
    if (!s_vtable_init)
        return 0;
    return self->vtable == &s_vtable;
}

PlayerControllerTest *PlayerControllerTest_create(struct Minecraft *var1) {
    if (!s_vtable_init) {
        initVtable();
        s_vtable_init = 1;
    }
    PlayerControllerTest *self = (PlayerControllerTest *)calloc(1, sizeof(PlayerControllerTest));
    self->base.vtable = &s_vtable;
    self->base.mc = var1;
    self->base.field_1064_b = 1;
    return self;
}
