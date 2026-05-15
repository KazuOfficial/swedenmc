#ifndef PLAYER_CONTROLLER_TEST_H
#define PLAYER_CONTROLLER_TEST_H

#include "PlayerController.h"

typedef struct PlayerControllerTest {
    PlayerController base;
} PlayerControllerTest;

PlayerControllerTest *PlayerControllerTest_create(struct Minecraft *var1);

int PlayerControllerTest_isInstance(PlayerController *self);

#endif
