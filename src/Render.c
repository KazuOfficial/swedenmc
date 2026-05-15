#include "Renderer.h"

#include "Render.h"
#include "Entity.h"
#include "Block.h"
#include "Tessellator.h"
#include "RenderEngine.h"
#include "MathHelper.h"
#include "World.h"
#include "AxisAlignedBB.h"
#include "ModelBiped.h"
#include "ModelRenderer.h"

struct FontRenderer;
#include <stdlib.h>
#include <math.h>

void Render_init(Render *self) {
    ModelBiped *biped = (ModelBiped *)calloc(1, sizeof(ModelBiped));
    ModelBiped_init0(biped);
    self->modelBase = (ModelBase *)biped;

    RenderBlocks *rb = (RenderBlocks *)calloc(1, sizeof(RenderBlocks));
    RenderBlocks_init(rb, NULL);
    self->renderBlocks = rb;

    self->shadowSize = 0.0f;
    self->field_194_c = 1.0f;
}

void Render_loadTexture(Render *self, const char *var1) {
    RenderEngine *var2 = self->renderManager->renderEngine;
    RenderEngine_bindTexture(var2, RenderEngine_getTexture(var2, var1));
}

int Render_loadDownloadableImageTexture(Render *self, const char *var1, const char *var2) {
    RenderEngine *var3 = self->renderManager->renderEngine;
    int var4 = RenderEngine_getTextureForDownloadableImage(var3, var1, var2);
    if (var4 >= 0) {
        RenderEngine_bindTexture(var3, var4);
        return 1;
    } else {
        return 0;
    }
}

static World *Render_getWorldFromRenderManager(Render *self) {
    return self->renderManager->worldObj;
}

static void Render_renderShadowOnBlock(Render *self, Block *var1, double var2, double var4,
                                       double var6, int var8, int var9, int var10, float var11,
                                       float var12, double var13, double var15, double var17) {
    Tessellator *var19 = &Tessellator_instance;
    if (Block_renderAsNormalBlock(var1)) {
        World *_w = Render_getWorldFromRenderManager(self);
        double var20 = ((double)var11 - (var4 - ((double)var9 + var15)) / 2.0) * 0.5 *
                       (double)_w->vtable->getLightBrightness(_w, var8, var9, var10);
        if (var20 >= 0.0) {
            if (var20 > 1.0) {
                var20 = 1.0;
            }
            Tessellator_setColorRGBA_F(var19, 1.0f, 1.0f, 1.0f, (float)var20);
            double var22 = (double)var8 + var1->minX + var13;
            double var24 = (double)var8 + var1->maxX + var13;
            double var26 = (double)var9 + var1->minY + var15 + 1.0 / 64.0;
            double var28 = (double)var10 + var1->minZ + var17;
            double var30 = (double)var10 + var1->maxZ + var17;
            float var32 = (float)((var2 - var22) / 2.0 / (double)var12 + 0.5);
            float var33 = (float)((var2 - var24) / 2.0 / (double)var12 + 0.5);
            float var34 = (float)((var6 - var28) / 2.0 / (double)var12 + 0.5);
            float var35 = (float)((var6 - var30) / 2.0 / (double)var12 + 0.5);
            Tessellator_addVertexWithUV(var19, var22, var26, var28, (double)var32, (double)var34);
            Tessellator_addVertexWithUV(var19, var22, var26, var30, (double)var32, (double)var35);
            Tessellator_addVertexWithUV(var19, var24, var26, var30, (double)var33, (double)var35);
            Tessellator_addVertexWithUV(var19, var24, var26, var28, (double)var33, (double)var34);
        }
    }
}

static void Render_renderShadow(Render *self, Entity *var1, double var2, double var4, double var6,
                                float var8, float var9) {
    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    RenderEngine *var10 = self->renderManager->renderEngine;
    RenderEngine_bindTexture(var10, RenderEngine_getTexture(var10, "%clamp%/misc/shadow.png"));
    World *var11 = Render_getWorldFromRenderManager(self);
    R_depthMask(R_FALSE);
    float var12 = self->shadowSize;
    double var13 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var9;
    double var15 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var9 +
                   (double)var1->vtable->getShadowSize(var1);
    double var17 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var9;
    int var19 = MathHelper_floor_double(var13 - (double)var12);
    int var20 = MathHelper_floor_double(var13 + (double)var12);
    int var21 = MathHelper_floor_double(var15 - (double)var12);
    int var22 = MathHelper_floor_double(var15);
    int var23 = MathHelper_floor_double(var17 - (double)var12);
    int var24 = MathHelper_floor_double(var17 + (double)var12);
    double var25 = var2 - var13;
    double var27 = var4 - var15;
    double var29 = var6 - var17;
    Tessellator *var31 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var31);

    for (int var32 = var19; var32 <= var20; ++var32) {
        for (int var33 = var21; var33 <= var22; ++var33) {
            for (int var34 = var23; var34 <= var24; ++var34) {
                int var35 = World_getBlockId(var11, var32, var33 - 1, var34);
                if (var35 > 0 && Block_blocksList[var35] &&
                    World_getBlockLightValue(var11, var32, var33, var34) > 3) {
                    Render_renderShadowOnBlock(self, Block_blocksList[var35], var2,
                                               var4 + (double)var1->vtable->getShadowSize(var1),
                                               var6, var32, var33, var34, var8, var12, var25,
                                               var27 + (double)var1->vtable->getShadowSize(var1),
                                               var29);
                }
            }
        }
    }

    Tessellator_draw(var31);
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    R_disable(R_BLEND);
    R_depthMask(R_TRUE);
}

