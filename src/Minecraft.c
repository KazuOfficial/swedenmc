#include "Renderer.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Minecraft.h"
#include "Entity.h"
#include "ScaledResolution.h"
#include "GuiScreen.h"
#include "RenderGlobal.h"
#include "EntityRenderer.h"
#include "ItemRenderer.h"
#include "GameSettings.h"
#include "RenderEngine.h"
#include "World.h"
#include "FontRenderer.h"
#include "GuiIngame.h"
#include "GuiIngameMenu.h"
#include "GuiGameOver.h"
#include "GuiConnecting.h"
#include "EntityClientPlayerMP.h"
#include "WorldClient.h"
#include "NetClientHandler.h"
#include "UnexpectedThrowable.h"
#include "OpenGlCapsChecker.h"
#include "GLAllocation.h"
#include "StatFileWriter.h"
#include "GuiMainMenu.h"
#include "SoundManager.h"
#include "EffectRenderer.h"
#include "PlayerControllerSP.h"
#include "EntityPlayerSP.h"
#include "MovementInputFromOptions.h"
#include "LoadingScreenRenderer.h"
#include "IProgressUpdate.h"
#include "ChunkProviderLoadOrGenerate.h"
#include "EntityPlayer.h"
#include "Teleporter.h"
#include "WorldProvider.h"
#include "InventoryPlayer.h"
#include "Block.h"
#include "Session.h"
#include "TexturePackList.h"
#include "TextureLavaFX.h"
#include "TextureWaterFX.h"
#include "TexturePortalFX.h"
#include "TextureCompassFX.h"
#include "TextureWatchFX.h"
#include "TextureWaterFlowFX.h"
#include "TextureLavaFlowFX.h"
#include "TextureFlamesFX.h"
#include "MathHelper.h"
#include "MapColor.h"
#include "Material.h"
#include "Item.h"
#include "Packet.h"
#include "ChunkBlockMap.h"
#include "BiomeGenBase.h"
#include "AchievementList.h"
#include "AchievementMap.h"
#include "StatStringFormatKeyInv.h"
#include "GuiInventory.h"
#include "GuiChat.h"
#include "GuiSleepMP.h"
#include "GuiParticle.h"
#include "GuiAchievement.h"
#include "RenderBlocks.h"
#include "EnumOptions.h"
#include "MinecraftError.h"
#include "StatList.h"
#include "StatBase.h"
#include "ColorizerWater.h"
#include "ColorizerGrass.h"
#include "ColorizerFoliage.h"
#include "Tessellator.h"

#include "ISaveFormat.h"
#include "SaveConverterMcRegion.h"
#include <GLFW/glfw3.h>

#include "WorldRenderer.h"
#include "ThreadDownloadResources.h"
#include "RenderManager.h"
#include "TileEntityRenderer.h"
#include "TileEntity.h"
#include "AxisAlignedBB.h"
#include "Vec3D.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef _WIN32
#include <sys/resource.h>
#endif
#include <sys/time.h>
#include "platform.h"

static void Minecraft_convertMapFormat(Minecraft *self, const char *var1, const char *var2);
static void Minecraft_func_6255_d(Minecraft *self, const char *var1);

int64_t Minecraft_hasPaidCheckTime = 0L;

long long Minecraft_frameTimes[512];

long long Minecraft_tickTimes[512];

int Minecraft_numRecordedFrameTimes = 0;

static int s_key_repeat_enabled = 0;

const char *g_assetsDir = NULL;
const char *g_assetIndex = NULL;

static Minecraft *s_mc = NULL;
static GLFWwindow *s_window = NULL;
static GuiScreen *s_gui_pending_free[4];
static int s_gui_pending_free_n = 0;

void *Minecraft_getWindow(void) { return s_window; }
void Minecraft_setWindow(void *win) { s_window = (GLFWwindow *)win; }

static int s_acc_dx = 0;
static int s_acc_dy = 0;
static double s_prev_mx = 0.0;
static double s_prev_my = 0.0;
static int s_mouse_first = 1;
static double s_cur_mx = 0.0;
static double s_cur_my = 0.0;

static int s_mouse_left_held = 0;
static int s_mouse_right_held = 0;

#define KEY_QUEUE_SIZE 128
typedef struct {
    int key;
    int action;
    int mods;
    char ch;
} KeyEvent;
static KeyEvent s_key_queue[KEY_QUEUE_SIZE];
static int s_key_head = 0;
static int s_key_tail = 0;

static void key_enqueue(int key, int action, int mods) {

    if (action == GLFW_REPEAT && !s_key_repeat_enabled)
        return;
    int next = (s_key_tail + 1) % KEY_QUEUE_SIZE;
    if (next != s_key_head) {
        s_key_queue[s_key_tail].key = key;
        s_key_queue[s_key_tail].action = action;
        s_key_queue[s_key_tail].mods = mods;
        s_key_queue[s_key_tail].ch = '\0';
        s_key_tail = next;
    }
}

static int key_dequeue(KeyEvent *ev) {
    if (s_key_head == s_key_tail)
        return 0;
    *ev = s_key_queue[s_key_head];
    s_key_head = (s_key_head + 1) % KEY_QUEUE_SIZE;
    return 1;
}

#define MOUSE_QUEUE_SIZE 64
typedef struct {
    int button;
    int action;
    double x;
    double y;
} MouseButtonEvent;
static MouseButtonEvent s_mouse_queue[MOUSE_QUEUE_SIZE];
static int s_mouse_head = 0;
static int s_mouse_tail = 0;

static void mouse_btn_enqueue(int button, int action, double x, double y) {
    int next = (s_mouse_tail + 1) % MOUSE_QUEUE_SIZE;
    if (next != s_mouse_head) {
        s_mouse_queue[s_mouse_tail].button = button;
        s_mouse_queue[s_mouse_tail].action = action;
        s_mouse_queue[s_mouse_tail].x = x;
        s_mouse_queue[s_mouse_tail].y = y;
        s_mouse_tail = next;
    }
}

static int mouse_btn_dequeue(MouseButtonEvent *ev) {
    if (s_mouse_head == s_mouse_tail)
        return 0;
    *ev = s_mouse_queue[s_mouse_head];
    s_mouse_head = (s_mouse_head + 1) % MOUSE_QUEUE_SIZE;
    return 1;
}

#ifdef __EMSCRIPTEN__

static int s_em_cursor_grabbed = 0;

static int s_em_focus_acquired = 0;
#endif

#define SCROLL_QUEUE_SIZE 64
static int s_scroll_queue[SCROLL_QUEUE_SIZE];
static int s_scroll_head = 0;
static int s_scroll_tail = 0;

static void scroll_enqueue(int delta) {
    int next = (s_scroll_tail + 1) % SCROLL_QUEUE_SIZE;
    if (next != s_scroll_head) {
        s_scroll_queue[s_scroll_tail] = delta;
        s_scroll_tail = next;
    }
}

static int scroll_dequeue(int *delta) {
    if (s_scroll_head == s_scroll_tail)
        return 0;
    *delta = s_scroll_queue[s_scroll_head];
    s_scroll_head = (s_scroll_head + 1) % SCROLL_QUEUE_SIZE;
    return 1;
}

static KeyEvent s_cur_key_event = {0, 0, 0, 0};
static MouseButtonEvent s_cur_mouse_event = {0, 0, 0.0, 0.0};

int Minecraft_getKeyboardEventKey(void) { return s_cur_key_event.key; }

int Minecraft_getKeyboardEventKeyState(void) { return s_cur_key_event.action != GLFW_RELEASE; }

char Minecraft_getKeyboardEventCharacter(void) { return s_cur_key_event.ch; }

int Minecraft_mouseNext(void) {
    MouseButtonEvent mev;
    if (!mouse_btn_dequeue(&mev))
        return 0;
    s_cur_mouse_event = mev;
    return 1;
}

int Minecraft_keyboardNext(void) {
    KeyEvent kev;
    if (!key_dequeue(&kev))
        return 0;
    s_cur_key_event = kev;
    return 1;
}

int Minecraft_getMouseEventX(void) { return (int)s_cur_mouse_event.x; }

int Minecraft_getMouseEventY(void) { return (int)s_cur_mouse_event.y; }

int Minecraft_getMouseEventButton(void) { return s_cur_mouse_event.button; }

int Minecraft_getMouseEventButtonState(void) { return s_cur_mouse_event.action == GLFW_PRESS; }

static void cursor_win_to_fb(GLFWwindow *w, double *x, double *y) {
    int winW, winH, fbW, fbH;
    glfwGetWindowSize(w, &winW, &winH);
    glfwGetFramebufferSize(w, &fbW, &fbH);
    if (winW > 0)
        *x = *x * fbW / winW;
    if (winH > 0)
        *y = *y * fbH / winH;
}

static void cb_cursor_pos(GLFWwindow *w, double mx, double my) {
    if (s_mouse_first) {
        s_prev_mx = mx;
        s_prev_my = my;
        s_mouse_first = 0;
        return;
    }

    s_acc_dx += (int)(mx - s_prev_mx);
    s_acc_dy -= (int)(my - s_prev_my);
    s_prev_mx = mx;
    s_prev_my = my;

    double fx = mx, fy = my;
    cursor_win_to_fb(w, &fx, &fy);
    s_cur_mx = fx;
    s_cur_my = fy;
}

static void cb_mouse_button(GLFWwindow *w, int button, int action, int mods) {
    (void)mods;
    double mx, my;
    glfwGetCursorPos(w, &mx, &my);
    cursor_win_to_fb(w, &mx, &my);
    mouse_btn_enqueue(button, action, mx, my);
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        s_mouse_left_held = (action != GLFW_RELEASE);
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        s_mouse_right_held = (action != GLFW_RELEASE);
#ifdef __EMSCRIPTEN__

    if (action == GLFW_PRESS && s_mc != NULL && !s_mc->inGameHasFocus && s_mc->thePlayer != NULL &&
        (s_mc->currentScreen == NULL ||
         (s_mc->currentScreen != NULL && s_mc->currentScreen->guiType == GUI_TYPE_INGAME_MENU))) {
        glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        s_em_cursor_grabbed = 1;
    }
#endif
}

static void cb_scroll(GLFWwindow *w, double xoffset, double yoffset) {
    (void)w;
    (void)xoffset;
    if (yoffset != 0.0) {
        scroll_enqueue(yoffset > 0.0 ? 1 : -1);
    }
}

