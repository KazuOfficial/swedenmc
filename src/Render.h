#ifndef RENDER_H
#define RENDER_H

#include "GameForward.h"
#include "RenderManager.h"
#include "ModelBase.h"
#include "RenderBlocks.h"

struct Render;
struct Entity;
struct AxisAlignedBB;

typedef struct Render_vtable {
    void (*doRender)(struct Render *self, struct Entity *var1, double var2, double var4,
                     double var6, float var8, float var9);
} Render_vtable;

typedef struct Render {
    const Render_vtable *vtable;
    RenderManager *renderManager;
    ModelBase *modelBase;
    RenderBlocks *renderBlocks;
    float shadowSize;
    float field_194_c;
} Render;

void Render_init(Render *self);

void Render_loadTexture(Render *self, const char *var1);

int Render_loadDownloadableImageTexture(Render *self, const char *var1, const char *var2);

void Render_renderOffsetAABB(AxisAlignedBB *var0, double var1, double var3, double var5);

void Render_renderAABB(AxisAlignedBB *var0);

void Render_setRenderManager(Render *self, RenderManager *var1);

void Render_doRenderShadowAndFire(Render *self, struct Entity *var1, double var2, double var4,
                                  double var6, float var8, float var9);

struct FontRenderer *Render_getFontRendererFromRenderManager(Render *self);

void Render_destroy(Render *self);

#endif
