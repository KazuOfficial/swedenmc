#ifndef WORLD_PROVIDER_H
#define WORLD_PROVIDER_H

#include "GameForward.h"
#include "WorldChunkManager.h"
#include "Vec3D.h"
#include <stdint.h>

typedef struct WorldProvider WorldProvider;

typedef struct WorldProviderVtable {

    void (*registerWorldChunkManager)(WorldProvider *self);

    void (*generateLightBrightnessTable)(WorldProvider *self);

    void *(*getChunkProvider)(WorldProvider *self);

    int (*canCoordinateBeSpawn)(WorldProvider *self, int x, int z);

    float (*calculateCelestialAngle)(WorldProvider *self, int64_t worldTime, float partialTick);

    float *(*calcSunriseSunsetColors)(WorldProvider *self, float angle, float partialTick);

    Vec3D *(*func_4096_a)(WorldProvider *self, float angle, float partialTick);

    int (*canRespawnHere)(WorldProvider *self);

    float (*getCloudHeight)(WorldProvider *self);

    int (*func_28112_c)(WorldProvider *self);

    void (*destroy)(WorldProvider *self);
} WorldProviderVtable;

struct WorldProvider {
    const WorldProviderVtable *vtable;

    World *worldObj;
    WorldChunkManager *worldChunkMgr;
    int isNether;
    int isHellWorld;
    int hasNoSky;
    float lightBrightnessTable[16];
    int worldType;
    float colorsSunriseSunset[4];
};

extern WorldProviderVtable WorldProvider_defaultVtable;

void WorldProvider_registerWorld(WorldProvider *self, World *world);

WorldProvider *WorldProvider_getProviderForDimension(int dim);

static inline void WorldProvider_registerWorldChunkManager(WorldProvider *p) {
    p->vtable->registerWorldChunkManager(p);
}
static inline void WorldProvider_generateLightBrightnessTable(WorldProvider *p) {
    p->vtable->generateLightBrightnessTable(p);
}
static inline void *WorldProvider_getChunkProvider(WorldProvider *p) {
    return p->vtable->getChunkProvider(p);
}
static inline int WorldProvider_canCoordinateBeSpawn(WorldProvider *p, int x, int z) {
    return p->vtable->canCoordinateBeSpawn(p, x, z);
}
static inline float WorldProvider_calculateCelestialAngle(WorldProvider *p, int64_t t, float f) {
    return p->vtable->calculateCelestialAngle(p, t, f);
}
static inline float *WorldProvider_calcSunriseSunsetColors(WorldProvider *p, float a, float f) {
    return p->vtable->calcSunriseSunsetColors(p, a, f);
}
static inline Vec3D *WorldProvider_func_4096_a(WorldProvider *p, float a, float f) {
    return p->vtable->func_4096_a(p, a, f);
}
static inline int WorldProvider_canRespawnHere(WorldProvider *p) {
    return p->vtable->canRespawnHere(p);
}
static inline float WorldProvider_getCloudHeight(WorldProvider *p) {
    return p->vtable->getCloudHeight(p);
}
static inline int WorldProvider_func_28112_c(WorldProvider *p) {
    return p->vtable->func_28112_c(p);
}
static inline void WorldProvider_destroy(WorldProvider *p) { p->vtable->destroy(p); }

#endif
