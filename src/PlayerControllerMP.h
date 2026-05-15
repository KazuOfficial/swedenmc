#ifndef PLAYER_CONTROLLER_MP_H
#define PLAYER_CONTROLLER_MP_H

#include "PlayerController.h"

struct NetClientHandler;

typedef struct PlayerControllerMP {
    PlayerController base;

    int currentBlockX;

    int currentBlockY;

    int currentblockZ;

    float curBlockDamageMP;

    float prevBlockDamageMP;

    float field_9441_h;

    int blockHitDelay;

    int isHittingBlock;

    struct NetClientHandler *netClientHandler;

    int currentPlayerItem;
} PlayerControllerMP;

PlayerControllerMP *PlayerControllerMP_create(struct Minecraft *var1,
                                              struct NetClientHandler *var2);

#endif
