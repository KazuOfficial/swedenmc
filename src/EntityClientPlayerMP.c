#include "EntityClientPlayerMP.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "World.h"
#include "MathHelper.h"
#include "NetClientHandler.h"
#include "Packet3Chat.h"
#include "Packet9Respawn.h"
#include "Packet10Flying.h"
#include "Packet11PlayerPosition.h"
#include "Packet12PlayerLook.h"
#include "Packet13PlayerLookMove.h"
#include "Packet14BlockDig.h"
#include "Packet18Animation.h"
#include "Packet19EntityAction.h"
#include "Packet101CloseWindow.h"
#include "Container.h"
#include "InventoryPlayer.h"

static EntityPlayerVtable EntityClientPlayerMP_vtable;
static int EntityClientPlayerMP_vtable_init = 0;

static void ensure_vtable(void);
static void EntityClientPlayerMP_sendInventoryChanged(EntityClientPlayerMP *self);

void EntityClientPlayerMP_construct(EntityClientPlayerMP *self, struct Minecraft *var1, World *var2,
                                    Session *var3, struct NetClientHandler *var4) {
    ensure_vtable();
    EntityPlayerSP_construct(&self->base, var2, var1, var3, NULL);
    self->base.base.base.base.vtable = (const EntityVtable *)&EntityClientPlayerMP_vtable;

    ((Entity *)self)->isClientPlayerMP = 1;
    self->sendQueue = var4;
    self->field_9380_bx = 0;
    self->field_21093_bH = 0;
    self->oldPosX = 0.0;
    self->field_9378_bz = 0.0;
    self->oldPosY = 0.0;
    self->oldPosZ = 0.0;
    self->oldRotationYaw = 0.0F;
    self->oldRotationPitch = 0.0F;
    self->field_9382_bF = 0;
    self->wasSneaking = 0;
    self->field_12242_bI = 0;
}

int EntityClientPlayerMP_attackEntityFrom(Entity *selfE, Entity *var1, int var2) {
    (void)selfE;
    (void)var1;
    (void)var2;
    return 0;
}

void EntityClientPlayerMP_heal(EntityLiving *selfE, int var1) {
    (void)selfE;
    (void)var1;
}

void EntityClientPlayerMP_onUpdate(Entity *selfE) {
    EntityClientPlayerMP *self = (EntityClientPlayerMP *)selfE;
    if (World_blockExists(selfE->worldObj, MathHelper_floor_double(selfE->posX), 64,
                          MathHelper_floor_double(selfE->posZ))) {
        EntityPlayer_onUpdate(selfE);
        EntityClientPlayerMP_func_4056_N(self);
    }
}

void EntityClientPlayerMP_func_4056_N(EntityClientPlayerMP *self) {
    Entity *e = &self->base.base.base.base;

    if (self->field_9380_bx++ == 20) {
        EntityClientPlayerMP_sendInventoryChanged(self);
        self->field_9380_bx = 0;
    }

    int var1 = e->vtable->isSneaking(e);
    if (var1 != self->wasSneaking) {
        if (var1) {
            NetClientHandler_addToSendQueue(self->sendQueue, Packet19EntityAction_createWith(e, 1));
        } else {
            NetClientHandler_addToSendQueue(self->sendQueue, Packet19EntityAction_createWith(e, 2));
        }
        self->wasSneaking = var1;
    }

    double var2 = e->posX - self->oldPosX;
    double var4 = e->boundingBox.minY - self->field_9378_bz;
    double var6 = e->posY - self->oldPosY;
    double var8 = e->posZ - self->oldPosZ;
    double var10 = (double)(e->rotationYaw - self->oldRotationYaw);
    double var12 = (double)(e->rotationPitch - self->oldRotationPitch);
    int var14 = (var4 != 0.0 || var6 != 0.0 || var2 != 0.0 || var8 != 0.0);
    int var15 = (var10 != 0.0 || var12 != 0.0);
    if (e->ridingEntity != NULL) {
        if (var15) {
            NetClientHandler_addToSendQueue(
                self->sendQueue, Packet11PlayerPosition_createWith(e->motionX, -999.0, -999.0,
                                                                   e->motionZ, e->onGround));
        } else {
            NetClientHandler_addToSendQueue(
                self->sendQueue,
                Packet13PlayerLookMove_createWith(e->motionX, -999.0, -999.0, e->motionZ,
                                                  e->rotationYaw, e->rotationPitch, e->onGround));
        }
        var14 = 0;
    } else if (var14 && var15) {
        NetClientHandler_addToSendQueue(
            self->sendQueue,
            Packet13PlayerLookMove_createWith(e->posX, e->boundingBox.minY, e->posY, e->posZ,
                                              e->rotationYaw, e->rotationPitch, e->onGround));
        self->field_12242_bI = 0;
    } else if (var14) {
        NetClientHandler_addToSendQueue(
            self->sendQueue, Packet11PlayerPosition_createWith(e->posX, e->boundingBox.minY,
                                                               e->posY, e->posZ, e->onGround));
        self->field_12242_bI = 0;
    } else if (var15) {
        NetClientHandler_addToSendQueue(
            self->sendQueue,
            Packet12PlayerLook_createWith(e->rotationYaw, e->rotationPitch, e->onGround));
        self->field_12242_bI = 0;
    } else {
        NetClientHandler_addToSendQueue(self->sendQueue, Packet10Flying_createWith(e->onGround));
        if (self->field_9382_bF == e->onGround && self->field_12242_bI <= 200) {
            ++self->field_12242_bI;
        } else {
            self->field_12242_bI = 0;
        }
    }

    self->field_9382_bF = e->onGround;
    if (var14) {
        self->oldPosX = e->posX;
        self->field_9378_bz = e->boundingBox.minY;
        self->oldPosY = e->posY;
        self->oldPosZ = e->posZ;
    }

    if (var15) {
        self->oldRotationYaw = e->rotationYaw;
        self->oldRotationPitch = e->rotationPitch;
    }
}

