#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "World.h"
#include "ItemStack.h"
#include "Entity.h"

struct Minecraft;
struct EntityPlayer;
struct EntityPlayerSP;

typedef struct PlayerController PlayerController;

typedef struct PlayerControllerVtable {
    void (*func_717_a)(PlayerController *self, World *world);
    void (*clickBlock)(PlayerController *self, int x, int y, int z, int side);
    int (*sendBlockRemoved)(PlayerController *self, int x, int y, int z, int side);
    void (*sendBlockRemoving)(PlayerController *self, int x, int y, int z, int side);
    void (*resetBlockRemoving)(PlayerController *self);
    void (*setPartialTime)(PlayerController *self, float partialTick);
    float (*getBlockReachDistance)(PlayerController *self);
    int (*sendUseItem)(PlayerController *self, struct EntityPlayer *player, World *world,
                       ItemStack *stack);
    void (*flipPlayer)(PlayerController *self, struct EntityPlayer *player);
    void (*updateController)(PlayerController *self);
    int (*shouldDrawHUD)(PlayerController *self);
    void (*func_6473_b)(PlayerController *self, struct EntityPlayer *player);
    int (*sendPlaceBlock)(PlayerController *self, struct EntityPlayer *player, World *world,
                          ItemStack *stack, int x, int y, int z, int side);
    struct EntityPlayer *(*createPlayer)(PlayerController *self, World *world);
    void (*interactWithEntity)(PlayerController *self, struct EntityPlayer *player, Entity *entity);
    void (*attackEntity)(PlayerController *self, struct EntityPlayer *player, Entity *entity);

    ItemStack *(*func_27174_a)(PlayerController *self, int var1, int var2, int var3, int var4,
                               struct EntityPlayer *var5);

    void (*func_20086_a)(PlayerController *self, int var1, struct EntityPlayer *var2);
} PlayerControllerVtable;

struct PlayerController {
    const PlayerControllerVtable *vtable;
    struct Minecraft *mc;
    int field_1064_b;
};

PlayerController *PlayerController_create(struct Minecraft *mc);
void PlayerController_destroy(PlayerController *self);

void PlayerController_func_717_a(PlayerController *self, World *world);
void PlayerController_clickBlock(PlayerController *self, int x, int y, int z, int side);
int PlayerController_sendBlockRemoved(PlayerController *self, int x, int y, int z, int side);
void PlayerController_sendBlockRemoving(PlayerController *self, int x, int y, int z, int side);
void PlayerController_resetBlockRemoving(PlayerController *self);
void PlayerController_setPartialTime(PlayerController *self, float partialTick);
float PlayerController_getBlockReachDistance(PlayerController *self);
int PlayerController_sendUseItem(PlayerController *self, struct EntityPlayer *player, World *world,
                                 ItemStack *stack);
void PlayerController_flipPlayer(PlayerController *self, struct EntityPlayer *player);
void PlayerController_updateController(PlayerController *self);
int PlayerController_shouldDrawHUD(PlayerController *self);
void PlayerController_func_6473_b(PlayerController *self, struct EntityPlayer *player);
int PlayerController_sendPlaceBlock(PlayerController *self, struct EntityPlayer *player,
                                    World *world, ItemStack *stack, int x, int y, int z, int side);
struct EntityPlayer *PlayerController_createPlayer(PlayerController *self, World *world);
void PlayerController_interactWithEntity(PlayerController *self, struct EntityPlayer *player,
                                         Entity *entity);
void PlayerController_attackEntity(PlayerController *self, struct EntityPlayer *player,
                                   Entity *entity);
ItemStack *PlayerController_func_27174_a(PlayerController *self, int var1, int var2, int var3,
                                         int var4, struct EntityPlayer *var5);
void PlayerController_func_20086_a(PlayerController *self, int var1, struct EntityPlayer *var2);

extern PlayerControllerVtable PlayerController_defaultVtable;

#endif
