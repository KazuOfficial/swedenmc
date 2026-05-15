#include "Renderer.h"

#include "RenderPlayer.h"
#include "RenderLiving.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "Entity.h"
#include "ModelBiped.h"
#include "ModelRenderer.h"
#include "ItemArmor.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"
#include "InventoryPlayer.h"
#include "RenderBlocks.h"
#include "ItemRenderer.h"
#include "MathHelper.h"
#include "FontRenderer.h"
#include "Tessellator.h"
#include "Minecraft.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char *const s_armorFilenamePrefix[] = {"cloth", "chain", "iron", "diamond", "gold"};

static int setArmorModel(RenderPlayer *self, EntityPlayer *var1, int var2, float var3) {
    (void)var3;
    ItemStack *var4 = InventoryPlayer_armorItemInSlot(&var1->inventory, 3 - var2);
    if (var4 != NULL) {
        Item *var5 = Item_itemsList[var4->itemID];
        if (ItemArmor_isInstance(var5)) {
            ItemArmor *var6 = (ItemArmor *)var5;
            char texbuf[128];
            snprintf(texbuf, sizeof(texbuf), "/armor/%s_%d.png",
                     s_armorFilenamePrefix[var6->renderIndex], var2 == 2 ? 2 : 1);
            Render_loadTexture(&self->base.base, texbuf);
            ModelBiped *var7 = var2 == 2 ? self->modelArmor : self->modelArmorChestplate;
            var7->bipedHead.showModel = (var2 == 0);
            var7->bipedHeadwear.showModel = (var2 == 0);
            var7->bipedBody.showModel = (var2 == 1 || var2 == 2);
            var7->bipedRightArm.showModel = (var2 == 1);
            var7->bipedLeftArm.showModel = (var2 == 1);
            var7->bipedRightLeg.showModel = (var2 == 2 || var2 == 3);
            var7->bipedLeftLeg.showModel = (var2 == 2 || var2 == 3);
            RenderLiving_setRenderPassModel(&self->base, (ModelBase *)var7);
            return 1;
        }
    }
    return 0;
}

static void renderPlayer(RenderPlayer *self, EntityPlayer *var1, double var2, double var4,
                         double var6, float var8, float var9) {
    ItemStack *var10 = InventoryPlayer_getCurrentItem(&var1->inventory);
    int var10_nn = (var10 != NULL);
    self->modelArmorChestplate->field_1278_i = self->modelArmor->field_1278_i =
        self->modelBipedMain->field_1278_i = var10_nn;
    int isneak = ((Entity *)var1)->vtable->isSneaking((Entity *)var1);
    self->modelArmorChestplate->isSneak = self->modelArmor->isSneak =
        self->modelBipedMain->isSneak = isneak;
    double var11 = var4 - (double)var1->base.base.yOffset;
    if (isneak && !((Entity *)var1)->isPlayerSP) {
        var11 -= 0.125;
    }
    RenderLiving_doRenderLiving(&self->base, (EntityLiving *)var1, var2, var11, var6, var8, var9);
    self->modelArmorChestplate->isSneak = self->modelArmor->isSneak =
        self->modelBipedMain->isSneak = 0;
    self->modelArmorChestplate->field_1278_i = self->modelArmor->field_1278_i =
        self->modelBipedMain->field_1278_i = 0;
}

