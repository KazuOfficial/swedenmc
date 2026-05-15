#include "EntityOtherPlayerMP.h"
#include "MathHelper.h"
#include "ItemStack.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

static EntityPlayerVtable EntityOtherPlayerMP_vtable;
static int EntityOtherPlayerMP_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityOtherPlayerMP_vtable_init)
        return;
    EntityPlayer_initVtable(&EntityOtherPlayerMP_vtable);
    EntityOtherPlayerMP_vtable.base.base.attackEntityFrom = EntityOtherPlayerMP_attackEntityFrom;
    EntityOtherPlayerMP_vtable.base.base.setPositionAndRotation2 =
        EntityOtherPlayerMP_setPositionAndRotation2;
    EntityOtherPlayerMP_vtable.base.base.onUpdate = EntityOtherPlayerMP_onUpdate;
    EntityOtherPlayerMP_vtable.base.base.getShadowSize = EntityOtherPlayerMP_getShadowSize;
    EntityOtherPlayerMP_vtable.base.onLivingUpdate = EntityOtherPlayerMP_onLivingUpdate;
    EntityOtherPlayerMP_vtable.resetHeight = EntityOtherPlayerMP_resetHeight;
    EntityOtherPlayerMP_vtable_init = 1;
}

void EntityOtherPlayerMP_construct(EntityOtherPlayerMP *self, World *var1, const char *var2) {
    ensure_vtable();
    EntityPlayer_construct(&self->base, var1);
    self->base.base.base.vtable = (const EntityVtable *)&EntityOtherPlayerMP_vtable;
    self->field_20924_a = 0.0F;
    if (var2 != NULL) {
        strncpy(self->base.username, var2, sizeof(self->base.username) - 1);
        self->base.username[sizeof(self->base.username) - 1] = '\0';
    } else {
        self->base.username[0] = '\0';
    }
    self->base.base.base.yOffset = 0.0F;
    self->base.base.base.stepHeight = 0.0F;
    if (var2 != NULL && var2[0] != '\0') {

        snprintf(self->base.base.base.skinUrl, sizeof(self->base.base.base.skinUrl),
                 "http://s3.amazonaws.com/MinecraftSkins/%s.png", var2);
    }
    self->base.base.base.noClip = 1;
    self->base.field_22062_y = 0.25F;
    self->base.base.base.renderDistanceWeight = 10.0;
}

EntityOtherPlayerMP *EntityOtherPlayerMP_create(World *var1, const char *var2) {
    EntityOtherPlayerMP *self = (EntityOtherPlayerMP *)calloc(1, sizeof(EntityOtherPlayerMP));
    EntityOtherPlayerMP_construct(self, var1, var2);
    return self;
}

void EntityOtherPlayerMP_resetHeight(EntityPlayer *selfP) { selfP->base.base.yOffset = 0.0F; }

int EntityOtherPlayerMP_attackEntityFrom(Entity *self, Entity *var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return 1;
}

void EntityOtherPlayerMP_setPositionAndRotation2(Entity *selfE, double var1, double var3,
                                                 double var5, float var7, float var8, int var9) {
    EntityOtherPlayerMP *self = (EntityOtherPlayerMP *)selfE;
    self->field_784_bh = var1;
    self->field_783_bi = var3;
    self->field_782_bj = var5;
    self->field_780_bk = (double)var7;
    self->field_786_bl = (double)var8;
    self->field_785_bg = var9;
}

void EntityOtherPlayerMP_onUpdate(Entity *selfE) {
    EntityOtherPlayerMP *self = (EntityOtherPlayerMP *)selfE;
    self->base.field_22062_y = 0.0F;

    EntityPlayer_defaultVtable.base.base.onUpdate(selfE);
    self->base.base.field_705_Q = self->base.base.field_704_R;
    double var1 = selfE->posX - selfE->prevPosX;
    double var3 = selfE->posZ - selfE->prevPosZ;
    float var5 = MathHelper_sqrt_double(var1 * var1 + var3 * var3) * 4.0F;
    if (var5 > 1.0F) {
        var5 = 1.0F;
    }
    self->base.base.field_704_R += (var5 - self->base.base.field_704_R) * 0.4F;
    self->base.base.field_703_S += self->base.base.field_704_R;
}

float EntityOtherPlayerMP_getShadowSize(Entity *self) {
    (void)self;
    return 0.0F;
}

void EntityOtherPlayerMP_onLivingUpdate(EntityLiving *selfL) {
    EntityOtherPlayerMP *self = (EntityOtherPlayerMP *)selfL;
    Entity *selfE = &selfL->base;

    EntityPlayer_defaultVtable.base.updatePlayerActionState(selfL);
    if (self->field_785_bg > 0) {
        double var1 = selfE->posX + (self->field_784_bh - selfE->posX) / (double)self->field_785_bg;
        double var3 = selfE->posY + (self->field_783_bi - selfE->posY) / (double)self->field_785_bg;
        double var5 = selfE->posZ + (self->field_782_bj - selfE->posZ) / (double)self->field_785_bg;
        double var7;
        for (var7 = self->field_780_bk - (double)selfE->rotationYaw; var7 < -180.0; var7 += 360.0) {
        }
        while (var7 >= 180.0) {
            var7 -= 360.0;
        }
        selfE->rotationYaw =
            (float)((double)selfE->rotationYaw + var7 / (double)self->field_785_bg);
        selfE->rotationPitch = (float)((double)selfE->rotationPitch +
                                       (self->field_786_bl - (double)selfE->rotationPitch) /
                                           (double)self->field_785_bg);
        --self->field_785_bg;
        selfE->vtable->setPosition(selfE, var1, var3, var5);
        Entity_setRotation(selfE, selfE->rotationYaw, selfE->rotationPitch);
    }
    self->base.field_775_e = self->base.field_774_f;
    float var9 =
        MathHelper_sqrt_double(selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ);
    float var2 = (float)atan(-selfE->motionY * (double)0.2F) * 15.0F;
    if (var9 > 0.1F) {
        var9 = 0.1F;
    }
    if (!selfE->onGround || selfL->health <= 0) {
        var9 = 0.0F;
    }
    if (selfE->onGround || selfL->health <= 0) {
        var2 = 0.0F;
    }
    self->base.field_774_f += (var9 - self->base.field_774_f) * 0.4F;
    selfL->field_9328_R += (var2 - selfL->field_9328_R) * 0.8F;
}

void EntityOtherPlayerMP_outfitWithItem(EntityOtherPlayerMP *self, int var1, int var2, int var3) {
    ItemStack *var4 = NULL;
    if (var2 >= 0) {
        var4 = ItemStack_new(var2, 1, var3);
    }
    if (var1 == 0) {
        self->base.inventory.mainInventory[self->base.inventory.currentItem] = var4;
    } else {
        self->base.inventory.armorInventory[var1 - 1] = var4;
    }
}

void EntityOtherPlayerMP_func_6420_o(EntityOtherPlayerMP *self) { (void)self; }
