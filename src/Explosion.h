#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "GameForward.h"
#include "ChunkPosition.h"
#include "JavaRandom.h"

struct World;
struct Entity;

typedef struct Explosion {

    int isFlaming;

    JavaRandom ExplosionRNG;

    struct World *worldObj;

    double explosionX;
    double explosionY;
    double explosionZ;

    struct Entity *exploder;

    float explosionSize;

    ChunkPosition *destroyedBlockPositions;
    int numDestroyed;
    int destroyedCap;
} Explosion;

Explosion *Explosion_create(struct World *world, struct Entity *exploder, double x, double y,
                            double z, float size);
void Explosion_destroy(Explosion *self);

void Explosion_doExplosionA(Explosion *self);

void Explosion_doExplosionB(Explosion *self, int spawnParticles);

#endif