static void renderName(RenderPlayer *self, EntityPlayer *var1, double var2, double var4,
                       double var6) {
    if (Minecraft_isGuiEnabled() &&
        (EntityLiving *)var1 != self->base.base.renderManager->livingPlayer) {
        float var8 = 1.6f;
        float var9 = (float)(1.0 / 60.0) * var8;
        float var10 = Entity_getDistanceToEntity(
            (Entity *)var1, (Entity *)self->base.base.renderManager->livingPlayer);
        float var11 = ((Entity *)var1)->vtable->isSneaking((Entity *)var1) ? 32.0f : 64.0f;
        if (var10 < var11) {
            const char *var12 = var1->username;
            if (!((Entity *)var1)->vtable->isSneaking((Entity *)var1)) {
                if (Entity_isPlayerSleeping((Entity *)var1)) {
                    RenderLiving_renderLivingLabel(&self->base, (EntityLiving *)var1, var12, var2,
                                                   var4 - 1.5, var6, 64);
                } else {
                    RenderLiving_renderLivingLabel(&self->base, (EntityLiving *)var1, var12, var2,
                                                   var4, var6, 64);
                }
            } else {
                FontRenderer *var13 = Render_getFontRendererFromRenderManager(&self->base.base);
                R_pushMatrix();
                R_translatef((float)var2 + 0.0f, (float)var4 + 2.3f, (float)var6);
                R_normal3f(0.0f, 1.0f, 0.0f);
                R_rotatef(-self->base.base.renderManager->playerViewY, 0.0f, 1.0f, 0.0f);
                R_rotatef(self->base.base.renderManager->playerViewX, 1.0f, 0.0f, 0.0f);
                R_scalef(-var9, -var9, var9);
                R_disable(R_LIGHTING);
                R_translatef(0.0f, 0.25f / var9, 0.0f);
                R_depthMask(R_FALSE);
                R_enable(R_BLEND);
                R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
                Tessellator *var14 = &Tessellator_instance;
                R_disable(R_TEXTURE_2D);
                Tessellator_startDrawingQuads(var14);
                int var15 = FontRenderer_getStringWidth(var13, var12) / 2;
                Tessellator_setColorRGBA_F(var14, 0.0f, 0.0f, 0.0f, 0.25f);
                Tessellator_addVertex(var14, (double)(-var15 - 1), -1.0, 0.0);
                Tessellator_addVertex(var14, (double)(-var15 - 1), 8.0, 0.0);
                Tessellator_addVertex(var14, (double)(var15 + 1), 8.0, 0.0);
                Tessellator_addVertex(var14, (double)(var15 + 1), -1.0, 0.0);
                Tessellator_draw(var14);
                R_enable(R_TEXTURE_2D);
                R_depthMask(R_TRUE);
                FontRenderer_drawString(
                    var13, var12, -FontRenderer_getStringWidth(var13, var12) / 2, 0, 553648127);
                R_enable(R_LIGHTING);
                R_disable(R_BLEND);
                R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
                R_popMatrix();
            }
        }
    }
}

