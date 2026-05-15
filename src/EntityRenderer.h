#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include "Minecraft.h"
#include "ItemRenderer.h"
#include "MouseFilter.h"
#include "JavaRandom.h"
#include <stdint.h>

typedef struct EntityRenderer {
    Minecraft *mc;
    float farPlaneDistance;
    ItemRenderer itemRenderer;
    int rendererUpdateCount;
    Entity *pointedEntity;

    MouseFilter mouseFilterXAxis;
    MouseFilter mouseFilterYAxis;
    MouseFilter mouseFilterDummy1;
    MouseFilter mouseFilterDummy2;
    MouseFilter mouseFilterDummy3;
    MouseFilter mouseFilterDummy4;

    float field_22228_r;
    float field_22227_s;
    float field_22226_t;
    float field_22225_u;
    float field_22224_v;
    float field_22223_w;
    float field_22222_x;
    float field_22221_y;
    float field_22220_z;
    float field_22230_A;

    int cloudFog;

    double cameraZoom;
    double cameraYaw;
    double cameraPitch;

    int64_t prevFrameTime;
    int64_t field_28133_I;

    JavaRandom random;
    int rainSoundCounter;

    volatile int field_1394_b;
    volatile int field_1393_c;

    float fogColorBuffer[4];
    float fogColorRed;
    float fogColorGreen;
    float fogColorBlue;
    float fogColor2;
    float fogColor1;

} EntityRenderer;

extern int EntityRenderer_field_28135_a;
extern int EntityRenderer_anaglyphField;

EntityRenderer *EntityRenderer_create(Minecraft *mc);
void EntityRenderer_destroy(EntityRenderer *self);

void EntityRenderer_updateRenderer(EntityRenderer *self);
void EntityRenderer_updateCameraAndRender(EntityRenderer *self, float partialTick);
void EntityRenderer_renderWorld(EntityRenderer *self, float partialTick, int64_t targetTimeNs);

void EntityRenderer_renderRainSnow(EntityRenderer *self, float partialTick);
void EntityRenderer_func_905_b(EntityRenderer *self);
void EntityRenderer_getMouseOver(EntityRenderer *self, float partialTick);

#endif
