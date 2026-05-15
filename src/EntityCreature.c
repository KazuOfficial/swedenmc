#include "EntityCreature.h"
#include "World.h"
#include "PathEntity.h"
#include "Vec3D.h"
#include "MathHelper.h"
#include <math.h>

static int def_isMovementCeased(EntityCreature *self) {
    (void)self;
    return 0;
}

static float def_getBlockPathWeight(EntityCreature *self, int x, int y, int z) {
    (void)self;
    (void)x;
    (void)y;
    (void)z;
    return 0.0f;
}

static Entity *def_findPlayerToAttack(EntityCreature *self) {
    (void)self;
    return NULL;
}

static void def_attackEntity(EntityCreature *self, Entity *var1, float var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

static void def_attackBlockedEntity(EntityCreature *self, Entity *var1, float var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

static void func_31026_E(EntityCreature *self) {
    EntityLiving *base = &self->base;
    Entity *e = &base->base;
    int var1 = 0;
    int var2 = -1, var3 = -1, var4 = -1;
    float var5 = -99999.0f;
    EntityCreatureVtable *vt = (EntityCreatureVtable *)e->vtable;

    int var6;
    for (var6 = 0; var6 < 10; ++var6) {
        int var7 =
            MathHelper_floor_double(e->posX + (double)JavaRandom_nextInt(&e->rand, 13) - 6.0);
        int var8 = MathHelper_floor_double(e->posY + (double)JavaRandom_nextInt(&e->rand, 7) - 3.0);
        int var9 =
            MathHelper_floor_double(e->posZ + (double)JavaRandom_nextInt(&e->rand, 13) - 6.0);
        float var10 = vt->getBlockPathWeight(self, var7, var8, var9);
        if (var10 > var5) {
            var5 = var10;
            var2 = var7;
            var3 = var8;
            var4 = var9;
            var1 = 1;
        }
    }

    if (var1) {
        PathEntity_destroy(self->pathToEntity);
        self->pathToEntity = World_getEntityPathToXYZ(e->worldObj, e, var2, var3, var4, 10.0f);
    }
}

void EntityCreature_updatePlayerActionState(EntityLiving *base) {
    EntityCreature *self = (EntityCreature *)base;
    Entity *e = &base->base;
    EntityCreatureVtable *vt = (EntityCreatureVtable *)e->vtable;

    self->hasAttacked = vt->isMovementCeased(self);

    float var1 = 16.0f;
    if (self->playerToAttack == NULL) {
        self->playerToAttack = vt->findPlayerToAttack(self);
        if (self->playerToAttack != NULL) {
            PathEntity_destroy(self->pathToEntity);
            self->pathToEntity = World_getPathToEntity(e->worldObj, e, self->playerToAttack, var1);
        }
    } else if (!self->playerToAttack->vtable->isEntityAlive(self->playerToAttack)) {

        self->playerToAttack = NULL;
    } else {
        float var2 = Entity_getDistanceToEntity(self->playerToAttack, e);
        if (EntityLiving_canEntityBeSeen(base, self->playerToAttack)) {
            vt->attackEntity(self, self->playerToAttack, var2);
        } else {
            vt->attackBlockedEntity(self, self->playerToAttack, var2);
        }
    }

    if (self->hasAttacked || self->playerToAttack == NULL ||
        (self->pathToEntity != NULL && JavaRandom_nextInt(&e->rand, 20) != 0)) {
        if (!self->hasAttacked &&
            ((self->pathToEntity == NULL && JavaRandom_nextInt(&e->rand, 80) == 0) ||
             JavaRandom_nextInt(&e->rand, 80) == 0)) {
            func_31026_E(self);
        }
    } else {
        PathEntity_destroy(self->pathToEntity);
        self->pathToEntity = World_getPathToEntity(e->worldObj, e, self->playerToAttack, var1);
    }

    int var21 = MathHelper_floor_double(e->boundingBox.minY + 0.5);
    int var3 = Entity_isInWater(e);
    int var4 = Entity_handleLavaMovement(e);

    e->rotationPitch = 0.0f;

    if (self->pathToEntity != NULL && JavaRandom_nextInt(&e->rand, 100) != 0) {
        Vec3D *var5 = PathEntity_getPosition(self->pathToEntity, e);
        double var6 = (double)(e->width * 2.0f);

        while (var5 != NULL &&
               Vec3D_squareDistanceTo3(var5, e->posX, var5->yCoord, e->posZ) < var6 * var6) {
            PathEntity_incrementPathIndex(self->pathToEntity);
            if (PathEntity_isFinished(self->pathToEntity)) {
                var5 = NULL;
                PathEntity_destroy(self->pathToEntity);
                self->pathToEntity = NULL;
            } else {
                var5 = PathEntity_getPosition(self->pathToEntity, e);
            }
        }

        base->isJumping = 0;
        if (var5 != NULL) {
            double var8 = var5->xCoord - e->posX;
            double var10 = var5->zCoord - e->posZ;
            double var12 = var5->yCoord - (double)var21;
            float var14 = (float)(atan2(var10, var8) * 180.0 / M_PI) - 90.0f;
            float var15 = var14 - e->rotationYaw;
            for (; var15 < -180.0f; var15 += 360.0f) {
            }
            while (var15 >= 180.0f) {
                var15 -= 360.0f;
            }
            if (var15 > 30.0f)
                var15 = 30.0f;
            if (var15 < -30.0f)
                var15 = -30.0f;
            e->rotationYaw += var15;

            base->moveForward = base->moveSpeed;

            if (self->hasAttacked && self->playerToAttack != NULL) {
                double var16 = self->playerToAttack->posX - e->posX;
                double var18 = self->playerToAttack->posZ - e->posZ;
                float var20 = e->rotationYaw;
                e->rotationYaw = (float)(atan2(var18, var16) * 180.0 / M_PI) - 90.0f;
                var15 = (var20 - e->rotationYaw + 90.0f) * (float)M_PI / 180.0f;
                base->moveStrafing = -MathHelper_sin(var15) * base->moveForward * 1.0f;
                base->moveForward = MathHelper_cos(var15) * base->moveForward * 1.0f;
            }

            if (var12 > 0.0) {
                base->isJumping = 1;
            }
        }

        if (self->playerToAttack != NULL) {
            EntityLiving_faceEntity(base, self->playerToAttack, 30.0f, 30.0f);
        }

        if (e->isCollidedHorizontally && !EntityCreature_hasPath(self)) {
            base->isJumping = 1;
        }

        if (JavaRandom_nextFloat(&e->rand) < 0.8f && (var3 || var4)) {
            base->isJumping = 1;
        }
    } else {

        extern EntityLivingVtable EntityLiving_defaultVtable;
        EntityLiving_defaultVtable.updatePlayerActionState(base);
        PathEntity_destroy(self->pathToEntity);
        self->pathToEntity = NULL;
    }
}

int EntityCreature_getCanSpawnHere(EntityCreature *self) {
    EntityLiving *base = &self->base;
    Entity *e = &base->base;
    int var1 = MathHelper_floor_double(e->posX);
    int var2 = MathHelper_floor_double(e->boundingBox.minY);
    int var3 = MathHelper_floor_double(e->posZ);
    EntityCreatureVtable *vt = (EntityCreatureVtable *)e->vtable;

    extern EntityLivingVtable EntityLiving_defaultVtable;
    if (!EntityLiving_defaultVtable.getCanSpawnHere(base))
        return 0;
    return vt->getBlockPathWeight(self, var1, var2, var3) >= 0.0f;
}

static int def_getCanSpawnHere(EntityLiving *base) {
    return EntityCreature_getCanSpawnHere((EntityCreature *)base);
}

static void def_updatePlayerActionState(EntityLiving *base) {
    EntityCreature_updatePlayerActionState(base);
}

void EntityCreature_initVtable(EntityCreatureVtable *vt) {
    EntityLiving_initVtable(&vt->base);
    vt->base.updatePlayerActionState = def_updatePlayerActionState;
    vt->base.getCanSpawnHere = def_getCanSpawnHere;
    vt->getBlockPathWeight = def_getBlockPathWeight;
    vt->findPlayerToAttack = def_findPlayerToAttack;
    vt->attackEntity = def_attackEntity;
    vt->attackBlockedEntity = def_attackBlockedEntity;
    vt->isMovementCeased = def_isMovementCeased;
}

void EntityCreature_construct(EntityCreature *self, World *world) {
    EntityLiving_construct(&self->base, world);
    self->pathToEntity = NULL;
    self->playerToAttack = NULL;
    self->hasAttacked = 0;
}

int EntityCreature_hasPath(EntityCreature *self) { return self->pathToEntity != NULL; }

void EntityCreature_setPathToEntity(EntityCreature *self, struct PathEntity *var1) {
    PathEntity_destroy(self->pathToEntity);
    self->pathToEntity = var1;
}

Entity *EntityCreature_getTarget(EntityCreature *self) { return self->playerToAttack; }

void EntityCreature_setTarget(EntityCreature *self, Entity *var1) { self->playerToAttack = var1; }