static void renderSpecials(RenderPlayer *self, EntityPlayer *var1, float var2) {

    ItemStack *var3 = InventoryPlayer_armorItemInSlot(&var1->inventory, 3);
    if (var3 != NULL && Item_itemsList[var3->itemID]->shiftedIndex < 256 &&
        Block_blocksList[var3->itemID]) {
        R_pushMatrix();
        ModelRenderer_postRender(&self->modelBipedMain->bipedHead, 1.0f / 16.0f);
        if (RenderBlocks_renderItemIn3d(Block_blocksList[var3->itemID]->vtable->getRenderType(
                Block_blocksList[var3->itemID]))) {
            float var4 = 10.0f / 16.0f;
            R_translatef(0.0f, -0.25f, 0.0f);
            R_rotatef(180.0f, 0.0f, 1.0f, 0.0f);
            R_scalef(var4, -var4, var4);
        }
        ItemRenderer_renderItem(self->base.base.renderManager->itemRenderer, (EntityLiving *)var1,
                                var3);
        R_popMatrix();
    }

    float var5;
    if (strcmp(var1->username, "deadmau5") == 0 &&
        Render_loadDownloadableImageTexture(&self->base.base, var1->base.base.skinUrl, NULL)) {
        int var19;
        for (var19 = 0; var19 < 2; ++var19) {
            var5 = var1->base.base.prevRotationYaw +
                   (var1->base.base.rotationYaw - var1->base.base.prevRotationYaw) * var2 -
                   (var1->base.prevRenderYawOffset +
                    (var1->base.renderYawOffset - var1->base.prevRenderYawOffset) * var2);
            float var6 = var1->base.base.prevRotationPitch +
                         (var1->base.base.rotationPitch - var1->base.base.prevRotationPitch) * var2;
            R_pushMatrix();
            R_rotatef(var5, 0.0f, 1.0f, 0.0f);
            R_rotatef(var6, 1.0f, 0.0f, 0.0f);
            R_translatef(6.0f / 16.0f * (float)(var19 * 2 - 1), 0.0f, 0.0f);
            R_translatef(0.0f, -(6.0f / 16.0f), 0.0f);
            R_rotatef(-var6, 1.0f, 0.0f, 0.0f);
            R_rotatef(-var5, 0.0f, 1.0f, 0.0f);
            float var7 = 4.0f / 3.0f;
            R_scalef(var7, var7, var7);
            ModelBiped_renderEars(self->modelBipedMain, 1.0f / 16.0f);
            R_popMatrix();
        }
    }

    if (Render_loadDownloadableImageTexture(&self->base.base, var1->playerCloakUrl, NULL)) {
        R_pushMatrix();
        R_translatef(0.0f, 0.0f, 2.0f / 16.0f);
        double var20 = var1->field_20066_r +
                       (var1->field_20063_u - var1->field_20066_r) * (double)var2 -
                       (var1->base.base.prevPosX +
                        (var1->base.base.posX - var1->base.base.prevPosX) * (double)var2);
        double var22 = var1->field_20065_s +
                       (var1->field_20062_v - var1->field_20065_s) * (double)var2 -
                       (var1->base.base.prevPosY +
                        (var1->base.base.posY - var1->base.base.prevPosY) * (double)var2);
        double var8 = var1->field_20064_t +
                      (var1->field_20061_w - var1->field_20064_t) * (double)var2 -
                      (var1->base.base.prevPosZ +
                       (var1->base.base.posZ - var1->base.base.prevPosZ) * (double)var2);
        float var10_c = var1->base.prevRenderYawOffset +
                        (var1->base.renderYawOffset - var1->base.prevRenderYawOffset) * var2;
        double var11 = (double)MathHelper_sin(var10_c * (float)3.14159265358979323846f / 180.0f);
        double var13 = (double)(-MathHelper_cos(var10_c * (float)3.14159265358979323846f / 180.0f));
        float var15 = (float)var22 * 10.0f;
        if (var15 < -6.0f) {
            var15 = -6.0f;
        }
        if (var15 > 32.0f) {
            var15 = 32.0f;
        }
        float var16 = (float)(var20 * var11 + var8 * var13) * 100.0f;
        float var17 = (float)(var20 * var13 - var8 * var11) * 100.0f;
        if (var16 < 0.0f) {
            var16 = 0.0f;
        }
        float var18 = var1->field_775_e + (var1->field_774_f - var1->field_775_e) * var2;
        var15 += MathHelper_sin((var1->base.base.prevDistanceWalkedModified +
                                 (var1->base.base.distanceWalkedModified -
                                  var1->base.base.prevDistanceWalkedModified) *
                                     var2) *
                                6.0f) *
                 32.0f * var18;
        if (((Entity *)var1)->vtable->isSneaking((Entity *)var1)) {
            var15 += 25.0f;
        }
        R_rotatef(6.0f + var16 / 2.0f + var15, 1.0f, 0.0f, 0.0f);
        R_rotatef(var17 / 2.0f, 0.0f, 0.0f, 1.0f);
        R_rotatef(-var17 / 2.0f, 0.0f, 1.0f, 0.0f);
        R_rotatef(180.0f, 0.0f, 1.0f, 0.0f);
        ModelBiped_renderCloak(self->modelBipedMain, 1.0f / 16.0f);
        R_popMatrix();
    }

    ItemStack *var21 = InventoryPlayer_getCurrentItem(&var1->inventory);
    if (var21 != NULL) {
        R_pushMatrix();
        ModelRenderer_postRender(&self->modelBipedMain->bipedRightArm, 1.0f / 16.0f);
        R_translatef(-(1.0f / 16.0f), 7.0f / 16.0f, 1.0f / 16.0f);
        ItemStack stick_stack;
        if (var1->fishEntity != NULL) {

            stick_stack.itemID = Item_stick->shiftedIndex;
            stick_stack.stackSize = 1;
            stick_stack.itemDamage = 0;
            stick_stack.animationsToGo = 0;
            var21 = &stick_stack;
        }
        if (var21->itemID < 256 && Block_blocksList[var21->itemID] &&
            RenderBlocks_renderItemIn3d(Block_blocksList[var21->itemID]->vtable->getRenderType(
                Block_blocksList[var21->itemID]))) {
            var5 = 0.5f;
            R_translatef(0.0f, 3.0f / 16.0f, -(5.0f / 16.0f));
            var5 *= 12.0f / 16.0f;
            R_rotatef(20.0f, 1.0f, 0.0f, 0.0f);
            R_rotatef(45.0f, 0.0f, 1.0f, 0.0f);
            R_scalef(var5, -var5, var5);
        } else if (Item_itemsList[var21->itemID]->vtable->isFull3D(Item_itemsList[var21->itemID])) {
            var5 = 10.0f / 16.0f;
            if (Item_itemsList[var21->itemID]->vtable->shouldRotateAroundWhenRendering(
                    Item_itemsList[var21->itemID])) {
                R_rotatef(180.0f, 0.0f, 0.0f, 1.0f);
                R_translatef(0.0f, -(2.0f / 16.0f), 0.0f);
            }
            R_translatef(0.0f, 3.0f / 16.0f, 0.0f);
            R_scalef(var5, -var5, var5);
            R_rotatef(-100.0f, 1.0f, 0.0f, 0.0f);
            R_rotatef(45.0f, 0.0f, 1.0f, 0.0f);
        } else {
            var5 = 6.0f / 16.0f;
            R_translatef(0.25f, 3.0f / 16.0f, -(3.0f / 16.0f));
            R_scalef(var5, var5, var5);
            R_rotatef(60.0f, 0.0f, 0.0f, 1.0f);
            R_rotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            R_rotatef(20.0f, 0.0f, 0.0f, 1.0f);
        }
        ItemRenderer_renderItem(self->base.base.renderManager->itemRenderer, (EntityLiving *)var1,
                                var21);
        R_popMatrix();
    }
}

