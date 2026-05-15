#include "Renderer.h"
#include "ItemRenderer.h"
#include "EntityPlayerSP.h"
#include "Tessellator.h"
#include "RenderHelper.h"
#include "RenderManager.h"
#include "RenderPlayer.h"
#include "Block.h"
#include "Item.h"
#include "ItemMap.h"
#include "Material.h"
#include "MathHelper.h"
#include <math.h>

#ifndef R_RESCALE_NORMAL
#define R_RESCALE_NORMAL 0x803A
#endif

static void ItemRenderer_renderInsideOfBlock(ItemRenderer *self, float partialTick, int texIndex);
static void ItemRenderer_renderWarpedTextureOverlay(ItemRenderer *self, float partialTick);
static void ItemRenderer_renderFireInFirstPerson(ItemRenderer *self, float partialTick);

void ItemRenderer_init(ItemRenderer *self, Minecraft *mc) {
    self->mc = mc;
    self->itemToRender = NULL;
    self->equippedProgress = 0.0f;
    self->prevEquippedProgress = 0.0f;
    self->field_20099_f = -1;
    RenderBlocks_init(&self->renderBlocksInstance, NULL);
    MapItemRenderer_init(&self->field_28131_f, mc->fontRenderer, mc->gameSettings,
                         mc->renderEngine);
}

