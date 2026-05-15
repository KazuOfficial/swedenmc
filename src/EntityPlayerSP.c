#include "EntityPlayerSP.h"
#include "MovementInputFromOptions.h"
#include "Minecraft.h"
#include "AchievementList.h"
#include "StatFileWriter.h"
#include "GuiAchievement.h"
#include "SoundManager.h"
#include "EffectRenderer.h"
#include "EntityPickupFX.h"
#include "GuiIngame.h"
#include "GuiEditSign.h"
#include "GuiChest.h"
#include "GuiCrafting.h"
#include "GuiFurnace.h"
#include "GuiDispenser.h"
#include "InventoryPlayer.h"
#include "MathHelper.h"
#include "NBTTagCompound.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static EntityPlayerVtable s_sp_vtable;
static int s_sp_vtable_init = 0;

static void sp_moveEntity(Entity *self, double var1, double var3, double var5) {
    EntityPlayer_defaultVtable.base.base.moveEntity(self, var1, var3, var5);
}

static void sp_updatePlayerActionState(EntityLiving *base) {
    EntityPlayerSP *self = (EntityPlayerSP *)base;
    EntityPlayer_updatePlayerActionStateBase(base);
    if (self->movementInput) {
        base->moveStrafing = self->movementInput->moveStrafe;
        base->moveForward = self->movementInput->moveForward;
        base->isJumping = self->movementInput->jump;
    }
}

static void sp_onLivingUpdate(EntityLiving *base) {
    EntityPlayerSP *self = (EntityPlayerSP *)base;
    Entity *entity = (Entity *)self;

    if (self->mc && self->mc->statFileWriter) {
        if (!StatFileWriter_hasAchievementUnlocked(self->mc->statFileWriter,
                                                   AchievementList_openInventory)) {
            if (self->mc->guiAchievement)
                GuiAchievement_queueAchievementInformation(self->mc->guiAchievement,
                                                           AchievementList_openInventory);
        }
    }

    EntityPlayer *player = (EntityPlayer *)self;
    player->prevTimeInPortal = player->timeInPortal;
    if (player->inPortal) {
        if (!entity->worldObj->multiplayerWorld && entity->ridingEntity != NULL) {
            Entity_mountEntity(entity, NULL);
        }
        if (self->mc->currentScreen != NULL) {
            Minecraft_displayGuiScreen(self->mc, (struct GuiScreen *)NULL);
        }
        if (player->timeInPortal == 0.0f) {
            SoundManager_playSoundFX((SoundManager *)self->mc->sndManager, "portal.trigger", 1.0f,
                                     JavaRandom_nextFloat(&entity->rand) * 0.4f + 0.8f);
        }
        player->timeInPortal += 0.0125f;
        if (player->timeInPortal >= 1.0f) {
            player->timeInPortal = 1.0f;
            if (!entity->worldObj->multiplayerWorld) {
                player->timeUntilPortal = 10;
                SoundManager_playSoundFX((SoundManager *)self->mc->sndManager, "portal.travel",
                                         1.0f, JavaRandom_nextFloat(&entity->rand) * 0.4f + 0.8f);
                Minecraft_usePortal(self->mc);
            }
        }
        player->inPortal = 0;
    } else {
        if (player->timeInPortal > 0.0f)
            player->timeInPortal -= 0.05f;
        if (player->timeInPortal < 0.0f)
            player->timeInPortal = 0.0f;
    }

    if (player->timeUntilPortal > 0) {
        --player->timeUntilPortal;
    }

    if (self->movementInput) {
        self->movementInput->vtable->updatePlayerMoveState(self->movementInput);
        if (self->movementInput->sneak && entity->ySize < 0.2f) {
            entity->ySize = 0.2f;
        }
    }

    EntityPlayerSP_pushOutOfBlocks(self, entity->posX - (double)entity->width * 0.35,
                                   entity->boundingBox.minY + 0.5,
                                   entity->posZ + (double)entity->width * 0.35);
    EntityPlayerSP_pushOutOfBlocks(self, entity->posX - (double)entity->width * 0.35,
                                   entity->boundingBox.minY + 0.5,
                                   entity->posZ - (double)entity->width * 0.35);
    EntityPlayerSP_pushOutOfBlocks(self, entity->posX + (double)entity->width * 0.35,
                                   entity->boundingBox.minY + 0.5,
                                   entity->posZ - (double)entity->width * 0.35);
    EntityPlayerSP_pushOutOfBlocks(self, entity->posX + (double)entity->width * 0.35,
                                   entity->boundingBox.minY + 0.5,
                                   entity->posZ + (double)entity->width * 0.35);

    EntityPlayer_onLivingUpdateBase(base);
}