static void func_186_b(RenderPlayer *self, EntityPlayer *var1, float var2) {
    (void)self;
    (void)var1;
    (void)var2;
    float var3 = 15.0f / 16.0f;
    R_scalef(var3, var3, var3);
}

void RenderPlayer_drawFirstPersonHand(RenderPlayer *self) {
    self->modelBipedMain->base.onGround = 0.0f;
    self->modelBipedMain->base.vtable->setRotationAngles((ModelBase *)self->modelBipedMain, 0.0f,
                                                         0.0f, 0.0f, 0.0f, 0.0f, 1.0f / 16.0f);
    ModelRenderer_render(&self->modelBipedMain->bipedRightArm, 1.0f / 16.0f);
}

static void func_22016_b(RenderPlayer *self, EntityPlayer *var1, double var2, double var4,
                         double var6) {
    if (var1->base.base.vtable->isEntityAlive((Entity *)var1) &&
        Entity_isPlayerSleeping((Entity *)var1)) {
        RenderLiving_func_22012_b(
            &self->base, (EntityLiving *)var1, var2 + (double)var1->field_22063_x,
            var4 + (double)var1->field_22062_y, var6 + (double)var1->field_22061_z);
    } else {
        RenderLiving_func_22012_b(&self->base, (EntityLiving *)var1, var2, var4, var6);
    }
}

