#ifndef MOVEMENT_INPUT_H
#define MOVEMENT_INPUT_H

typedef struct MovementInput MovementInput;

struct EntityPlayer;

typedef struct MovementInputVtable {
    void (*updatePlayerMoveState)(MovementInput *self);
    void (*resetKeyState)(MovementInput *self);

    void (*checkKeyForMovementInput)(MovementInput *self, int var1, int var2);
} MovementInputVtable;

struct MovementInput {
    const MovementInputVtable *vtable;
    float moveStrafe;
    float moveForward;

    int field_1177_c;
    int jump;
    int sneak;
};

#endif