void EntityPlayerSP_setHealth(EntityLiving *base, int var1) {
    int var2 = base->health - var1;
    if (var2 <= 0) {
        base->health = var1;
        if (var2 < 0) {
            ((Entity *)base)->heartsLife = base->heartsHalvesLife / 2;
        }
    } else {
        base->field_9346_af = var2;
        base->prevHealth = base->health;
        ((Entity *)base)->heartsLife = base->heartsHalvesLife;

        base->health -= var2;
        base->hurtTime = base->maxHurtTime = 10;
    }
}

static void sp_writeEntityToNBT(Entity *self, NBTTagCompound *var1) {
    EntityPlayer_defaultVtable.base.base.writeEntityToNBT(self, var1);
    NBTTagCompound_setInteger(var1, "Score", ((EntityPlayer *)self)->score);
}

static void sp_readEntityFromNBT(Entity *self, NBTTagCompound *var1) {
    EntityPlayer_defaultVtable.base.base.readEntityFromNBT(self, var1);
    ((EntityPlayer *)self)->score = NBTTagCompound_getInteger(var1, "Score");
}

void EntityPlayerSP_closeScreen(EntityPlayerSP *self) {
    EntityPlayer_closeScreen(&self->base);
    Minecraft_displayGuiScreen(self->mc, (struct GuiScreen *)NULL);
}

void EntityPlayerSP_displayGUIEditSign(EntityPlayerSP *self, TileEntitySign *var1) {
    GuiEditSign *gui = (GuiEditSign *)calloc(1, sizeof(GuiEditSign));
    GuiEditSign_construct(gui, var1);
    Minecraft_displayGuiScreen(self->mc, (struct GuiScreen *)gui);
}

void EntityPlayerSP_displayGUIChest(EntityPlayerSP *self, struct IInventory *var1) {
    GuiChest *gui = (GuiChest *)calloc(1, sizeof(GuiChest));
    GuiChest_construct(gui, (struct IInventory *)&self->base.inventory, var1);
    Minecraft_displayGuiScreen(self->mc, (struct GuiScreen *)gui);
}

void EntityPlayerSP_displayWorkbenchGUI(EntityPlayerSP *self, int var1, int var2, int var3) {
    GuiCrafting *gui = (GuiCrafting *)calloc(1, sizeof(GuiCrafting));
    GuiCrafting_construct(gui, &self->base.inventory, ((Entity *)self)->worldObj, var1, var2, var3);
    Minecraft_displayGuiScreen(self->mc, (struct GuiScreen *)gui);
}

void EntityPlayerSP_displayGUIFurnace(EntityPlayerSP *self, struct TileEntityFurnace *var1) {
    GuiFurnace *gui = (GuiFurnace *)calloc(1, sizeof(GuiFurnace));
    GuiFurnace_construct(gui, &self->base.inventory, var1);
    Minecraft_displayGuiScreen(self->mc, (struct GuiScreen *)gui);
}

void EntityPlayerSP_displayGUIDispenser(EntityPlayerSP *self, struct TileEntityDispenser *var1) {
    GuiDispenser *gui = (GuiDispenser *)calloc(1, sizeof(GuiDispenser));
    GuiDispenser_construct(gui, &self->base.inventory, var1);
    Minecraft_displayGuiScreen(self->mc, (struct GuiScreen *)gui);
}

void EntityPlayerSP_onItemPickup(EntityPlayerSP *self, Entity *var1, int var2) {
    (void)var2;
    if (self->mc && self->mc->effectRenderer) {
        EntityPickupFX *fx = EntityPickupFX_create(self->mc->theWorld, var1, (Entity *)self, -0.5f);
        EffectRenderer_addEffect((EffectRenderer *)self->mc->effectRenderer, (struct EntityFX *)fx);
    }
}

int EntityPlayerSP_getPlayerArmorValue(EntityPlayerSP *self) {
    return InventoryPlayer_getTotalArmorValue(&self->base.inventory);
}

void EntityPlayerSP_sendChatMessage(EntityPlayerSP *self, const char *var1) {
    (void)self;
    (void)var1;
}