void EntityClientPlayerMP_dropCurrentItem(EntityClientPlayerMP *self) {
    NetClientHandler_addToSendQueue(self->sendQueue, Packet14BlockDig_createWith(4, 0, 0, 0, 0));
}

static void EntityClientPlayerMP_sendInventoryChanged(EntityClientPlayerMP *self) { (void)self; }

void EntityClientPlayerMP_joinEntityItemWithWorld(EntityClientPlayerMP *self,
                                                  struct EntityItem *var1) {
    (void)self;
    (void)var1;
}

void EntityClientPlayerMP_sendChatMessage(EntityClientPlayerMP *self, const char *var1) {
    NetClientHandler_addToSendQueue(self->sendQueue, Packet3Chat_createWith(var1));
}

void EntityClientPlayerMP_swingItem(EntityPlayer *selfP) {
    EntityClientPlayerMP *self = (EntityClientPlayerMP *)selfP;

    selfP->base.swingProgressInt = -1;
    selfP->base.isSwingInProgress = 1;
    NetClientHandler_addToSendQueue(self->sendQueue,
                                    Packet18Animation_createWith(&selfP->base.base, 1));
}

void EntityClientPlayerMP_respawnPlayer(EntityClientPlayerMP *self) {
    EntityPlayer *ep = &self->base.base;
    EntityClientPlayerMP_sendInventoryChanged(self);
    NetClientHandler_addToSendQueue(self->sendQueue,
                                    Packet9Respawn_createWith((int8_t)ep->dimension));
}

void EntityClientPlayerMP_damageEntity(EntityLiving *selfE, Entity *source, int var1) {
    (void)source;
    selfE->health -= var1;
}

void EntityClientPlayerMP_closeScreen(EntityPlayer *selfP) {
    EntityClientPlayerMP *self = (EntityClientPlayerMP *)selfP;
    NetClientHandler_addToSendQueue(
        self->sendQueue, Packet101CloseWindow_createWith(selfP->craftingInventory->windowId));
    InventoryPlayer_setItemStack(&selfP->inventory, NULL);
    EntityPlayerSP_closeScreen((EntityPlayerSP *)self);
}

void EntityClientPlayerMP_setHealth(EntityLiving *selfE, int var1) {
    EntityClientPlayerMP *self = (EntityClientPlayerMP *)selfE;
    if (self->field_21093_bH) {
        EntityPlayerSP_setHealth(selfE, var1);
    } else {
        selfE->health = var1;
        self->field_21093_bH = 1;
    }
}

void EntityClientPlayerMP_addStat(EntityClientPlayerMP *self, StatBase *var1, int var2) {
    if (var1 != NULL) {
        if (var1->field_27088_g) {
            EntityPlayerSP_addStat((EntityPlayer *)self, var1, var2);
        }
    }
}

void EntityClientPlayerMP_func_27027_b(EntityClientPlayerMP *self, StatBase *var1, int var2) {
    if (var1 != NULL) {
        if (!var1->field_27088_g) {
            EntityPlayerSP_addStat((EntityPlayer *)self, var1, var2);
        }
    }
}

static void ensure_vtable(void) {
    if (!EntityClientPlayerMP_vtable_init) {
        EntityPlayerSP_initVtable(&EntityClientPlayerMP_vtable);
        EntityClientPlayerMP_vtable.base.base.attackEntityFrom =
            EntityClientPlayerMP_attackEntityFrom;
        EntityClientPlayerMP_vtable.base.base.onUpdate = EntityClientPlayerMP_onUpdate;
        EntityClientPlayerMP_vtable.base.heal = EntityClientPlayerMP_heal;
        EntityClientPlayerMP_vtable.base.setHealth = EntityClientPlayerMP_setHealth;
        EntityClientPlayerMP_vtable.base.damageEntity = EntityClientPlayerMP_damageEntity;
        EntityClientPlayerMP_vtable.swingItem = EntityClientPlayerMP_swingItem;
        EntityClientPlayerMP_vtable.respawnPlayer =
            (void (*)(EntityPlayer *))EntityClientPlayerMP_respawnPlayer;
        EntityClientPlayerMP_vtable_init = 1;
    }
}