void ItemRenderer_renderItem(ItemRenderer *self, EntityLiving *entity, ItemStack *stack) {
    R_pushMatrix();

    Block *blk = (stack->itemID < 256) ? Block_blocksList[stack->itemID] : NULL;
    if (blk != NULL && RenderBlocks_renderItemIn3d(blk->vtable->getRenderType(blk))) {
        R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/terrain.png"));
        RenderBlocks_renderBlockOnInventory(
            &self->renderBlocksInstance, blk, ItemStack_getItemDamage(stack),
            ((Entity *)entity)->vtable->getEntityBrightness((Entity *)entity, 1.0f));
    } else {
        if (stack->itemID < 256)
            R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/terrain.png"));
        else
            R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/gui/items.png"));

        Tessellator *tess = &Tessellator_instance;
        int iconIdx =
            ((EntityLivingVtable *)((Entity *)entity)->vtable)->getItemIcon(entity, stack);
        float u0 = ((float)(iconIdx % 16 * 16) + 0.0f) / 256.0f;
        float u1 = ((float)(iconIdx % 16 * 16) + 15.99f) / 256.0f;
        float v0 = ((float)(iconIdx / 16 * 16) + 0.0f) / 256.0f;
        float v1 = ((float)(iconIdx / 16 * 16) + 15.99f) / 256.0f;
        float w = 1.0f;
        float depth = 1.0f / 16.0f;
        R_enable(R_RESCALE_NORMAL);
        R_translatef(0.0f, -0.3f, 0.0f);
        float scale = 1.5f;
        R_scalef(scale, scale, scale);
        R_rotatef(50.0f, 0.0f, 1.0f, 0.0f);
        R_rotatef(335.0f, 0.0f, 0.0f, 1.0f);
        R_translatef(-(15.0f / 16.0f), -(1.0f / 16.0f), 0.0f);

        Tessellator_startDrawingQuads(tess);
        Tessellator_setNormal(tess, 0.0f, 0.0f, 1.0f);
        Tessellator_addVertexWithUV(tess, 0.0, 0.0, 0.0, (double)u1, (double)v1);
        Tessellator_addVertexWithUV(tess, (double)w, 0.0, 0.0, (double)u0, (double)v1);
        Tessellator_addVertexWithUV(tess, (double)w, 1.0, 0.0, (double)u0, (double)v0);
        Tessellator_addVertexWithUV(tess, 0.0, 1.0, 0.0, (double)u1, (double)v0);
        Tessellator_draw(tess);

        Tessellator_startDrawingQuads(tess);
        Tessellator_setNormal(tess, 0.0f, 0.0f, -1.0f);
        Tessellator_addVertexWithUV(tess, 0.0, 1.0, -(double)depth, (double)u1, (double)v0);
        Tessellator_addVertexWithUV(tess, (double)w, 1.0, -(double)depth, (double)u0, (double)v0);
        Tessellator_addVertexWithUV(tess, (double)w, 0.0, -(double)depth, (double)u0, (double)v1);
        Tessellator_addVertexWithUV(tess, 0.0, 0.0, -(double)depth, (double)u1, (double)v1);
        Tessellator_draw(tess);

        Tessellator_startDrawingQuads(tess);
        Tessellator_setNormal(tess, -1.0f, 0.0f, 0.0f);
        for (int i = 0; i < 16; i++) {
            float t = (float)i / 16.0f;
            float u = u1 + (u0 - u1) * t - 0.001953125f;
            float xv = w * t;
            Tessellator_addVertexWithUV(tess, (double)xv, 0.0, -(double)depth, (double)u,
                                        (double)v1);
            Tessellator_addVertexWithUV(tess, (double)xv, 0.0, 0.0, (double)u, (double)v1);
            Tessellator_addVertexWithUV(tess, (double)xv, 1.0, 0.0, (double)u, (double)v0);
            Tessellator_addVertexWithUV(tess, (double)xv, 1.0, -(double)depth, (double)u,
                                        (double)v0);
        }
        Tessellator_draw(tess);

        Tessellator_startDrawingQuads(tess);
        Tessellator_setNormal(tess, 1.0f, 0.0f, 0.0f);
        for (int i = 0; i < 16; i++) {
            float t = (float)i / 16.0f;
            float u = u1 + (u0 - u1) * t - 0.001953125f;
            float xv = w * t + 1.0f / 16.0f;
            Tessellator_addVertexWithUV(tess, (double)xv, 1.0, -(double)depth, (double)u,
                                        (double)v0);
            Tessellator_addVertexWithUV(tess, (double)xv, 1.0, 0.0, (double)u, (double)v0);
            Tessellator_addVertexWithUV(tess, (double)xv, 0.0, 0.0, (double)u, (double)v1);
            Tessellator_addVertexWithUV(tess, (double)xv, 0.0, -(double)depth, (double)u,
                                        (double)v1);
        }
        Tessellator_draw(tess);

        Tessellator_startDrawingQuads(tess);
        Tessellator_setNormal(tess, 0.0f, 1.0f, 0.0f);
        for (int i = 0; i < 16; i++) {
            float t = (float)i / 16.0f;
            float v = v1 + (v0 - v1) * t - 0.001953125f;
            float yv = w * t + 1.0f / 16.0f;
            Tessellator_addVertexWithUV(tess, 0.0, (double)yv, 0.0, (double)u1, (double)v);
            Tessellator_addVertexWithUV(tess, (double)w, (double)yv, 0.0, (double)u0, (double)v);
            Tessellator_addVertexWithUV(tess, (double)w, (double)yv, -(double)depth, (double)u0,
                                        (double)v);
            Tessellator_addVertexWithUV(tess, 0.0, (double)yv, -(double)depth, (double)u1,
                                        (double)v);
        }
        Tessellator_draw(tess);

        Tessellator_startDrawingQuads(tess);
        Tessellator_setNormal(tess, 0.0f, -1.0f, 0.0f);
        for (int i = 0; i < 16; i++) {
            float t = (float)i / 16.0f;
            float v = v1 + (v0 - v1) * t - 0.001953125f;
            float yv = w * t;
            Tessellator_addVertexWithUV(tess, (double)w, (double)yv, 0.0, (double)u0, (double)v);
            Tessellator_addVertexWithUV(tess, 0.0, (double)yv, 0.0, (double)u1, (double)v);
            Tessellator_addVertexWithUV(tess, 0.0, (double)yv, -(double)depth, (double)u1,
                                        (double)v);
            Tessellator_addVertexWithUV(tess, (double)w, (double)yv, -(double)depth, (double)u0,
                                        (double)v);
        }
        Tessellator_draw(tess);
        R_disable(R_RESCALE_NORMAL);
    }

    R_popMatrix();
}