int EntityPlayerSP_isSneaking(EntityPlayerSP *self) {
    return self->movementInput && self->movementInput->sneak && !self->base.sleeping;
}
static int sp_isSneaking(Entity *e) { return EntityPlayerSP_isSneaking((EntityPlayerSP *)e); }

void EntityPlayerSP_respawnPlayer(EntityPlayerSP *self) { Minecraft_respawn(self->mc, 0, 0); }

void EntityPlayerSP_func_6420_o(EntityPlayerSP *self) { (void)self; }

void EntityPlayerSP_addChatMessage(EntityPlayerSP *self, const char *var1) {
    if (self->mc && self->mc->ingameGUI)
        GuiIngame_addChatMessageTranslate((GuiIngame *)self->mc->ingameGUI, var1);
}

void EntityPlayerSP_addStat(EntityPlayer *self_, struct StatBase *var1, int var2) {
    EntityPlayerSP *self = (EntityPlayerSP *)self_;
    if (var1 == NULL)
        return;
    if (!self->mc || !self->mc->statFileWriter)
        return;
    if (StatBase_func_25067_a(var1)) {
        Achievement *var3 = (Achievement *)var1;
        if (var3->parentAchievement == NULL ||
            StatFileWriter_hasAchievementUnlocked(self->mc->statFileWriter,
                                                  var3->parentAchievement)) {
            if (!StatFileWriter_hasAchievementUnlocked(self->mc->statFileWriter, var3)) {
                if (self->mc->guiAchievement)
                    GuiAchievement_queueTakenAchievement(self->mc->guiAchievement, var3);
            }
            StatFileWriter_readStat(self->mc->statFileWriter, var1, var2);
        }
    } else {
        StatFileWriter_readStat(self->mc->statFileWriter, var1, var2);
    }
}

static int sp_isBlockTranslucent(EntityPlayerSP *self, int var1, int var2, int var3) {
    return World_isBlockNormalCube(((Entity *)self)->worldObj, var1, var2, var3);
}

int EntityPlayerSP_pushOutOfBlocks(EntityPlayerSP *self, double var1, double var3, double var5) {
    Entity *entity = (Entity *)self;
    int var7 = MathHelper_floor_double(var1);
    int var8 = MathHelper_floor_double(var3);
    int var9 = MathHelper_floor_double(var5);
    double var10 = var1 - (double)var7;
    double var12 = var5 - (double)var9;
    if (sp_isBlockTranslucent(self, var7, var8, var9) ||
        sp_isBlockTranslucent(self, var7, var8 + 1, var9)) {
        int var14 = !sp_isBlockTranslucent(self, var7 - 1, var8, var9) &&
                    !sp_isBlockTranslucent(self, var7 - 1, var8 + 1, var9);
        int var15 = !sp_isBlockTranslucent(self, var7 + 1, var8, var9) &&
                    !sp_isBlockTranslucent(self, var7 + 1, var8 + 1, var9);
        int var16 = !sp_isBlockTranslucent(self, var7, var8, var9 - 1) &&
                    !sp_isBlockTranslucent(self, var7, var8 + 1, var9 - 1);
        int var17 = !sp_isBlockTranslucent(self, var7, var8, var9 + 1) &&
                    !sp_isBlockTranslucent(self, var7, var8 + 1, var9 + 1);
        int var18 = -1;
        double var19 = 9999.0;
        if (var14 && var10 < var19) {
            var19 = var10;
            var18 = 0;
        }
        if (var15 && 1.0 - var10 < var19) {
            var19 = 1.0 - var10;
            var18 = 1;
        }
        if (var16 && var12 < var19) {
            var19 = var12;
            var18 = 4;
        }
        if (var17 && 1.0 - var12 < var19) {
            var19 = 1.0 - var12;
            var18 = 5;
        }
        (void)var19;
        float var21 = 0.1f;
        if (var18 == 0)
            entity->motionX = (double)(-var21);
        if (var18 == 1)
            entity->motionX = (double)var21;
        if (var18 == 4)
            entity->motionZ = (double)(-var21);
        if (var18 == 5)
            entity->motionZ = (double)var21;
    }
    return 0;
}

static void sp_onItemPickup(EntityPlayer *base, Entity *var1, int var2) {
    EntityPlayerSP_onItemPickup((EntityPlayerSP *)base, var1, var2);
}

