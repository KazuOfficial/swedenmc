#ifndef ENTITY_PLAYER_SP_H
#define ENTITY_PLAYER_SP_H

#include "EntityPlayer.h"
#include "MouseFilter.h"
#include "Session.h"
#include "PlayerController.h"
#include "MovementInput.h"
#include "TileEntitySign.h"
#include "StatBase.h"

struct Minecraft;
struct IInventory;
struct TileEntityFurnace;
struct TileEntityDispenser;

typedef struct EntityPlayerSP {
    EntityPlayer base;
    struct Minecraft *mc;
    Session *field_9232_g;
    PlayerController *playerController;
    MouseFilter field_21903_bJ;
    MouseFilter field_21904_bK;
    MouseFilter field_21902_bL;

    MovementInput *movementInput;

} EntityPlayerSP;

void EntityPlayerSP_construct(EntityPlayerSP *self, World *world, struct Minecraft *mc,
                              Session *session, PlayerController *controller);
EntityPlayerSP *EntityPlayerSP_create(World *world, struct Minecraft *mc, Session *session,
                                      PlayerController *controller);

void EntityPlayerSP_turn(EntityPlayerSP *self, float par1, float par2);

void EntityPlayerSP_updatePlayerMoveState(EntityPlayerSP *self);

void EntityPlayerSP_resetPlayerKeyState(EntityPlayerSP *self);

void EntityPlayerSP_handleKeyPress(EntityPlayerSP *self, int var1, int var2);

void EntityPlayerSP_closeScreen(EntityPlayerSP *self);

void EntityPlayerSP_displayGUIEditSign(EntityPlayerSP *self, TileEntitySign *var1);

void EntityPlayerSP_displayGUIChest(EntityPlayerSP *self, struct IInventory *var1);

void EntityPlayerSP_displayWorkbenchGUI(EntityPlayerSP *self, int var1, int var2, int var3);

void EntityPlayerSP_displayGUIFurnace(EntityPlayerSP *self, struct TileEntityFurnace *var1);

void EntityPlayerSP_displayGUIDispenser(EntityPlayerSP *self, struct TileEntityDispenser *var1);

void EntityPlayerSP_onItemPickup(EntityPlayerSP *self, Entity *var1, int var2);

int EntityPlayerSP_getPlayerArmorValue(EntityPlayerSP *self);

void EntityPlayerSP_sendChatMessage(EntityPlayerSP *self, const char *var1);

int EntityPlayerSP_isSneaking(EntityPlayerSP *self);

void EntityPlayerSP_respawnPlayer(EntityPlayerSP *self);

void EntityPlayerSP_func_6420_o(EntityPlayerSP *self);

void EntityPlayerSP_addChatMessage(EntityPlayerSP *self, const char *var1);

void EntityPlayerSP_setHealth(EntityLiving *self, int var1);

void EntityPlayerSP_addStat(EntityPlayer *self_, struct StatBase *var1, int var2);

int EntityPlayerSP_pushOutOfBlocks(EntityPlayerSP *self, double var1, double var3, double var5);
void EntityPlayerSP_initVtable(EntityPlayerVtable *vt);

static inline float EntityPlayerSP_getSwingProgress(EntityPlayerSP *self, float partialTick) {
    return EntityLiving_getSwingProgress(&self->base.base, partialTick);
}

#endif