static void Render_renderEntityOnFire(Render *self, Entity *var1, double var2, double var4,
                                      double var6, float var8) {
    (void)var8;
    R_disable(R_LIGHTING);
    int var9 = Block_fire->blockIndexInTexture;
    int var10 = (var9 & 15) << 4;
    int var11 = var9 & 240;
    float var12 = (float)var10 / 256.0f;
    float var13 = ((float)var10 + 15.99f) / 256.0f;
    float var14 = (float)var11 / 256.0f;
    float var15 = ((float)var11 + 15.99f) / 256.0f;
    R_pushMatrix();
    R_translatef((float)var2, (float)var4, (float)var6);
    float var16 = var1->width * 1.4f;
    R_scalef(var16, var16, var16);
    Render_loadTexture(self, "/terrain.png");
    Tessellator *var17 = &Tessellator_instance;
    float var18 = 0.5f;
    float var19 = 0.0f;
    float var20 = var1->height / var16;
    float var21 = (float)(var1->posY - var1->boundingBox.minY);
    R_rotatef(-self->renderManager->playerViewY, 0.0f, 1.0f, 0.0f);
    R_translatef(0.0f, 0.0f, -0.3f + (float)((int)var20) * 0.02f);
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    float var22 = 0.0f;
    int var23 = 0;
    Tessellator_startDrawingQuads(var17);

    while (var20 > 0.0f) {
        if (var23 % 2 == 0) {
            var12 = (float)var10 / 256.0f;
            var13 = ((float)var10 + 15.99f) / 256.0f;
            var14 = (float)var11 / 256.0f;
            var15 = ((float)var11 + 15.99f) / 256.0f;
        } else {
            var12 = (float)var10 / 256.0f;
            var13 = ((float)var10 + 15.99f) / 256.0f;
            var14 = (float)(var11 + 16) / 256.0f;
            var15 = ((float)(var11 + 16) + 15.99f) / 256.0f;
        }

        if (var23 / 2 % 2 == 0) {
            float var24 = var13;
            var13 = var12;
            var12 = var24;
        }

        Tessellator_addVertexWithUV(var17, (double)(var18 - var19), (double)(0.0f - var21),
                                    (double)var22, (double)var13, (double)var15);
        Tessellator_addVertexWithUV(var17, (double)(-var18 - var19), (double)(0.0f - var21),
                                    (double)var22, (double)var12, (double)var15);
        Tessellator_addVertexWithUV(var17, (double)(-var18 - var19), (double)(1.4f - var21),
                                    (double)var22, (double)var12, (double)var14);
        Tessellator_addVertexWithUV(var17, (double)(var18 - var19), (double)(1.4f - var21),
                                    (double)var22, (double)var13, (double)var14);
        var20 -= 0.45f;
        var21 -= 0.45f;
        var18 *= 0.9f;
        var22 += 0.03f;
        ++var23;
    }

    Tessellator_draw(var17);
    R_popMatrix();
    R_enable(R_LIGHTING);
}

