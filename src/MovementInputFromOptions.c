#include "MovementInputFromOptions.h"
#include <stdlib.h>

static void mio_checkKeyForMovementInput(MovementInput *base, int var1, int var2) {
    MovementInputFromOptions *self = (MovementInputFromOptions *)base;
    GameSettings *gs = self->gameSettings;
    int var3 = -1;
    if (var1 == gs->keyBindForward.keyCode)
        var3 = 0;
    if (var1 == gs->keyBindBack.keyCode)
        var3 = 1;
    if (var1 == gs->keyBindLeft.keyCode)
        var3 = 2;
    if (var1 == gs->keyBindRight.keyCode)
        var3 = 3;
    if (var1 == gs->keyBindJump.keyCode)
        var3 = 4;
    if (var1 == gs->keyBindSneak.keyCode)
        var3 = 5;
    if (var3 >= 0) {
        self->movementKeyStates[var3] = var2;
    }
}

static void mio_updatePlayerMoveState(MovementInput *base) {
    MovementInputFromOptions *self = (MovementInputFromOptions *)base;
    base->moveStrafe = 0.0f;
    base->moveForward = 0.0f;
    if (self->movementKeyStates[0])
        ++base->moveForward;
    if (self->movementKeyStates[1])
        --base->moveForward;
    if (self->movementKeyStates[2])
        ++base->moveStrafe;
    if (self->movementKeyStates[3])
        --base->moveStrafe;
    base->jump = self->movementKeyStates[4];
    base->sneak = self->movementKeyStates[5];
    if (base->sneak) {
        base->moveStrafe = (float)((double)base->moveStrafe * 0.3);
        base->moveForward = (float)((double)base->moveForward * 0.3);
    }
}

static void mio_resetKeyState(MovementInput *base) {
    MovementInputFromOptions *self = (MovementInputFromOptions *)base;
    for (int var1 = 0; var1 < 10; ++var1) {
        self->movementKeyStates[var1] = 0;
    }
}

static MovementInputVtable s_vtable = {
    mio_updatePlayerMoveState,
    mio_resetKeyState,
    mio_checkKeyForMovementInput,
};

MovementInputFromOptions *MovementInputFromOptions_create(GameSettings *gs) {
    MovementInputFromOptions *self =
        (MovementInputFromOptions *)calloc(1, sizeof(MovementInputFromOptions));
    self->base.vtable = &s_vtable;
    self->gameSettings = gs;
    return self;
}

void MovementInputFromOptions_destroy(MovementInputFromOptions *self) { free(self); }
