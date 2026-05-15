#include "PlayerControllerSP.h"
#include "EntityPlayerSP.h"
#include "EntityPlayer.h"
#include "Block.h"
#include "ItemStack.h"
#include "Minecraft.h"
#include "StepSound.h"
#include "SoundManager.h"
#include "GuiIngame.h"
#include "RenderGlobal.h"
#include <stdlib.h>

static void sp_flipPlayer(PlayerController *self, EntityPlayer *player) {
    (void)self;
    ((Entity *)player)->rotationYaw = -180.0f;
}

static int sp_sendBlockRemoved(PlayerController *self, int x, int y, int z, int side) {
    Minecraft *mc = self->mc;
    int id = World_getBlockId(mc->theWorld, x, y, z);
    int meta = World_getBlockMetadata(mc->theWorld, x, y, z);

    int result = PlayerController_defaultVtable.sendBlockRemoved(self, x, y, z, side);

    ItemStack *held = EntityPlayer_getCurrentEquippedItem((EntityPlayer *)mc->thePlayer);
    int canHarvest = EntityPlayer_canHarvestBlock(
        (EntityPlayer *)mc->thePlayer,
        (id > 0 && Block_blocksList[id]) ? Block_blocksList[id] : NULL);
    if (held != NULL) {
        ItemStack_onDestroyBlock(held, id, x, y, z, (EntityPlayer *)mc->thePlayer);
        if (held->stackSize == 0) {
            ItemStack_func_1097_a(held, (EntityPlayer *)mc->thePlayer);
            EntityPlayer_destroyCurrentEquippedItem((EntityPlayer *)mc->thePlayer);
        }
    }
    if (result && canHarvest && id > 0 && Block_blocksList[id]) {
        Block_blocksList[id]->vtable->harvestBlock(Block_blocksList[id], mc->theWorld,
                                                   (EntityPlayer *)mc->thePlayer, x, y, z, meta);
    }
    return result;
}

static void sp_clickBlock(PlayerController *self, int x, int y, int z, int side) {
    PlayerControllerSP *sp = (PlayerControllerSP *)self;
    Minecraft *mc = self->mc;

    World_onBlockHit(mc->theWorld, (EntityPlayer *)mc->thePlayer, x, y, z, side);
    int id = World_getBlockId(mc->theWorld, x, y, z);
    if (id > 0 && Block_blocksList[id] && sp->curBlockDamage == 0.0f) {
        Block_blocksList[id]->vtable->onBlockClicked(Block_blocksList[id], mc->theWorld, x, y, z,
                                                     (EntityPlayer *)mc->thePlayer);
    }
    if (id > 0 && Block_blocksList[id] &&
        Block_blocksList[id]->vtable->blockStrength(Block_blocksList[id],
                                                    (EntityPlayer *)mc->thePlayer) >= 1.0f) {
        sp_sendBlockRemoved(self, x, y, z, side);
    }
}

static void sp_resetBlockRemoving(PlayerController *self) {
    PlayerControllerSP *sp = (PlayerControllerSP *)self;
    sp->curBlockDamage = 0.0f;
    sp->blockHitWait = 0;
}

static void sp_sendBlockRemoving(PlayerController *self, int x, int y, int z, int side) {
    PlayerControllerSP *sp = (PlayerControllerSP *)self;
    Minecraft *mc = self->mc;

    if (sp->blockHitWait > 0) {
        --sp->blockHitWait;
        return;
    }
    if (x == sp->field_1074_c && y == sp->field_1073_d && z == sp->field_1072_e) {
        int id = World_getBlockId(mc->theWorld, x, y, z);
        if (id == 0)
            return;

        Block *block = Block_blocksList[id];
        sp->curBlockDamage += block->vtable->blockStrength(block, (EntityPlayer *)mc->thePlayer);
        if (((int)sp->field_1069_h) % 4 == 0 && block != NULL) {
            SoundManager_playSound(mc->sndManager, StepSound_func_1145_d(block->stepSound),
                                   (float)x + 0.5f, (float)y + 0.5f, (float)z + 0.5f,
                                   (StepSound_getVolume(block->stepSound) + 1.0f) / 8.0f,
                                   StepSound_getPitch(block->stepSound) * 0.5f);
        }
        ++sp->field_1069_h;
        if (sp->curBlockDamage >= 1.0f) {
            sp_sendBlockRemoved(self, x, y, z, side);
            sp->curBlockDamage = 0.0f;
            sp->prevBlockDamage = 0.0f;
            sp->field_1069_h = 0.0f;
            sp->blockHitWait = 5;
        }
    } else {
        sp->curBlockDamage = 0.0f;
        sp->prevBlockDamage = 0.0f;
        sp->field_1069_h = 0.0f;
        sp->field_1074_c = x;
        sp->field_1073_d = y;
        sp->field_1072_e = z;
    }
}

static void sp_setPartialTime(PlayerController *self, float partialTick) {
    PlayerControllerSP *sp = (PlayerControllerSP *)self;
    Minecraft *mc = self->mc;
    if (sp->curBlockDamage <= 0.0f) {
        mc->ingameGUI->damageGuiPartialTime = 0.0f;
        mc->renderGlobal->damagePartialTime = 0.0f;
    } else {
        float var2 = sp->prevBlockDamage + (sp->curBlockDamage - sp->prevBlockDamage) * partialTick;
        mc->ingameGUI->damageGuiPartialTime = var2;
        mc->renderGlobal->damagePartialTime = var2;
    }
}

static float sp_getBlockReachDistance(PlayerController *self) {
    (void)self;
    return 4.0f;
}

static void sp_func_717_a(PlayerController *self, World *world) {

    (void)self;
    (void)world;
}

static void sp_updateController(PlayerController *self) {
    PlayerControllerSP *sp = (PlayerControllerSP *)self;
    sp->prevBlockDamage = sp->curBlockDamage;
    SoundManager_playRandomMusicIfReady(self->mc->sndManager);
}

static PlayerControllerVtable s_sp_vtable;
static int s_sp_vtable_init = 0;

static void sp_vtable_ensure_init(void) {
    if (s_sp_vtable_init)
        return;
    s_sp_vtable = PlayerController_defaultVtable;
    s_sp_vtable.func_717_a = sp_func_717_a;
    s_sp_vtable.clickBlock = sp_clickBlock;
    s_sp_vtable.sendBlockRemoved = sp_sendBlockRemoved;
    s_sp_vtable.sendBlockRemoving = sp_sendBlockRemoving;
    s_sp_vtable.resetBlockRemoving = sp_resetBlockRemoving;
    s_sp_vtable.setPartialTime = sp_setPartialTime;
    s_sp_vtable.getBlockReachDistance = sp_getBlockReachDistance;
    s_sp_vtable.flipPlayer = sp_flipPlayer;
    s_sp_vtable.updateController = sp_updateController;
    s_sp_vtable_init = 1;
}

PlayerControllerSP *PlayerControllerSP_create(struct Minecraft *mc) {
    sp_vtable_ensure_init();
    PlayerControllerSP *self = (PlayerControllerSP *)calloc(1, sizeof(PlayerControllerSP));
    self->base.vtable = &s_sp_vtable;
    self->base.mc = mc;
    self->field_1074_c = -1;
    self->field_1073_d = -1;
    self->field_1072_e = -1;
    return self;
}

void PlayerControllerSP_destroy(PlayerControllerSP *self) { free(self); }
