#include "Renderer.h"

#include "GuiIngame.h"
#include "Minecraft.h"
#include "Block.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "FontRenderer.h"
#include "Material.h"
#include "MathHelper.h"
#include "GuiChat.h"
#include "StringTranslate.h"
#include "ScaledResolution.h"
#include "RenderHelper.h"
#include "Tessellator.h"
#include "EntityRenderer.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "RenderItem.h"

#ifndef _WIN32
#include <sys/resource.h>
#include <unistd.h>
#else
#include <windows.h>
#include <psapi.h>
#endif

static int Color_HSBtoRGB(float hue, float saturation, float brightness) {
    int r = 0, g = 0, b = 0;
    if (saturation == 0.0f) {
        r = g = b = (int)(brightness * 255.0f + 0.5f);
    } else {
        float h = (hue - floorf(hue)) * 6.0f;
        float f = h - floorf(h);
        float p = brightness * (1.0f - saturation);
        float q = brightness * (1.0f - saturation * f);
        float t = brightness * (1.0f - (saturation * (1.0f - f)));
        switch ((int)h) {
        case 0:
            r = (int)(brightness * 255.0f + 0.5f);
            g = (int)(t * 255.0f + 0.5f);
            b = (int)(p * 255.0f + 0.5f);
            break;
        case 1:
            r = (int)(q * 255.0f + 0.5f);
            g = (int)(brightness * 255.0f + 0.5f);
            b = (int)(p * 255.0f + 0.5f);
            break;
        case 2:
            r = (int)(p * 255.0f + 0.5f);
            g = (int)(brightness * 255.0f + 0.5f);
            b = (int)(t * 255.0f + 0.5f);
            break;
        case 3:
            r = (int)(p * 255.0f + 0.5f);
            g = (int)(q * 255.0f + 0.5f);
            b = (int)(brightness * 255.0f + 0.5f);
            break;
        case 4:
            r = (int)(t * 255.0f + 0.5f);
            g = (int)(p * 255.0f + 0.5f);
            b = (int)(brightness * 255.0f + 0.5f);
            break;
        case 5:
            r = (int)(brightness * 255.0f + 0.5f);
            g = (int)(p * 255.0f + 0.5f);
            b = (int)(q * 255.0f + 0.5f);
            break;
        }
    }
    return (int)0xff000000 | (r << 16) | (g << 8) | b;
}

static RenderItem *itemRenderer_static = NULL;

GuiIngame *GuiIngame_create(struct Minecraft *var1) {
    GuiIngame *self = (GuiIngame *)calloc(1, sizeof(GuiIngame));
    if (!self)
        return NULL;
    Gui_init(&self->base);
    self->mc = var1;
    if (itemRenderer_static == NULL) {
        itemRenderer_static = RenderItem_create();
    }
    self->itemRenderer = itemRenderer_static;
    self->chatMessageCount = 0;
    JavaRandom_init(&self->rand, 0LL);
    self->field_933_a = NULL;
    self->updateCounter = 0;
    self->recordPlaying[0] = '\0';
    self->recordPlayingUpFor = 0;
    self->field_22065_l = 0;
    self->damageGuiPartialTime = 0.0F;
    self->prevVignetteBrightness = 1.0F;
    return self;
}

void GuiIngame_destroy(GuiIngame *self) {
    if (!self)
        return;
    for (int i = 0; i < self->chatMessageCount; ++i) {
        ChatLine_free(self->chatMessageList[i]);
        self->chatMessageList[i] = NULL;
    }
    free(self);
}