void ItemRenderer_renderItemInFirstPerson(ItemRenderer *self, float partialTick) {
    float equip = self->prevEquippedProgress +
                  (self->equippedProgress - self->prevEquippedProgress) * partialTick;
    EntityPlayerSP *player = self->mc->thePlayer;
    Entity *pe = (Entity *)player;
    float pitch = pe->prevRotationPitch + (pe->rotationPitch - pe->prevRotationPitch) * partialTick;

    R_pushMatrix();
    R_rotatef(pitch, 1.0f, 0.0f, 0.0f);
    R_rotatef(pe->prevRotationYaw + (pe->rotationYaw - pe->prevRotationYaw) * partialTick, 0.0f,
              1.0f, 0.0f);
    RenderHelper_enableStandardItemLighting();
    R_popMatrix();

    ItemStack *stack = self->itemToRender;

    int floorX = MathHelper_floor_double(pe->posX);
    int floorY = MathHelper_floor_double(pe->posY);
    int floorZ = MathHelper_floor_double(pe->posZ);
    float brightness =
        self->mc->theWorld->vtable->getLightBrightness(self->mc->theWorld, floorX, floorY, floorZ);

    if (stack != NULL) {
        Item *item = Item_itemsList[stack->itemID];
        int color = item->vtable->getColorFromDamage(item, ItemStack_getItemDamage(stack));
        float r = (float)((color >> 16) & 255) / 255.0f;
        float g = (float)((color >> 8) & 255) / 255.0f;
        float b = (float)(color & 255) / 255.0f;
        R_color4f(brightness * r, brightness * g, brightness * b, 1.0f);
    } else {
        R_color4f(brightness, brightness, brightness, 1.0f);
    }

    float sw, sinSw, sinSqSw;

    if (stack != NULL && stack->itemID == Item_mapItem->shiftedIndex) {

        R_pushMatrix();
        float armScale = 0.8f;
        sw = EntityPlayerSP_getSwingProgress(player, partialTick);
        sinSw = MathHelper_sin(sw * (float)M_PI);
        sinSqSw = MathHelper_sin(MathHelper_sqrt_float(sw) * (float)M_PI);
        R_translatef(-sinSqSw * 0.4f,
                     MathHelper_sin(MathHelper_sqrt_float(sw) * (float)M_PI * 2.0f) * 0.2f,
                     -sinSw * 0.2f);
        float pitchFac = 1.0f - pitch / 45.0f + 0.1f;
        if (pitchFac < 0.0f)
            pitchFac = 0.0f;
        if (pitchFac > 1.0f)
            pitchFac = 1.0f;
        pitchFac = -MathHelper_cos(pitchFac * (float)M_PI) * 0.5f + 0.5f;
        R_translatef(0.0f, 0.0f * armScale - (1.0f - equip) * 1.2f - pitchFac * 0.5f + 0.04f,
                     -0.9f * armScale);
        R_rotatef(90.0f, 0.0f, 1.0f, 0.0f);
        R_rotatef(pitchFac * -85.0f, 0.0f, 0.0f, 1.0f);
        R_enable(R_RESCALE_NORMAL);
        R_bindTexture(RenderEngine_getTextureForDownloadableImage(
            self->mc->renderEngine, pe->skinUrl,
            ((EntityLivingVtable *)pe->vtable)->getEntityTexture((EntityLiving *)pe)));
        for (int h = 0; h < 2; h++) {
            int side = h * 2 - 1;
            R_pushMatrix();
            R_translatef(-0.0f, -0.6f, 1.1f * (float)side);
            R_rotatef((float)(-45 * side), 1.0f, 0.0f, 0.0f);
            R_rotatef(-90.0f, 0.0f, 0.0f, 1.0f);
            R_rotatef(59.0f, 0.0f, 0.0f, 1.0f);
            R_rotatef((float)(-65 * side), 0.0f, 1.0f, 0.0f);

            {
                float var13 = 1.0f;
                R_scalef(var13, var13, var13);
                RenderPlayer *var12 = (RenderPlayer *)RenderManager_instance->renderPlayer;
                RenderPlayer_drawFirstPersonHand(var12);
            }
            R_popMatrix();
        }
        sw = EntityPlayerSP_getSwingProgress(player, partialTick);
        sinSw = MathHelper_sin(sw * sw * (float)M_PI);
        sinSqSw = MathHelper_sin(MathHelper_sqrt_float(sw) * (float)M_PI);
        R_rotatef(-sinSw * 20.0f, 0.0f, 1.0f, 0.0f);
        R_rotatef(-sinSqSw * 20.0f, 0.0f, 0.0f, 1.0f);
        R_rotatef(-sinSqSw * 80.0f, 1.0f, 0.0f, 0.0f);
        float mscale = 0.38f;
        R_scalef(mscale, mscale, mscale);
        R_rotatef(90.0f, 0.0f, 1.0f, 0.0f);
        R_rotatef(180.0f, 0.0f, 0.0f, 1.0f);
        R_translatef(-1.0f, -1.0f, 0.0f);
        float pxScale = 0.015625f;
        R_scalef(pxScale, pxScale, pxScale);
        R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/misc/mapbg.png"));
        Tessellator *tess = &Tessellator_instance;
        R_normal3f(0.0f, 0.0f, -1.0f);
        Tessellator_startDrawingQuads(tess);
        int pad = 7;
        Tessellator_addVertexWithUV(tess, (double)(0 - pad), (double)(128 + pad), 0.0, 0.0, 1.0);
        Tessellator_addVertexWithUV(tess, (double)(128 + pad), (double)(128 + pad), 0.0, 1.0, 1.0);
        Tessellator_addVertexWithUV(tess, (double)(128 + pad), (double)(0 - pad), 0.0, 1.0, 0.0);
        Tessellator_addVertexWithUV(tess, (double)(0 - pad), (double)(0 - pad), 0.0, 0.0, 0.0);
        Tessellator_draw(tess);

        MapData *var22 = ItemMap_func_28012_a(Item_mapItem, stack, self->mc->theWorld);
        MapItemRenderer_func_28157_a(&self->field_28131_f,
                                     (struct EntityPlayer *)self->mc->thePlayer,
                                     self->mc->renderEngine, var22);
        R_popMatrix();

    } else if (stack != NULL) {

        R_pushMatrix();
        float armScale = 0.8f;
        sw = EntityPlayerSP_getSwingProgress(player, partialTick);
        sinSw = MathHelper_sin(sw * (float)M_PI);
        sinSqSw = MathHelper_sin(MathHelper_sqrt_float(sw) * (float)M_PI);
        R_translatef(-sinSqSw * 0.4f,
                     MathHelper_sin(MathHelper_sqrt_float(sw) * (float)M_PI * 2.0f) * 0.2f,
                     -sinSw * 0.2f);
        R_translatef(0.7f * armScale, -0.65f * armScale - (1.0f - equip) * 0.6f, -0.9f * armScale);
        R_rotatef(45.0f, 0.0f, 1.0f, 0.0f);
        R_enable(R_RESCALE_NORMAL);
        sw = EntityPlayerSP_getSwingProgress(player, partialTick);
        sinSw = MathHelper_sin(sw * sw * (float)M_PI);
        sinSqSw = MathHelper_sin(MathHelper_sqrt_float(sw) * (float)M_PI);
        R_rotatef(-sinSw * 20.0f, 0.0f, 1.0f, 0.0f);
        R_rotatef(-sinSqSw * 20.0f, 0.0f, 0.0f, 1.0f);
        R_rotatef(-sinSqSw * 80.0f, 1.0f, 0.0f, 0.0f);
        float iscale = 0.4f;
        R_scalef(iscale, iscale, iscale);
        Item *heldItem = Item_itemsList[stack->itemID];
        if (heldItem->vtable->shouldRotateAroundWhenRendering(heldItem))
            R_rotatef(180.0f, 0.0f, 1.0f, 0.0f);
        ItemRenderer_renderItem(self, (EntityLiving *)player, stack);
        R_popMatrix();

    } else {

        R_pushMatrix();
        float armScale = 0.8f;
        sw = EntityPlayerSP_getSwingProgress(player, partialTick);
        sinSw = MathHelper_sin(sw * (float)M_PI);
        sinSqSw = MathHelper_sin(MathHelper_sqrt_float(sw) * (float)M_PI);
        R_translatef(-sinSqSw * 0.3f,
                     MathHelper_sin(MathHelper_sqrt_float(sw) * (float)M_PI * 2.0f) * 0.4f,
                     -sinSw * 0.4f);
        R_translatef(0.8f * armScale, -(12.0f / 16.0f) * armScale - (1.0f - equip) * 0.6f,
                     -0.9f * armScale);
        R_rotatef(45.0f, 0.0f, 1.0f, 0.0f);
        R_enable(R_RESCALE_NORMAL);
        sw = EntityPlayerSP_getSwingProgress(player, partialTick);
        sinSw = MathHelper_sin(sw * sw * (float)M_PI);
        sinSqSw = MathHelper_sin(MathHelper_sqrt_float(sw) * (float)M_PI);
        R_rotatef(sinSqSw * 70.0f, 0.0f, 1.0f, 0.0f);
        R_rotatef(-sinSw * 20.0f, 0.0f, 0.0f, 1.0f);

        R_bindTexture(RenderEngine_getTextureForDownloadableImage(
            self->mc->renderEngine, pe->skinUrl,
            ((EntityLivingVtable *)pe->vtable)->getEntityTexture((EntityLiving *)pe)));
        R_translatef(-1.0f, 3.6f, 3.5f);
        R_rotatef(120.0f, 0.0f, 0.0f, 1.0f);
        R_rotatef(200.0f, 1.0f, 0.0f, 0.0f);
        R_rotatef(-135.0f, 0.0f, 1.0f, 0.0f);
        R_scalef(1.0f, 1.0f, 1.0f);
        R_translatef(5.6f, 0.0f, 0.0f);

        {
            float var10 = 1.0f;
            R_scalef(var10, var10, var10);
            RenderPlayer *var16 = (RenderPlayer *)RenderManager_instance->renderPlayer;
            RenderPlayer_drawFirstPersonHand(var16);
        }
        R_popMatrix();
    }

    R_disable(R_RESCALE_NORMAL);
    RenderHelper_disableStandardItemLighting();
}

