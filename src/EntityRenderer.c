#include "Renderer.h"
#include "EntityRenderer.h"
#include "ScaledResolution.h"
#include "Minecraft.h"
#include "GuiScreen.h"
#include "GuiIngame.h"
#include "GuiParticle.h"
#include "EntityPlayerSP.h"
#include "EntityPlayer.h"
#include "ClippingHelperImpl.h"
#include "Frustrum.h"
#include "Tessellator.h"
#include "RenderHelper.h"
#include "Block.h"
#include "Material.h"
#include "MathHelper.h"
#include "BiomeGenBase.h"
#include "WorldChunkManager.h"
#include "PlayerControllerTest.h"
#include "EffectRenderer.h"
#include "EntitySmokeFX.h"
#include "EntityRainFX.h"
#include "EntityFX.h"
#include "RenderGlobal.h"
#include "ChunkProviderLoadOrGenerate.h"
#include "Vec3D.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GLFW/glfw3.h>

int EntityRenderer_field_28135_a = 0;
int EntityRenderer_anaglyphField = 0;

static float EntityRenderer_getFOVModifier(EntityRenderer *self, float partialTick);
static void EntityRenderer_hurtCameraEffect(EntityRenderer *self, float partialTick);
static void EntityRenderer_setupViewBobbing(EntityRenderer *self, float partialTick);
static void EntityRenderer_orientCamera(EntityRenderer *self, float partialTick);
static void EntityRenderer_setupCameraTransform(EntityRenderer *self, float partialTick,
                                                int anaglyphPass);
static void EntityRenderer_func_4135_b(EntityRenderer *self, float partialTick, int anaglyphPass);
static void EntityRenderer_addRainParticles(EntityRenderer *self);
static void EntityRenderer_updateFogColor(EntityRenderer *self, float partialTick);
static void EntityRenderer_setupFog(EntityRenderer *self, int mode, float partialTick);

static int64_t millis_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000LL + (int64_t)ts.tv_nsec / 1000000LL;
}
static int64_t nanos_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000000000LL + (int64_t)ts.tv_nsec;
}

static void mc_gluPerspective(float fovY, float aspect, float zNear, float zFar) {
    R_perspective(fovY, aspect, zNear, zFar);
}

EntityRenderer *EntityRenderer_create(Minecraft *mc) {
    EntityRenderer *self = (EntityRenderer *)calloc(1, sizeof(EntityRenderer));
    self->mc = mc;
    ItemRenderer_init(&self->itemRenderer, mc);
    self->field_22228_r = 4.0f;
    self->field_22227_s = 4.0f;
    self->cameraZoom = 1.0;
    self->prevFrameTime = millis_now();
    JavaRandom_init(&self->random, (long long)time(NULL));
    return self;
}

void EntityRenderer_destroy(EntityRenderer *self) { free(self); }

void EntityRenderer_updateRenderer(EntityRenderer *self) {
    self->fogColor2 = self->fogColor1;
    self->field_22227_s = self->field_22228_r;
    self->field_22225_u = self->field_22226_t;
    self->field_22223_w = self->field_22224_v;
    self->field_22221_y = self->field_22222_x;
    self->field_22230_A = self->field_22220_z;

    Minecraft *mc = self->mc;
    if (mc->renderViewEntity == NULL)
        mc->renderViewEntity = (EntityLiving *)mc->thePlayer;

    Entity *ve = (Entity *)mc->renderViewEntity;
    if (ve == NULL)
        return;
    float brightness = mc->theWorld->vtable->getLightBrightness(
        mc->theWorld, MathHelper_floor_double(ve->posX), MathHelper_floor_double(ve->posY),
        MathHelper_floor_double(ve->posZ));

    float t = (float)(3 - mc->gameSettings->renderDistance) / 3.0f;
    float br = brightness * (1.0f - t) + t;
    self->fogColor1 += (br - self->fogColor1) * 0.1f;

    ++self->rendererUpdateCount;
    ItemRenderer_updateEquippedItem(&self->itemRenderer);
    EntityRenderer_addRainParticles(self);
}

void EntityRenderer_getMouseOver(EntityRenderer *self, float partialTick) {
    Minecraft *mc = self->mc;
    if (mc->renderViewEntity == NULL || mc->theWorld == NULL)
        return;

    double var2 = (double)PlayerController_getBlockReachDistance(mc->playerController);
    Entity *ve = (Entity *)mc->renderViewEntity;
    MovingObjectPosition_free(mc->objectMouseOver);
    mc->objectMouseOver = Entity_rayTrace(ve, var2, partialTick);

    double var4 = var2;

    Vec3D var6_val;
    var6_val.xCoord = ve->prevPosX + (ve->posX - ve->prevPosX) * (double)partialTick;
    var6_val.yCoord = ve->prevPosY + (ve->posY - ve->prevPosY) * (double)partialTick;
    var6_val.zCoord = ve->prevPosZ + (ve->posZ - ve->prevPosZ) * (double)partialTick;
    Vec3D *var6 = &var6_val;
    if (mc->objectMouseOver != NULL) {
        var4 = Vec3D_distanceTo(mc->objectMouseOver->hitVec, var6);
    }

    if (PlayerControllerTest_isInstance(mc->playerController)) {
        var2 = 32.0;
    } else {
        if (var4 > 3.0)
            var4 = 3.0;
        var2 = var4;
    }

    Vec3D *var7 = Entity_getLook(ve, partialTick);

    Vec3D *var8 =
        Vec3D_addVector(var6, var7->xCoord * var2, var7->yCoord * var2, var7->zCoord * var2);

    self->pointedEntity = NULL;
    float var9 = 1.0f;

    AxisAlignedBB *_searchBB =
        AxisAlignedBB_expand(AxisAlignedBB_addCoord(&ve->boundingBox, var7->xCoord * var2,
                                                    var7->yCoord * var2, var7->zCoord * var2),
                             (double)var9, (double)var9, (double)var9);
#define ENTITY_LIST_MAX 512
    Entity *var10[ENTITY_LIST_MAX];
    int var10_size = World_getEntitiesWithinAABBExcludingEntity(mc->theWorld, ve, _searchBB, var10,
                                                                ENTITY_LIST_MAX);
#undef ENTITY_LIST_MAX
    double var11 = 0.0;

    for (int var13 = 0; var13 < var10_size; ++var13) {
        Entity *var14 = var10[var13];
        if (var14->vtable->canBeCollidedWith(var14)) {
            float var15 = var14->vtable->getCollisionBorderSize(var14);
            AxisAlignedBB var16_val = var14->boundingBox;
            AxisAlignedBB *var16 =
                AxisAlignedBB_expand(&var16_val, (double)var15, (double)var15, (double)var15);
            MovingObjectPosition *var17 = AxisAlignedBB_func_1169_a(var16, var6, var8);
            if (AxisAlignedBB_isVecInside(var16, var6)) {
                if (0.0 < var11 || var11 == 0.0) {
                    self->pointedEntity = var14;
                    var11 = 0.0;
                }
            } else if (var17 != NULL) {
                double var18 = Vec3D_distanceTo(var6, var17->hitVec);
                if (var18 < var11 || var11 == 0.0) {
                    self->pointedEntity = var14;
                    var11 = var18;
                }
            }
            MovingObjectPosition_free(var17);
        }
    }

    if (self->pointedEntity != NULL && !PlayerControllerTest_isInstance(mc->playerController)) {
        MovingObjectPosition_free(mc->objectMouseOver);
        mc->objectMouseOver = MovingObjectPosition_createEntity(self->pointedEntity);
    }
}

