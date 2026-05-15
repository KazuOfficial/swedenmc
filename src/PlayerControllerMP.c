#include "PlayerControllerMP.h"
#include "NetClientHandler.h"
#include "EntityPlayer.h"
#include "EntityClientPlayerMP.h"
#include "Block.h"
#include "ItemStack.h"
#include "World.h"
#include "Minecraft.h"
#include "SoundManager.h"
#include "Container.h"
#include "GuiIngame.h"
#include "RenderGlobal.h"
#include "StepSound.h"
#include "Packet14BlockDig.h"
#include "Packet15Place.h"
#include "Packet16BlockItemSwitch.h"
#include "Packet7UseEntity.h"
#include "Packet102WindowClick.h"
#include <stdlib.h>
#include <math.h>

static void syncCurrentPlayItem(PlayerControllerMP *self) {
    if (!self->base.mc->thePlayer)
        return;
    int var1 = ((EntityPlayer *)self->base.mc->thePlayer)->inventory.currentItem;
    if (var1 != self->currentPlayerItem) {
        self->currentPlayerItem = var1;
        NetClientHandler_addToSendQueue(
            self->netClientHandler, Packet16BlockItemSwitch_createWith(self->currentPlayerItem));
    }
}

static void vt_flipPlayer(PlayerController *base, EntityPlayer *var1) {
    (void)base;
    ((Entity *)var1)->rotationYaw = -180.0f;
}

static int vt_sendBlockRemoved(PlayerController *base, int var1, int var2, int var3, int var4) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    int var5 = World_getBlockId(self->base.mc->theWorld, var1, var2, var3);
    int var6 = PlayerController_defaultVtable.sendBlockRemoved(base, var1, var2, var3, var4);
    ItemStack *var7 = EntityPlayer_getCurrentEquippedItem((EntityPlayer *)self->base.mc->thePlayer);
    if (var7 != NULL) {
        ItemStack_onDestroyBlock(var7, var5, var1, var2, var3,
                                 (EntityPlayer *)self->base.mc->thePlayer);
        if (var7->stackSize == 0) {
            ItemStack_func_1097_a(var7, (EntityPlayer *)self->base.mc->thePlayer);
            EntityPlayer_destroyCurrentEquippedItem((EntityPlayer *)self->base.mc->thePlayer);
        }
    }
    return var6;
}

static void vt_clickBlock(PlayerController *base, int var1, int var2, int var3, int var4) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    if (!self->isHittingBlock || var1 != self->currentBlockX || var2 != self->currentBlockY ||
        var3 != self->currentblockZ) {
        NetClientHandler_addToSendQueue(self->netClientHandler,
                                        Packet14BlockDig_createWith(0, var1, var2, var3, var4));
        int var5 = World_getBlockId(self->base.mc->theWorld, var1, var2, var3);
        if (var5 > 0 && self->curBlockDamageMP == 0.0f) {
            Block_blocksList[var5]->vtable->onBlockClicked(
                Block_blocksList[var5], self->base.mc->theWorld, var1, var2, var3,
                (EntityPlayer *)self->base.mc->thePlayer);
        }
        if (var5 > 0 &&
            Block_blocksList[var5]->vtable->blockStrength(
                Block_blocksList[var5], (EntityPlayer *)self->base.mc->thePlayer) >= 1.0f) {
            vt_sendBlockRemoved(base, var1, var2, var3, var4);
        } else {
            self->isHittingBlock = 1;
            self->currentBlockX = var1;
            self->currentBlockY = var2;
            self->currentblockZ = var3;
            self->curBlockDamageMP = 0.0f;
            self->prevBlockDamageMP = 0.0f;
            self->field_9441_h = 0.0f;
        }
    }
}

static void vt_resetBlockRemoving(PlayerController *base) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    self->curBlockDamageMP = 0.0f;
    self->isHittingBlock = 0;
}