void GuiIngame_renderGameOverlay(GuiIngame *self, float var1, int var2, int var3, int var4) {
    (void)var2;
    (void)var3;
    (void)var4;
    ScaledResolution var5;
    ScaledResolution_init(&var5, self->mc->gameSettings, self->mc->displayWidth,
                          self->mc->displayHeight);
    int var6 = ScaledResolution_getScaledWidth(&var5);
    int var7 = ScaledResolution_getScaledHeight(&var5);
    FontRenderer *var8 = self->mc->fontRenderer;
    EntityRenderer_func_905_b(self->mc->entityRenderer);
    R_enable(R_BLEND);

    if (self->mc->thePlayer == NULL)
        return;
    if (self->mc->gameSettings->fancyGraphics) {
        GuiIngame_renderVignette(
            self, Entity_getEntityBrightness((Entity *)self->mc->thePlayer, var1), var6, var7);
    }
    ItemStack *var9 =
        InventoryPlayer_armorItemInSlot(&((EntityPlayer *)self->mc->thePlayer)->inventory, 3);
    if (!self->mc->gameSettings->thirdPersonView && var9 != NULL &&
        var9->itemID == Block_pumpkin->blockID) {
        GuiIngame_renderPumpkinBlur(self, var6, var7);
    }
    float var10 = ((EntityPlayer *)self->mc->thePlayer)->prevTimeInPortal +
                  (((EntityPlayer *)self->mc->thePlayer)->timeInPortal -
                   ((EntityPlayer *)self->mc->thePlayer)->prevTimeInPortal) *
                      var1;
    if (var10 > 0.0F) {
        GuiIngame_renderPortalOverlay(self, var10, var6, var7);
    }
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
    R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/gui/gui.png"));
    InventoryPlayer *var11 = &((EntityPlayer *)self->mc->thePlayer)->inventory;
    self->base.zLevel = -90.0F;
    Gui_drawTexturedModalRect(&self->base, var6 / 2 - 91, var7 - 22, 0, 0, 182, 22);
    Gui_drawTexturedModalRect(&self->base, var6 / 2 - 91 - 1 + var11->currentItem * 20,
                              var7 - 22 - 1, 0, 22, 24, 22);
    R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/gui/icons.png"));
    R_enable(R_BLEND);
    R_blendFunc(R_ONE_MINUS_DST_COLOR, R_ONE_MINUS_SRC_COLOR);
    Gui_drawTexturedModalRect(&self->base, var6 / 2 - 7, var7 / 2 - 7, 0, 0, 16, 16);
    R_disable(R_BLEND);
    int var12 = (((Entity *)self->mc->thePlayer)->heartsLife / 3 % 2 == 1) ? 1 : 0;
    if (((Entity *)self->mc->thePlayer)->heartsLife < 10) {
        var12 = 0;
    }
    int var13 = ((EntityLiving *)self->mc->thePlayer)->health;
    int var14 = ((EntityLiving *)self->mc->thePlayer)->prevHealth;
    JavaRandom_setSeed(&self->rand, (int64_t)((unsigned int)self->updateCounter * 312871U));
    int var15;
    int var16;
    int var17;
    if (PlayerController_shouldDrawHUD(self->mc->playerController)) {

        var15 = EntityPlayer_getPlayerArmorValue((EntityPlayer *)self->mc->thePlayer);
        int var18;
        for (var16 = 0; var16 < 10; ++var16) {
            var17 = var7 - 32;
            if (var15 > 0) {
                var18 = var6 / 2 + 91 - var16 * 8 - 9;
                if (var16 * 2 + 1 < var15) {
                    Gui_drawTexturedModalRect(&self->base, var18, var17, 34, 9, 9, 9);
                }
                if (var16 * 2 + 1 == var15) {
                    Gui_drawTexturedModalRect(&self->base, var18, var17, 25, 9, 9, 9);
                }
                if (var16 * 2 + 1 > var15) {
                    Gui_drawTexturedModalRect(&self->base, var18, var17, 16, 9, 9, 9);
                }
            }
            int var28 = 0;
            if (var12) {
                var28 = 1;
            }
            int var19 = var6 / 2 - 91 + var16 * 8;
            if (var13 <= 4) {
                var17 += JavaRandom_nextInt(&self->rand, 2);
            }
            Gui_drawTexturedModalRect(&self->base, var19, var17, 16 + var28 * 9, 0, 9, 9);
            if (var12) {
                if (var16 * 2 + 1 < var14) {
                    Gui_drawTexturedModalRect(&self->base, var19, var17, 70, 0, 9, 9);
                }
                if (var16 * 2 + 1 == var14) {
                    Gui_drawTexturedModalRect(&self->base, var19, var17, 79, 0, 9, 9);
                }
            }
            if (var16 * 2 + 1 < var13) {
                Gui_drawTexturedModalRect(&self->base, var19, var17, 52, 0, 9, 9);
            }
            if (var16 * 2 + 1 == var13) {
                Gui_drawTexturedModalRect(&self->base, var19, var17, 61, 0, 9, 9);
            }
        }
        if (Entity_isInsideOfMaterial((Entity *)self->mc->thePlayer, Material_water)) {
            var16 = (int)ceil((double)(((Entity *)self->mc->thePlayer)->air - 2) * 10.0 / 300.0);
            var17 = (int)ceil((double)((Entity *)self->mc->thePlayer)->air * 10.0 / 300.0) - var16;
            for (var18 = 0; var18 < var16 + var17; ++var18) {
                if (var18 < var16) {
                    Gui_drawTexturedModalRect(&self->base, var6 / 2 - 91 + var18 * 8, var7 - 32 - 9,
                                              16, 18, 9, 9);
                } else {
                    Gui_drawTexturedModalRect(&self->base, var6 / 2 - 91 + var18 * 8, var7 - 32 - 9,
                                              25, 18, 9, 9);
                }
            }
        }
    }
    R_disable(R_BLEND);
    R_enable(R_RESCALE_NORMAL);
    R_pushMatrix();
    R_rotatef(120.0F, 1.0F, 0.0F, 0.0F);
    RenderHelper_enableStandardItemLighting();
    R_popMatrix();
    for (var15 = 0; var15 < 9; ++var15) {
        var16 = var6 / 2 - 90 + var15 * 20 + 2;
        var17 = var7 - 16 - 3;
        GuiIngame_renderInventorySlot(self, var15, var16, var17, var1);
    }
    RenderHelper_disableStandardItemLighting();
    R_disable(R_RESCALE_NORMAL);

    if (EntityPlayer_func_22060_M((EntityPlayer *)self->mc->thePlayer) > 0) {
        R_disable(R_DEPTH_TEST);
        R_disable(R_ALPHA_TEST);
        var15 = EntityPlayer_func_22060_M((EntityPlayer *)self->mc->thePlayer);
        float var27 = (float)var15 / 100.0F;
        if (var27 > 1.0F) {
            var27 = 1.0F - (float)(var15 - 100) / 10.0F;
        }
        var17 = (int)(((unsigned int)(int)(220.0F * var27) << 24) | 1052704U);
        Gui_drawRect(&self->base, 0, 0, var6, var7, var17);
        R_enable(R_ALPHA_TEST);
        R_enable(R_DEPTH_TEST);
    }
    if (self->mc->gameSettings->showDebugInfo) {
        R_pushMatrix();
        if (Minecraft_hasPaidCheckTime > 0L) {
            R_translatef(0.0F, 32.0F, 0.0F);
        }

        {
            char debugStr[512];
            snprintf(debugStr, sizeof(debugStr), "Minecraft Beta 1.7.3 (%s)", self->mc->debug);
            FontRenderer_drawStringWithShadow(var8, debugStr, 2, 2, 16777215);
        }

        FontRenderer_drawStringWithShadow(var8, Minecraft_func_6241_m(self->mc), 2, 12, 16777215);

        FontRenderer_drawStringWithShadow(var8, Minecraft_func_6262_n(self->mc), 2, 22, 16777215);

        FontRenderer_drawStringWithShadow(var8, Minecraft_func_6245_o(self->mc), 2, 32, 16777215);

        FontRenderer_drawStringWithShadow(var8, Minecraft_func_21002_o(self->mc), 2, 42, 16777215);

        {
#ifdef _WIN32
            MEMORYSTATUSEX ms;
            ms.dwLength = sizeof(ms);
            GlobalMemoryStatusEx(&ms);
            long maxMem = (long)ms.ullTotalPhys;
            PROCESS_MEMORY_COUNTERS pmc;
            GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
            long totalMem = (long)pmc.WorkingSetSize;
            long freeMem = 0L;
#else
            long maxMem = (long)sysconf(_SC_PHYS_PAGES) * (long)sysconf(_SC_PAGESIZE);
            struct rusage ru;
            getrusage(RUSAGE_SELF, &ru);

#ifdef __APPLE__
            long totalMem = (long)ru.ru_maxrss;
#else
            long totalMem = (long)ru.ru_maxrss * 1024L;
#endif
            long freeMem = 0L;
#endif
            long usedMem = totalMem - freeMem;
            long maxMB = maxMem / (1024L * 1024L);
            long totalMB = totalMem / (1024L * 1024L);
            long usedMB = usedMem / (1024L * 1024L);
            char var23[256];
            snprintf(var23, sizeof(var23), "Used memory: %ld%% (%ldMB) of %ldMB",
                     maxMem > 0 ? usedMem * 100L / maxMem : 0L, usedMB, maxMB);
            Gui_drawString(&self->base, var8, var23,
                           var6 - FontRenderer_getStringWidth(var8, var23) - 2, 2, 14737632);
            snprintf(var23, sizeof(var23), "Allocated memory: %ld%% (%ldMB)",
                     maxMem > 0 ? totalMem * 100L / maxMem : 0L, totalMB);
            Gui_drawString(&self->base, var8, var23,
                           var6 - FontRenderer_getStringWidth(var8, var23) - 2, 12, 14737632);
        }
        char posX[64];
        snprintf(posX, sizeof(posX), "x: %f", ((Entity *)self->mc->thePlayer)->posX);
        char posY[64];
        snprintf(posY, sizeof(posY), "y: %f", ((Entity *)self->mc->thePlayer)->posY);
        char posZ[64];
        snprintf(posZ, sizeof(posZ), "z: %f", ((Entity *)self->mc->thePlayer)->posZ);
        char posF[64];
        snprintf(
            posF, sizeof(posF), "f: %d",
            (MathHelper_floor_double(
                 (double)(((Entity *)self->mc->thePlayer)->rotationYaw * 4.0F / 360.0F) + 0.5) &
             3));
        Gui_drawString(&self->base, var8, posX, 2, 64, 14737632);
        Gui_drawString(&self->base, var8, posY, 2, 72, 14737632);
        Gui_drawString(&self->base, var8, posZ, 2, 80, 14737632);
        Gui_drawString(&self->base, var8, posF, 2, 88, 14737632);
        R_popMatrix();
    }
    if (self->recordPlayingUpFor > 0) {
        float var25 = (float)self->recordPlayingUpFor - var1;
        var16 = (int)(var25 * 256.0F / 20.0F);
        if (var16 > 255) {
            var16 = 255;
        }
        if (var16 > 0) {
            R_pushMatrix();
            R_translatef((float)(var6 / 2), (float)(var7 - 48), 0.0F);
            R_enable(R_BLEND);
            R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
            var17 = 16777215;
            if (self->field_22065_l) {
                var17 = Color_HSBtoRGB(var25 / 50.0F, 0.7F, 0.6F) & 16777215;
            }
            int strWidth = FontRenderer_getStringWidth(var8, self->recordPlaying);
            FontRenderer_drawString(var8, self->recordPlaying, -strWidth / 2, -4,
                                    (int)((unsigned int)var17 + ((unsigned int)var16 << 24)));
            R_disable(R_BLEND);
            R_popMatrix();
        }
    }
    int var26 = 10;
    int var31 = 0;

    if (self->mc->currentScreen != NULL && self->mc->currentScreen->guiType == GUI_TYPE_CHAT) {
        var26 = 20;
        var31 = 1;
    }
    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    R_disable(R_ALPHA_TEST);
    R_pushMatrix();
    R_translatef(0.0F, (float)(var7 - 48), 0.0F);
    for (var17 = 0; var17 < self->chatMessageCount && var17 < var26; ++var17) {
        if (self->chatMessageList[var17]->updateCounter < 200 || var31) {
            double var32 = (double)self->chatMessageList[var17]->updateCounter / 200.0;
            var32 = 1.0 - var32;
            var32 *= 10.0;
            if (var32 < 0.0) {
                var32 = 0.0;
            }
            if (var32 > 1.0) {
                var32 = 1.0;
            }
            var32 *= var32;
            int var20 = (int)(255.0 * var32);
            if (var31) {
                var20 = 255;
            }
            if (var20 > 0) {
                int var33 = 2;
                int var22 = -var17 * 9;
                const char *var23 = self->chatMessageList[var17]->message;
                Gui_drawRect(&self->base, var33, var22 - 1, var33 + 320, var22 + 8,
                             (int)((unsigned int)(var20 / 2) << 24));
                R_enable(R_BLEND);
                FontRenderer_drawStringWithShadow(var8, var23, var33, var22,
                                                  (int)(16777215U + ((unsigned int)var20 << 24)));
            }
        }
    }
    R_popMatrix();
    R_enable(R_ALPHA_TEST);
    R_disable(R_BLEND);
}

