#include "Renderer.h"

#include "RenderItem.h"
#include "Render.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"
#include "Tessellator.h"
#include "RenderBlocks.h"
#include "RenderManager.h"
#include "FontRenderer.h"
#include "RenderEngine.h"
#include "MathHelper.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef R_RESCALE_NORMAL
#define R_RESCALE_NORMAL 0x803A
#endif

static void renderQuad(RenderItem *self, Tessellator *var1, int var2, int var3, int var4, int var5,
                       int var6) {
    (void)self;
    Tessellator_startDrawingQuads(var1);
    Tessellator_setColorOpaque_I(var1, var6);
    Tessellator_addVertex(var1, (double)(var2 + 0), (double)(var3 + 0), 0.0);
    Tessellator_addVertex(var1, (double)(var2 + 0), (double)(var3 + var5), 0.0);
    Tessellator_addVertex(var1, (double)(var2 + var4), (double)(var3 + var5), 0.0);
    Tessellator_addVertex(var1, (double)(var2 + var4), (double)(var3 + 0), 0.0);
    Tessellator_draw(var1);
}

void RenderItem_renderTexturedQuad(RenderItem *self, int var1, int var2, int var3, int var4,
                                   int var5, int var6) {
    (void)self;
    float var7 = 0.0f;
    float var8 = 0.00390625f;
    float var9 = 0.00390625f;
    Tessellator *var10 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var10);
    Tessellator_addVertexWithUV(var10, (double)(var1 + 0), (double)(var2 + var6), (double)var7,
                                (double)((float)(var3 + 0) * var8),
                                (double)((float)(var4 + var6) * var9));
    Tessellator_addVertexWithUV(var10, (double)(var1 + var5), (double)(var2 + var6), (double)var7,
                                (double)((float)(var3 + var5) * var8),
                                (double)((float)(var4 + var6) * var9));
    Tessellator_addVertexWithUV(var10, (double)(var1 + var5), (double)(var2 + 0), (double)var7,
                                (double)((float)(var3 + var5) * var8),
                                (double)((float)(var4 + 0) * var9));
    Tessellator_addVertexWithUV(var10, (double)(var1 + 0), (double)(var2 + 0), (double)var7,
                                (double)((float)(var3 + 0) * var8),
                                (double)((float)(var4 + 0) * var9));
    Tessellator_draw(var10);
}

