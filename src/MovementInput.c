#include "MovementInput.h"

static void MovementInput_updatePlayerMoveState_default(MovementInput *self) { (void)self; }

static void MovementInput_resetKeyState_default(MovementInput *self) { (void)self; }

static void MovementInput_checkKeyForMovementInput_default(MovementInput *self, int var1,
                                                           int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

static const MovementInputVtable s_defaultVtable = {
    MovementInput_updatePlayerMoveState_default,
    MovementInput_resetKeyState_default,
    MovementInput_checkKeyForMovementInput_default,
};

const MovementInputVtable *MovementInput_defaultVtable = &s_defaultVtable;