static void func_22017_a(RenderPlayer *self, EntityPlayer *var1, float var2, float var3,
                         float var4) {
    if (var1->base.base.vtable->isEntityAlive((Entity *)var1) &&
        Entity_isPlayerSleeping((Entity *)var1)) {
        R_rotatef(EntityPlayer_getBedOrientationInDegrees(var1), 0.0f, 1.0f, 0.0f);
        R_rotatef(((RenderLiving_vtable *)self->base.base.vtable)
                      ->getDeathMaxRotation(&self->base, (EntityLiving *)var1),
                  0.0f, 0.0f, 1.0f);
        R_rotatef(270.0f, 0.0f, 1.0f, 0.0f);
    } else {
        RenderLiving_rotateCorpse(&self->base, (EntityLiving *)var1, var2, var3, var4);
    }
}

static void vt_passSpecialRender(RenderLiving *base, EntityLiving *var1, double var2, double var4,
                                 double var6) {
    renderName((RenderPlayer *)base, (EntityPlayer *)var1, var2, var4, var6);
}

static void vt_preRenderCallback(RenderLiving *base, EntityLiving *var1, float var2) {
    func_186_b((RenderPlayer *)base, (EntityPlayer *)var1, var2);
}

static int vt_shouldRenderPass(RenderLiving *base, EntityLiving *var1, int var2, float var3) {
    return setArmorModel((RenderPlayer *)base, (EntityPlayer *)var1, var2, var3);
}

static void vt_renderEquippedItems(RenderLiving *base, EntityLiving *var1, float var2) {
    renderSpecials((RenderPlayer *)base, (EntityPlayer *)var1, var2);
}

static void vt_rotateCorpse(RenderLiving *base, EntityLiving *var1, float var2, float var3,
                            float var4) {
    func_22017_a((RenderPlayer *)base, (EntityPlayer *)var1, var2, var3, var4);
}

static void vt_func_22012_b(RenderLiving *base, EntityLiving *var1, double var2, double var4,
                            double var6) {
    func_22016_b((RenderPlayer *)base, (EntityPlayer *)var1, var2, var4, var6);
}

static void vt_doRenderLiving(RenderLiving *base, EntityLiving *var1, double var2, double var4,
                              double var6, float var8, float var9) {
    renderPlayer((RenderPlayer *)base, (EntityPlayer *)var1, var2, var4, var6, var8, var9);
}

static void vt_doRender(struct Render *base, struct Entity *var1, double var2, double var4,
                        double var6, float var8, float var9) {
    renderPlayer((RenderPlayer *)base, (EntityPlayer *)var1, var2, var4, var6, var8, var9);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderPlayer *RenderPlayer_create(void) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.base.doRender = vt_doRender;
        s_vtable.doRenderLiving = vt_doRenderLiving;
        s_vtable.func_22012_b = vt_func_22012_b;
        s_vtable.rotateCorpse = vt_rotateCorpse;
        s_vtable.renderEquippedItems = vt_renderEquippedItems;
        s_vtable.shouldRenderPass = vt_shouldRenderPass;
        s_vtable.preRenderCallback = vt_preRenderCallback;
        s_vtable.passSpecialRender = vt_passSpecialRender;
        s_vtable_init = 1;
    }

    ModelBiped *mainModel = (ModelBiped *)calloc(1, sizeof(ModelBiped));
    ModelBiped_init1(mainModel, 0.0f);

    ModelBiped *chestplate = (ModelBiped *)calloc(1, sizeof(ModelBiped));
    ModelBiped_init1(chestplate, 1.0f);

    ModelBiped *armor = (ModelBiped *)calloc(1, sizeof(ModelBiped));
    ModelBiped_init1(armor, 0.5f);

    RenderPlayer *self = (RenderPlayer *)calloc(1, sizeof(RenderPlayer));
    RenderLiving_init(&self->base, (ModelBase *)mainModel, 0.5f);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;

    self->modelBipedMain = mainModel;
    self->modelArmorChestplate = chestplate;
    self->modelArmor = armor;

    return self;
}