void RenderItem_doRenderItem(RenderItem *self, EntityItem *var1, double var2, double var4,
                             double var6, float var8, float var9) {
    (void)var8;
    JavaRandom_setSeed(&self->random, 187L);
    ItemStack *var10 = &var1->item;
    R_pushMatrix();
    float var11 =
        MathHelper_sin(((float)var1->age + var9) / 10.0f + var1->field_804_d) * 0.1f + 0.1f;
    float var12 = (((float)var1->age + var9) / 20.0f + var1->field_804_d) * (180.0f / (float)M_PI);
    int var13 = 1;
    if (var1->item.stackSize > 1) {
        var13 = 2;
    }
    if (var1->item.stackSize > 5) {
        var13 = 3;
    }
    if (var1->item.stackSize > 20) {
        var13 = 4;
    }
    R_translatef((float)var2, (float)var4 + var11, (float)var6);
    R_enable(R_RESCALE_NORMAL);
    float var16, var17, var18;
    if (var10->itemID < 256 && Block_blocksList[var10->itemID] &&
        RenderBlocks_renderItemIn3d(Block_getRenderType(Block_blocksList[var10->itemID]))) {
        R_rotatef(var12, 0.0f, 1.0f, 0.0f);
        Render_loadTexture(&self->base, "/terrain.png");
        float var28 = 0.25f;
        if (!Block_renderAsNormalBlock(Block_blocksList[var10->itemID]) &&
            var10->itemID != Block_stairSingle->blockID &&
            Block_getRenderType(Block_blocksList[var10->itemID]) != 16) {
            var28 = 0.5f;
        }
        R_scalef(var28, var28, var28);
        for (int var29 = 0; var29 < var13; ++var29) {
            R_pushMatrix();
            if (var29 > 0) {
                var16 = (JavaRandom_nextFloat(&self->random) * 2.0f - 1.0f) * 0.2f / var28;
                var17 = (JavaRandom_nextFloat(&self->random) * 2.0f - 1.0f) * 0.2f / var28;
                var18 = (JavaRandom_nextFloat(&self->random) * 2.0f - 1.0f) * 0.2f / var28;
                R_translatef(var16, var17, var18);
            }
            RenderBlocks_renderBlockOnInventory(
                self->renderBlocks, Block_blocksList[var10->itemID], ItemStack_getItemDamage(var10),
                var1->base.vtable->getEntityBrightness(&var1->base, var9));
            R_popMatrix();
        }
    } else {
        R_scalef(0.5f, 0.5f, 0.5f);
        int var14 = ItemStack_getIconIndex(var10);
        if (var10->itemID < 256) {
            Render_loadTexture(&self->base, "/terrain.png");
        } else {
            Render_loadTexture(&self->base, "/gui/items.png");
        }
        Tessellator *var15 = &Tessellator_instance;
        var16 = (float)(var14 % 16 * 16 + 0) / 256.0f;
        var17 = (float)(var14 % 16 * 16 + 16) / 256.0f;
        var18 = (float)(var14 / 16 * 16 + 0) / 256.0f;
        float var19 = (float)(var14 / 16 * 16 + 16) / 256.0f;
        float var20 = 1.0f;
        float var21 = 0.5f;
        float var22 = 0.25f;
        int var23;
        float var24, var25, var26;
        if (self->field_27004_a) {
            var23 = Item_itemsList[var10->itemID]->vtable->getColorFromDamage(
                Item_itemsList[var10->itemID], ItemStack_getItemDamage(var10));
            var24 = (float)(var23 >> 16 & 255) / 255.0f;
            var25 = (float)(var23 >> 8 & 255) / 255.0f;
            var26 = (float)(var23 & 255) / 255.0f;
            float var27 = var1->base.vtable->getEntityBrightness(&var1->base, var9);
            R_color4f(var24 * var27, var25 * var27, var26 * var27, 1.0f);
        }
        for (var23 = 0; var23 < var13; ++var23) {
            R_pushMatrix();
            if (var23 > 0) {
                var24 = (JavaRandom_nextFloat(&self->random) * 2.0f - 1.0f) * 0.3f;
                var25 = (JavaRandom_nextFloat(&self->random) * 2.0f - 1.0f) * 0.3f;
                var26 = (JavaRandom_nextFloat(&self->random) * 2.0f - 1.0f) * 0.3f;
                R_translatef(var24, var25, var26);
            }
            R_rotatef(180.0f - self->base.renderManager->playerViewY, 0.0f, 1.0f, 0.0f);
            Tessellator_startDrawingQuads(var15);
            Tessellator_setNormal(var15, 0.0f, 1.0f, 0.0f);
            Tessellator_addVertexWithUV(var15, (double)(0.0f - var21), (double)(0.0f - var22), 0.0,
                                        (double)var16, (double)var19);
            Tessellator_addVertexWithUV(var15, (double)(var20 - var21), (double)(0.0f - var22), 0.0,
                                        (double)var17, (double)var19);
            Tessellator_addVertexWithUV(var15, (double)(var20 - var21), (double)(1.0f - var22), 0.0,
                                        (double)var17, (double)var18);
            Tessellator_addVertexWithUV(var15, (double)(0.0f - var21), (double)(1.0f - var22), 0.0,
                                        (double)var16, (double)var18);
            Tessellator_draw(var15);
            R_popMatrix();
        }
    }
    R_disable(R_RESCALE_NORMAL);
    R_popMatrix();
}

