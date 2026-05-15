#ifndef MOVEMENT_INPUT_FROM_OPTIONS_H
#define MOVEMENT_INPUT_FROM_OPTIONS_H

#include "MovementInput.h"
#include "GameSettings.h"

typedef struct MovementInputFromOptions {
    MovementInput base;
    int movementKeyStates[10];
    GameSettings *gameSettings;
} MovementInputFromOptions;

MovementInputFromOptions *MovementInputFromOptions_create(GameSettings *gameSettings);
void MovementInputFromOptions_destroy(MovementInputFromOptions *self);

#endif