void GuiIngame_renderPumpkinBlur(GuiIngame *self, int var1, int var2) {
    R_disable(R_DEPTH_TEST);
    R_depthMask(R_FALSE);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
    R_disable(R_ALPHA_TEST);
    R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "%blur%/misc/pumpkinblur.png"));
    Tessellator *var3 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var3);
    Tessellator_addVertexWithUV(var3, 0.0, (double)var2, -90.0, 0.0, 1.0);
    Tessellator_addVertexWithUV(var3, (double)var1, (double)var2, -90.0, 1.0, 1.0);
    Tessellator_addVertexWithUV(var3, (double)var1, 0.0, -90.0, 1.0, 0.0);
    Tessellator_addVertexWithUV(var3, 0.0, 0.0, -90.0, 0.0, 0.0);
    Tessellator_draw(var3);
    R_depthMask(R_TRUE);
    R_enable(R_DEPTH_TEST);
    R_enable(R_ALPHA_TEST);
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
}

void GuiIngame_renderVignette(GuiIngame *self, float var1, int var2, int var3) {
    var1 = 1.0F - var1;
    if (var1 < 0.0F) {
        var1 = 0.0F;
    }
    if (var1 > 1.0F) {
        var1 = 1.0F;
    }
    self->prevVignetteBrightness = (float)((double)self->prevVignetteBrightness +
                                           (double)(var1 - self->prevVignetteBrightness) * 0.01);
    R_disable(R_DEPTH_TEST);
    R_depthMask(R_FALSE);

#ifdef __EMSCRIPTEN__
    R_colorMask(R_TRUE, R_TRUE, R_TRUE, R_FALSE);
#endif
    R_blendFunc(R_ZERO, R_ONE_MINUS_SRC_COLOR);
    R_color4f(self->prevVignetteBrightness, self->prevVignetteBrightness,
              self->prevVignetteBrightness, 1.0F);
    R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "%blur%/misc/vignette.png"));
    Tessellator *var4 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var4);
    Tessellator_addVertexWithUV(var4, 0.0, (double)var3, -90.0, 0.0, 1.0);
    Tessellator_addVertexWithUV(var4, (double)var2, (double)var3, -90.0, 1.0, 1.0);
    Tessellator_addVertexWithUV(var4, (double)var2, 0.0, -90.0, 1.0, 0.0);
    Tessellator_addVertexWithUV(var4, 0.0, 0.0, -90.0, 0.0, 0.0);
    Tessellator_draw(var4);
