#include "Renderer.h"

#include "RenderMinecart.h"
#include "Render.h"
#include "Block.h"
#include "MathHelper.h"
#include "RenderBlocks.h"
#include "Vec3D.h"
#include <math.h>
#include <stdlib.h>

static void func_152_a(RenderMinecart *self, EntityMinecart *var1, double var2, double var4,
                       double var6, float var8, float var9) {
    R_pushMatrix();
    double var10 =
        var1->base.lastTickPosX + (var1->base.posX - var1->base.lastTickPosX) * (double)var9;
    double var12 =
        var1->base.lastTickPosY + (var1->base.posY - var1->base.lastTickPosY) * (double)var9;
    double var14 =
        var1->base.lastTickPosZ + (var1->base.posZ - var1->base.lastTickPosZ) * (double)var9;
    double var16 = (double)0.3f;
    Vec3D *var18 = EntityMinecart_func_514_g(var1, var10, var12, var14);
    float var19 = var1->base.prevRotationPitch +
                  (var1->base.rotationPitch - var1->base.prevRotationPitch) * var9;
    if (var18 != NULL) {
        Vec3D *var20 = EntityMinecart_func_515_a(var1, var10, var12, var14, var16);
        Vec3D *var21 = EntityMinecart_func_515_a(var1, var10, var12, var14, -var16);
        if (var20 == NULL) {
            var20 = var18;
        }
        if (var21 == NULL) {
            var21 = var18;
        }
        var2 += var18->xCoord - var10;
        var4 += (var20->yCoord + var21->yCoord) / 2.0 - var12;
        var6 += var18->zCoord - var14;
        Vec3D *var22 = Vec3D_addVector(var21, -var20->xCoord, -var20->yCoord, -var20->zCoord);
        if (Vec3D_lengthVector(var22) != 0.0) {
            var22 = Vec3D_normalize(var22);
            var8 = (float)(atan2(var22->zCoord, var22->xCoord) * 180.0 / M_PI);
            var19 = (float)(atan(var22->yCoord) * 73.0);
        }
    }
    R_translatef((float)var2, (float)var4, (float)var6);
    R_rotatef(180.0f - var8, 0.0f, 1.0f, 0.0f);
    R_rotatef(-var19, 0.0f, 0.0f, 1.0f);
    float var23 = (float)var1->minecartTimeSinceHit - var9;
    float var24 = (float)var1->minecartCurrentDamage - var9;
    if (var24 < 0.0f) {
        var24 = 0.0f;
    }
    if (var23 > 0.0f) {
        R_rotatef(MathHelper_sin(var23) * var23 * var24 / 10.0f *
                      (float)var1->minecartRockDirection,
                  1.0f, 0.0f, 0.0f);
    }
    if (var1->minecartType != 0) {
        Render_loadTexture(&self->base, "/terrain.png");
        float var25 = 12.0f / 16.0f;
        R_scalef(var25, var25, var25);
        R_translatef(0.0f, 5.0f / 16.0f, 0.0f);
        R_rotatef(90.0f, 0.0f, 1.0f, 0.0f);
        if (var1->minecartType == 1) {
            RenderBlocks tmpRB;
            RenderBlocks_init(&tmpRB, NULL);
            RenderBlocks_renderBlockOnInventory(
                &tmpRB, Block_chest, 0, var1->base.vtable->getEntityBrightness(&var1->base, var9));
        } else if (var1->minecartType == 2) {
            RenderBlocks tmpRB;
            RenderBlocks_init(&tmpRB, NULL);
            RenderBlocks_renderBlockOnInventory(
                &tmpRB, Block_stoneOvenIdle, 0,
                var1->base.vtable->getEntityBrightness(&var1->base, var9));
        }
        R_rotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        R_translatef(0.0f, -(5.0f / 16.0f), 0.0f);
        R_scalef(1.0f / var25, 1.0f / var25, 1.0f / var25);
    }
    Render_loadTexture(&self->base, "/item/cart.png");
    R_scalef(-1.0f, -1.0f, 1.0f);
    self->modelMinecart->vtable->render(self->modelMinecart, 0.0f, 0.0f, -0.1f, 0.0f, 0.0f,
                                        1.0f / 16.0f);
    R_popMatrix();
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    func_152_a((RenderMinecart *)base, (EntityMinecart *)var1, var2, var4, var6, var8, var9);
}

static const Render_vtable s_vtable = {.doRender = vt_doRender};

RenderMinecart *RenderMinecart_create(void) {
    RenderMinecart *self = (RenderMinecart *)calloc(1, sizeof(RenderMinecart));
    self->base.vtable = &s_vtable;
    Render_init(&self->base);
    self->base.shadowSize = 0.5f;
    ModelMinecart *mm = (ModelMinecart *)calloc(1, sizeof(ModelMinecart));
    ModelMinecart_init(mm);
    self->modelMinecart = (ModelBase *)mm;
    return self;
}