void ItemRenderer_renderOverlays(ItemRenderer *self, float partialTick) {
    R_disable(R_ALPHA_TEST);
    EntityPlayerSP *player = self->mc->thePlayer;
    Entity *pe = (Entity *)player;

    if (Entity_isBurning(pe)) {
        R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/terrain.png"));
        ItemRenderer_renderFireInFirstPerson(self, partialTick);
    }

    if (Entity_isEntityInsideOpaqueBlock(pe)) {
        int bx = MathHelper_floor_double(pe->posX);
        int by = MathHelper_floor_double(pe->posY);
        int bz = MathHelper_floor_double(pe->posZ);
        R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/terrain.png"));
        int blockId = World_getBlockId(self->mc->theWorld, bx, by, bz);
        if (World_isBlockNormalCube(self->mc->theWorld, bx, by, bz)) {
            ItemRenderer_renderInsideOfBlock(
                self, partialTick,
                Block_blocksList[blockId]->vtable->getBlockTextureFromSide(
                    Block_blocksList[blockId], 2));
        } else {
            for (int corner = 0; corner < 8; corner++) {
                float cx = ((float)((corner >> 0) % 2) - 0.5f) * pe->width * 0.9f;
                float cy = ((float)((corner >> 1) % 2) - 0.5f) * pe->height * 0.2f;
                float cz = ((float)((corner >> 2) % 2) - 0.5f) * pe->width * 0.9f;
                int nx = MathHelper_floor_float((float)bx + cx);
                int ny = MathHelper_floor_float((float)by + cy);
                int nz = MathHelper_floor_float((float)bz + cz);
                if (World_isBlockNormalCube(self->mc->theWorld, nx, ny, nz))
                    blockId = World_getBlockId(self->mc->theWorld, nx, ny, nz);
            }
        }
        if (Block_blocksList[blockId] != NULL)
            ItemRenderer_renderInsideOfBlock(
                self, partialTick,
                Block_blocksList[blockId]->vtable->getBlockTextureFromSide(
                    Block_blocksList[blockId], 2));
    }

    if (Entity_isInsideOfMaterial(pe, Material_water)) {
        R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/misc/water.png"));
        ItemRenderer_renderWarpedTextureOverlay(self, partialTick);
    }

    R_enable(R_ALPHA_TEST);
}