static void cb_framebuffer_size(GLFWwindow *w, int width, int height) {
    (void)w;

    if (width <= 0)
        width = 1;
    if (height <= 0)
        height = 1;
    R_viewport(0, 0, width, height);
    if (s_mc) {
        s_mc->displayWidth = width;
        s_mc->displayHeight = height;
        if (s_mc->currentScreen != NULL && s_mc->gameSettings != NULL) {
            ScaledResolution sr;
            ScaledResolution_init(&sr, s_mc->gameSettings, width, height);
            GuiScreen_setWorldAndResolution(s_mc->currentScreen, s_mc,
                                            ScaledResolution_getScaledWidth(&sr),
                                            ScaledResolution_getScaledHeight(&sr));
        }
    }
}

static void cb_key(GLFWwindow *w, int key, int scancode, int action, int mods) {
    (void)w;
    (void)scancode;
    key_enqueue(key, action, mods);

    if (s_mc && s_mc->gameSettings) {
        GameSettings *gs = s_mc->gameSettings;
        int pressed = (action != GLFW_RELEASE);
        for (int i = 0; i < NUM_KEY_BINDINGS; i++) {
            if (gs->keyBindings[i] && gs->keyBindings[i]->keyCode == key)
                gs->keyBindings[i]->pressed = pressed;
        }
    }
}

static void cb_char(GLFWwindow *w, unsigned int codepoint) {
    (void)w;
    if (codepoint == 0 || codepoint >= 128)
        return;

    if (s_key_tail != s_key_head) {
        int last = (s_key_tail - 1 + KEY_QUEUE_SIZE) % KEY_QUEUE_SIZE;
        s_key_queue[last].ch = (char)codepoint;
    }
}

void Minecraft_getCursorPos(double *outX, double *outY) {
    *outX = s_cur_mx;
    *outY = s_cur_my;
}

void MouseHelper_mouseXYChange(MouseHelper *self) {
    self->deltaX = s_acc_dx;
    self->deltaY = s_acc_dy;
    s_acc_dx = 0;
    s_acc_dy = 0;
}

void MouseHelper_grabMouseCursor(MouseHelper *self) {
    if (s_window)
        glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    s_mouse_first = 1;
    self->deltaX = 0;
    self->deltaY = 0;
}

void MouseHelper_ungrabMouseCursor(MouseHelper *self) {
    (void)self;
    if (s_window && s_mc)
        glfwSetCursorPos(s_window, s_mc->displayWidth / 2, s_mc->displayHeight / 2);
    if (s_window)
        glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

Minecraft *Minecraft_getMinecraft(void) { return s_mc; }

void Minecraft_invalidateItemToRender(ItemStack *stack) {
    if (!s_mc || !s_mc->entityRenderer || !stack)
        return;
    EntityRenderer *er = (EntityRenderer *)s_mc->entityRenderer;
    if (er->itemRenderer.itemToRender == stack)
        er->itemRenderer.itemToRender = NULL;
}

int Minecraft_isAmbientOcclusionEnabled(void) {
    return s_mc && s_mc->gameSettings ? s_mc->gameSettings->ambientOcclusion : 0;
}

int Minecraft_isMouseButtonDown(int button) {
    if (button == 0)
        return s_mouse_left_held;
    if (button == 1)
        return s_mouse_right_held;
    return 0;
}

void Minecraft_setIngameFocus(Minecraft *self) {
#ifdef __EMSCRIPTEN__
    if (!self->inGameHasFocus && s_em_cursor_grabbed) {

        self->inGameHasFocus = 1;
        s_em_focus_acquired = 5;
        s_em_cursor_grabbed = 0;
        Minecraft_displayGuiScreen(self, NULL);
        self->leftClickCounter = 10000;
        self->mouseTicksRan = self->ticksRan + 10000;
    }

#else

    if (s_window && glfwGetWindowAttrib(s_window, GLFW_FOCUSED)) {
        if (!self->inGameHasFocus) {
            self->inGameHasFocus = 1;
            MouseHelper_grabMouseCursor(&self->mouseHelper);
            Minecraft_displayGuiScreen(self, NULL);
            self->leftClickCounter = 10000;
            self->mouseTicksRan = self->ticksRan + 10000;
        }
    }
#endif
}

void Minecraft_setIngameNotInFocus(Minecraft *self) {
#ifdef __EMSCRIPTEN__

    s_em_cursor_grabbed = 0;
    if (s_window)
        glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif
    if (self->inGameHasFocus) {
        if (self->thePlayer != NULL) {
            EntityPlayerSP_resetPlayerKeyState(self->thePlayer);
        }
        self->inGameHasFocus = 0;
#ifndef __EMSCRIPTEN__
        MouseHelper_ungrabMouseCursor(&self->mouseHelper);
#endif
    }
}

Minecraft *Minecraft_create(int width, int height, int fullscreen) {

    AchievementMap_init();

    StatList_func_27360_a();

    Minecraft *self = (Minecraft *)calloc(1, sizeof(Minecraft));
    if (!self)
        return NULL;

    self->tempDisplayHeight = height;

    self->isFullScreen = fullscreen;

    self->displayWidth = width;
    self->displayHeight = height;

    s_mc = self;
    Entity_registerViewEntityRef(&self->renderViewEntity);
    Entity_registerPlayerRef((EntityLiving **)&self->thePlayer);
    return self;
}

void Minecraft_func_6274_a(Minecraft *self, int var1, int var2, int var3, int var4, int var5,
                           int var6) {
    (void)self;
    float var7 = 0.00390625f;
    float var8 = 0.00390625f;
    Tessellator *var9 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var9);
    Tessellator_addVertexWithUV(var9, (double)(var1 + 0), (double)(var2 + var6), 0.0,
                                (double)((float)(var3 + 0) * var7),
                                (double)((float)(var4 + var6) * var8));
    Tessellator_addVertexWithUV(var9, (double)(var1 + var5), (double)(var2 + var6), 0.0,
                                (double)((float)(var3 + var5) * var7),
                                (double)((float)(var4 + var6) * var8));
    Tessellator_addVertexWithUV(var9, (double)(var1 + var5), (double)(var2 + 0), 0.0,
                                (double)((float)(var3 + var5) * var7),
                                (double)((float)(var4 + 0) * var8));
    Tessellator_addVertexWithUV(var9, (double)(var1 + 0), (double)(var2 + 0), 0.0,
                                (double)((float)(var3 + 0) * var7),
                                (double)((float)(var4 + 0) * var8));
    Tessellator_draw(var9);
}

static void Minecraft_loadScreen(Minecraft *self) {
    R_beginFrame();
    ScaledResolution var1;
    ScaledResolution_init(&var1, self->gameSettings, self->displayWidth, self->displayHeight);
    R_clear(R_DEPTH_BUFFER_BIT | R_COLOR_BUFFER_BIT);
    R_matrixMode(R_PROJECTION);
    R_loadIdentity();
    R_ortho(0.0, (double)ScaledResolution_getScaledWidth(&var1),
            (double)ScaledResolution_getScaledHeight(&var1), 0.0, 1000.0, 3000.0);
    R_matrixMode(R_MODELVIEW);
    R_loadIdentity();
    R_translatef(0.0f, 0.0f, -2000.0f);
    R_viewport(0, 0, self->displayWidth, self->displayHeight);
    R_clearColor(0.0f, 0.0f, 0.0f, 0.0f);
    Tessellator *var2 = &Tessellator_instance;
    R_disable(R_LIGHTING);
    R_enable(R_TEXTURE_2D);
    R_disable(R_FOG);
    R_bindTexture(RenderEngine_getTexture(self->renderEngine, "/title/mojang.png"));
    Tessellator_startDrawingQuads(var2);
    Tessellator_setColorOpaque_I(var2, 16777215);
    Tessellator_addVertexWithUV(var2, 0.0, (double)self->displayHeight, 0.0, 0.0, 0.0);
    Tessellator_addVertexWithUV(var2, (double)self->displayWidth, (double)self->displayHeight, 0.0,
                                0.0, 0.0);
    Tessellator_addVertexWithUV(var2, (double)self->displayWidth, 0.0, 0.0, 0.0, 0.0);
    Tessellator_addVertexWithUV(var2, 0.0, 0.0, 0.0, 0.0, 0.0);
    Tessellator_draw(var2);
    short var3 = 256;
    short var4 = 256;
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    Tessellator_setColorOpaque_I(var2, 16777215);
    Minecraft_func_6274_a(self, (ScaledResolution_getScaledWidth(&var1) - var3) / 2,
                          (ScaledResolution_getScaledHeight(&var1) - var4) / 2, 0, 0, var3, var4);
    R_disable(R_LIGHTING);
    R_disable(R_FOG);
    R_enable(R_ALPHA_TEST);
    R_alphaFunc(R_GREATER, 0.1f);
    R_endFrame();
    glfwPollEvents();
}