void EntityRenderer_updateCameraAndRender(EntityRenderer *self, float partialTick) {
    Minecraft *mc = self->mc;

#ifndef __EMSCRIPTEN__
    GLFWwindow *_win = (GLFWwindow *)Minecraft_getWindow();
    if (!_win || !glfwGetWindowAttrib(_win, GLFW_FOCUSED)) {
        if (millis_now() - self->prevFrameTime > 500LL) {
            Minecraft_displayInGameMenu(mc);
        }
    } else {
        self->prevFrameTime = millis_now();
    }
#else
    self->prevFrameTime = millis_now();
#endif

    if (mc->inGameHasFocus) {

        MouseHelper_mouseXYChange(&mc->mouseHelper);
        float sens = mc->gameSettings->mouseSensitivity * 0.6f + 0.2f;
        float s3 = sens * sens * sens * 8.0f;
        float dx = (float)mc->mouseHelper.deltaX * s3;
        float dy = (float)mc->mouseHelper.deltaY * s3;
        int inv = mc->gameSettings->invertMouse ? -1 : 1;

        if (mc->gameSettings->smoothCamera) {
            dx = MouseFilter_func_22386_a(&self->mouseFilterXAxis, dx, 0.05f * s3);
            dy = MouseFilter_func_22386_a(&self->mouseFilterYAxis, dy, 0.05f * s3);
        }

        if (mc->thePlayer)
            Entity_func_346_d((Entity *)mc->thePlayer, dx, dy * (float)inv);
    }

    if (!mc->skipRenderWorld) {
        EntityRenderer_field_28135_a = mc->gameSettings->anaglyph;

        ScaledResolution var13;
        ScaledResolution_init(&var13, mc->gameSettings, mc->displayWidth, mc->displayHeight);
        int var14 = var13.scaledWidth;
        int var15 = var13.scaledHeight;
        double cx, cy;
        Minecraft_getCursorPos(&cx, &cy);
        int var16 = (mc->displayWidth > 0) ? (int)(cx * var14 / mc->displayWidth) : 0;
        int var17 = (mc->displayHeight > 0) ? ((int)(cy * var15 / mc->displayHeight) - 1) : 0;

        short limitFPS = 200;
        if (mc->gameSettings->limitFramerate == 1)
            limitFPS = 120;
        if (mc->gameSettings->limitFramerate == 2)
            limitFPS = 40;

        if (mc->theWorld != NULL) {

            if (mc->gameSettings->limitFramerate == 0) {
                EntityRenderer_renderWorld(self, partialTick, 0LL);
            } else {
                EntityRenderer_renderWorld(self, partialTick,
                                           self->field_28133_I + (int64_t)(1000000000 / limitFPS));
            }

            if (mc->gameSettings->limitFramerate == 2) {
                int64_t var8 =
                    (self->field_28133_I + (int64_t)(1000000000 / limitFPS) - nanos_now()) /
                    1000000LL;
                if (var8 > 0LL && var8 < 500LL) {
                    struct timespec ts = {0, var8 * 1000000LL};
                    nanosleep(&ts, NULL);
                }
            }

            self->field_28133_I = nanos_now();

            if (!mc->gameSettings->hideGUI || mc->currentScreen != NULL) {
                GuiIngame_renderGameOverlay(mc->ingameGUI, partialTick, mc->currentScreen != NULL,
                                            var16, var17);
            }
        } else {
            R_viewport(0, 0, mc->displayWidth, mc->displayHeight);
            R_matrixMode(R_PROJECTION);
            R_loadIdentity();
            R_matrixMode(R_MODELVIEW);
            R_loadIdentity();
            EntityRenderer_func_905_b(self);
            if (mc->gameSettings->limitFramerate == 2) {
                int64_t var8 =
                    (self->field_28133_I + (int64_t)(1000000000 / limitFPS) - nanos_now()) /
                    1000000LL;
                if (var8 < 0LL) {
                    var8 += 10LL;
                }
                if (var8 > 0LL && var8 < 500LL) {
                    struct timespec ts = {0, var8 * 1000000LL};
                    nanosleep(&ts, NULL);
                }
            }
            self->field_28133_I = nanos_now();
        }

        if (mc->currentScreen != NULL) {
            R_clear(R_DEPTH_BUFFER_BIT);
            mc->currentScreen->vtable->drawScreen(mc->currentScreen, var16, var17, partialTick);
            if (mc->currentScreen != NULL && mc->currentScreen->field_25091_h != NULL) {
                GuiParticle_func_25087_a(mc->currentScreen->field_25091_h, partialTick);
            }
        }
    }
}