#ifdef __EMSCRIPTEN__
    R_colorMask(R_TRUE, R_TRUE, R_TRUE, R_TRUE);
#endif
    R_depthMask(R_TRUE);
    R_enable(R_DEPTH_TEST);
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
}

void GuiIngame_renderPortalOverlay(GuiIngame *self, float var1, int var2, int var3) {
    if (var1 < 1.0F) {
        var1 *= var1;
        var1 *= var1;
        var1 = var1 * 0.8F + 0.2F;
    }
    R_disable(R_ALPHA_TEST);
    R_disable(R_DEPTH_TEST);
    R_depthMask(R_FALSE);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    R_color4f(1.0F, 1.0F, 1.0F, var1);
    R_bindTexture(RenderEngine_getTexture(self->mc->renderEngine, "/terrain.png"));
    float var4 = (float)(Block_portal->blockIndexInTexture % 16) / 16.0F;
    float var5 = (float)(Block_portal->blockIndexInTexture / 16) / 16.0F;
    float var6 = (float)(Block_portal->blockIndexInTexture % 16 + 1) / 16.0F;
    float var7 = (float)(Block_portal->blockIndexInTexture / 16 + 1) / 16.0F;
    Tessellator *var8 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var8);
    Tessellator_addVertexWithUV(var8, 0.0, (double)var3, -90.0, (double)var4, (double)var7);
    Tessellator_addVertexWithUV(var8, (double)var2, (double)var3, -90.0, (double)var6,
                                (double)var7);
    Tessellator_addVertexWithUV(var8, (double)var2, 0.0, -90.0, (double)var6, (double)var5);
    Tessellator_addVertexWithUV(var8, 0.0, 0.0, -90.0, (double)var4, (double)var5);
    Tessellator_draw(var8);
    R_depthMask(R_TRUE);
    R_enable(R_DEPTH_TEST);
    R_enable(R_ALPHA_TEST);
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
}