static void sp_vtable_ensure_init(void) {
    if (s_sp_vtable_init)
        return;
    EntityPlayer_initVtable(&s_sp_vtable);
    s_sp_vtable.base.base.moveEntity = sp_moveEntity;
    s_sp_vtable.base.base.writeEntityToNBT = sp_writeEntityToNBT;
    s_sp_vtable.base.base.readEntityFromNBT = sp_readEntityFromNBT;
    s_sp_vtable.base.updatePlayerActionState = sp_updatePlayerActionState;
    s_sp_vtable.base.onLivingUpdate = sp_onLivingUpdate;
    s_sp_vtable.base.setHealth = EntityPlayerSP_setHealth;
    s_sp_vtable.onItemPickup = sp_onItemPickup;
    s_sp_vtable.addStat = EntityPlayerSP_addStat;
    s_sp_vtable.base.base.isSneaking = sp_isSneaking;
    s_sp_vtable.respawnPlayer = (void (*)(EntityPlayer *))EntityPlayerSP_respawnPlayer;
    s_sp_vtable_init = 1;
}

void EntityPlayerSP_initVtable(EntityPlayerVtable *vt) {
    sp_vtable_ensure_init();
    *vt = s_sp_vtable;
}

void EntityPlayerSP_construct(EntityPlayerSP *self, World *world, struct Minecraft *mc,
                              Session *session, PlayerController *controller) {
    sp_vtable_ensure_init();
    EntityPlayer_construct(&self->base, world);
    ((Entity *)self)->vtable = &s_sp_vtable.base.base;
    ((Entity *)self)->isPlayerSP = 1;

    self->mc = mc;
    self->field_9232_g = session;
    self->playerController = controller;

    self->base.dimension = (world && world->worldProvider) ? world->worldProvider->worldType : 0;

    if (session != NULL && session->username[0] != '\0') {
        snprintf(((Entity *)self)->skinUrl, sizeof(((Entity *)self)->skinUrl),
                 "http://s3.amazonaws.com/MinecraftSkins/%s.png", session->username);
    }
    if (session != NULL) {
        strncpy(self->base.username, session->username, sizeof(self->base.username) - 1);
        self->base.username[sizeof(self->base.username) - 1] = '\0';
    }

    memset(&self->field_21903_bJ, 0, sizeof(MouseFilter));
    memset(&self->field_21904_bK, 0, sizeof(MouseFilter));
    memset(&self->field_21902_bL, 0, sizeof(MouseFilter));

    if (mc && mc->gameSettings)
        self->movementInput = (MovementInput *)MovementInputFromOptions_create(mc->gameSettings);
}

EntityPlayerSP *EntityPlayerSP_create(World *world, struct Minecraft *mc, Session *session,
                                      PlayerController *controller) {
    EntityPlayerSP *self = (EntityPlayerSP *)calloc(1, sizeof(EntityPlayerSP));
    EntityPlayerSP_construct(self, world, mc, session, controller);
    return self;
}

void EntityPlayerSP_turn(EntityPlayerSP *self, float par1, float par2) {
    Entity *entity = (Entity *)self;
    float f = entity->rotationPitch;
    float f1 = entity->rotationYaw;
    entity->rotationYaw += par1 * 0.15f;
    entity->rotationPitch -= par2 * 0.15f;
    if (entity->rotationPitch < -90.0f)
        entity->rotationPitch = -90.0f;
    if (entity->rotationPitch > 90.0f)
        entity->rotationPitch = 90.0f;
    entity->prevRotationPitch += entity->rotationPitch - f;
    entity->prevRotationYaw += entity->rotationYaw - f1;
}

void EntityPlayerSP_updatePlayerMoveState(EntityPlayerSP *self) {
    if (!self->movementInput)
        return;
    self->movementInput->vtable->updatePlayerMoveState(self->movementInput);
    EntityLiving *living = (EntityLiving *)self;
    living->moveForward = self->movementInput->moveForward;
    living->moveStrafing = self->movementInput->moveStrafe;
    living->isJumping = self->movementInput->jump;
}

void EntityPlayerSP_resetPlayerKeyState(EntityPlayerSP *self) {
    if (self->movementInput) {
        self->movementInput->vtable->resetKeyState(self->movementInput);
    }
}

void EntityPlayerSP_handleKeyPress(EntityPlayerSP *self, int var1, int var2) {
    if (self->movementInput) {
        self->movementInput->vtable->checkKeyForMovementInput(self->movementInput, var1, var2);
    }
}