void EntityRenderer_renderWorld(EntityRenderer *self, float partialTick, int64_t targetTimeNs) {
    Minecraft *mc = self->mc;
    RenderGlobal *var5 = mc->renderGlobal;

    R_enable(R_CULL_FACE);
    R_enable(R_DEPTH_TEST);

    if (mc->renderViewEntity == NULL)
        mc->renderViewEntity = (EntityLiving *)mc->thePlayer;

    EntityRenderer_getMouseOver(self, partialTick);

    EntityLiving *var4 = mc->renderViewEntity;
    if (var4 == NULL)
        return;
    Entity *vee = (Entity *)var4;

    double var7 = vee->lastTickPosX + (vee->posX - vee->lastTickPosX) * (double)partialTick;
    double var9 = vee->lastTickPosY + (vee->posY - vee->lastTickPosY) * (double)partialTick;
    double var11 = vee->lastTickPosZ + (vee->posZ - vee->lastTickPosZ) * (double)partialTick;

    IChunkProvider *var13 = World_getIChunkProvider(mc->theWorld);
    if (var13 != NULL && var13->vtable == &ChunkProviderLoadOrGenerate_vtable) {
        ChunkProviderLoadOrGenerate *var14 = (ChunkProviderLoadOrGenerate *)var13;
        int var15 = MathHelper_floor_float((float)((int)var7)) >> 4;
        int var16 = MathHelper_floor_float((float)((int)var11)) >> 4;
        ChunkProviderLoadOrGenerate_setCurrentChunkOver(var14, var15, var16);
    }

    for (int var18 = 0; var18 < 2; ++var18) {
        if (mc->gameSettings->anaglyph) {
            EntityRenderer_anaglyphField = var18;
            if (var18 == 0)
                R_colorMask(R_FALSE, R_TRUE, R_TRUE, R_FALSE);
            else
                R_colorMask(R_TRUE, R_FALSE, R_FALSE, R_FALSE);
        }

        R_viewport(0, 0, mc->displayWidth, mc->displayHeight);
        EntityRenderer_updateFogColor(self, partialTick);
        R_clear(R_DEPTH_BUFFER_BIT | R_COLOR_BUFFER_BIT);
        R_enable(R_CULL_FACE);
        EntityRenderer_setupCameraTransform(self, partialTick, var18);

        ClippingHelperImpl_getInstance();

        if (mc->gameSettings->renderDistance < 2) {
            EntityRenderer_setupFog(self, -1, partialTick);
            RenderGlobal_renderSky(var5, partialTick);
        }

        R_enable(R_FOG);
        EntityRenderer_setupFog(self, 1, partialTick);
        if (mc->gameSettings->ambientOcclusion)
            R_shadeModel(R_SMOOTH);

        Frustrum var19;
        Frustrum_init(&var19);
        Frustrum_setPosition(&var19, var7, var9, var11);
        RenderGlobal_clipRenderersByFrustrum(var5, (ICamera *)&var19, partialTick);

        if (var18 == 0) {
            while (!RenderGlobal_updateRenderers(var5, var4, 0) && targetTimeNs != 0LL) {
                int64_t var20 = targetTimeNs - nanos_now();
                if (var20 < 0LL || var20 > 1000000000LL)
                    break;
            }
        }

        EntityRenderer_setupFog(self, 0, partialTick);
        R_enable(R_FOG);
        R_bindTexture(RenderEngine_getTexture(mc->renderEngine, "/terrain.png"));
        RenderHelper_disableStandardItemLighting();
        RenderGlobal_sortAndRender(var5, var4, 0, (double)partialTick);

        R_shadeModel(R_FLAT);
        RenderHelper_enableStandardItemLighting();

        {
            Vec3D *var4pos;
            if (partialTick == 1.0f)
                var4pos = Vec3D_createVector(vee->posX, vee->posY, vee->posZ);
            else
                var4pos = Vec3D_createVector(
                    vee->prevPosX + (vee->posX - vee->prevPosX) * (double)partialTick,
                    vee->prevPosY + (vee->posY - vee->prevPosY) * (double)partialTick,
                    vee->prevPosZ + (vee->posZ - vee->prevPosZ) * (double)partialTick);
            RenderGlobal_renderEntities(var5, var4pos, (ICamera *)&var19, partialTick);
        }

        EffectRenderer_func_1187_b(mc->effectRenderer, (Entity *)var4, partialTick);

        RenderHelper_disableStandardItemLighting();
        EntityRenderer_setupFog(self, 0, partialTick);

        EffectRenderer_renderParticles(mc->effectRenderer, (Entity *)var4, partialTick);

        if (mc->objectMouseOver != NULL && Entity_isInsideOfMaterial(vee, Material_water) &&
            vee->isPlayer) {
            EntityPlayer *var21 = (EntityPlayer *)var4;
            R_disable(R_ALPHA_TEST);
            RenderGlobal_drawBlockBreaking(var5, var21, mc->objectMouseOver, 0,
                                           InventoryPlayer_getCurrentItem(&var21->inventory),
                                           partialTick);
            RenderGlobal_drawSelectionBox(var5, var21, mc->objectMouseOver, 0,
                                          InventoryPlayer_getCurrentItem(&var21->inventory),
                                          partialTick);
            R_enable(R_ALPHA_TEST);
        }

        R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
        EntityRenderer_setupFog(self, 0, partialTick);
        R_enable(R_BLEND);
        R_disable(R_CULL_FACE);
        R_bindTexture(RenderEngine_getTexture(mc->renderEngine, "/terrain.png"));

        if (mc->gameSettings->fancyGraphics) {
            if (mc->gameSettings->ambientOcclusion)
                R_shadeModel(R_SMOOTH);
            R_colorMask(R_FALSE, R_FALSE, R_FALSE, R_FALSE);
            int var16 = RenderGlobal_sortAndRender(var5, var4, 1, (double)partialTick);
            if (mc->gameSettings->anaglyph) {
                if (EntityRenderer_anaglyphField == 0)
                    R_colorMask(R_FALSE, R_TRUE, R_TRUE, R_TRUE);
                else
                    R_colorMask(R_TRUE, R_FALSE, R_FALSE, R_TRUE);
            } else {
                R_colorMask(R_TRUE, R_TRUE, R_TRUE, R_TRUE);
            }
            if (var16 > 0)
                RenderGlobal_renderAllRenderLists(var5, 1, (double)partialTick);
            R_shadeModel(R_FLAT);
        } else {
            RenderGlobal_sortAndRender(var5, var4, 1, (double)partialTick);
        }

        R_depthMask(R_TRUE);
        R_enable(R_CULL_FACE);
        R_disable(R_BLEND);

        if (self->cameraZoom == 1.0 && vee->isPlayer && mc->objectMouseOver != NULL &&
            !Entity_isInsideOfMaterial(vee, Material_water)) {
            EntityPlayer *var21 = (EntityPlayer *)var4;
            R_disable(R_ALPHA_TEST);
            RenderGlobal_drawBlockBreaking(var5, var21, mc->objectMouseOver, 0,
                                           InventoryPlayer_getCurrentItem(&var21->inventory),
                                           partialTick);
            RenderGlobal_drawSelectionBox(var5, var21, mc->objectMouseOver, 0,
                                          InventoryPlayer_getCurrentItem(&var21->inventory),
                                          partialTick);
            R_enable(R_ALPHA_TEST);
        }

        EntityRenderer_renderRainSnow(self, partialTick);
        R_disable(R_FOG);

        EntityRenderer_setupFog(self, 0, partialTick);
        R_enable(R_FOG);
        RenderGlobal_renderClouds(var5, partialTick);
        R_disable(R_FOG);
        EntityRenderer_setupFog(self, 1, partialTick);

        if (self->cameraZoom == 1.0) {
            R_clear(R_DEPTH_BUFFER_BIT);
            EntityRenderer_func_4135_b(self, partialTick, var18);
        }

        if (!mc->gameSettings->anaglyph)
            return;
    }

    R_colorMask(R_TRUE, R_TRUE, R_TRUE, R_FALSE);
}