void GuiIngame_renderInventorySlot(GuiIngame *self, int var1, int var2, int var3, float var4) {
    ItemStack *var5 = ((EntityPlayer *)self->mc->thePlayer)->inventory.mainInventory[var1];
    if (var5 != NULL) {
        float var6 = (float)var5->animationsToGo - var4;
        if (var6 > 0.0F) {
            R_pushMatrix();
            float var7 = 1.0F + var6 / 5.0F;
            R_translatef((float)(var2 + 8), (float)(var3 + 12), 0.0F);
            R_scalef(1.0F / var7, (var7 + 1.0F) / 2.0F, 1.0F);
            R_translatef((float)(-(var2 + 8)), (float)(-(var3 + 12)), 0.0F);
        }
        RenderItem_renderItemIntoGUI(self->itemRenderer, self->mc->fontRenderer,
                                     self->mc->renderEngine, var5, var2, var3);
        if (var6 > 0.0F) {
            R_popMatrix();
        }
        RenderItem_renderItemOverlayIntoGUI(self->itemRenderer, self->mc->fontRenderer,
                                            self->mc->renderEngine, var5, var2, var3);
    }
}

void GuiIngame_updateTick(GuiIngame *self) {
    if (self->recordPlayingUpFor > 0) {
        --self->recordPlayingUpFor;
    }
    ++self->updateCounter;
    for (int var1 = 0; var1 < self->chatMessageCount; ++var1) {
        ++self->chatMessageList[var1]->updateCounter;
    }
}

