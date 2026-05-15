#ifndef PLAYER_CONTROLLER_SP_H
#define PLAYER_CONTROLLER_SP_H

#include "PlayerController.h"

struct Minecraft;

typedef struct PlayerControllerSP {
    PlayerController base;
    int field_1074_c;
    int field_1073_d;
    int field_1072_e;
    float curBlockDamage;
    float prevBlockDamage;
    float field_1069_h;
    int blockHitWait;
} PlayerControllerSP;

PlayerControllerSP *PlayerControllerSP_create(struct Minecraft *mc);
void PlayerControllerSP_destroy(PlayerControllerSP *self);

#endif
