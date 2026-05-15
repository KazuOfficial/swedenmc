#include "TileEntityMobSpawner.h"
#include "TileEntity.h"
#include "World.h"
#include "EntityLiving.h"
#include "EntityList.h"
#include "AxisAlignedBB.h"
#include "NBTTagCompound.h"
#include <stdlib.h>
#include <string.h>

static const void *spawner_match_vtable = NULL;
static int spawner_match_class(struct Entity *e) { return e->vtable == spawner_match_vtable; }

static void updateDelay(TileEntityMobSpawner *self) {
    self->delay = 200 + JavaRandom_nextInt(&self->base.worldObj->rand, 600);
}

static int anyPlayerInRange(TileEntityMobSpawner *self) {
    return World_getClosestPlayer(self->base.worldObj, (double)self->base.xCoord + 0.5,
                                  (double)self->base.yCoord + 0.5, (double)self->base.zCoord + 0.5,
                                  16.0) != NULL;
}

static void spawner_updateEntity(TileEntity *base) {
    TileEntityMobSpawner *self = (TileEntityMobSpawner *)base;
    self->yaw2 = self->yaw;
    if (!anyPlayerInRange(self))
        return;

    World *worldObj = base->worldObj;
    JavaRandom *rand = &worldObj->rand;

    double var1 = (double)((float)base->xCoord + JavaRandom_nextFloat(rand));
    double var3 = (double)((float)base->yCoord + JavaRandom_nextFloat(rand));
    double var5 = (double)((float)base->zCoord + JavaRandom_nextFloat(rand));
    World_spawnParticle(worldObj, "smoke", var1, var3, var5, 0.0, 0.0, 0.0);
    World_spawnParticle(worldObj, "flame", var1, var3, var5, 0.0, 0.0, 0.0);

    for (self->yaw += (double)(1000.0f / ((float)self->delay + 200.0f)); self->yaw > 360.0;
         self->yaw2 -= 360.0) {
        self->yaw -= 360.0;
    }

    if (!worldObj->multiplayerWorld) {
        if (self->delay == -1) {
            updateDelay(self);
        }
        if (self->delay > 0) {
            --self->delay;
            return;
        }

        int var7 = 4;
        for (int var8 = 0; var8 < var7; var8++) {
            Entity *rawvar9 = EntityList_createEntityInWorld(self->mobID, worldObj);
            if (rawvar9 == NULL)
                return;
            EntityLiving *var9 = (EntityLiving *)rawvar9;

            AxisAlignedBB *bb = AxisAlignedBB_getBoundingBoxFromPool(
                (double)base->xCoord, (double)base->yCoord, (double)base->zCoord,
                (double)(base->xCoord + 1), (double)(base->yCoord + 1), (double)(base->zCoord + 1));
            AxisAlignedBB_expand(bb, 8.0, 4.0, 8.0);
            spawner_match_vtable = rawvar9->vtable;
            Entity *matchList[64];
            int var10 =
                World_getEntitiesWithinAABB(worldObj, spawner_match_class, bb, matchList, 64);
            if (var10 >= 6) {
                updateDelay(self);
                return;
            }

            double var11 = (double)base->xCoord +
                           (JavaRandom_nextDouble(rand) - JavaRandom_nextDouble(rand)) * 4.0;
            double var13 = (double)(base->yCoord + JavaRandom_nextInt(rand, 3) - 1);
            double var15 = (double)base->zCoord +
                           (JavaRandom_nextDouble(rand) - JavaRandom_nextDouble(rand)) * 4.0;
            rawvar9->vtable->setLocationAndAngles(rawvar9, var11, var13, var15,
                                                  JavaRandom_nextFloat(rand) * 360.0f, 0.0f);
            if (((EntityLivingVtable *)rawvar9->vtable)->getCanSpawnHere(var9)) {
                World_entityJoinedWorld(worldObj, rawvar9);

                for (int var17 = 0; var17 < 20; var17++) {
                    var1 = (double)base->xCoord + 0.5 +
                           ((double)JavaRandom_nextFloat(rand) - 0.5) * 2.0;
                    var3 = (double)base->yCoord + 0.5 +
                           ((double)JavaRandom_nextFloat(rand) - 0.5) * 2.0;
                    var5 = (double)base->zCoord + 0.5 +
                           ((double)JavaRandom_nextFloat(rand) - 0.5) * 2.0;
                    World_spawnParticle(worldObj, "smoke", var1, var3, var5, 0.0, 0.0, 0.0);
                    World_spawnParticle(worldObj, "flame", var1, var3, var5, 0.0, 0.0, 0.0);
                }

                EntityLiving_spawnExplosionParticle(var9);
                updateDelay(self);
            }
        }
    }

    (void)base;
}

static int spawner_needsUpdate(TileEntity *self) {
    (void)self;
    return 1;
}

static void spawner_readFromNBT(TileEntity *base, NBTTagCompound *var1) {
    TileEntityMobSpawner *self = (TileEntityMobSpawner *)base;
    TileEntity_defaultVtable.readFromNBT(base, var1);
    const char *id = NBTTagCompound_getString(var1, "EntityId");
    if (id)
        strncpy(self->mobID, id, sizeof(self->mobID) - 1);
    self->delay = (int)NBTTagCompound_getShort(var1, "Delay");
}

static void spawner_writeToNBT(TileEntity *base, NBTTagCompound *var1) {
    TileEntityMobSpawner *self = (TileEntityMobSpawner *)base;
    TileEntity_defaultVtable.writeToNBT(base, var1);
    NBTTagCompound_setString(var1, "EntityId", self->mobID);
    NBTTagCompound_setShort(var1, "Delay", (short)self->delay);
}

static void spawner_destroy(TileEntity *base) { free(base); }

TileEntityMobSpawner *TileEntityMobSpawner_create(void) {
    static TileEntityVtable vt;
    static int once = 0;
    if (!once) {
        TileEntity_initVtable(&vt);
        vt.typeName = "MobSpawner";
        vt.updateEntity = spawner_updateEntity;
        vt.needsUpdate = spawner_needsUpdate;
        vt.writeToNBT = spawner_writeToNBT;
        vt.readFromNBT = spawner_readFromNBT;
        vt.destroy = spawner_destroy;
        once = 1;
    }
    TileEntityMobSpawner *self = (TileEntityMobSpawner *)calloc(1, sizeof(TileEntityMobSpawner));
    TileEntity_init(&self->base);
    self->base.vtable = &vt;
    self->base.tileEntityTypeId = TE_TYPE_MOB_SPAWNER;
    self->delay = 20;
    strncpy(self->mobID, "Pig", sizeof(self->mobID) - 1);

    return self;
}

const char *TileEntityMobSpawner_getMobID(TileEntityMobSpawner *self) { return self->mobID; }

void TileEntityMobSpawner_setMobID(TileEntityMobSpawner *self, const char *var1) {
    strncpy(self->mobID, var1, sizeof(self->mobID) - 1);
    self->mobID[sizeof(self->mobID) - 1] = '\0';
}
