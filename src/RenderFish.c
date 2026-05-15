#include "Renderer.h"

#include "RenderFish.h"
#include "Render.h"
#include "EntityLiving.h"
#include "MathHelper.h"
#include "Tessellator.h"
#include "Vec3D.h"
#include "RenderManager.h"
#include <math.h>
#include <stdlib.h>

#ifndef R_RESCALE_NORMAL
#define R_RESCALE_NORMAL 0x803A
#endif

static void func_4011_a(RenderFish *self, EntityFish *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    (void)var8;
    R_pushMatrix();
    R_translatef((float)var2, (float)var4, (float)var6);
    R_enable(R_RESCALE_NORMAL);
    R_scalef(0.5f, 0.5f, 0.5f);
    int var10 = 1;
    int var11 = 2;
    Render_loadTexture(&self->base, "/particles.png");
    Tessellator *var12 = &Tessellator_instance;
    float var13 = (float)(var10 * 8 + 0) / 128.0f;
    float var14 = (float)(var10 * 8 + 8) / 128.0f;
    float var15 = (float)(var11 * 8 + 0) / 128.0f;
    float var16 = (float)(var11 * 8 + 8) / 128.0f;
    float var17 = 1.0f;
    float var18 = 0.5f;
    float var19 = 0.5f;
    R_rotatef(180.0f - self->base.renderManager->playerViewY, 0.0f, 1.0f, 0.0f);
    R_rotatef(-self->base.renderManager->playerViewX, 1.0f, 0.0f, 0.0f);
    Tessellator_startDrawingQuads(var12);
    Tessellator_setNormal(var12, 0.0f, 1.0f, 0.0f);
    Tessellator_addVertexWithUV(var12, (double)(0.0f - var18), (double)(0.0f - var19), 0.0,
                                (double)var13, (double)var16);
    Tessellator_addVertexWithUV(var12, (double)(var17 - var18), (double)(0.0f - var19), 0.0,
                                (double)var14, (double)var16);
    Tessellator_addVertexWithUV(var12, (double)(var17 - var18), (double)(1.0f - var19), 0.0,
                                (double)var14, (double)var15);
    Tessellator_addVertexWithUV(var12, (double)(0.0f - var18), (double)(1.0f - var19), 0.0,
                                (double)var13, (double)var15);
    Tessellator_draw(var12);
    R_disable(R_RESCALE_NORMAL);
    R_popMatrix();
    if (var1->angler != NULL) {
        float var20 =
            (var1->angler->base.base.prevRotationYaw +
             (var1->angler->base.base.rotationYaw - var1->angler->base.base.prevRotationYaw) *
                 var9) *
            (float)M_PI / 180.0f;
        double var21 = (double)MathHelper_sin(var20);
        double var23 = (double)MathHelper_cos(var20);
        float var25 = EntityLiving_getSwingProgress(&var1->angler->base, var9);
        float var26 = MathHelper_sin(MathHelper_sqrt_float(var25) * (float)M_PI);
        Vec3D *var27 = Vec3D_createVector(-0.5, 0.03, 0.8);
        Vec3D_rotateAroundX(var27, -(var1->angler->base.base.prevRotationPitch +
                                     (var1->angler->base.base.rotationPitch -
                                      var1->angler->base.base.prevRotationPitch) *
                                         var9) *
                                       (float)M_PI / 180.0f);
        Vec3D_rotateAroundY(var27, -(var1->angler->base.base.prevRotationYaw +
                                     (var1->angler->base.base.rotationYaw -
                                      var1->angler->base.base.prevRotationYaw) *
                                         var9) *
                                       (float)M_PI / 180.0f);
        Vec3D_rotateAroundY(var27, var26 * 0.5f);
        Vec3D_rotateAroundX(var27, -var26 * 0.7f);
        double var28 =
            var1->angler->base.base.prevPosX +
            (var1->angler->base.base.posX - var1->angler->base.base.prevPosX) * (double)var9 +
            var27->xCoord;
        double var30 =
            var1->angler->base.base.prevPosY +
            (var1->angler->base.base.posY - var1->angler->base.base.prevPosY) * (double)var9 +
            var27->yCoord;
        double var32 =
            var1->angler->base.base.prevPosZ +
            (var1->angler->base.base.posZ - var1->angler->base.base.prevPosZ) * (double)var9 +
            var27->zCoord;
        if (self->base.renderManager->options->thirdPersonView) {
            var20 = (var1->angler->base.prevRenderYawOffset +
                     (var1->angler->base.renderYawOffset - var1->angler->base.prevRenderYawOffset) *
                         var9) *
                    (float)M_PI / 180.0f;
            var21 = (double)MathHelper_sin(var20);
            var23 = (double)MathHelper_cos(var20);
            var28 =
                var1->angler->base.base.prevPosX +
                (var1->angler->base.base.posX - var1->angler->base.base.prevPosX) * (double)var9 -
                var23 * 0.35 - var21 * 0.85;
            var30 =
                var1->angler->base.base.prevPosY +
                (var1->angler->base.base.posY - var1->angler->base.base.prevPosY) * (double)var9 -
                0.45;
            var32 =
                var1->angler->base.base.prevPosZ +
                (var1->angler->base.base.posZ - var1->angler->base.base.prevPosZ) * (double)var9 -
                var21 * 0.35 + var23 * 0.85;
        }
        double var34 = var1->base.prevPosX + (var1->base.posX - var1->base.prevPosX) * (double)var9;
        double var36 =
            var1->base.prevPosY + (var1->base.posY - var1->base.prevPosY) * (double)var9 + 0.25;
        double var38 = var1->base.prevPosZ + (var1->base.posZ - var1->base.prevPosZ) * (double)var9;
        double var40 = (double)((float)(var28 - var34));
        double var42 = (double)((float)(var30 - var36));
        double var44 = (double)((float)(var32 - var38));
        R_disable(R_TEXTURE_2D);
        R_disable(R_LIGHTING);
        Tessellator_startDrawing(var12, 3);
        Tessellator_setColorOpaque_I(var12, 0);
        int var46 = 16;
        for (int var47 = 0; var47 <= var46; ++var47) {
            float var48 = (float)var47 / (float)var46;
            Tessellator_addVertex(var12, var2 + var40 * (double)var48,
                                  var4 + var42 * (double)(var48 * var48 + var48) * 0.5 + 0.25,
                                  var6 + var44 * (double)var48);
        }
        Tessellator_draw(var12);
        R_enable(R_LIGHTING);
        R_enable(R_TEXTURE_2D);
    }
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    func_4011_a((RenderFish *)base, (EntityFish *)var1, var2, var4, var6, var8, var9);
}

static const Render_vtable s_vtable = {.doRender = vt_doRender};

RenderFish *RenderFish_create(void) {
    RenderFish *self = (RenderFish *)calloc(1, sizeof(RenderFish));
    self->base.vtable = &s_vtable;
    Render_init(&self->base);
    return self;
}