static void ItemRenderer_renderInsideOfBlock(ItemRenderer *self, float partialTick, int texIndex) {
    Tessellator *tess = &Tessellator_instance;
    ((Entity *)self->mc->thePlayer)
        ->vtable->getEntityBrightness((Entity *)self->mc->thePlayer, partialTick);
    float dim = 0.1f;
    R_color4f(dim, dim, dim, 0.5f);
    R_pushMatrix();
    float x0 = -1.0f, x1 = 1.0f, y0 = -1.0f, y1 = 1.0f, z = -0.5f;
    float eps = 0.0078125f;
    float u0 = (float)(texIndex % 16) / 256.0f - eps;
    float u1 = ((float)(texIndex % 16) + 15.99f) / 256.0f + eps;
    float v0 = (float)(texIndex / 16) / 256.0f - eps;
    float v1 = ((float)(texIndex / 16) + 15.99f) / 256.0f + eps;
    Tessellator_startDrawingQuads(tess);
    Tessellator_addVertexWithUV(tess, (double)x0, (double)y0, (double)z, (double)u1, (double)v1);
    Tessellator_addVertexWithUV(tess, (double)x1, (double)y0, (double)z, (double)u0, (double)v1);
    Tessellator_addVertexWithUV(tess, (double)x1, (double)y1, (double)z, (double)u0, (double)v0);
    Tessellator_addVertexWithUV(tess, (double)x0, (double)y1, (double)z, (double)u1, (double)v0);
    Tessellator_draw(tess);
    R_popMatrix();
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
}