static void vt_sendBlockRemoving(PlayerController *base, int var1, int var2, int var3, int var4) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    if (self->isHittingBlock) {
        syncCurrentPlayItem(self);
        if (self->blockHitDelay > 0) {
            --self->blockHitDelay;
        } else {
            if (var1 == self->currentBlockX && var2 == self->currentBlockY &&
                var3 == self->currentblockZ) {
                int var5 = World_getBlockId(self->base.mc->theWorld, var1, var2, var3);
                if (var5 == 0) {
                    self->isHittingBlock = 0;
                    return;
                }
                Block *var6 = Block_blocksList[var5];
                self->curBlockDamageMP +=
                    var6->vtable->blockStrength(var6, (EntityPlayer *)self->base.mc->thePlayer);
                if (fmodf(self->field_9441_h, 4.0f) == 0.0f && var6 != NULL) {
                    SoundManager_playSound(
                        self->base.mc->sndManager, StepSound_func_1145_d(var6->stepSound),
                        (float)var1 + 0.5f, (float)var2 + 0.5f, (float)var3 + 0.5f,
                        (StepSound_getVolume(var6->stepSound) + 1.0f) / 8.0f,
                        StepSound_getPitch(var6->stepSound) * 0.5f);
                }
                ++self->field_9441_h;
                if (self->curBlockDamageMP >= 1.0f) {
                    self->isHittingBlock = 0;
                    NetClientHandler_addToSendQueue(
                        self->netClientHandler,
                        Packet14BlockDig_createWith(2, var1, var2, var3, var4));
                    vt_sendBlockRemoved(base, var1, var2, var3, var4);
                    self->curBlockDamageMP = 0.0f;
                    self->prevBlockDamageMP = 0.0f;
                    self->field_9441_h = 0.0f;
                    self->blockHitDelay = 5;
                }
            } else {
                vt_clickBlock(base, var1, var2, var3, var4);
            }
        }
    }
}

static void vt_setPartialTime(PlayerController *base, float var1) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    if (self->curBlockDamageMP <= 0.0f) {
        self->base.mc->ingameGUI->damageGuiPartialTime = 0.0f;
        self->base.mc->renderGlobal->damagePartialTime = 0.0f;
    } else {
        float var2 =
            self->prevBlockDamageMP + (self->curBlockDamageMP - self->prevBlockDamageMP) * var1;
        self->base.mc->ingameGUI->damageGuiPartialTime = var2;
        self->base.mc->renderGlobal->damagePartialTime = var2;
    }
}

static float vt_getBlockReachDistance(PlayerController *base) {
    (void)base;
    return 4.0f;
}

static void vt_func_717_a(PlayerController *base, World *var1) {

    PlayerController_defaultVtable.func_717_a(base, var1);
}

static void vt_updateController(PlayerController *base) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    syncCurrentPlayItem(self);
    self->prevBlockDamageMP = self->curBlockDamageMP;
    SoundManager_playRandomMusicIfReady(self->base.mc->sndManager);
}

static int vt_sendPlaceBlock(PlayerController *base, EntityPlayer *var1, World *var2,
                             ItemStack *var3, int var4, int var5, int var6, int var7) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    syncCurrentPlayItem(self);
    NetClientHandler_addToSendQueue(
        self->netClientHandler,
        Packet15Place_createWith(var4, var5, var6, var7,
                                 InventoryPlayer_getCurrentItem(&var1->inventory)));
    int var8 = PlayerController_defaultVtable.sendPlaceBlock(base, var1, var2, var3, var4, var5,
                                                             var6, var7);
    return var8;
}

static int vt_sendUseItem(PlayerController *base, EntityPlayer *var1, World *var2,
                          ItemStack *var3) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    syncCurrentPlayItem(self);
    NetClientHandler_addToSendQueue(
        self->netClientHandler,
        Packet15Place_createWith(-1, -1, -1, 255,
                                 InventoryPlayer_getCurrentItem(&var1->inventory)));
    int var4 = PlayerController_defaultVtable.sendUseItem(base, var1, var2, var3);
    return var4;
}

