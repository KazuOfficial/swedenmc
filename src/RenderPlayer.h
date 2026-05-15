#ifndef RENDER_PLAYER_H
#define RENDER_PLAYER_H

#include "RenderLiving.h"
#include "EntityPlayer.h"
#include "ModelBiped.h"

typedef struct RenderPlayer {
    RenderLiving base;

    ModelBiped *modelBipedMain;

    ModelBiped *modelArmorChestplate;

    ModelBiped *modelArmor;
} RenderPlayer;

RenderPlayer *RenderPlayer_create(void);

void RenderPlayer_drawFirstPersonHand(RenderPlayer *self);

#endif