void EntityRenderer_func_905_b(EntityRenderer *self) {
    Minecraft *mc = self->mc;
    ScaledResolution var1;
    ScaledResolution_init(&var1, mc->gameSettings, mc->displayWidth, mc->displayHeight);
    R_clear(R_DEPTH_BUFFER_BIT);
    R_matrixMode(R_PROJECTION);
    R_loadIdentity();
    R_ortho(0.0, var1.field_25121_a, var1.field_25120_b, 0.0, 1000.0, 3000.0);
    R_matrixMode(R_MODELVIEW);
    R_loadIdentity();
    R_translatef(0.0f, 0.0f, -2000.0f);
}

void EntityRenderer_renderRainSnow(EntityRenderer *self, float partialTick) {
    Minecraft *mc = self->mc;
    float rainStrength = World_getRainStrength(mc->theWorld, partialTick);
    if (rainStrength <= 0.0f)
        return;

    EntityLiving *ve = mc->renderViewEntity;
    Entity *vee = (Entity *)ve;
    World *w = mc->theWorld;
    Tessellator *tess = &Tessellator_instance;

    int centerX = MathHelper_floor_double(vee->posX);
    int centerY = MathHelper_floor_double(vee->posY);
    int centerZ = MathHelper_floor_double(vee->posZ);

    double rx = vee->lastTickPosX + (vee->posX - vee->lastTickPosX) * (double)partialTick;
    double ry = vee->lastTickPosY + (vee->posY - vee->lastTickPosY) * (double)partialTick;
    double rz = vee->lastTickPosZ + (vee->posZ - vee->lastTickPosZ) * (double)partialTick;

    int floorRy = MathHelper_floor_double(ry);

    R_disable(R_CULL_FACE);
    R_normal3f(0.0f, 1.0f, 0.0f);
    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    R_alphaFunc(R_GREATER, 0.01f);

    int range = mc->gameSettings->fancyGraphics ? 10 : 5;

    R_bindTexture(RenderEngine_getTexture(mc->renderEngine, "/environment/snow.png"));
    int biomeCount = (range * 2 + 1) * (range * 2 + 1);
    BiomeGenBase **biomes =
        WorldChunkManager_loadBiomeData(World_getWorldChunkManager(w), NULL, centerX - range,
                                        centerZ - range, range * 2 + 1, range * 2 + 1);

    int bi = 0;
    for (int bx = centerX - range; bx <= centerX + range; bx++) {
        for (int bz = centerZ - range; bz <= centerZ + range; bz++) {
            BiomeGenBase *biome = (bi < biomeCount) ? biomes[bi++] : NULL;
            if (biome == NULL || !biome->enableSnow)
                continue;

            int top = World_findTopSolidBlock(w, bx, bz);
            if (top < 0)
                top = 0;

            int var23 = (top < floorRy) ? floorRy : top;
            int yLo = centerY - range;
            int yHi = centerY + range;
            int drawLo = (yLo < top) ? top : yLo;
            int drawHi = (yHi < top) ? top : yHi;
            if (drawLo == drawHi)
                continue;

            JavaRandom_setSeed(
                &self->random,
                (long long)(int)((unsigned)bx * (unsigned)bx * 3121u + (unsigned)bx * 45238971u +
                                 (unsigned)bz * (unsigned)bz * 418711u + (unsigned)bz * 13761u));
            float tick = (float)self->rendererUpdateCount + partialTick;
            float uoff = ((float)(self->rendererUpdateCount & 511) + partialTick) / 512.0f;
            float rnd1 = JavaRandom_nextFloat(&self->random) +
                         tick * 0.01f * (float)JavaRandom_nextGaussian(&self->random);
            float rnd2 = JavaRandom_nextFloat(&self->random) +
                         tick * (float)JavaRandom_nextGaussian(&self->random) * 0.001f;
            double dx = (double)((float)bx + 0.5f) - vee->posX;
            double dz = (double)((float)bz + 0.5f) - vee->posZ;
            float dist = MathHelper_sqrt_double(dx * dx + dz * dz) / (float)range;
            float alpha = ((1.0f - dist * dist) * 0.3f + 0.5f) * rainStrength;

            float lv = w->vtable->getLightBrightness(w, bx, var23, bz);
            R_color4f(lv, lv, lv, alpha);

            float sc = 1.0f;
            Tessellator_startDrawingQuads(tess);
            Tessellator_setTranslationD(tess, -rx, -ry, -rz);
            Tessellator_addVertexWithUV(tess, (double)(bx + 0), (double)drawLo, (double)bz + 0.5,
                                        (double)(0.0f * sc + rnd1),
                                        (double)((float)drawLo * sc / 4 + uoff * sc + rnd2));
            Tessellator_addVertexWithUV(tess, (double)(bx + 1), (double)drawLo, (double)bz + 0.5,
                                        (double)(1.0f * sc + rnd1),
                                        (double)((float)drawLo * sc / 4 + uoff * sc + rnd2));
            Tessellator_addVertexWithUV(tess, (double)(bx + 1), (double)drawHi, (double)bz + 0.5,
                                        (double)(1.0f * sc + rnd1),
                                        (double)((float)drawHi * sc / 4 + uoff * sc + rnd2));
            Tessellator_addVertexWithUV(tess, (double)(bx + 0), (double)drawHi, (double)bz + 0.5,
                                        (double)(0.0f * sc + rnd1),
                                        (double)((float)drawHi * sc / 4 + uoff * sc + rnd2));
            Tessellator_addVertexWithUV(tess, (double)bx + 0.5, (double)drawLo, (double)(bz + 0),
                                        (double)(0.0f * sc + rnd1),
                                        (double)((float)drawLo * sc / 4 + uoff * sc + rnd2));
            Tessellator_addVertexWithUV(tess, (double)bx + 0.5, (double)drawLo, (double)(bz + 1),
                                        (double)(1.0f * sc + rnd1),
                                        (double)((float)drawLo * sc / 4 + uoff * sc + rnd2));
            Tessellator_addVertexWithUV(tess, (double)bx + 0.5, (double)drawHi, (double)bz + 1,
                                        (double)(1.0f * sc + rnd1),
                                        (double)((float)drawHi * sc / 4 + uoff * sc + rnd2));
            Tessellator_addVertexWithUV(tess, (double)bx + 0.5, (double)drawHi, (double)(bz + 0),
                                        (double)(0.0f * sc + rnd1),
                                        (double)((float)drawHi * sc / 4 + uoff * sc + rnd2));
            Tessellator_setTranslationD(tess, 0.0, 0.0, 0.0);
            Tessellator_draw(tess);
        }
    }

    R_bindTexture(RenderEngine_getTexture(mc->renderEngine, "/environment/rain.png"));
    bi = 0;
    for (int bx = centerX - range; bx <= centerX + range; bx++) {
        for (int bz = centerZ - range; bz <= centerZ + range; bz++) {
            BiomeGenBase *biome = (bi < biomeCount) ? biomes[bi++] : NULL;
            if (biome == NULL || !BiomeGenBase_canSpawnLightningBolt(biome))
                continue;

            int top = World_findTopSolidBlock(w, bx, bz);
            int yLo = centerY - range;
            int yHi = centerY + range;
            int drawLo = (yLo < top) ? top : yLo;
            int drawHi = (yHi < top) ? top : yHi;
            if (drawLo == drawHi)
                continue;

            JavaRandom_setSeed(
                &self->random,
                (long long)(int)((unsigned)bx * (unsigned)bx * 3121u + (unsigned)bx * 45238971u +
                                 (unsigned)bz * (unsigned)bz * 418711u + (unsigned)bz * 13761u));
            float tickOff =
                ((float)(((unsigned)self->rendererUpdateCount +
                          (unsigned)bx * (unsigned)bx * 3121u + (unsigned)bx * 45238971u +
                          (unsigned)bz * (unsigned)bz * 418711u + (unsigned)bz * 13761u) &
                         31u) +
                 partialTick) /
                32.0f * (3.0f + JavaRandom_nextFloat(&self->random));
            double dx = (double)((float)bx + 0.5f) - vee->posX;
            double dz = (double)((float)bz + 0.5f) - vee->posZ;
            float dist = MathHelper_sqrt_double(dx * dx + dz * dz) / (float)range;
            float lv = w->vtable->getLightBrightness(w, bx, 128, bz) * 0.85f + 0.15f;
            float alpha = ((1.0f - dist * dist) * 0.5f + 0.5f) * rainStrength;
            R_color4f(lv, lv, lv, alpha);

            float sc = 1.0f;
            Tessellator_startDrawingQuads(tess);
            Tessellator_setTranslationD(tess, -rx, -ry, -rz);
            Tessellator_addVertexWithUV(tess, (double)(bx + 0), (double)drawLo, (double)bz + 0.5,
                                        (double)(0.0f * sc),
                                        (double)((float)drawLo * sc / 4 + tickOff * sc));
            Tessellator_addVertexWithUV(tess, (double)(bx + 1), (double)drawLo, (double)bz + 0.5,
                                        (double)(1.0f * sc),
                                        (double)((float)drawLo * sc / 4 + tickOff * sc));
            Tessellator_addVertexWithUV(tess, (double)(bx + 1), (double)drawHi, (double)bz + 0.5,
                                        (double)(1.0f * sc),
                                        (double)((float)drawHi * sc / 4 + tickOff * sc));
            Tessellator_addVertexWithUV(tess, (double)(bx + 0), (double)drawHi, (double)bz + 0.5,
                                        (double)(0.0f * sc),
                                        (double)((float)drawHi * sc / 4 + tickOff * sc));
            Tessellator_addVertexWithUV(tess, (double)bx + 0.5, (double)drawLo, (double)(bz + 0),
                                        (double)(0.0f * sc),
                                        (double)((float)drawLo * sc / 4 + tickOff * sc));
            Tessellator_addVertexWithUV(tess, (double)bx + 0.5, (double)drawLo, (double)(bz + 1),
                                        (double)(1.0f * sc),
                                        (double)((float)drawLo * sc / 4 + tickOff * sc));
            Tessellator_addVertexWithUV(tess, (double)bx + 0.5, (double)drawHi, (double)(bz + 1),
                                        (double)(1.0f * sc),
                                        (double)((float)drawHi * sc / 4 + tickOff * sc));
            Tessellator_addVertexWithUV(tess, (double)bx + 0.5, (double)drawHi, (double)(bz + 0),
                                        (double)(0.0f * sc),
                                        (double)((float)drawHi * sc / 4 + tickOff * sc));
            Tessellator_setTranslationD(tess, 0.0, 0.0, 0.0);
            Tessellator_draw(tess);
        }
    }

    free(biomes);
    R_enable(R_CULL_FACE);
    R_disable(R_BLEND);
    R_alphaFunc(R_GREATER, 0.1f);
}

