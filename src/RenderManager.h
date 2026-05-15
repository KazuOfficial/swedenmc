#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "RenderEngine.h"
#include "GameSettings.h"
#include "World.h"
#include "ItemRenderer.h"
#include "EntityLiving.h"

struct Entity;
struct FontRenderer;
struct Render;

typedef struct RenderManager {
    RenderEngine *renderEngine;
    World *worldObj;
    struct FontRenderer *fontRenderer;
    EntityLiving *livingPlayer;
    ItemRenderer *itemRenderer;
    float playerViewY;
    float playerViewX;
    GameSettings *options;
    double field_1222_l;
    double field_1221_m;
    double field_1220_n;

    struct Render *renderSpider;
    struct Render *renderPig;
    struct Render *renderSheep;
    struct Render *renderCow;
    struct Render *renderWolf;
    struct Render *renderChicken;
    struct Render *renderCreeper;
    struct Render *renderSkeleton;
    struct Render *renderZombie;
    struct Render *renderSlime;
    struct Render *renderPlayer;
    struct Render *renderGiantZombie;
    struct Render *renderGhast;
    struct Render *renderSquid;
    struct Render *renderLiving;
    struct Render *renderEntityBase;
    struct Render *renderPainting;
    struct Render *renderArrow;
    struct Render *renderSnowball;
    struct Render *renderEgg;
    struct Render *renderFireball;
    struct Render *renderItemEntity;
    struct Render *renderTNTPrimed;
    struct Render *renderFallingSand;
    struct Render *renderMinecart;
    struct Render *renderBoat;
    struct Render *renderFish;
    struct Render *renderLightningBolt;
} RenderManager;

extern RenderManager *RenderManager_instance;
extern double RenderManager_renderPosX;
extern double RenderManager_renderPosY;
extern double RenderManager_renderPosZ;

RenderManager *RenderManager_create(void);
void RenderManager_destroy(RenderManager *self);

struct Render *RenderManager_getEntityRenderObject(RenderManager *self, struct Entity *var1);

void RenderManager_renderEntity(RenderManager *self, struct Entity *var1, float var2);
void RenderManager_renderEntityWithPosYaw(RenderManager *self, struct Entity *entity, double x,
                                          double y, double z, float yaw, float partialTick);

void RenderManager_cacheActiveRenderInfo(RenderManager *self, World *var1, RenderEngine *var2,
                                         struct FontRenderer *var3, EntityLiving *var4,
                                         GameSettings *var5, float var6);

double RenderManager_func_851_a(RenderManager *self, double var1, double var3, double var5);

struct FontRenderer *RenderManager_getFontRenderer(RenderManager *self);

void RenderManager_func_852_a(RenderManager *self, World *var1);

#endif
