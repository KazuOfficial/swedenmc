#include "MapGenCaves.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <math.h>

#define BLOCK_STONE 1
#define BLOCK_GRASS 2
#define BLOCK_DIRT 3
#define BLOCK_WATER_MOVING 8
#define BLOCK_WATER_STILL 9
#define BLOCK_LAVA_MOVING 10

static void cave_carve(MapGenCaves *self, int tgtCX, int tgtCZ, uint8_t *blocks, double cx,
                       double cy, double cz, float radius, float yaw, float pitch, int startStep,
                       int maxSteps, double widthScale);

static void release_entity_skin(MapGenCaves *self, int tgtCX, int tgtCZ, uint8_t *blocks, double cx,
                                double cy, double cz, float radius, float yaw, float pitch,
                                int startStep, int maxSteps, double widthScale) {

    double chunkCX = (double)(tgtCX * 16 + 8);
    double chunkCZ = (double)(tgtCZ * 16 + 8);
    float dYaw = 0.0f, dPitch = 0.0f;
    JavaRandom stepRand;
    JavaRandom_setSeed(&stepRand, JavaRandom_nextLong(&self->base.rand));

    if (maxSteps <= 0) {
        int r = self->base.range * 16 - 16;
        maxSteps = r - JavaRandom_nextInt(&stepRand, r / 4);
    }

    int isSingle = (startStep == -1);
    if (isSingle)
        startStep = maxSteps / 2;

    int branch = JavaRandom_nextInt(&stepRand, maxSteps / 2) + maxSteps / 4;
    int widen = (JavaRandom_nextInt(&stepRand, 6) == 0);

    for (; startStep < maxSteps; startStep++) {
        double r1 =
            1.5 + (double)(sinf((float)startStep * 3.14159265f / (float)maxSteps) * radius * 1.0f);
        double r2 = r1 * widthScale;
        float cp = cosf(pitch), sp = sinf(pitch);
        cx += (double)(cosf(yaw) * cp);
        cy += (double)sp;
        cz += (double)(sinf(yaw) * cp);
        pitch *= widen ? 0.92f : 0.7f;
        pitch += dPitch * 0.1f;
        yaw += dYaw * 0.1f;
        dPitch *= 0.9f;
        dYaw *= 12.0f / 16.0f;
        dPitch += (JavaRandom_nextFloat(&stepRand) - JavaRandom_nextFloat(&stepRand)) *
                  JavaRandom_nextFloat(&stepRand) * 2.0f;
        dYaw += (JavaRandom_nextFloat(&stepRand) - JavaRandom_nextFloat(&stepRand)) *
                JavaRandom_nextFloat(&stepRand) * 4.0f;

        if (!isSingle && startStep == branch && radius > 1.0f) {
            release_entity_skin(self, tgtCX, tgtCZ, blocks, cx, cy, cz,
                                JavaRandom_nextFloat(&stepRand) * 0.5f + 0.5f, yaw - 1.5707963f,
                                pitch / 3.0f, startStep, maxSteps, 1.0);
            release_entity_skin(self, tgtCX, tgtCZ, blocks, cx, cy, cz,
                                JavaRandom_nextFloat(&stepRand) * 0.5f + 0.5f, yaw + 1.5707963f,
                                pitch / 3.0f, startStep, maxSteps, 1.0);
            return;
        }

        if (isSingle || JavaRandom_nextInt(&stepRand, 4) != 0) {
            double dx = cx - chunkCX, dz = cz - chunkCZ;
            double stepsLeft = (double)(maxSteps - startStep);
            double maxReach = (double)(radius + 2.0f + 16.0f);
            if (dx * dx + dz * dz - stepsLeft * stepsLeft > maxReach * maxReach)
                return;

            if (cx >= chunkCX - 16 - r1 * 2 && cz >= chunkCZ - 16 - r1 * 2 &&
                cx <= chunkCX + 16 + r1 * 2 && cz <= chunkCZ + 16 + r1 * 2) {

                int x0 = MathHelper_floor_double(cx - r1) - tgtCX * 16 - 1;
                if (x0 < 0)
                    x0 = 0;
                int x1 = MathHelper_floor_double(cx + r1) - tgtCX * 16 + 1;
                if (x1 > 16)
                    x1 = 16;
                int y0 = MathHelper_floor_double(cy - r2) - 1;
                if (y0 < 1)
                    y0 = 1;
                int y1 = MathHelper_floor_double(cy + r2) + 1;
                if (y1 > 120)
                    y1 = 120;
                int z0 = MathHelper_floor_double(cz - r1) - tgtCZ * 16 - 1;
                if (z0 < 0)
                    z0 = 0;
                int z1 = MathHelper_floor_double(cz + r1) - tgtCZ * 16 + 1;
                if (z1 > 16)
                    z1 = 16;

                int hasWater = 0;
                for (int bx = x0; !hasWater && bx < x1; bx++)
                    for (int bz = z0; !hasWater && bz < z1; bz++)
                        for (int by = y1 + 1; !hasWater && by >= y0 - 1; by--) {
                            int idx = (bx * 16 + bz) * 128 + by;
                            if (by >= 0 && by < 128) {
                                if (blocks[idx] == BLOCK_WATER_MOVING ||
                                    blocks[idx] == BLOCK_WATER_STILL)
                                    hasWater = 1;
                                if (by != y0 - 1 && bx != x0 && bx != x1 - 1 && bz != z0 &&
                                    bz != z1 - 1)
                                    by = y0;
                            }
                        }
                if (hasWater)
                    continue;

                for (int bx = x0; bx < x1; bx++) {
                    double nx = ((double)(bx + tgtCX * 16) + 0.5 - cx) / r1;
                    for (int bz = z0; bz < z1; bz++) {
                        double nz = ((double)(bz + tgtCZ * 16) + 0.5 - cz) / r1;
                        int idx = (bx * 16 + bz) * 128 + y1;
                        int below_grass = 0;
                        if (nx * nx + nz * nz < 1.0) {
                            for (int by = y1 - 1; by >= y0; by--) {
                                double ny = ((double)by + 0.5 - cy) / r2;
                                if (ny > -0.7 && nx * nx + ny * ny + nz * nz < 1.0) {
                                    uint8_t b = blocks[idx];
                                    if (b == BLOCK_GRASS)
                                        below_grass = 1;
                                    if (b == BLOCK_STONE || b == BLOCK_DIRT || b == BLOCK_GRASS) {
                                        blocks[idx] = (by < 10) ? (uint8_t)BLOCK_LAVA_MOVING : 0;
                                        if (!blocks[idx] && below_grass &&
                                            blocks[idx - 1] == BLOCK_DIRT)
                                            blocks[idx - 1] = BLOCK_GRASS;
                                    }
                                }
                                idx--;
                            }
                        }
                    }
                }
                if (isSingle)
                    return;
            }
        }
    }
    (void)cave_carve;
}