void Minecraft_startGame(Minecraft *self) {

    MathHelper_init();
    MapColor_init();
    Material_init();
    StatBase_init_statics();
    Block_initRegistry();
    Item_initRegistry();
    StatList_init();
    Packet_initRegistry();
    Session_initRegisteredBlocksList();
    ChunkBlockMap_init();
    BiomeGenBase_init();
    AchievementList_init();
    AchievementMap_init();

    ColorizerWater_init();
    ColorizerGrass_init();
    ColorizerFoliage_init();

    if (g_renderer_is_bgfx) {

        if (!s_window) {
            fprintf(stderr, "[Minecraft] no window set for bgfx path\n");
            self->hasCrashed = 1;
            return;
        }
        glfwSetWindowTitle(s_window, "Minecraft Beta 1.7.3");
    } else {
        if (!glfwInit()) {
            fprintf(stderr, "[Minecraft] glfwInit failed\n");
            self->hasCrashed = 1;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        GLFWmonitor *monitor = self->isFullScreen ? glfwGetPrimaryMonitor() : NULL;
        s_window = glfwCreateWindow(self->displayWidth, self->displayHeight, "Minecraft Beta 1.7.3",
                                    monitor, NULL);
        if (!s_window) {
            fprintf(stderr, "[Minecraft] glfwCreateWindow failed\n");
            glfwTerminate();
            self->hasCrashed = 1;
            return;
        }

        R_init(s_window);
        glfwSwapInterval(0);
    }

    glfwSetCursorPosCallback(s_window, cb_cursor_pos);
    glfwSetMouseButtonCallback(s_window, cb_mouse_button);
    glfwSetScrollCallback(s_window, cb_scroll);
    glfwSetFramebufferSizeCallback(s_window, cb_framebuffer_size);
    glfwSetKeyCallback(s_window, cb_key);
    glfwSetCharCallback(s_window, cb_char);

    if (!g_renderer_is_bgfx) {
        printf("[Minecraft] OpenGL %s\n", glGetString(GL_VERSION));
    }

    {
        int fbw, fbh;
        glfwGetFramebufferSize(s_window, &fbw, &fbh);
        self->displayWidth = fbw;
        self->displayHeight = fbh;
        R_viewport(0, 0, fbw, fbh);
    }

    R_enable(R_TEXTURE_2D);
    R_shadeModel(R_SMOOTH);
    R_clearDepth(1.0);
    R_enable(R_DEPTH_TEST);
    R_depthFunc(R_LEQUAL);
    R_enable(R_ALPHA_TEST);
    R_alphaFunc(R_GREATER, 0.1f);
    R_cullFace(R_BACK);
    R_matrixMode(R_PROJECTION);
    R_loadIdentity();
    R_matrixMode(R_MODELVIEW);

    const char *mcDataDir = Minecraft_getMinecraftDir();

    char savesPath[4096];
    snprintf(savesPath, sizeof(savesPath), "%s/saves", mcDataDir);
    self->saveLoader = (struct ISaveFormat *)SaveConverterMcRegion_create(savesPath);

    self->gameSettings = (GameSettings *)calloc(1, sizeof(GameSettings));
    GameSettings_init(self->gameSettings, self, mcDataDir);

    self->texturePackList = TexturePackList_create(self, mcDataDir);
    self->renderEngine = RenderEngine_create(self->texturePackList, self->gameSettings);

    self->fontRenderer =
        FontRenderer_create(self->gameSettings, "/font/default.png", self->renderEngine);

    self->loadingScreen = (struct LoadingScreenRenderer *)calloc(1, sizeof(LoadingScreenRenderer));
    LoadingScreenRenderer_init(self->loadingScreen, self);

    ColorizerWater_func_28182_a(
        RenderEngine_func_28149_a(self->renderEngine, "/misc/watercolor.png"));
    ColorizerGrass_func_28181_a(
        RenderEngine_func_28149_a(self->renderEngine, "/misc/grasscolor.png"));
    ColorizerFoliage_func_28152_a(
        RenderEngine_func_28149_a(self->renderEngine, "/misc/foliagecolor.png"));

    self->entityRenderer = (struct EntityRenderer *)EntityRenderer_create(self);

    {
        RenderManager *rm = RenderManager_create();
        rm->renderEngine = self->renderEngine;
    }
    TileEntityRenderer_create();
    {
        ItemRenderer *rm_ir = (ItemRenderer *)calloc(1, sizeof(ItemRenderer));
        ItemRenderer_init(rm_ir, self);
        RenderManager_instance->itemRenderer = rm_ir;
    }

    self->statFileWriter = StatFileWriter_create(self->session, mcDataDir);

    self->guiAchievement = (struct GuiAchievement *)calloc(1, sizeof(GuiAchievement));
    GuiAchievement_construct((GuiAchievement *)self->guiAchievement, self);

    Achievement_setStatStringFormatter(AchievementList_openInventory,
                                       (IStatStringFormat *)StatStringFormatKeyInv_create(self));

    Minecraft_loadScreen(self);

    self->sndManager = (struct SoundManager *)SoundManager_create();
    SoundManager_loadSoundSettings((SoundManager *)self->sndManager, self->gameSettings);

    RenderEngine_registerTextureFX(self->renderEngine, (TextureFX *)TextureLavaFX_create());
    RenderEngine_registerTextureFX(self->renderEngine, (TextureFX *)TextureWaterFX_create());
    RenderEngine_registerTextureFX(self->renderEngine, (TextureFX *)TexturePortalFX_create());
    RenderEngine_registerTextureFX(self->renderEngine, (TextureFX *)TextureCompassFX_create(self));
    RenderEngine_registerTextureFX(self->renderEngine, (TextureFX *)TextureWatchFX_create(self));
    RenderEngine_registerTextureFX(self->renderEngine, (TextureFX *)TextureWaterFlowFX_create());
    RenderEngine_registerTextureFX(self->renderEngine, (TextureFX *)TextureLavaFlowFX_create());
    RenderEngine_registerTextureFX(self->renderEngine, (TextureFX *)TextureFlamesFX_create(0));
    RenderEngine_registerTextureFX(self->renderEngine, (TextureFX *)TextureFlamesFX_create(1));

    self->renderGlobal = (struct RenderGlobal *)RenderGlobal_create(self, self->renderEngine);

    self->effectRenderer = (struct EffectRenderer *)EffectRenderer_create(NULL, self->renderEngine);

    self->downloadResourcesThread =
        ThreadDownloadResources_create(mcDataDir, self, g_assetsDir, g_assetIndex);

    self->ingameGUI = (struct GuiIngame *)GuiIngame_create(self);

    if (self->serverName[0] != '\0') {
        GuiConnecting *gc = (GuiConnecting *)calloc(1, sizeof(GuiConnecting));
        GuiConnecting_construct(gc, self, self->serverName, self->serverPort);
        Minecraft_displayGuiScreen(self, (struct GuiScreen *)gc);
    } else {
        Minecraft_displayGuiScreen(self, (struct GuiScreen *)GuiMainMenu_create());
    }

    Timer_init(&self->timer, 20);

    self->prevFrameTime = -1LL;

    self->running = 1;
    printf("[Minecraft] startGame complete\n");
}

void Minecraft_changeWorld1(Minecraft *self, World *world) {
    Minecraft_changeWorld2(self, world, "");
}

void Minecraft_changeWorld(Minecraft *self, World *world, const char *loadingMessage,
                           EntityPlayer *var3) {
    if (self->statFileWriter)
        StatFileWriter_func_27175_b(self->statFileWriter);
    if (self->statFileWriter)
        StatFileWriter_syncStats(self->statFileWriter);
    self->renderViewEntity = NULL;
    LoadingScreenRenderer_printText(self->loadingScreen, loadingMessage);
    LoadingScreenRenderer_displayLoadingString(self->loadingScreen, "");
    if (self->sndManager)
        SoundManager_playStreaming((SoundManager *)self->sndManager, NULL, 0.0f, 0.0f, 0.0f, 0.0f,
                                   0.0f);
    World *old_world = self->theWorld;
    if (old_world)
        World_saveWorldIndirectly(old_world, (IProgressUpdate *)self->loadingScreen);
    self->theWorld = world;

    if (self->renderGlobal)
        RenderGlobal_changeWorld((RenderGlobal *)self->renderGlobal, world);
    if (self->effectRenderer)
        EffectRenderer_clearEffects((EffectRenderer *)self->effectRenderer, world);

    if (self->entityRenderer)
        ((EntityRenderer *)self->entityRenderer)->itemRenderer.itemToRender = NULL;

    if (world != NULL) {
        PlayerController_func_717_a(self->playerController, world);
        if (!Minecraft_isMultiplayerWorld(self)) {
            if (var3 == NULL) {
                self->thePlayer = (struct EntityPlayerSP *)World_func_4085_a(world);
            }
        } else if (self->thePlayer != NULL) {
            Entity_preparePlayerToSpawn((Entity *)self->thePlayer);
            ((Entity *)self->thePlayer)->worldObj = world;
            World_entityJoinedWorld(world, (Entity *)self->thePlayer);
        }

        if (!world->multiplayerWorld) {
            Minecraft_func_6255_d(self, loadingMessage);
        }

        if (self->thePlayer == NULL) {
            self->thePlayer = (struct EntityPlayerSP *)PlayerController_createPlayer(
                self->playerController, world);
            Entity_preparePlayerToSpawn((Entity *)self->thePlayer);
            PlayerController_flipPlayer(self->playerController, (EntityPlayer *)self->thePlayer);
        }

        if (self->thePlayer->movementInput)
            free(self->thePlayer->movementInput);
        self->thePlayer->movementInput =
            (MovementInput *)MovementInputFromOptions_create(self->gameSettings);
        PlayerController_func_6473_b(self->playerController, (EntityPlayer *)self->thePlayer);
        if (var3 != NULL) {
            World_emptyMethod1(world);
        }

        IChunkProvider *var4 = World_getIChunkProvider(world);
        if (var4 != NULL && var4->vtable == &ChunkProviderLoadOrGenerate_vtable) {
            ChunkProviderLoadOrGenerate *var5 = (ChunkProviderLoadOrGenerate *)var4;
            int var6 = MathHelper_floor_float((float)((int)((Entity *)self->thePlayer)->posX)) >> 4;
            int var7 = MathHelper_floor_float((float)((int)((Entity *)self->thePlayer)->posZ)) >> 4;
            ChunkProviderLoadOrGenerate_setCurrentChunkOver(var5, var6, var7);
        }

        World_spawnPlayerWithLoadedChunks(world, (Entity *)self->thePlayer);
        if (world->isNewWorld) {
            World_saveWorldIndirectly(world, (IProgressUpdate *)self->loadingScreen);
        }
        self->renderViewEntity = (EntityLiving *)self->thePlayer;
    } else {
        self->thePlayer = NULL;
        self->renderViewEntity = NULL;
    }

    self->systemTime = 0L;

    if (old_world != NULL) {
        if (var3 != NULL) {

            for (int i = 0; i < old_world->numEntities; i++) {
                if (old_world->loadedEntityList[i] == (Entity *)var3) {
                    old_world->loadedEntityList[i] =
                        old_world->loadedEntityList[--old_world->numEntities];
                    break;
                }
            }
            if (old_world->deadPlayer == (Entity *)var3)
                old_world->deadPlayer = NULL;
        }

        if (world == NULL && old_world->multiplayerWorld) {
            WorldClient *wc = (WorldClient *)old_world;
            if (wc->sendQueue) {
                NetClientHandler_destroy(wc->sendQueue);
                wc->sendQueue = NULL;

                if (self->thePlayer != NULL && ((Entity *)self->thePlayer)->isClientPlayerMP) {
                    ((EntityClientPlayerMP *)self->thePlayer)->sendQueue = NULL;
                }
            }
        }

        if (self->pendingDestroyWorld && self->pendingDestroyWorld != old_world) {

            if (self->pendingDestroyWorld2)
                World_destroy(self->pendingDestroyWorld2);
            self->pendingDestroyWorld2 = self->pendingDestroyWorld;
        }
        self->pendingDestroyWorld = old_world;
    }
}

void Minecraft_changeWorld2(Minecraft *self, World *world, const char *loadingMessage) {
    Minecraft_changeWorld(self, world, loadingMessage, NULL);
}

static void Minecraft_forceReload(Minecraft *self) {
    printf("FORCING RELOAD!\n");
    self->sndManager = (struct SoundManager *)SoundManager_create();
    SoundManager_loadSoundSettings((SoundManager *)self->sndManager, self->gameSettings);
    ThreadDownloadResources_reloadResources(self->downloadResourcesThread);
}

static void Minecraft_func_6254_a(Minecraft *self, int var1, int var2) {
    if (self->thePlayer == NULL)
        return;
    if (!self->playerController->field_1064_b) {
        if (!var2) {
            self->leftClickCounter = 0;
        }
        if (var1 != 0 || self->leftClickCounter <= 0) {
            if (var2 && self->objectMouseOver != NULL &&
                self->objectMouseOver->typeOfHit == EnumMovingObjectType_TILE && var1 == 0) {
                int var3 = self->objectMouseOver->blockX;
                int var4 = self->objectMouseOver->blockY;
                int var5 = self->objectMouseOver->blockZ;
                PlayerController_sendBlockRemoving(self->playerController, var3, var4, var5,
                                                   self->objectMouseOver->sideHit);
                EffectRenderer_addBlockHitEffects(self->effectRenderer, var3, var4, var5,
                                                  self->objectMouseOver->sideHit);
            } else {
                PlayerController_resetBlockRemoving(self->playerController);
            }
        }
    }
}

static void Minecraft_clickMouse(Minecraft *self, int var1) {
    if (self->thePlayer == NULL)
        return;
    if (var1 != 0 || self->leftClickCounter <= 0) {
        if (var1 == 0) {
            EntityPlayer_swingItem((EntityPlayer *)self->thePlayer);
        }
        int var2 = 1;
        if (self->objectMouseOver == NULL) {
            if (var1 == 0) {

                self->leftClickCounter = 10;
            }
        } else if (self->objectMouseOver->typeOfHit == EnumMovingObjectType_ENTITY) {
            if (var1 == 0) {
                PlayerController_attackEntity(self->playerController,
                                              (EntityPlayer *)self->thePlayer,
                                              self->objectMouseOver->entityHit);
            }
            if (var1 == 1) {
                PlayerController_interactWithEntity(self->playerController,
                                                    (EntityPlayer *)self->thePlayer,
                                                    self->objectMouseOver->entityHit);
            }
        } else if (self->objectMouseOver->typeOfHit == EnumMovingObjectType_TILE) {
            int var3 = self->objectMouseOver->blockX;
            int var4 = self->objectMouseOver->blockY;
            int var5 = self->objectMouseOver->blockZ;
            int var6 = self->objectMouseOver->sideHit;
            if (var1 == 0) {
                PlayerController_clickBlock(self->playerController, var3, var4, var5, var6);
            } else {
                ItemStack *var7 =
                    InventoryPlayer_getCurrentItem(&((EntityPlayer *)self->thePlayer)->inventory);
                int var8 = var7 != NULL ? var7->stackSize : 0;
                if (PlayerController_sendPlaceBlock(self->playerController,
                                                    (EntityPlayer *)self->thePlayer, self->theWorld,
                                                    var7, var3, var4, var5, var6)) {
                    var2 = 0;
                    EntityPlayer_swingItem((EntityPlayer *)self->thePlayer);
                }
                if (var7 == NULL) {
                    return;
                }
                if (var7->stackSize == 0) {
                    Minecraft_invalidateItemToRender(var7);
                    ItemStack_free(var7);
                    ((EntityPlayer *)self->thePlayer)
                        ->inventory
                        .mainInventory[((EntityPlayer *)self->thePlayer)->inventory.currentItem] =
                        NULL;
                } else if (var7->stackSize != var8) {
                    ItemRenderer_func_9449_b(&self->entityRenderer->itemRenderer);
                }
            }
        }
        if (var2 && var1 == 1) {
            ItemStack *var9 =
                InventoryPlayer_getCurrentItem(&((EntityPlayer *)self->thePlayer)->inventory);
            if (var9 != NULL && PlayerController_sendUseItem(self->playerController,
                                                             (EntityPlayer *)self->thePlayer,
                                                             self->theWorld, var9)) {
                ItemRenderer_func_9450_c(&self->entityRenderer->itemRenderer);
            }
        }
    }
}

static void Minecraft_clickMiddleMouseButton(Minecraft *self) {
    if (self->objectMouseOver != NULL) {
        int var1 = World_getBlockId(self->theWorld, self->objectMouseOver->blockX,
                                    self->objectMouseOver->blockY, self->objectMouseOver->blockZ);
        if (var1 == Block_grass->blockID) {
            var1 = Block_dirt->blockID;
        }
        if (var1 == Block_stairDouble->blockID) {
            var1 = Block_stairSingle->blockID;
        }
        if (var1 == Block_bedrock->blockID) {
            var1 = Block_stone->blockID;
        }

        InventoryPlayer_setCurrentItem(&((EntityPlayer *)self->thePlayer)->inventory, var1, 0);
    }
}

static void Minecraft_func_28001_B(Minecraft *self) { (void)self; }

void Minecraft_runTick(Minecraft *self) {
    Entity_flush_destroy_queue();

    if (self->renderGlobal)
        RenderGlobal_purgeStaleTileEntities((RenderGlobal *)self->renderGlobal);
    TileEntity_flush_destroy_queue();

    if (self->ticksRan == 6000) {
        Minecraft_func_28001_B(self);
    }

    if (self->statFileWriter)
        StatFileWriter_func_27178_d(self->statFileWriter);

    if (self->ingameGUI)
        GuiIngame_updateTick((GuiIngame *)self->ingameGUI);

    if (self->entityRenderer)
        EntityRenderer_getMouseOver((EntityRenderer *)self->entityRenderer, 1.0f);

    if (self->thePlayer != NULL) {
        IChunkProvider *var1 = World_getIChunkProvider(self->theWorld);
        if (var1 != NULL && var1->vtable == &ChunkProviderLoadOrGenerate_vtable) {
            ChunkProviderLoadOrGenerate *var2 = (ChunkProviderLoadOrGenerate *)var1;
            int var3 = MathHelper_floor_float((float)((int)((Entity *)self->thePlayer)->posX)) >> 4;
            int var4 = MathHelper_floor_float((float)((int)((Entity *)self->thePlayer)->posZ)) >> 4;
            ChunkProviderLoadOrGenerate_setCurrentChunkOver(var2, var3, var4);
        }
    }

    if (!self->isGamePaused && self->theWorld != NULL && self->playerController)
        PlayerController_updateController(self->playerController);

    if (self->renderEngine)
        R_bindTexture(RenderEngine_getTexture(self->renderEngine, "/terrain.png"));

    if (!self->isGamePaused && self->renderEngine)
        RenderEngine_updateDynamicTextures(self->renderEngine);

    if (self->currentScreen == NULL && self->thePlayer != NULL) {
        if (((EntityLiving *)self->thePlayer)->health <= 0) {
            Minecraft_displayGuiScreen(self, NULL);
        } else if (EntityPlayer_isPlayerSleeping((EntityPlayer *)self->thePlayer) &&
                   self->theWorld != NULL && self->theWorld->multiplayerWorld) {
            GuiSleepMP *sleepGui = (GuiSleepMP *)calloc(1, sizeof(GuiSleepMP));
            GuiSleepMP_construct(sleepGui);
            Minecraft_displayGuiScreen(self, (GuiScreen *)sleepGui);
        }
    } else if (self->currentScreen != NULL && self->currentScreen->guiType == GUI_TYPE_SLEEP_MP &&
               self->thePlayer != NULL &&
               !EntityPlayer_isPlayerSleeping((EntityPlayer *)self->thePlayer)) {
        Minecraft_displayGuiScreen(self, NULL);
    }

    if (self->currentScreen != NULL) {
        self->leftClickCounter = 10000;
        self->mouseTicksRan = self->ticksRan + 10000;
    }

    if (self->currentScreen != NULL) {
        GuiScreen_handleInput(self->currentScreen);
    }

    for (int _i = 0; _i < s_gui_pending_free_n; _i++)
        GuiScreen_free(s_gui_pending_free[_i]);
    s_gui_pending_free_n = 0;
    if (self->currentScreen != NULL) {
        if (self->currentScreen->field_25091_h != NULL)
            GuiParticle_func_25088_a(self->currentScreen->field_25091_h);
        self->currentScreen->vtable->updateScreen(self->currentScreen);
    }

    if (self->currentScreen == NULL || self->currentScreen->field_948_f) {

        MouseButtonEvent mev;
        while (mouse_btn_dequeue(&mev)) {
            s_cur_mouse_event = mev;

            int dw;
            while (scroll_dequeue(&dw)) {

                if (self->thePlayer != NULL) {
                    InventoryPlayer_changeCurrentItem(&((EntityPlayer *)self->thePlayer)->inventory,
                                                      dw);
                    if (self->gameSettings && self->gameSettings->field_22275_C) {
                        int var3s = dw;
                        if (var3s > 0)
                            var3s = 1;
                        if (var3s < 0)
                            var3s = -1;
                        self->gameSettings->field_22272_F += (float)var3s * 0.25f;
                    }
                }
            }

            if (self->currentScreen == NULL) {

                if (!self->inGameHasFocus && mev.action == GLFW_PRESS) {
                    Minecraft_setIngameFocus(self);
                } else {
                    if (mev.button == GLFW_MOUSE_BUTTON_LEFT && mev.action == GLFW_PRESS) {
                        Minecraft_clickMouse(self, 0);
                        self->mouseTicksRan = self->ticksRan;
                    }
                    if (mev.button == GLFW_MOUSE_BUTTON_RIGHT && mev.action == GLFW_PRESS) {
                        Minecraft_clickMouse(self, 1);
                        self->mouseTicksRan = self->ticksRan;
                    }
                    if (mev.button == GLFW_MOUSE_BUTTON_MIDDLE && mev.action == GLFW_PRESS) {
                        Minecraft_clickMiddleMouseButton(self);
                    }
                }
            } else {

                self->currentScreen->vtable->handleMouseInput(self->currentScreen);
            }
        }

#ifdef __EMSCRIPTEN__

        if (s_em_cursor_grabbed && !self->inGameHasFocus) {
            s_em_cursor_grabbed = 0;
            if (s_window)
                glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
#endif

        {
            int dw;
            while (scroll_dequeue(&dw)) {

                if (self->thePlayer != NULL) {
                    InventoryPlayer_changeCurrentItem(&((EntityPlayer *)self->thePlayer)->inventory,
                                                      dw);
                    if (self->gameSettings && self->gameSettings->field_22275_C) {
                        int var3s = dw;
                        if (var3s > 0)
                            var3s = 1;
                        if (var3s < 0)
                            var3s = -1;
                        self->gameSettings->field_22272_F += (float)var3s * 0.25f;
                    }
                }
            }
        }

        if (self->leftClickCounter > 0) {
            --self->leftClickCounter;
        }

        KeyEvent kev;
        while (key_dequeue(&kev)) {
            s_cur_key_event = kev;

            if (self->thePlayer != NULL) {
                int pressed = (kev.action != GLFW_RELEASE);
                EntityPlayerSP_handleKeyPress(self->thePlayer, kev.key, pressed);
            }

            if (kev.action == GLFW_PRESS || kev.action == GLFW_REPEAT) {
                if (kev.key == GLFW_KEY_F11) {
                    Minecraft_toggleFullscreen(self);
                } else {
                    if (self->currentScreen != NULL) {
                        self->currentScreen->vtable->handleKeyboardInput(self->currentScreen);
                    } else {
#ifdef __EMSCRIPTEN__
                        if (kev.key == GLFW_KEY_ESCAPE && self->inGameHasFocus &&
                            !s_em_focus_acquired)
#else
                        if (kev.key == GLFW_KEY_ESCAPE && self->inGameHasFocus)
#endif
                            Minecraft_displayInGameMenu(self);
                        if (kev.key == GLFW_KEY_S &&
                            glfwGetKey(s_window, GLFW_KEY_F3) == GLFW_PRESS)
                            Minecraft_forceReload(self);
                        if (kev.key == GLFW_KEY_F1 && self->gameSettings)
                            self->gameSettings->hideGUI = !self->gameSettings->hideGUI;
                        if (kev.key == GLFW_KEY_F3 && self->gameSettings)
                            self->gameSettings->showDebugInfo = !self->gameSettings->showDebugInfo;
                        if (kev.key == GLFW_KEY_F5 && self->gameSettings)
                            self->gameSettings->thirdPersonView =
                                !self->gameSettings->thirdPersonView;
                        if (kev.key == GLFW_KEY_F8 && self->gameSettings)
                            self->gameSettings->smoothCamera = !self->gameSettings->smoothCamera;
                        if (self->gameSettings &&
                            kev.key == self->gameSettings->keyBindInventory.keyCode &&
                            self->thePlayer != NULL) {
                            GuiInventory *inv = (GuiInventory *)calloc(1, sizeof(GuiInventory));
                            GuiInventory_construct(inv, (EntityPlayer *)self->thePlayer);
                            Minecraft_displayGuiScreen(self, (GuiScreen *)inv);
                        }
                        if (self->gameSettings &&
                            kev.key == self->gameSettings->keyBindDrop.keyCode &&
                            self->thePlayer != NULL)
                            EntityPlayer_dropCurrentItem((EntityPlayer *)self->thePlayer);
                        if (self->gameSettings && Minecraft_isMultiplayerWorld(self) &&
                            kev.key == self->gameSettings->keyBindChat.keyCode) {
                            GuiChat *chat = (GuiChat *)calloc(1, sizeof(GuiChat));
                            GuiChat_construct(chat);
                            Minecraft_displayGuiScreen(self, (GuiScreen *)chat);
                        }
                    }

                    if (self->thePlayer != NULL) {
                        for (int var6 = 0; var6 < 9; ++var6) {
                            if (kev.key == GLFW_KEY_1 + var6)
                                ((EntityPlayer *)self->thePlayer)->inventory.currentItem = var6;
                        }
                    }
                    if (self->gameSettings &&
                        kev.key == self->gameSettings->keyBindToggleFog.keyCode) {
                        int shift = (glfwGetKey(s_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                                     glfwGetKey(s_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
                        GameSettings_setOptionValue(self->gameSettings, EnumOptions_RENDER_DISTANCE,
                                                    shift ? -1 : 1);
                    }
                }
            }
        }

        if (self->currentScreen == NULL) {
            if (s_mouse_left_held &&
                (float)(self->ticksRan - self->mouseTicksRan) >=
                    self->timer.ticksPerSecond / 4.0f &&
                self->inGameHasFocus) {
                Minecraft_clickMouse(self, 0);
                self->mouseTicksRan = self->ticksRan;
            }
            if (s_mouse_right_held &&
                (float)(self->ticksRan - self->mouseTicksRan) >=
                    self->timer.ticksPerSecond / 4.0f &&
                self->inGameHasFocus) {
                Minecraft_clickMouse(self, 1);
                self->mouseTicksRan = self->ticksRan;
            }
        }
        Minecraft_func_6254_a(
            self, 0, self->currentScreen == NULL && s_mouse_left_held && self->inGameHasFocus);
    }

    if (self->theWorld != NULL) {

        if (self->thePlayer != NULL) {
            ++self->joinPlayerCounter;
            if (self->joinPlayerCounter == 30) {
                self->joinPlayerCounter = 0;
                World_joinEntityInSurroundings(self->theWorld, (struct Entity *)self->thePlayer);
            }
        }

        if (self->gameSettings)
            self->theWorld->difficultySetting = self->gameSettings->difficulty;
        if (self->theWorld->multiplayerWorld)
            self->theWorld->difficultySetting = 3;

        if (!self->isGamePaused && self->entityRenderer)
            EntityRenderer_updateRenderer((EntityRenderer *)self->entityRenderer);

        if (!self->isGamePaused && self->renderGlobal)
            RenderGlobal_updateClouds(self->renderGlobal);

        if (!self->isGamePaused) {
            if (self->theWorld->field_27172_i > 0)
                --self->theWorld->field_27172_i;
            World_updateEntities(self->theWorld);
        }

        if (!self->isGamePaused || Minecraft_isMultiplayerWorld(self)) {
            World_setAllowedMobSpawns(self->theWorld,
                                      self->gameSettings && self->gameSettings->difficulty > 0, 1);
            self->theWorld->vtable->tick(self->theWorld);
        }

        if (!self->isGamePaused && self->thePlayer != NULL) {
            Entity *pe = (Entity *)self->thePlayer;
            World_randomDisplayUpdates(self->theWorld, MathHelper_floor_double(pe->posX),
                                       MathHelper_floor_double(pe->posY),
                                       MathHelper_floor_double(pe->posZ));
        }

        if (!self->isGamePaused && self->effectRenderer)
            EffectRenderer_updateEffects((EffectRenderer *)self->effectRenderer);
    }

    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        self->systemTime = (long long)tv.tv_sec * 1000LL + tv.tv_usec / 1000LL;
    }

    if (self->pendingDestroyWorld2) {
        World_destroy(self->pendingDestroyWorld2);
        self->pendingDestroyWorld2 = NULL;
    }
    if (self->pendingDestroyWorld) {
        World_destroy(self->pendingDestroyWorld);
        self->pendingDestroyWorld = NULL;
    }
}

#include "ScreenShotHelper.h"
static void Minecraft_screenshotListener(Minecraft *self) {
    if (s_window && glfwGetKey(s_window, GLFW_KEY_F2) == GLFW_PRESS) {
        if (!self->isTakingScreenshot) {
            self->isTakingScreenshot = 1;
            if (self->ingameGUI) {
                const char *msg =
                    ScreenShotHelper_saveScreenshot(".", self->displayWidth, self->displayHeight);
                GuiIngame_addChatMessage((GuiIngame *)self->ingameGUI, msg);
            }
        }
    } else {
        self->isTakingScreenshot = 0;
    }
}

static void Minecraft_displayDebugInfo(Minecraft *self, long long var1) {
    long long var3 = 16666666LL;
    if (self->prevFrameTime == -1LL) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        self->prevFrameTime = (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
    }
    struct timespec ts2;
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    long long var5 = (long long)ts2.tv_sec * 1000000000LL + ts2.tv_nsec;

    Minecraft_tickTimes[Minecraft_numRecordedFrameTimes & (512 - 1)] = var1;
    Minecraft_frameTimes[Minecraft_numRecordedFrameTimes++ & (512 - 1)] =
        var5 - self->prevFrameTime;
    self->prevFrameTime = var5;

    R_clear(R_DEPTH_BUFFER_BIT);
    R_matrixMode(R_PROJECTION);
    R_loadIdentity();
    R_ortho(0.0, (double)self->displayWidth, (double)self->displayHeight, 0.0, 1000.0, 3000.0);
    R_matrixMode(R_MODELVIEW);
    R_loadIdentity();
    R_translatef(0.0f, 0.0f, -2000.0f);
    R_lineWidth(1.0f);
    R_disable(R_TEXTURE_2D);
    Tessellator *var7 = &Tessellator_instance;

    Tessellator_startDrawing(var7, 7);
    int var8 = (int)(var3 / 200000LL);

    Tessellator_setColorOpaque_I(var7, 536870912);
    Tessellator_addVertex(var7, 0.0, (double)(self->displayHeight - var8), 0.0);
    Tessellator_addVertex(var7, 0.0, (double)self->displayHeight, 0.0);
    Tessellator_addVertex(var7, (double)512, (double)self->displayHeight, 0.0);
    Tessellator_addVertex(var7, (double)512, (double)(self->displayHeight - var8), 0.0);

    Tessellator_setColorOpaque_I(var7, 538968064);
    Tessellator_addVertex(var7, 0.0, (double)(self->displayHeight - var8 * 2), 0.0);
    Tessellator_addVertex(var7, 0.0, (double)(self->displayHeight - var8), 0.0);
    Tessellator_addVertex(var7, (double)512, (double)(self->displayHeight - var8), 0.0);
    Tessellator_addVertex(var7, (double)512, (double)(self->displayHeight - var8 * 2), 0.0);
    Tessellator_draw(var7);

    long long var9 = 0;
    for (int var11i = 0; var11i < 512; ++var11i)
        var9 += Minecraft_frameTimes[var11i];
    int var11 = (int)(var9 / 200000LL / 512LL);
    Tessellator_startDrawing(var7, 7);
    Tessellator_setColorOpaque_I(var7, 541065216);
    Tessellator_addVertex(var7, 0.0, (double)(self->displayHeight - var11), 0.0);
    Tessellator_addVertex(var7, 0.0, (double)self->displayHeight, 0.0);
    Tessellator_addVertex(var7, (double)512, (double)self->displayHeight, 0.0);
    Tessellator_addVertex(var7, (double)512, (double)(self->displayHeight - var11), 0.0);
    Tessellator_draw(var7);
    Tessellator_startDrawing(var7, 1);
    for (int var12 = 0; var12 < 512; ++var12) {
        int var13 = (var12 - Minecraft_numRecordedFrameTimes & (512 - 1)) * 255 / 512;
        int var14 = var13 * var13 / 255;
        var14 = var14 * var14 / 255;
        int var15 = var14 * var14 / 255;
        var15 = var15 * var15 / 255;
        (void)var15;
        if (Minecraft_frameTimes[var12] > var3) {
            Tessellator_setColorOpaque_I(var7, -16777216 + var14 * 65536);
        } else {
            Tessellator_setColorOpaque_I(var7, -16777216 + var14 * 256);
        }
        long long var16 = Minecraft_frameTimes[var12] / 200000LL;
        long long var18 = Minecraft_tickTimes[var12] / 200000LL;
        Tessellator_addVertex(var7, (double)((float)var12 + 0.5f),
                              (double)((float)((long long)self->displayHeight - var16) + 0.5f),
                              0.0);
        Tessellator_addVertex(var7, (double)((float)var12 + 0.5f),
                              (double)((float)self->displayHeight + 0.5f), 0.0);
        Tessellator_setColorOpaque_I(var7, -16777216 + var14 * 65536 + var14 * 256 + var14 * 1);
        Tessellator_addVertex(var7, (double)((float)var12 + 0.5f),
                              (double)((float)((long long)self->displayHeight - var16) + 0.5f),
                              0.0);
        Tessellator_addVertex(
            var7, (double)((float)var12 + 0.5f),
            (double)((float)((long long)self->displayHeight - (var16 - var18)) + 0.5f), 0.0);
    }
    Tessellator_draw(var7);
    R_enable(R_TEXTURE_2D);
}

static void Minecraft_checkGLError(const char *location) {
    if (g_renderer_is_bgfx) {
        (void)location;
        return;
    }
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "########## GL ERROR ##########\n@ %s\n%d\n", location, (int)err);
    }
}

void Minecraft_runGameLoop(Minecraft *self) {

    AxisAlignedBB_clearBoundingBoxPool();
    Vec3D_initialize();

    if (glfwWindowShouldClose(s_window))
        Minecraft_shutdown(self);

    if (self->isGamePaused && self->theWorld != NULL) {
        float var4 = self->timer.renderPartialTicks;
        Timer_updateTimer(&self->timer);
        self->timer.renderPartialTicks = var4;
    } else {
        Timer_updateTimer(&self->timer);
    }

    struct timespec ts_tick_start;
    clock_gettime(CLOCK_MONOTONIC, &ts_tick_start);

    for (int i = 0; i < self->timer.elapsedTicks; i++) {
        self->ticksRan++;
        Minecraft_runTick(self);
    }

    struct timespec ts_tick_end;
    clock_gettime(CLOCK_MONOTONIC, &ts_tick_end);
    long long var24 = ((long long)ts_tick_end.tv_sec * 1000000000LL + ts_tick_end.tv_nsec) -
                      ((long long)ts_tick_start.tv_sec * 1000000000LL + ts_tick_start.tv_nsec);

    Minecraft_checkGLError("Pre render");

    if (self->gameSettings)
        RenderBlocks_fancyGrass = self->gameSettings->fancyGraphics;

    if (self->sndManager)
        SoundManager_setListener((SoundManager *)self->sndManager,
                                 (struct EntityLiving *)self->thePlayer,
                                 self->timer.renderPartialTicks);

    R_enable(R_TEXTURE_2D);

    if (self->theWorld != NULL)
        World_updatingLighting(self->theWorld);

    R_endFrame();
    R_beginFrame();
    glfwPollEvents();

    if (self->thePlayer != NULL && self->gameSettings &&
        Entity_isEntityInsideOpaqueBlock((Entity *)self->thePlayer))
        self->gameSettings->thirdPersonView = 0;

    if (!self->skipRenderWorld) {
        if (self->playerController)
            PlayerController_setPartialTime(self->playerController, self->timer.renderPartialTicks);
        if (self->entityRenderer)
            EntityRenderer_updateCameraAndRender((EntityRenderer *)self->entityRenderer,
                                                 self->timer.renderPartialTicks);
    }

    if (s_window && !glfwGetWindowAttrib(s_window, GLFW_FOCUSED)) {
        if (self->isFullScreen)
            Minecraft_toggleFullscreen(self);
    }

    if (self->gameSettings && self->gameSettings->showDebugInfo) {
        Minecraft_displayDebugInfo(self, var24);
    } else {
        struct timespec ts_now;
        clock_gettime(CLOCK_MONOTONIC, &ts_now);
        self->prevFrameTime = (long long)ts_now.tv_sec * 1000000000LL + ts_now.tv_nsec;
    }

    if (self->guiAchievement)
        GuiAchievement_updateAchievementWindow((GuiAchievement *)self->guiAchievement);

    Minecraft_screenshotListener(self);

    Minecraft_checkGLError("Post render");

    self->frameCounter++;

    self->isGamePaused = !Minecraft_isMultiplayerWorld(self) && self->currentScreen != NULL &&
                         self->currentScreen->vtable->doesGuiPauseGame(self->currentScreen);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long now = (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
    if (self->debugLastSync == 0)
        self->debugLastSync = now;
    while (now >= self->debugLastSync + 1000LL) {
        snprintf(self->debug, sizeof(self->debug), "%d fps, %d chunk updates", self->frameCounter,
                 WorldRenderer_chunksUpdated);
        WorldRenderer_chunksUpdated = 0;
        self->frameCounter = 0;
        self->debugLastSync += 1000LL;
    }
}

#ifdef __EMSCRIPTEN__
static Minecraft *s_em_mc = NULL;
static void Minecraft_emTick(void) {
    if (s_em_focus_acquired > 0)
        s_em_focus_acquired--;
    if (!s_em_mc->running || s_em_mc->hasCrashed) {
        emscripten_cancel_main_loop();
        Minecraft_shutdownMinecraftApplet(s_em_mc);
        return;
    }
    MinecraftError_active = 1;
    if (setjmp(MinecraftError_jmpbuf) == 0) {
        Minecraft_runGameLoop(s_em_mc);
    } else {
        MinecraftError_active = 0;
        emscripten_cancel_main_loop();
        Minecraft_shutdownMinecraftApplet(s_em_mc);
        return;
    }
    MinecraftError_active = 0;
}
#endif

void Minecraft_run(Minecraft *self) {
    self->running = 1;

    Minecraft_startGame(self);
    if (self->hasCrashed) {

        UnexpectedThrowable *ut = UnexpectedThrowable_create("Failed to start game", "");
        Minecraft_onMinecraftCrash(self, ut);
        UnexpectedThrowable_destroy(ut);
        Minecraft_shutdownMinecraftApplet(self);
        return;
    }

#ifdef __EMSCRIPTEN__

    s_em_mc = self;
    emscripten_set_main_loop(Minecraft_emTick, 0, 1);

#else

    MinecraftError_active = 1;
    if (setjmp(MinecraftError_jmpbuf) == 0) {
        while (self->running && !self->hasCrashed) {
            Minecraft_runGameLoop(self);
        }
    }

    MinecraftError_active = 0;

    Minecraft_shutdownMinecraftApplet(self);
#endif
}

void Minecraft_shutdownMinecraftApplet(Minecraft *self) {
    if (self->statFileWriter) {
        StatFileWriter_func_27175_b(self->statFileWriter);
        StatFileWriter_syncStats(self->statFileWriter);
    }

    if (self->downloadResourcesThread)
        ThreadDownloadResources_closeMinecraft(self->downloadResourcesThread);
    printf("Stopping!\n");

    MinecraftError_active = 1;
    if (setjmp(MinecraftError_jmpbuf) == 0) {
        Minecraft_changeWorld1(self, NULL);
        if (self->pendingDestroyWorld) {
            World_destroy(self->pendingDestroyWorld);
            self->pendingDestroyWorld = NULL;
        }
    }
    MinecraftError_active = 0;
    GLAllocation_deleteTexturesAndDisplayLists();
    if (self->sndManager)
        SoundManager_closeMinecraft((SoundManager *)self->sndManager);

    if (s_window) {
        glfwDestroyWindow(s_window);
        s_window = NULL;
    }
    glfwTerminate();
    if (!self->hasCrashed) {
        exit(0);
    }
}

void Minecraft_stopGame(Minecraft *self) {
    self->running = 0;

    if (self->playerController) {
        PlayerController_destroy(self->playerController);
        self->playerController = NULL;
    }
    if (self->thePlayer) {
        free(self->thePlayer);
        self->thePlayer = NULL;
        self->renderViewEntity = NULL;
    }
    if (self->pendingDestroyWorld2) {
        World_destroy(self->pendingDestroyWorld2);
        self->pendingDestroyWorld2 = NULL;
    }
    if (self->pendingDestroyWorld) {
        World_destroy(self->pendingDestroyWorld);
        self->pendingDestroyWorld = NULL;
    }
    if (self->theWorld) {
        World_destroy(self->theWorld);
        self->theWorld = NULL;
    }
    if (self->ingameGUI) {
        GuiIngame_destroy((GuiIngame *)self->ingameGUI);
        self->ingameGUI = NULL;
    }
    if (self->sndManager) {
        SoundManager_destroy((SoundManager *)self->sndManager);
        self->sndManager = NULL;
    }
    if (self->effectRenderer) {
        EffectRenderer_destroy((EffectRenderer *)self->effectRenderer);
        self->effectRenderer = NULL;
    }
    if (self->renderGlobal) {
        RenderGlobal_destroy((RenderGlobal *)self->renderGlobal);
        self->renderGlobal = NULL;
    }
    if (self->entityRenderer) {
        EntityRenderer_destroy((EntityRenderer *)self->entityRenderer);
        self->entityRenderer = NULL;
    }
    if (self->fontRenderer) {
        FontRenderer_destroy(self->fontRenderer);
        self->fontRenderer = NULL;
    }
    if (self->renderEngine) {
        RenderEngine_destroy(self->renderEngine);
        self->renderEngine = NULL;
    }
    if (self->session) {
        Session_free(self->session);
        self->session = NULL;
    }
    if (self->gameSettings) {
        free(self->gameSettings);
        self->gameSettings = NULL;
    }
}

void Minecraft_destroy(Minecraft *self) {
    if (!self)
        return;
    for (int _i = 0; _i < s_gui_pending_free_n; _i++)
        GuiScreen_free(s_gui_pending_free[_i]);
    s_gui_pending_free_n = 0;
    if (self->currentScreen) {
        GuiScreen_free(self->currentScreen);
        self->currentScreen = NULL;
    }
    if (self->pendingDestroyWorld2) {
        World_destroy(self->pendingDestroyWorld2);
        self->pendingDestroyWorld2 = NULL;
    }
    if (self->pendingDestroyWorld) {
        World_destroy(self->pendingDestroyWorld);
        self->pendingDestroyWorld = NULL;
    }
    if (self->theWorld) {
        World_destroy(self->theWorld);
        self->theWorld = NULL;
    }
    if (self->playerController) {
        PlayerController_destroy(self->playerController);
        self->playerController = NULL;
    }
    free(self);
    s_mc = NULL;
}

void Minecraft_displayGuiScreen(Minecraft *self, struct GuiScreen *screen) {

    if (self->currentScreen != NULL && self->currentScreen->guiType == GUI_TYPE_UNUSED)
        return;

    if (self->currentScreen != NULL) {
        self->currentScreen->vtable->onGuiClosed(self->currentScreen);

        if (screen != NULL && screen->parentScreen == self->currentScreen) {

        } else {

            if (self->currentScreen->parentScreen == screen)
                self->currentScreen->parentScreen = NULL;
            if (s_gui_pending_free_n < 4)
                s_gui_pending_free[s_gui_pending_free_n++] = self->currentScreen;
            else
                GuiScreen_free(self->currentScreen);
        }
        self->currentScreen = NULL;
    }

    if (screen != NULL && screen->guiType == GUI_TYPE_MAIN_MENU && self->statFileWriter != NULL)
        StatFileWriter_func_27175_b(self->statFileWriter);

    if (self->statFileWriter != NULL)
        StatFileWriter_syncStats(self->statFileWriter);

    if (screen == NULL && self->theWorld == NULL) {
        screen = (GuiScreen *)GuiMainMenu_create();
    } else if (screen == NULL && self->thePlayer != NULL &&
               ((EntityLiving *)self->thePlayer)->health <= 0) {
        screen = (GuiScreen *)GuiGameOver_create();
    }

    if (screen != NULL && screen->guiType == GUI_TYPE_MAIN_MENU && self->ingameGUI != NULL)
        GuiIngame_clearChatMessages(self->ingameGUI);

    self->currentScreen = screen;
    if (screen != NULL) {
        Minecraft_setIngameNotInFocus(self);
        ScaledResolution var2;
        ScaledResolution_init(&var2, self->gameSettings, self->displayWidth, self->displayHeight);
        GuiScreen_setWorldAndResolution(screen, self, ScaledResolution_getScaledWidth(&var2),
                                        ScaledResolution_getScaledHeight(&var2));
        self->skipRenderWorld = 0;
    } else {
        Minecraft_setIngameFocus(self);
    }
}

void Minecraft_displayInGameMenu(Minecraft *self) {
    if (self->currentScreen == NULL) {
        Minecraft_displayGuiScreen(self, (struct GuiScreen *)GuiIngameMenu_create());
    }
}

void Minecraft_shutdown(Minecraft *self) { self->running = 0; }

void Minecraft_toggleFullscreen(Minecraft *self) {
    self->isFullScreen = !self->isFullScreen;
    if (self->isFullScreen) {

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        self->tempDisplayWidth = self->displayWidth;
        self->tempDisplayHeight = self->displayHeight;
        self->displayWidth = mode->width;
        self->displayHeight = mode->height;
        if (self->displayWidth <= 0)
            self->displayWidth = 1;
        if (self->displayHeight <= 0)
            self->displayHeight = 1;
        glfwSetWindowMonitor(s_window, monitor, 0, 0, self->displayWidth, self->displayHeight,
                             GLFW_DONT_CARE);
    } else {

        self->displayWidth = self->tempDisplayWidth;
        self->displayHeight = self->tempDisplayHeight;
        if (self->displayWidth <= 0)
            self->displayWidth = 1;
        if (self->displayHeight <= 0)
            self->displayHeight = 1;
        glfwSetWindowMonitor(s_window, NULL, 0, 0, self->displayWidth, self->displayHeight, 0);
    }

    if (self->currentScreen != NULL) {
        ScaledResolution sr;
        ScaledResolution_init(&sr, self->gameSettings, self->displayWidth, self->displayHeight);
        GuiScreen_setWorldAndResolution(self->currentScreen, self,
                                        ScaledResolution_getScaledWidth(&sr),
                                        ScaledResolution_getScaledHeight(&sr));
    }
}

void Minecraft_usePortal(Minecraft *self) {
    EntityPlayer *thePlayer = (EntityPlayer *)self->thePlayer;
    Entity *playerEnt = (Entity *)self->thePlayer;
    if (thePlayer->dimension == -1) {
        thePlayer->dimension = 0;
    } else {
        thePlayer->dimension = -1;
    }
    World_setEntityDead(self->theWorld, playerEnt);
    playerEnt->isDead = 0;
    double var1 = playerEnt->posX;
    double var3 = playerEnt->posZ;
    double var5 = 8.0;
    World *var7;
    if (thePlayer->dimension == -1) {
        var1 /= var5;
        var3 /= var5;
        playerEnt->vtable->setLocationAndAngles(playerEnt, var1, playerEnt->posY, var3,
                                                playerEnt->rotationYaw, playerEnt->rotationPitch);
        if (playerEnt->vtable->isEntityAlive(playerEnt)) {
            World_updateEntityWithOptionalForce(self->theWorld, playerEnt, 0);
        }
        var7 = NULL;
        var7 = World_createFromWorld(self->theWorld, WorldProvider_getProviderForDimension(-1));
        Minecraft_changeWorld(self, var7, "Entering the Nether", thePlayer);
    } else {
        var1 *= var5;
        var3 *= var5;
        playerEnt->vtable->setLocationAndAngles(playerEnt, var1, playerEnt->posY, var3,
                                                playerEnt->rotationYaw, playerEnt->rotationPitch);
        if (playerEnt->vtable->isEntityAlive(playerEnt)) {
            World_updateEntityWithOptionalForce(self->theWorld, playerEnt, 0);
        }
        var7 = NULL;
        var7 = World_createFromWorld(self->theWorld, WorldProvider_getProviderForDimension(0));
        Minecraft_changeWorld(self, var7, "Leaving the Nether", thePlayer);
    }
    playerEnt->worldObj = self->theWorld;
    if (playerEnt->vtable->isEntityAlive(playerEnt)) {
        playerEnt->vtable->setLocationAndAngles(playerEnt, var1, playerEnt->posY, var3,
                                                playerEnt->rotationYaw, playerEnt->rotationPitch);
        World_updateEntityWithOptionalForce(self->theWorld, playerEnt, 0);
        Teleporter *teleporter = Teleporter_create();
        Teleporter_func_4107_a(teleporter, self->theWorld, playerEnt);
        Teleporter_destroy(teleporter);
    }
}

int Minecraft_isDebugInfoEnabled(void) {
    return s_mc && s_mc->gameSettings ? s_mc->gameSettings->showDebugInfo : 0;
}

int Minecraft_isGuiEnabled(void) {
    return s_mc && s_mc->gameSettings ? !s_mc->gameSettings->hideGUI : 1;
}

int Minecraft_isMultiplayerWorld(Minecraft *self) {
    return self->theWorld != NULL && self->theWorld->multiplayerWorld;
}

struct ISaveFormat *Minecraft_getSaveLoader(Minecraft *self) { return self->saveLoader; }

void Minecraft_startWorld(Minecraft *self, const char *var1, const char *var2, int64_t var3) {
    Minecraft_changeWorld1(self, (World *)NULL);
    if (self->saveLoader == NULL)
        return;
    if (ISaveFormat_isOldMapFormat(self->saveLoader, var1)) {
        Minecraft_convertMapFormat(self, var1, var2);
        (void)var3;
    } else {
        ISaveHandler *var5 = ISaveFormat_getSaveLoader(self->saveLoader, var1, 0);

        World *var6 = World_create_from_save(var5, var2, var3);
        if (var6 == NULL)
            return;
        if (var6->isNewWorld) {
            if (self->statFileWriter) {
                StatFileWriter_readStat(self->statFileWriter, StatList_createWorldStat, 1);
                StatFileWriter_readStat(self->statFileWriter, StatList_startGameStat, 1);
            }
            Minecraft_changeWorld2(self, var6, "Generating level");
        } else {
            if (self->statFileWriter) {
                StatFileWriter_readStat(self->statFileWriter, StatList_loadWorldStat, 1);
                StatFileWriter_readStat(self->statFileWriter, StatList_startGameStat, 1);
            }
            Minecraft_changeWorld2(self, var6, "Loading level");
        }
    }
}

static void Minecraft_convertMapFormat(Minecraft *self, const char *var1, const char *var2) {
    char buf[256];
    snprintf(buf, sizeof(buf), "Converting World to %s",
             ISaveFormat_func_22178_a(self->saveLoader));
    LoadingScreenRenderer_printText(self->loadingScreen, buf);
    LoadingScreenRenderer_displayLoadingString(self->loadingScreen, "This may take a while :)");
    ISaveFormat_convertMapFormat(self->saveLoader, var1, (IProgressUpdate *)self->loadingScreen);
    Minecraft_startWorld(self, var1, var2, 0L);
}

static void Minecraft_func_6255_d(Minecraft *self, const char *var1) {
    LoadingScreenRenderer_printText(self->loadingScreen, var1);
    LoadingScreenRenderer_displayLoadingString(self->loadingScreen, "Building terrain");
    short var2 = 128;
    int var3 = 0;
    int var4 = var2 * 2 / 16 + 1;
    var4 *= var4;
    IChunkProvider *var5 = World_getIChunkProvider(self->theWorld);
    ChunkCoordinates var6 = World_getSpawnPoint(self->theWorld);
    if (self->thePlayer != NULL) {
        var6.x = (int)((Entity *)self->thePlayer)->posX;
        var6.z = (int)((Entity *)self->thePlayer)->posZ;
    }
    if (var5 != NULL && var5->vtable == &ChunkProviderLoadOrGenerate_vtable) {
        ChunkProviderLoadOrGenerate *var7 = (ChunkProviderLoadOrGenerate *)var5;
        ChunkProviderLoadOrGenerate_setCurrentChunkOver(var7, var6.x >> 4, var6.z >> 4);
    }
    for (int var10 = -var2; var10 <= var2; var10 += 16) {
        for (int var8 = -var2; var8 <= var2; var8 += 16) {
            LoadingScreenRenderer_setLoadingProgress(self->loadingScreen, var3++ * 100 / var4);
            World_getBlockId(self->theWorld, var6.x + var10, 64, var6.z + var8);
            while (World_updatingLighting(self->theWorld)) {
                glfwPollEvents();
            }
        }
    }
    LoadingScreenRenderer_displayLoadingString(self->loadingScreen, "Simulating world for a bit");
    int var9 = 1;
    (void)var9;
    World_func_656_j(self->theWorld);
}

const char *Minecraft_func_6241_m(Minecraft *self) {
    static char buf[256];
    if (self->renderGlobal) {
        return RenderGlobal_getDebugInfoRenders(self->renderGlobal, buf, sizeof(buf));
    }
    return "";
}

const char *Minecraft_func_6262_n(Minecraft *self) {
    static char buf[256];
    if (self->renderGlobal) {
        return RenderGlobal_getDebugInfoEntities(self->renderGlobal, buf, sizeof(buf));
    }
    return "";
}

const char *Minecraft_func_21002_o(Minecraft *self) {
    static char buf[256];
    if (self->theWorld) {
        return World_func_21119_g(self->theWorld, buf, sizeof(buf));
    }
    return "";
}

const char *Minecraft_func_6245_o(Minecraft *self) {
    static char buf[256];
    char effects[64] = "";
    char entities[64] = "";
    if (self->effectRenderer) {
        EffectRenderer_getStatistics(self->effectRenderer, effects, sizeof(effects));
    }
    if (self->theWorld) {
        World_func_687_d(self->theWorld, entities, sizeof(entities));
    }
    snprintf(buf, sizeof(buf), "P: %s. T: %s", effects, entities);
    return buf;
}

const char *Minecraft_getAppDir(const char *var0) {
    static char buf[512];
    const char *var1 = getenv("HOME");
    if (var1 == NULL)
        var1 = ".";

#if defined(__APPLE__)
    snprintf(buf, sizeof(buf), "%s/Library/Application Support/%s", var1, var0);
#elif defined(_WIN32)
    const char *var3 = getenv("APPDATA");
    if (var3 != NULL) {
        snprintf(buf, sizeof(buf), "%s/.%s/", var3, var0);
    } else {
        snprintf(buf, sizeof(buf), "%s/.%s/", var1, var0);
    }
#else
    snprintf(buf, sizeof(buf), "%s/.%s/", var1, var0);
#endif

    mc_mkdir(buf);
    return buf;
}

const char *Minecraft_getMinecraftDir(void) {
    static const char *minecraftDir = NULL;
    if (minecraftDir == NULL) {
        minecraftDir = Minecraft_getAppDir("minecraft");
    }
    return minecraftDir;
}

void Sys_openURL(const char *url) {
    char cmd[1024];
#if defined(__APPLE__)
    snprintf(cmd, sizeof(cmd), "open \"%s\"", url);
#elif defined(_WIN32)
    snprintf(cmd, sizeof(cmd), "start \"\" \"%s\"", url);
#else
    snprintf(cmd, sizeof(cmd), "xdg-open \"%s\"", url);
#endif
    system(cmd);
}

void Minecraft_enableKeyRepeatEvents(int enable) { s_key_repeat_enabled = enable; }

int Minecraft_lineIsCommand(Minecraft *self, const char *var1) {
    (void)self;
    if (var1 != NULL && var1[0] == '/') {
    }
    return 0;
}

void Minecraft_respawn(Minecraft *self, int var1, int var2) {
    if (!self->theWorld->multiplayerWorld &&
        !WorldProvider_canRespawnHere(self->theWorld->worldProvider)) {
        Minecraft_usePortal(self);
    }

    ChunkCoordinates *var3 = NULL;
    ChunkCoordinates *var4 = NULL;
    int var5 = 1;

    if (self->thePlayer != NULL && !var1) {
        var3 = EntityPlayer_getPlayerSpawnCoordinate((EntityPlayer *)self->thePlayer);
        if (var3 != NULL) {
            var4 = EntityPlayer_func_25060_a(self->theWorld, var3);
            if (var4 == NULL) {
                EntityPlayer_addChatMessage((EntityPlayer *)self->thePlayer, "tile.bed.notValid");
            }
        }
    }

    if (var4 == NULL) {
        ChunkCoordinates sp = World_getSpawnPoint(self->theWorld);
        static ChunkCoordinates s_sp;
        s_sp = sp;
        var4 = &s_sp;
        var5 = 0;
    }

    IChunkProvider *var6 = World_getIChunkProvider(self->theWorld);
    if (var6 != NULL && var6->vtable == &ChunkProviderLoadOrGenerate_vtable) {
        ChunkProviderLoadOrGenerate *var7 = (ChunkProviderLoadOrGenerate *)var6;
        ChunkProviderLoadOrGenerate_setCurrentChunkOver(var7, var4->x >> 4, var4->z >> 4);
    }

    self->theWorld->vtable->setSpawnLocation(self->theWorld);
    World_updateEntityList(self->theWorld);

    int var8 = 0;
    if (self->thePlayer != NULL) {
        var8 = ((Entity *)self->thePlayer)->entityId;
        World_setEntityDead(self->theWorld, (Entity *)self->thePlayer);
    }

    self->renderViewEntity = NULL;
    self->thePlayer = (struct EntityPlayerSP *)PlayerController_createPlayer(self->playerController,
                                                                             self->theWorld);
    ((EntityPlayer *)self->thePlayer)->dimension = var2;
    self->renderViewEntity = (EntityLiving *)self->thePlayer;
    Entity_preparePlayerToSpawn((Entity *)self->thePlayer);

    if (var5) {
        EntityPlayer_setPlayerSpawnCoordinate((EntityPlayer *)self->thePlayer, var3);
        ((Entity *)self->thePlayer)
            ->vtable->setLocationAndAngles(
                (Entity *)self->thePlayer, (double)((float)var4->x + 0.5F),
                (double)((float)var4->y + 0.1F), (double)((float)var4->z + 0.5F), 0.0F, 0.0F);
    }

    PlayerController_flipPlayer(self->playerController, (EntityPlayer *)self->thePlayer);
    World_spawnPlayerWithLoadedChunks(self->theWorld, (Entity *)self->thePlayer);

    free(self->thePlayer->movementInput);
    self->thePlayer->movementInput =
        (MovementInput *)MovementInputFromOptions_create(self->gameSettings);

    ((Entity *)self->thePlayer)->entityId = var8;
    EntityPlayerSP_func_6420_o(self->thePlayer);
    PlayerController_func_6473_b(self->playerController, (EntityPlayer *)self->thePlayer);
    Minecraft_func_6255_d(self, "Respawning");

    if (self->currentScreen != NULL && self->currentScreen->guiType == GUI_TYPE_GAME_OVER) {
        Minecraft_displayGuiScreen(self, (struct GuiScreen *)NULL);
    }
}

void Minecraft_installResource(Minecraft *self, const char *var1, const char *filePath) {
    const char *slash = strchr(var1, '/');
    if (!slash)
        return;
    int prefixLen = (int)(slash - var1);
    char var4[64];
    if (prefixLen >= (int)sizeof(var4))
        return;
    strncpy(var4, var1, (size_t)prefixLen);
    var4[prefixLen] = '\0';
    const char *name = slash + 1;
    if (strcasecmp(var4, "sound") == 0 || strcasecmp(var4, "newsound") == 0) {
        SoundManager_addSound(self->sndManager, name, filePath);
    } else if (strcasecmp(var4, "streaming") == 0) {
        SoundManager_addStreaming(self->sndManager, name, filePath);
    } else if (strcasecmp(var4, "music") == 0 || strcasecmp(var4, "newmusic") == 0) {
        SoundManager_addMusic(self->sndManager, name, filePath);
    }
}

void Minecraft_setServer(Minecraft *self, const char *serverName, int serverPort) {
    if (serverName) {
        strncpy(self->serverName, serverName, sizeof(self->serverName) - 1);
        self->serverName[sizeof(self->serverName) - 1] = '\0';
    } else {
        self->serverName[0] = '\0';
    }
    self->serverPort = serverPort;
}

void Minecraft_func_28002_e(Minecraft *self) {
    if (self->renderGlobal)
        RenderGlobal_func_28137_f((RenderGlobal *)self->renderGlobal);
    AxisAlignedBB_func_28196_a();
    Vec3D_clearPool();
    Minecraft_changeWorld1(self, NULL);
}

struct NetClientHandler *Minecraft_getSendQueue(Minecraft *self) {
    if (self->thePlayer == NULL)
        return NULL;
    if (!((Entity *)self->thePlayer)->isClientPlayerMP)
        return NULL;
    return ((EntityClientPlayerMP *)self->thePlayer)->sendQueue;
}

int Minecraft_isFancyGraphicsEnabled(void) {
    return s_mc && s_mc->gameSettings ? s_mc->gameSettings->fancyGraphics : 0;
}

void Minecraft_onMinecraftCrash(Minecraft *self, UnexpectedThrowable *var1) {
    self->hasCrashed = 1;
    (void)var1;
}

OpenGlCapsChecker *Minecraft_getOpenGlCapsChecker(Minecraft *self) { return &self->glCapabilities; }