static EntityPlayer *vt_createPlayer(PlayerController *base, World *var1) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    EntityClientPlayerMP *p = (EntityClientPlayerMP *)calloc(1, sizeof(EntityClientPlayerMP));
    EntityClientPlayerMP_construct(p, self->base.mc, var1, self->base.mc->session,
                                   self->netClientHandler);
    return (EntityPlayer *)p;
}

static void vt_attackEntity(PlayerController *base, EntityPlayer *var1, Entity *var2) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    syncCurrentPlayItem(self);
    NetClientHandler_addToSendQueue(
        self->netClientHandler,
        Packet7UseEntity_createWith(((Entity *)var1)->entityId, var2->entityId, 1));
    EntityPlayer_attackTargetEntityWithCurrentItem(var1, var2);
}

static void vt_interactWithEntity(PlayerController *base, EntityPlayer *var1, Entity *var2) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    syncCurrentPlayItem(self);
    NetClientHandler_addToSendQueue(
        self->netClientHandler,
        Packet7UseEntity_createWith(((Entity *)var1)->entityId, var2->entityId, 0));
    EntityPlayer_useCurrentItemOnEntity(var1, var2);
}

static ItemStack *vt_func_27174_a(PlayerController *base, int var1, int var2, int var3, int var4,
                                  EntityPlayer *var5) {
    PlayerControllerMP *self = (PlayerControllerMP *)base;
    short var6 = Container_func_20111_a(var5->craftingInventory, &var5->inventory);
    ItemStack *var7 =
        PlayerController_defaultVtable.func_27174_a(base, var1, var2, var3, var4, var5);
    NetClientHandler_addToSendQueue(
        self->netClientHandler,
        Packet102WindowClick_createWith(var1, var2, var3, var4, var7, var6));
    return var7;
}

static void vt_func_20086_a(PlayerController *base, int var1, EntityPlayer *var2) {
    (void)base;
    (void)var2;
    if (var1 != -9999) {
    }
}

static PlayerControllerVtable s_vtable;
static int s_vtable_init = 0;

static void initVtable(void) {
    s_vtable = PlayerController_defaultVtable;
    s_vtable.flipPlayer = vt_flipPlayer;
    s_vtable.sendBlockRemoved = vt_sendBlockRemoved;
    s_vtable.clickBlock = vt_clickBlock;
    s_vtable.resetBlockRemoving = vt_resetBlockRemoving;
    s_vtable.sendBlockRemoving = vt_sendBlockRemoving;
    s_vtable.setPartialTime = vt_setPartialTime;
    s_vtable.getBlockReachDistance = vt_getBlockReachDistance;
    s_vtable.func_717_a = vt_func_717_a;
    s_vtable.updateController = vt_updateController;
    s_vtable.sendPlaceBlock = vt_sendPlaceBlock;
    s_vtable.sendUseItem = vt_sendUseItem;
    s_vtable.createPlayer = vt_createPlayer;
    s_vtable.attackEntity = vt_attackEntity;
    s_vtable.interactWithEntity = vt_interactWithEntity;
    s_vtable.func_27174_a = vt_func_27174_a;
    s_vtable.func_20086_a = vt_func_20086_a;
}

PlayerControllerMP *PlayerControllerMP_create(struct Minecraft *var1,
                                              struct NetClientHandler *var2) {
    if (!s_vtable_init) {
        initVtable();
        s_vtable_init = 1;
    }
    PlayerControllerMP *self = (PlayerControllerMP *)calloc(1, sizeof(PlayerControllerMP));
    self->base.vtable = &s_vtable;
    self->base.mc = var1;
    self->base.field_1064_b = 0;
    self->currentBlockX = -1;
    self->currentBlockY = -1;
    self->currentblockZ = -1;
    self->curBlockDamageMP = 0.0f;
    self->prevBlockDamageMP = 0.0f;
    self->field_9441_h = 0.0f;
    self->blockHitDelay = 0;
    self->isHittingBlock = 0;
    self->netClientHandler = var2;
    self->currentPlayerItem = 0;
    return self;
}