void Render_renderOffsetAABB(AxisAlignedBB *var0, double var1, double var3, double var5) {
    R_disable(R_TEXTURE_2D);
    Tessellator *var7 = &Tessellator_instance;
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    Tessellator_startDrawingQuads(var7);
    Tessellator_setTranslationD(var7, var1, var3, var5);
    Tessellator_setNormal(var7, 0.0f, 0.0f, -1.0f);
    Tessellator_addVertex(var7, var0->minX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var7, var0->maxX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var7, var0->maxX, var0->minY, var0->minZ);
    Tessellator_addVertex(var7, var0->minX, var0->minY, var0->minZ);
    Tessellator_setNormal(var7, 0.0f, 0.0f, 1.0f);
    Tessellator_addVertex(var7, var0->minX, var0->minY, var0->maxZ);
    Tessellator_addVertex(var7, var0->maxX, var0->minY, var0->maxZ);
    Tessellator_addVertex(var7, var0->maxX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var7, var0->minX, var0->maxY, var0->maxZ);
    Tessellator_setNormal(var7, 0.0f, -1.0f, 0.0f);
    Tessellator_addVertex(var7, var0->minX, var0->minY, var0->minZ);
    Tessellator_addVertex(var7, var0->maxX, var0->minY, var0->minZ);
    Tessellator_addVertex(var7, var0->maxX, var0->minY, var0->maxZ);
    Tessellator_addVertex(var7, var0->minX, var0->minY, var0->maxZ);
    Tessellator_setNormal(var7, 0.0f, 1.0f, 0.0f);
    Tessellator_addVertex(var7, var0->minX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var7, var0->maxX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var7, var0->maxX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var7, var0->minX, var0->maxY, var0->minZ);
    Tessellator_setNormal(var7, -1.0f, 0.0f, 0.0f);
    Tessellator_addVertex(var7, var0->minX, var0->minY, var0->maxZ);
    Tessellator_addVertex(var7, var0->minX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var7, var0->minX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var7, var0->minX, var0->minY, var0->minZ);
    Tessellator_setNormal(var7, 1.0f, 0.0f, 0.0f);
    Tessellator_addVertex(var7, var0->maxX, var0->minY, var0->minZ);
    Tessellator_addVertex(var7, var0->maxX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var7, var0->maxX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var7, var0->maxX, var0->minY, var0->maxZ);
    Tessellator_setTranslationD(var7, 0.0, 0.0, 0.0);
    Tessellator_draw(var7);
    R_enable(R_TEXTURE_2D);
}

void Render_renderAABB(AxisAlignedBB *var0) {
    Tessellator *var1 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var1);
    Tessellator_addVertex(var1, var0->minX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var1, var0->maxX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var1, var0->maxX, var0->minY, var0->minZ);
    Tessellator_addVertex(var1, var0->minX, var0->minY, var0->minZ);
    Tessellator_addVertex(var1, var0->minX, var0->minY, var0->maxZ);
    Tessellator_addVertex(var1, var0->maxX, var0->minY, var0->maxZ);
    Tessellator_addVertex(var1, var0->maxX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var1, var0->minX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var1, var0->minX, var0->minY, var0->minZ);
    Tessellator_addVertex(var1, var0->maxX, var0->minY, var0->minZ);
    Tessellator_addVertex(var1, var0->maxX, var0->minY, var0->maxZ);
    Tessellator_addVertex(var1, var0->minX, var0->minY, var0->maxZ);
    Tessellator_addVertex(var1, var0->minX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var1, var0->maxX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var1, var0->maxX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var1, var0->minX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var1, var0->minX, var0->minY, var0->maxZ);
    Tessellator_addVertex(var1, var0->minX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var1, var0->minX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var1, var0->minX, var0->minY, var0->minZ);
    Tessellator_addVertex(var1, var0->maxX, var0->minY, var0->minZ);
    Tessellator_addVertex(var1, var0->maxX, var0->maxY, var0->minZ);
    Tessellator_addVertex(var1, var0->maxX, var0->maxY, var0->maxZ);
    Tessellator_addVertex(var1, var0->maxX, var0->minY, var0->maxZ);
    Tessellator_draw(var1);
}

void Render_setRenderManager(Render *self, RenderManager *var1) { self->renderManager = var1; }

void Render_doRenderShadowAndFire(Render *self, Entity *var1, double var2, double var4, double var6,
                                  float var8, float var9) {
    (void)var8;
    if (self->renderManager->options->fancyGraphics && self->shadowSize > 0.0f) {
        double var10 =
            RenderManager_func_851_a(self->renderManager, var1->posX, var1->posY, var1->posZ);
        float var12 = (float)((1.0 - var10 / 256.0) * (double)self->field_194_c);
        if (var12 > 0.0f) {
            Render_renderShadow(self, var1, var2, var4, var6, var12, var9);
        }
    }

    if (Entity_isBurning(var1)) {
        Render_renderEntityOnFire(self, var1, var2, var4, var6, var9);
    }
}

struct FontRenderer *Render_getFontRendererFromRenderManager(Render *self) {
    return RenderManager_getFontRenderer(self->renderManager);
}

void Render_destroy(Render *self) {
    if (self->modelBase) {
        ModelBiped_destroy((ModelBiped *)self->modelBase);
        free(self->modelBase);
        self->modelBase = NULL;
    }
    if (self->renderBlocks) {
        free(self->renderBlocks);
        self->renderBlocks = NULL;
    }
}