static void ItemRenderer_renderWarpedTextureOverlay(ItemRenderer *self, float partialTick) {
    EntityPlayerSP *player = self->mc->thePlayer;
    Entity *pe = (Entity *)player;
    Tessellator *tess = &Tessellator_instance;
    float bright = pe->vtable->getEntityBrightness(pe, partialTick);
    R_color4f(bright, bright, bright, 0.5f);
    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    R_pushMatrix();
    float scale = 4.0f;
    float x0 = -1.0f, x1 = 1.0f, y0 = -1.0f, y1 = 1.0f, z = -0.5f;
    float ox = -pe->rotationYaw / 64.0f;
    float oy = pe->rotationPitch / 64.0f;
    Tessellator_startDrawingQuads(tess);
    Tessellator_addVertexWithUV(tess, (double)x0, (double)y0, (double)z, (double)(scale + ox),
                                (double)(scale + oy));
    Tessellator_addVertexWithUV(tess, (double)x1, (double)y0, (double)z, (double)(0.0f + ox),
                                (double)(scale + oy));
    Tessellator_addVertexWithUV(tess, (double)x1, (double)y1, (double)z, (double)(0.0f + ox),
                                (double)(0.0f + oy));
    Tessellator_addVertexWithUV(tess, (double)x0, (double)y1, (double)z, (double)(scale + ox),
                                (double)(0.0f + oy));
    Tessellator_draw(tess);
    R_popMatrix();
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    R_disable(R_BLEND);
}