static float EntityRenderer_getFOVModifier(EntityRenderer *self, float partialTick) {
    EntityLiving *ve = self->mc->renderViewEntity;
    float fov = 70.0f;
    if (Entity_isInsideOfMaterial((Entity *)ve, Material_water))
        fov = 60.0f;
    if (ve->health <= 0) {
        float t = (float)ve->deathTime + partialTick;
        fov /= (1.0f - 500.0f / (t + 500.0f)) * 2.0f + 1.0f;
    }
    return fov + self->field_22221_y + (self->field_22222_x - self->field_22221_y) * partialTick;
}

static void EntityRenderer_hurtCameraEffect(EntityRenderer *self, float partialTick) {
    EntityLiving *ve = self->mc->renderViewEntity;
    float t = (float)ve->hurtTime - partialTick;
    if (ve->health <= 0) {
        float d = (float)ve->deathTime + partialTick;
        R_rotatef(40.0f - 8000.0f / (d + 200.0f), 0.0f, 0.0f, 1.0f);
    }
    if (t >= 0.0f) {
        t /= (float)ve->maxHurtTime;
        t = MathHelper_sin(t * t * t * t * (float)M_PI);
        R_rotatef(-ve->attackedAtYaw, 0.0f, 1.0f, 0.0f);
        R_rotatef(-t * 14.0f, 0.0f, 0.0f, 1.0f);
        R_rotatef(ve->attackedAtYaw, 0.0f, 1.0f, 0.0f);
    }
}

static void EntityRenderer_setupViewBobbing(EntityRenderer *self, float partialTick) {
    Minecraft *mc = self->mc;

    if (!((Entity *)mc->renderViewEntity)->isPlayer)
        return;
    EntityPlayer *var2 = (EntityPlayer *)mc->renderViewEntity;
    float var3 =
        var2->base.base.distanceWalkedModified - var2->base.base.prevDistanceWalkedModified;
    float var4 = -(var2->base.base.distanceWalkedModified + var3 * partialTick);
    float var5 = var2->field_775_e + (var2->field_774_f - var2->field_775_e) * partialTick;
    float var6 =
        var2->base.cameraPitch + (var2->base.field_9328_R - var2->base.cameraPitch) * partialTick;
    R_translatef(MathHelper_sin(var4 * (float)M_PI) * var5 * 0.5f,
                 -(float)fabs(MathHelper_cos(var4 * (float)M_PI) * var5), 0.0f);
    R_rotatef(MathHelper_sin(var4 * (float)M_PI) * var5 * 3.0f, 0.0f, 0.0f, 1.0f);
    R_rotatef((float)fabs(MathHelper_cos(var4 * (float)M_PI - 0.2f) * var5) * 5.0f, 1.0f, 0.0f,
              0.0f);
    R_rotatef(var6, 1.0f, 0.0f, 0.0f);
}