void RenderItem_drawItemIntoGui(RenderItem *self, FontRenderer *var1, RenderEngine *var2, int var3,
                                int var4, int var5, int var6, int var7) {
    (void)var1;
    float var11;
    if (var3 < 256 && Block_blocksList[var3] &&
        RenderBlocks_renderItemIn3d(Block_getRenderType(Block_blocksList[var3]))) {
        RenderEngine_bindTexture(var2, RenderEngine_getTexture(var2, "/terrain.png"));
        Block *var14 = Block_blocksList[var3];
        R_pushMatrix();
        R_translatef((float)(var6 - 2), (float)(var7 + 3), -3.0f);
        R_scalef(10.0f, 10.0f, 10.0f);
        R_translatef(1.0f, 0.5f, 1.0f);
        R_scalef(1.0f, 1.0f, -1.0f);
        R_rotatef(210.0f, 1.0f, 0.0f, 0.0f);
        R_rotatef(45.0f, 0.0f, 1.0f, 0.0f);
        int var15 = Item_itemsList[var3]->vtable->getColorFromDamage(Item_itemsList[var3], var4);
        var11 = (float)(var15 >> 16 & 255) / 255.0f;
        float var12 = (float)(var15 >> 8 & 255) / 255.0f;
        float var13 = (float)(var15 & 255) / 255.0f;
        if (self->field_27004_a) {
            R_color4f(var11, var12, var13, 1.0f);
        }
        R_rotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        self->renderBlocks->field_31088_b = self->field_27004_a;
        RenderBlocks_renderBlockOnInventory(self->renderBlocks, var14, var4, 1.0f);
        self->renderBlocks->field_31088_b = 1;
        R_popMatrix();
    } else if (var5 >= 0) {
        R_disable(R_LIGHTING);
        if (var3 < 256) {
            RenderEngine_bindTexture(var2, RenderEngine_getTexture(var2, "/terrain.png"));
        } else {
            RenderEngine_bindTexture(var2, RenderEngine_getTexture(var2, "/gui/items.png"));
        }
        int var8 = Item_itemsList[var3]->vtable->getColorFromDamage(Item_itemsList[var3], var4);
        float var9 = (float)(var8 >> 16 & 255) / 255.0f;
        float var10 = (float)(var8 >> 8 & 255) / 255.0f;
        var11 = (float)(var8 & 255) / 255.0f;
        if (self->field_27004_a) {
            R_color4f(var9, var10, var11, 1.0f);
        }
        RenderItem_renderTexturedQuad(self, var6, var7, var5 % 16 * 16, var5 / 16 * 16, 16, 16);
        R_enable(R_LIGHTING);
    }
    R_enable(R_CULL_FACE);
}

void RenderItem_renderItemIntoGUI(RenderItem *self, FontRenderer *var1, RenderEngine *var2,
                                  ItemStack *var3, int var4, int var5) {
    if (var3 != NULL) {
        RenderItem_drawItemIntoGui(self, var1, var2, var3->itemID, ItemStack_getItemDamage(var3),
                                   ItemStack_getIconIndex(var3), var4, var5);
    }
}

void RenderItem_renderItemOverlayIntoGUI(RenderItem *self, FontRenderer *var1, RenderEngine *var2,
                                         ItemStack *var3, int var4, int var5) {
    (void)var2;
    if (var3 != NULL) {
        if (var3->stackSize > 1) {
            char var6[32];
            snprintf(var6, sizeof(var6), "%d", var3->stackSize);
            R_disable(R_LIGHTING);
            R_disable(R_DEPTH_TEST);
            FontRenderer_drawStringWithShadow(
                var1, var6, var4 + 19 - 2 - FontRenderer_getStringWidth(var1, var6), var5 + 6 + 3,
                16777215);
            R_enable(R_LIGHTING);
            R_enable(R_DEPTH_TEST);
        }
        if (ItemStack_isItemDamaged(var3)) {
            int var11 = (int)round(13.0 - (double)ItemStack_getItemDamageForDisplay(var3) * 13.0 /
                                              (double)ItemStack_getMaxDamage(var3));
            int var7 = (int)round(255.0 - (double)ItemStack_getItemDamageForDisplay(var3) * 255.0 /
                                              (double)ItemStack_getMaxDamage(var3));
            R_disable(R_LIGHTING);
            R_disable(R_DEPTH_TEST);
            R_disable(R_TEXTURE_2D);
            Tessellator *var8 = &Tessellator_instance;
            int var9 = (255 - var7) << 16 | var7 << 8;
            int var10 = (255 - var7) / 4 << 16 | 16128;
            renderQuad(self, var8, var4 + 2, var5 + 13, 13, 2, 0);
            renderQuad(self, var8, var4 + 2, var5 + 13, 12, 1, var10);
            renderQuad(self, var8, var4 + 2, var5 + 13, var11, 1, var9);
            R_enable(R_TEXTURE_2D);
            R_enable(R_LIGHTING);
            R_enable(R_DEPTH_TEST);
            R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    RenderItem_doRenderItem((RenderItem *)base, (EntityItem *)var1, var2, var4, var6, var8, var9);
}

static const Render_vtable s_vtable = {.doRender = vt_doRender};

RenderItem *RenderItem_create(void) {
    RenderItem *self = (RenderItem *)calloc(1, sizeof(RenderItem));
    self->base.vtable = &s_vtable;
    Render_init(&self->base);
    self->base.shadowSize = 0.15f;
    self->base.field_194_c = 12.0f / 16.0f;
    self->renderBlocks = (RenderBlocks *)calloc(1, sizeof(RenderBlocks));
    RenderBlocks_init(self->renderBlocks, NULL);
    JavaRandom_init(&self->random, 0);
    self->field_27004_a = 1;
    return self;
}