static void ItemRenderer_renderFireInFirstPerson(ItemRenderer *self, float partialTick) {
    (void)self;
    (void)partialTick;
    Tessellator *tess = &Tessellator_instance;
    R_color4f(1.0f, 1.0f, 1.0f, 0.9f);
    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    float w = 1.0f;
    for (int f = 0; f < 2; f++) {
        R_pushMatrix();
        int texIdx = Block_fire->blockIndexInTexture + f * 16;
        int tx = (texIdx & 15) << 4;
        int ty = texIdx & 240;
        float u0 = (float)tx / 256.0f;
        float u1 = ((float)tx + 15.99f) / 256.0f;
        float v0 = (float)ty / 256.0f;
        float v1 = ((float)ty + 15.99f) / 256.0f;
        float hx = (0.0f - w) / 2.0f;
        float hx1 = hx + w;
        float hy = 0.0f - w / 2.0f;
        float hy1 = hy + w;
        float z = -0.5f;
        R_translatef((float)(-(f * 2 - 1)) * 0.24f, -0.3f, 0.0f);
        R_rotatef((float)(f * 2 - 1) * 10.0f, 0.0f, 1.0f, 0.0f);
        Tessellator_startDrawingQuads(tess);
        Tessellator_addVertexWithUV(tess, (double)hx, (double)hy, (double)z, (double)u1,
                                    (double)v1);
        Tessellator_addVertexWithUV(tess, (double)hx1, (double)hy, (double)z, (double)u0,
                                    (double)v1);
        Tessellator_addVertexWithUV(tess, (double)hx1, (double)hy1, (double)z, (double)u0,
                                    (double)v0);
        Tessellator_addVertexWithUV(tess, (double)hx, (double)hy1, (double)z, (double)u1,
                                    (double)v0);
        Tessellator_draw(tess);
        R_popMatrix();
    }
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    R_disable(R_BLEND);
}

void ItemRenderer_updateEquippedItem(ItemRenderer *self) {
    self->prevEquippedProgress = self->equippedProgress;
    EntityPlayerSP *player = self->mc->thePlayer;
    ItemStack *current = InventoryPlayer_getCurrentItem(&player->base.inventory);

    int sameItem = (self->field_20099_f == player->base.inventory.currentItem &&
                    current == self->itemToRender);
    if (self->itemToRender == NULL && current == NULL)
        sameItem = 1;

    if (current != NULL && self->itemToRender != NULL && current != self->itemToRender &&
        current->itemID == self->itemToRender->itemID &&
        ItemStack_getItemDamage(current) == ItemStack_getItemDamage(self->itemToRender)) {
        self->itemToRender = current;
        sameItem = 1;
    }

    float speed = 0.4f;
    float target = sameItem ? 1.0f : 0.0f;
    float delta = target - self->equippedProgress;
    if (delta < -speed)
        delta = -speed;
    if (delta > speed)
        delta = speed;
    self->equippedProgress += delta;

    if (self->equippedProgress < 0.1f) {
        self->itemToRender = current;
        self->field_20099_f = player->base.inventory.currentItem;
    }
}

void ItemRenderer_func_9449_b(ItemRenderer *self) { self->equippedProgress = 0.0f; }

void ItemRenderer_func_9450_c(ItemRenderer *self) { self->equippedProgress = 0.0f; }