static void EntityRenderer_orientCamera(EntityRenderer *self, float partialTick) {
    Minecraft *mc = self->mc;
    EntityLiving *var2 = mc->renderViewEntity;
    Entity *vee = (Entity *)var2;
    float var3 = var2->base.yOffset - 1.62f;
    double var4 = vee->prevPosX + (vee->posX - vee->prevPosX) * (double)partialTick;
    double var6 = vee->prevPosY + (vee->posY - vee->prevPosY) * (double)partialTick - (double)var3;
    double var8 = vee->prevPosZ + (vee->posZ - vee->prevPosZ) * (double)partialTick;

    R_rotatef(self->field_22230_A + (self->field_22220_z - self->field_22230_A) * partialTick, 0.0f,
              0.0f, 1.0f);

    int isSleeping = Entity_isPlayerSleeping((Entity *)var2);

    if (isSleeping) {
        var3 = (float)((double)var3 + 1.0);
        R_translatef(0.0f, 0.3f, 0.0f);
        if (!mc->gameSettings->field_22273_E) {
            int var10 = World_getBlockId(mc->theWorld, MathHelper_floor_double(vee->posX),
                                         MathHelper_floor_double(vee->posY),
                                         MathHelper_floor_double(vee->posZ));
            if (var10 == Block_blockBed->blockID) {
                int var11 = World_getBlockMetadata(mc->theWorld, MathHelper_floor_double(vee->posX),
                                                   MathHelper_floor_double(vee->posY),
                                                   MathHelper_floor_double(vee->posZ));
                int var12 = var11 & 3;
                R_rotatef((float)(var12 * 90), 0.0f, 1.0f, 0.0f);
            }
            R_rotatef(vee->prevRotationYaw +
                          (vee->rotationYaw - vee->prevRotationYaw) * partialTick + 180.0f,
                      0.0f, -1.0f, 0.0f);
            R_rotatef(vee->prevRotationPitch +
                          (vee->rotationPitch - vee->prevRotationPitch) * partialTick,
                      -1.0f, 0.0f, 0.0f);
        }
    } else if (mc->gameSettings->thirdPersonView) {
        double var27 = (double)(self->field_22227_s +
                                (self->field_22228_r - self->field_22227_s) * partialTick);
        float var13;
        float var28;
        if (mc->gameSettings->field_22273_E) {
            var28 = self->field_22225_u + (self->field_22226_t - self->field_22225_u) * partialTick;
            var13 = self->field_22223_w + (self->field_22224_v - self->field_22223_w) * partialTick;
            R_translatef(0.0f, 0.0f, (float)(-var27));
            R_rotatef(var13, 1.0f, 0.0f, 0.0f);
            R_rotatef(var28, 0.0f, 1.0f, 0.0f);
        } else {
            var28 = vee->rotationYaw;
            var13 = vee->rotationPitch;
            double var14 = (double)(-MathHelper_sin(var28 / 180.0f * (float)M_PI) *
                                    MathHelper_cos(var13 / 180.0f * (float)M_PI)) *
                           var27;
            double var16 = (double)(MathHelper_cos(var28 / 180.0f * (float)M_PI) *
                                    MathHelper_cos(var13 / 180.0f * (float)M_PI)) *
                           var27;
            double var18 = (double)(-MathHelper_sin(var13 / 180.0f * (float)M_PI)) * var27;

            int var20;
            for (var20 = 0; var20 < 8; ++var20) {
                float var21 = (float)((var20 & 1) * 2 - 1);
                float var22 = (float)((var20 >> 1 & 1) * 2 - 1);
                float var23 = (float)((var20 >> 2 & 1) * 2 - 1);
                var21 *= 0.1f;
                var22 *= 0.1f;
                var23 *= 0.1f;
                MovingObjectPosition *var24 = World_rayTraceBlocks(
                    mc->theWorld,
                    Vec3D_createVector(var4 + (double)var21, var6 + (double)var22,
                                       var8 + (double)var23),
                    Vec3D_createVector(var4 - var14 + (double)var21 + (double)var23,
                                       var6 - var18 + (double)var22, var8 - var16 + (double)var23));
                if (var24 != NULL) {
                    double var25 =
                        Vec3D_distanceTo(var24->hitVec, Vec3D_createVector(var4, var6, var8));
                    if (var25 < var27) {
                        var27 = var25;
                    }
                    MovingObjectPosition_free(var24);
                }
            }

            R_rotatef(vee->rotationPitch - var13, 1.0f, 0.0f, 0.0f);
            R_rotatef(vee->rotationYaw - var28, 0.0f, 1.0f, 0.0f);
            R_translatef(0.0f, 0.0f, (float)(-var27));
            R_rotatef(var28 - vee->rotationYaw, 0.0f, 1.0f, 0.0f);
            R_rotatef(var13 - vee->rotationPitch, 1.0f, 0.0f, 0.0f);
        }
    } else {
        R_translatef(0.0f, 0.0f, -0.1f);
    }

    if (!mc->gameSettings->field_22273_E) {
        R_rotatef(vee->prevRotationPitch +
                      (vee->rotationPitch - vee->prevRotationPitch) * partialTick,
                  1.0f, 0.0f, 0.0f);
        R_rotatef(vee->prevRotationYaw + (vee->rotationYaw - vee->prevRotationYaw) * partialTick +
                      180.0f,
                  0.0f, 1.0f, 0.0f);
    }

    R_translatef(0.0f, var3, 0.0f);
    var4 = vee->prevPosX + (vee->posX - vee->prevPosX) * (double)partialTick;
    var6 = vee->prevPosY + (vee->posY - vee->prevPosY) * (double)partialTick - (double)var3;
    var8 = vee->prevPosZ + (vee->posZ - vee->prevPosZ) * (double)partialTick;
    self->cloudFog = RenderGlobal_func_27307_a(mc->renderGlobal, var4, var6, var8, partialTick);
}