void GuiIngame_clearChatMessages(GuiIngame *self) {
    for (int i = 0; i < self->chatMessageCount; ++i) {
        ChatLine_free(self->chatMessageList[i]);
        self->chatMessageList[i] = NULL;
    }
    self->chatMessageCount = 0;
}

void GuiIngame_addChatMessage(GuiIngame *self, const char *var1) {
    while (FontRenderer_getStringWidth(self->mc->fontRenderer, var1) > 320) {
        int var2;

        for (var2 = 1; var2 < (int)strlen(var1); ++var2) {
            char sub[512];
            int copyLen = (var2 + 1) < (int)(sizeof(sub) - 1) ? (var2 + 1) : (int)(sizeof(sub) - 1);
            strncpy(sub, var1, (size_t)copyLen);
            sub[copyLen] = '\0';
            if (FontRenderer_getStringWidth(self->mc->fontRenderer, sub) > 320)
                break;
        }

        {
            char sub[512];
            int copyLen = var2 < (int)(sizeof(sub) - 1) ? var2 : (int)(sizeof(sub) - 1);
            strncpy(sub, var1, (size_t)copyLen);
            sub[copyLen] = '\0';
            GuiIngame_addChatMessage(self, sub);
        }
        var1 = var1 + var2;
    }

    ChatLine *newLine = (ChatLine *)calloc(1, sizeof(ChatLine));
    if (!newLine)
        return;
    ChatLine_construct(newLine, var1);

    if (self->chatMessageCount >= GUI_INGAME_CHAT_MAX) {
        ChatLine_free(self->chatMessageList[GUI_INGAME_CHAT_MAX - 1]);
        self->chatMessageList[GUI_INGAME_CHAT_MAX - 1] = NULL;
        self->chatMessageCount = GUI_INGAME_CHAT_MAX - 1;
    }

    for (int i = self->chatMessageCount; i > 0; --i) {
        self->chatMessageList[i] = self->chatMessageList[i - 1];
    }
    self->chatMessageList[0] = newLine;
    ++self->chatMessageCount;

    while (self->chatMessageCount > 50) {
        ChatLine_free(self->chatMessageList[self->chatMessageCount - 1]);
        self->chatMessageList[self->chatMessageCount - 1] = NULL;
        --self->chatMessageCount;
    }
}

void GuiIngame_setRecordPlayingMessage(GuiIngame *self, const char *var1) {
    snprintf(self->recordPlaying, sizeof(self->recordPlaying), "Now playing: %s", var1);
    self->recordPlayingUpFor = 60;
    self->field_22065_l = 1;
}

void GuiIngame_addChatMessageTranslate(GuiIngame *self, const char *var1) {
    StringTranslate *var2 = StringTranslate_getInstance();
    const char *var3 = StringTranslate_translateKey(var2, var1);
    GuiIngame_addChatMessage(self, var3);
}