static void cave_carve(MapGenCaves *self, int tgtCX, int tgtCZ, uint8_t *blocks, double cx,
                       double cy, double cz, float radius, float yaw, float pitch, int startStep,
                       int maxSteps, double widthScale) {

    release_entity_skin(self, tgtCX, tgtCZ, blocks, cx, cy, cz, radius, yaw, pitch, startStep,
                        maxSteps, widthScale);
}

static void caves_func_868_a(MapGenBase *base, World *world, int srcCX, int srcCZ, int tgtCX,
                             int tgtCZ, uint8_t *blocks) {
    (void)world;
    MapGenCaves *self = (MapGenCaves *)base;

    int count = JavaRandom_nextInt(
        &base->rand, JavaRandom_nextInt(&base->rand, JavaRandom_nextInt(&base->rand, 40) + 1) + 1);
    if (JavaRandom_nextInt(&base->rand, 15) != 0)
        count = 0;

    for (int i = 0; i < count; i++) {
        double ox = (double)(srcCX * 16 + JavaRandom_nextInt(&base->rand, 16));
        double oy =
            (double)(JavaRandom_nextInt(&base->rand, JavaRandom_nextInt(&base->rand, 120) + 8));
        double oz = (double)(srcCZ * 16 + JavaRandom_nextInt(&base->rand, 16));
        int branches = 1;

        if (JavaRandom_nextInt(&base->rand, 4) == 0) {
            float r = 1.0f + JavaRandom_nextFloat(&base->rand) * 6.0f;
            cave_carve(self, tgtCX, tgtCZ, blocks, ox, oy, oz, r, 0.0f, 0.0f, -1, -1, 0.5);
            branches += JavaRandom_nextInt(&base->rand, 4);
        }

        for (int b = 0; b < branches; b++) {
            float yaw = JavaRandom_nextFloat(&base->rand) * 3.14159265f * 2.0f;
            float pitch = (JavaRandom_nextFloat(&base->rand) - 0.5f) * 2.0f / 8.0f;
            float rad =
                JavaRandom_nextFloat(&base->rand) * 2.0f + JavaRandom_nextFloat(&base->rand);
            release_entity_skin(self, tgtCX, tgtCZ, blocks, ox, oy, oz, rad, yaw, pitch, 0, 0, 1.0);
        }
    }
}

static MapGenBaseVtable caves_vtable = {caves_func_868_a};

MapGenCaves *MapGenCaves_create(void) {
    MapGenCaves *self = (MapGenCaves *)calloc(1, sizeof(MapGenCaves));
    self->base.vtable = &caves_vtable;
    self->base.range = 8;
    return self;
}

void MapGenCaves_free(MapGenCaves *self) { free(self); }