static void EntityRenderer_setupCameraTransform(EntityRenderer *self, float partialTick,
                                                int anaglyphPass) {
    Minecraft *mc = self->mc;
    self->farPlaneDistance = (float)(256 >> mc->gameSettings->renderDistance);

    R_matrixMode(R_PROJECTION);
    R_loadIdentity();

    float anaOffset = 0.07f;
    if (mc->gameSettings->anaglyph)
        R_translatef((float)(-(anaglyphPass * 2 - 1)) * anaOffset, 0.0f, 0.0f);

    if (self->cameraZoom != 1.0) {
        R_translatef((float)self->cameraYaw, (float)(-self->cameraPitch), 0.0f);
        R_scalef((float)self->cameraZoom, (float)self->cameraZoom, 1.0f);
    }

    mc_gluPerspective(EntityRenderer_getFOVModifier(self, partialTick),
                      (float)mc->displayWidth / (float)mc->displayHeight, 0.05f,
                      self->farPlaneDistance * 2.0f);

    R_matrixMode(R_MODELVIEW);
    R_loadIdentity();

    if (mc->gameSettings->anaglyph)
        R_translatef((float)(anaglyphPass * 2 - 1) * 0.1f, 0.0f, 0.0f);

    EntityRenderer_hurtCameraEffect(self, partialTick);
    if (mc->gameSettings->viewBobbing)
        EntityRenderer_setupViewBobbing(self, partialTick);

    EntityPlayerSP *sp = mc->thePlayer;
    float portalTime = sp->base.prevTimeInPortal +
                       (sp->base.timeInPortal - sp->base.prevTimeInPortal) * partialTick;
    if (portalTime > 0.0f) {
        float s = 5.0f / (portalTime * portalTime + 5.0f) - portalTime * 0.04f;
        s *= s;
        float angle = ((float)self->rendererUpdateCount + partialTick) * 20.0f;
        R_rotatef(angle, 0.0f, 1.0f, 1.0f);
        R_scalef(1.0f / s, 1.0f, 1.0f);
        R_rotatef(-angle, 0.0f, 1.0f, 1.0f);
    }

    EntityRenderer_orientCamera(self, partialTick);
}

static void EntityRenderer_func_4135_b(EntityRenderer *self, float partialTick, int anaglyphPass) {
    Minecraft *mc = self->mc;
    R_loadIdentity();
    if (mc->gameSettings->anaglyph)
        R_translatef((float)(anaglyphPass * 2 - 1) * 0.1f, 0.0f, 0.0f);

    R_pushMatrix();
    EntityRenderer_hurtCameraEffect(self, partialTick);
    if (mc->gameSettings->viewBobbing)
        EntityRenderer_setupViewBobbing(self, partialTick);

    EntityLiving *ve = mc->renderViewEntity;
    if (!mc->gameSettings->thirdPersonView && !Entity_isPlayerSleeping((Entity *)ve) &&
        !mc->gameSettings->hideGUI) {
        ItemRenderer_renderItemInFirstPerson(&self->itemRenderer, partialTick);
    }

    R_popMatrix();

    if (!mc->gameSettings->thirdPersonView && !Entity_isPlayerSleeping((Entity *)ve)) {
        ItemRenderer_renderOverlays(&self->itemRenderer, partialTick);
        EntityRenderer_hurtCameraEffect(self, partialTick);
    }

    if (mc->gameSettings->viewBobbing)
        EntityRenderer_setupViewBobbing(self, partialTick);
}

static void EntityRenderer_addRainParticles(EntityRenderer *self) {
    Minecraft *mc = self->mc;

    float var1 = World_getRainStrength(mc->theWorld, 1.0f);
    if (!mc->gameSettings->fancyGraphics) {
        var1 /= 2.0f;
    }
    if (var1 == 0.0f)
        return;

    JavaRandom_setSeed(&self->random, (long long)self->rendererUpdateCount * 312987231LL);
    EntityLiving *var2 = mc->renderViewEntity;
    Entity *ve = (Entity *)var2;
    World *var3 = mc->theWorld;
    int var4 = MathHelper_floor_double(ve->posX);
    int var5 = MathHelper_floor_double(ve->posY);
    int var6 = MathHelper_floor_double(ve->posZ);
    int var7 = 10;
    double var8 = 0.0;
    double var10 = 0.0;
    double var12 = 0.0;
    int var14 = 0;

    for (int var15 = 0; var15 < (int)(100.0f * var1 * var1); ++var15) {
        int var16 = var4 + JavaRandom_nextInt(&self->random, var7) -
                    JavaRandom_nextInt(&self->random, var7);
        int var17 = var6 + JavaRandom_nextInt(&self->random, var7) -
                    JavaRandom_nextInt(&self->random, var7);
        int var18 = World_findTopSolidBlock(var3, var16, var17);
        int var19 = World_getBlockId(var3, var16, var18 - 1, var17);
        if (var18 <= var5 + var7 && var18 >= var5 - var7 &&
            BiomeGenBase_canSpawnLightningBolt(
                WorldChunkManager_getBiomeGenAt(World_getWorldChunkManager(var3), var16, var17))) {
            float var20 = JavaRandom_nextFloat(&self->random);
            float var21 = JavaRandom_nextFloat(&self->random);
            if (var19 > 0) {
                if (Block_blocksList[var19] != NULL &&
                    Block_blocksList[var19]->blockMaterial == Material_lava) {
                    EffectRenderer_addEffect(
                        mc->effectRenderer,
                        (struct EntityFX *)EntitySmokeFX_create1(
                            var3, (double)((float)var16 + var20),
                            (double)((float)var18 + 0.1f) - Block_blocksList[var19]->minY,
                            (double)((float)var17 + var21), 0.0, 0.0, 0.0));
                } else {
                    ++var14;
                    if (JavaRandom_nextInt(&self->random, var14) == 0) {
                        var8 = (double)((float)var16 + var20);
                        var10 =
                            (double)((float)var18 + 0.1f) -
                            (Block_blocksList[var19] != NULL ? Block_blocksList[var19]->minY : 0.0);
                        var12 = (double)((float)var17 + var21);
                    }
                    EffectRenderer_addEffect(
                        mc->effectRenderer,
                        (struct EntityFX *)EntityRainFX_create(
                            var3, (double)((float)var16 + var20),
                            (double)((float)var18 + 0.1f) - (Block_blocksList[var19] != NULL
                                                                 ? Block_blocksList[var19]->minY
                                                                 : 0.0),
                            (double)((float)var17 + var21)));
                }
            }
        }
    }

    if (var14 > 0 && JavaRandom_nextInt(&self->random, 3) < self->rainSoundCounter++) {
        self->rainSoundCounter = 0;
        if (var10 > ve->posY + 1.0 &&
            World_findTopSolidBlock(var3, MathHelper_floor_double(ve->posX),
                                    MathHelper_floor_double(ve->posZ)) >
                MathHelper_floor_double(ve->posY)) {
            World_playSoundEffect(var3, var8, var10, var12, "ambient.weather.rain", 0.1f, 0.5f);
        } else {
            World_playSoundEffect(var3, var8, var10, var12, "ambient.weather.rain", 0.2f, 1.0f);
        }
    }
}

