#ifndef MINECRAFT_H
#define MINECRAFT_H

#include <stdint.h>
#include "GameSettings.h"
#include "RenderEngine.h"
#include "World.h"
#include "EntityLiving.h"
#include "MovingObjectPosition.h"
#include "Timer.h"
#include "PlayerController.h"
#include "Session.h"
#include "FontRenderer.h"
#include "OpenGlCapsChecker.h"
#include "UnexpectedThrowable.h"

struct EntityPlayerSP;
struct GuiScreen;
struct GuiIngame;
struct SoundManager;
struct StatFileWriter;
struct RenderGlobal;
struct EffectRenderer;
struct GuiAchievement;
struct EntityRenderer;
struct TexturePackList;
struct ISaveFormat;
struct LoadingScreenRenderer;

typedef struct MouseHelper {
    int deltaX;
    int deltaY;
} MouseHelper;

void MouseHelper_mouseXYChange(MouseHelper *self);
void MouseHelper_grabMouseCursor(MouseHelper *self);
void MouseHelper_ungrabMouseCursor(MouseHelper *self);

typedef struct Minecraft {
    GameSettings *gameSettings;
    RenderEngine *renderEngine;
    FontRenderer *fontRenderer;

    World *theWorld;
    World *pendingDestroyWorld;
    World *pendingDestroyWorld2;
    struct EntityPlayerSP *thePlayer;
    EntityLiving *renderViewEntity;

    struct RenderGlobal *renderGlobal;
    struct EntityRenderer *entityRenderer;
    struct EffectRenderer *effectRenderer;

    int displayWidth;
    int displayHeight;
    int isFullScreen;

    MouseHelper mouseHelper;

    struct GuiScreen *currentScreen;
    int isGamePaused;
    int inGameHasFocus;
    int skipRenderWorld;

    MovingObjectPosition *objectMouseOver;

    PlayerController *playerController;

    struct GuiIngame *ingameGUI;
    struct GuiAchievement *guiAchievement;

    Session *session;

    Timer timer;
    int ticksRan;

    int leftClickCounter;

    int mouseTicksRan;

    int joinPlayerCounter;

    int running;
    int hasCrashed;

    int hideQuitButton;

    struct SoundManager *sndManager;
    struct StatFileWriter *statFileWriter;

    struct ThreadDownloadResources *downloadResourcesThread;

    struct LoadingScreenRenderer *loadingScreen;

    struct TexturePackList *texturePackList;

    char debug[256];

    long long systemTime;

    long long debugLastSync;

    int frameCounter;

    struct ISaveFormat *saveLoader;

    int tempDisplayWidth;
    int tempDisplayHeight;

    int isTakingScreenshot;

    OpenGlCapsChecker glCapabilities;

    long long prevFrameTime;

    int isRaining;

    char minecraftUri[256];

    char serverName[256];

    int serverPort;
} Minecraft;

Minecraft *Minecraft_create(int width, int height, int fullscreen);
void Minecraft_destroy(Minecraft *self);

void Minecraft_setWindow(void *glfwWin);
void Minecraft_run(Minecraft *self);
void Minecraft_startGame(Minecraft *self);
void Minecraft_runGameLoop(Minecraft *self);
void Minecraft_runTick(Minecraft *self);

void Minecraft_shutdownMinecraftApplet(Minecraft *self);

void Minecraft_stopGame(Minecraft *self);

void Minecraft_changeWorld1(Minecraft *self, World *world);
void Minecraft_changeWorld2(Minecraft *self, World *world, const char *loadingMessage);

void Minecraft_changeWorld(Minecraft *self, World *world, const char *loadingMessage,
                           struct EntityPlayer *var3);

void Minecraft_displayGuiScreen(Minecraft *self, struct GuiScreen *screen);
void Minecraft_displayInGameMenu(Minecraft *self);

void Minecraft_toggleFullscreen(Minecraft *self);

void Minecraft_shutdown(Minecraft *self);

void Minecraft_usePortal(Minecraft *self);

void Minecraft_respawn(Minecraft *self, int var1, int var2);

Minecraft *Minecraft_getMinecraft(void);

void Minecraft_invalidateItemToRender(ItemStack *stack);

extern int64_t Minecraft_hasPaidCheckTime;
extern const char *g_assetsDir;
extern const char *g_assetIndex;

int Minecraft_isAmbientOcclusionEnabled(void);

int Minecraft_mouseNext(void);
int Minecraft_keyboardNext(void);

int Minecraft_getKeyboardEventKey(void);
int Minecraft_getKeyboardEventKeyState(void);
char Minecraft_getKeyboardEventCharacter(void);
int Minecraft_getMouseEventX(void);
int Minecraft_getMouseEventY(void);
int Minecraft_getMouseEventButton(void);
int Minecraft_getMouseEventButtonState(void);

void Minecraft_getCursorPos(double *outX, double *outY);

int Minecraft_isMouseButtonDown(int button);

void Minecraft_setIngameFocus(Minecraft *self);

void Minecraft_setIngameNotInFocus(Minecraft *self);

int Minecraft_isDebugInfoEnabled(void);

int Minecraft_isGuiEnabled(void);

int Minecraft_isMultiplayerWorld(Minecraft *self);

struct ISaveFormat *Minecraft_getSaveLoader(Minecraft *self);

void Minecraft_startWorld(Minecraft *self, const char *var1, const char *var2, int64_t var3);

const char *Minecraft_func_6241_m(Minecraft *self);

const char *Minecraft_func_6262_n(Minecraft *self);

const char *Minecraft_func_21002_o(Minecraft *self);

const char *Minecraft_func_6245_o(Minecraft *self);

void Minecraft_enableKeyRepeatEvents(int enable);

int Minecraft_lineIsCommand(Minecraft *self, const char *var1);

const char *Minecraft_getMinecraftDir(void);

const char *Minecraft_getAppDir(const char *var0);

void Sys_openURL(const char *url);

void Minecraft_installResource(Minecraft *self, const char *var1, const char *filePath);

void Minecraft_setServer(Minecraft *self, const char *serverName, int serverPort);

void Minecraft_onMinecraftCrash(Minecraft *self, UnexpectedThrowable *var1);

void Minecraft_shutdownMinecraftApplet(Minecraft *self);

OpenGlCapsChecker *Minecraft_getOpenGlCapsChecker(Minecraft *self);

void Minecraft_func_28002_e(Minecraft *self);

void Minecraft_func_6274_a(Minecraft *self, int var1, int var2, int var3, int var4, int var5,
                           int var6);

struct NetClientHandler *Minecraft_getSendQueue(Minecraft *self);

int Minecraft_isFancyGraphicsEnabled(void);

extern long long Minecraft_frameTimes[512];
extern long long Minecraft_tickTimes[512];
extern int Minecraft_numRecordedFrameTimes;

void *Minecraft_getWindow(void);

#endif