static void EntityRenderer_updateFogColor(EntityRenderer *self, float partialTick) {
    Minecraft *mc = self->mc;
    EntityLiving *ve = mc->renderViewEntity;
    World *w = mc->theWorld;

    float blend = 1.0f / (float)(4 - mc->gameSettings->renderDistance);
    blend = 1.0f - (float)pow((double)blend, 0.25);

    Vec3D *var5 = World_getSkyColor(w, (Entity *)ve, partialTick);
    float var6 = (float)var5->xCoord;
    float var7 = (float)var5->yCoord;
    float var8 = (float)var5->zCoord;

    Vec3D *var9 = World_getFogColor(w, partialTick);
    self->fogColorRed = (float)var9->xCoord;
    self->fogColorGreen = (float)var9->yCoord;
    self->fogColorBlue = (float)var9->zCoord;
    self->fogColorRed += (var6 - self->fogColorRed) * blend;
    self->fogColorGreen += (var7 - self->fogColorGreen) * blend;
    self->fogColorBlue += (var8 - self->fogColorBlue) * blend;

    float rain = World_getRainStrength(w, partialTick);
    if (rain > 0.0f) {
        float r1 = 1.0f - rain * 0.5f;
        float r2 = 1.0f - rain * 0.4f;
        self->fogColorRed *= r1;
        self->fogColorGreen *= r1;
        self->fogColorBlue *= r2;
    }

    float thunder = World_getThunderStrength(w, partialTick);
    if (thunder > 0.0f) {
        float t = 1.0f - thunder * 0.5f;
        self->fogColorRed *= t;
        self->fogColorGreen *= t;
        self->fogColorBlue *= t;
    }

    if (self->cloudFog) {
        Vec3D *cloudColor = World_getCloudColor(w, partialTick);
        self->fogColorRed = (float)cloudColor->xCoord;
        self->fogColorGreen = (float)cloudColor->yCoord;
        self->fogColorBlue = (float)cloudColor->zCoord;
    } else if (Entity_isInsideOfMaterial((Entity *)ve, Material_water)) {
        self->fogColorRed = 0.02f;
        self->fogColorGreen = 0.02f;
        self->fogColorBlue = 0.2f;
    } else if (Entity_isInsideOfMaterial((Entity *)ve, Material_lava)) {
        self->fogColorRed = 0.6f;
        self->fogColorGreen = 0.1f;
        self->fogColorBlue = 0.0f;
    }

    float brightBlend = self->fogColor2 + (self->fogColor1 - self->fogColor2) * partialTick;
    self->fogColorRed *= brightBlend;
    self->fogColorGreen *= brightBlend;
    self->fogColorBlue *= brightBlend;

    if (mc->gameSettings->anaglyph) {
        float r = self->fogColorRed, g = self->fogColorGreen, b = self->fogColorBlue;
        self->fogColorRed = (r * 30 + g * 59 + b * 11) / 100.0f;
        self->fogColorGreen = (r * 30 + g * 70) / 100.0f;
        self->fogColorBlue = (r * 30 + b * 70) / 100.0f;
    }

    R_clearColor(self->fogColorRed, self->fogColorGreen, self->fogColorBlue, 0.0f);
}

static void EntityRenderer_setupFog(EntityRenderer *self, int mode, float partialTick) {
    Minecraft *mc = self->mc;
    EntityLiving *ve = mc->renderViewEntity;
    (void)partialTick;

    float col[4] = {self->fogColorRed, self->fogColorGreen, self->fogColorBlue, 1.0f};
    R_fogfv(R_FOG_COLOR, col);
    R_normal3f(0.0f, -1.0f, 0.0f);
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);

    float var4, var5, var6, var7, var8, var9;
    if (self->cloudFog) {
        R_fogi(R_FOG_MODE, R_EXP);
        R_fogf(R_FOG_DENSITY, 0.1f);
        var4 = 1.0f;
        var5 = 1.0f;
        var6 = 1.0f;
        if (mc->gameSettings->anaglyph) {
            var7 = (var4 * 30.0f + var5 * 59.0f + var6 * 11.0f) / 100.0f;
            var8 = (var4 * 30.0f + var5 * 70.0f) / 100.0f;
            var9 = (var4 * 30.0f + var6 * 70.0f) / 100.0f;
        }
    } else if (Entity_isInsideOfMaterial((Entity *)ve, Material_water)) {
        R_fogi(R_FOG_MODE, R_EXP);
        R_fogf(R_FOG_DENSITY, 0.1f);
        var4 = 0.4f;
        var5 = 0.4f;
        var6 = 0.9f;
        if (mc->gameSettings->anaglyph) {
            var7 = (var4 * 30.0f + var5 * 59.0f + var6 * 11.0f) / 100.0f;
            var8 = (var4 * 30.0f + var5 * 70.0f) / 100.0f;
            var9 = (var4 * 30.0f + var6 * 70.0f) / 100.0f;
        }
    } else if (Entity_isInsideOfMaterial((Entity *)ve, Material_lava)) {
        R_fogi(R_FOG_MODE, R_EXP);
        R_fogf(R_FOG_DENSITY, 2.0f);
        var4 = 0.4f;
        var5 = 0.3f;
        var6 = 0.3f;
        if (mc->gameSettings->anaglyph) {
            var7 = (var4 * 30.0f + var5 * 59.0f + var6 * 11.0f) / 100.0f;
            var8 = (var4 * 30.0f + var5 * 70.0f) / 100.0f;
            var9 = (var4 * 30.0f + var6 * 70.0f) / 100.0f;
        }
    } else {
        R_fogi(R_FOG_MODE, R_LINEAR);
        R_fogf(R_FOG_START, self->farPlaneDistance * 0.25f);
        R_fogf(R_FOG_END, self->farPlaneDistance);
        if (mode < 0) {
            R_fogf(R_FOG_START, 0.0f);
            R_fogf(R_FOG_END, self->farPlaneDistance * 0.8f);
        }
        if (glfwExtensionSupported("GL_NV_fog_distance")) {
            R_fogi(0x855A, 0x855B);
        }
        if (World_isNether(mc->theWorld)) {
            R_fogf(R_FOG_START, 0.0f);
        }
    }

    (void)var7;
    (void)var8;
    (void)var9;
    R_enable(R_COLOR_MATERIAL);
    R_colorMaterial(R_FRONT, R_AMBIENT);
}
