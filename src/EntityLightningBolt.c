#include "EntityLightningBolt.h"
#include "World.h"
#include "Block.h"
#include "MathHelper.h"
#include "AxisAlignedBB.h"
#include <stdlib.h>

static EntityVtable EntityLightningBolt_vtable;
static int EntityLightningBolt_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityLightningBolt_vtable_init)
        return;
    EntityWeatherEffect_initVtable(&EntityLightningBolt_vtable);
    EntityLightningBolt_vtable.onUpdate = EntityLightningBolt_onUpdate;
    EntityLightningBolt_vtable.entityInit = EntityLightningBolt_entityInit;
    EntityLightningBolt_vtable.readEntityFromNBT = EntityLightningBolt_readEntityFromNBT;
    EntityLightningBolt_vtable.writeEntityToNBT = EntityLightningBolt_writeEntityToNBT;
    EntityLightningBolt_vtable.isInRangeToRenderVec3D = EntityLightningBolt_isInRangeToRenderVec3D;
    EntityLightningBolt_vtable_init = 1;
}

void EntityLightningBolt_construct(EntityLightningBolt *self, World *var1, double var2, double var4,
                                   double var6) {
    ensure_vtable();
    EntityWeatherEffect_construct(&self->base, var1);
    self->base.base.vtable = &EntityLightningBolt_vtable;
    self->base.base.isLightningBolt = 1;
    self->base.base.vtable->setLocationAndAngles(&self->base.base, var2, var4, var6, 0.0F, 0.0F);
    self->field_27028_b = 2;
    self->field_27029_a = JavaRandom_nextLong(&self->base.base.rand);
    self->field_27030_c = JavaRandom_nextInt(&self->base.base.rand, 3) + 1;
    if (var1->difficultySetting >= 2 &&
        World_doChunksNearChunkExist(var1, MathHelper_floor_double(var2),
                                     MathHelper_floor_double(var4), MathHelper_floor_double(var6),
                                     10)) {
        int var8 = MathHelper_floor_double(var2);
        int var9 = MathHelper_floor_double(var4);
        int var10 = MathHelper_floor_double(var6);
        if (World_getBlockId(var1, var8, var9, var10) == 0 &&
            Block_fire->vtable->canPlaceBlockAt(Block_fire, var1, var8, var9, var10)) {
            World_setBlockWithNotify(var1, var8, var9, var10, Block_fire->blockID);
        }
        for (var8 = 0; var8 < 4; ++var8) {
            var9 = MathHelper_floor_double(var2) + JavaRandom_nextInt(&self->base.base.rand, 3) - 1;
            var10 =
                MathHelper_floor_double(var4) + JavaRandom_nextInt(&self->base.base.rand, 3) - 1;
            int var11 =
                MathHelper_floor_double(var6) + JavaRandom_nextInt(&self->base.base.rand, 3) - 1;
            if (World_getBlockId(var1, var9, var10, var11) == 0 &&
                Block_fire->vtable->canPlaceBlockAt(Block_fire, var1, var9, var10, var11)) {
                World_setBlockWithNotify(var1, var9, var10, var11, Block_fire->blockID);
            }
        }
    }
}

EntityLightningBolt *EntityLightningBolt_create(World *var1, double var2, double var4,
                                                double var6) {
    EntityLightningBolt *self = (EntityLightningBolt *)calloc(1, sizeof(EntityLightningBolt));
    EntityLightningBolt_construct(self, var1, var2, var4, var6);
    return self;
}

void EntityLightningBolt_entityInit(Entity *self) { (void)self; }

void EntityLightningBolt_onUpdate(Entity *selfE) {
    EntityLightningBolt *self = (EntityLightningBolt *)selfE;
    Entity_defaultVtable.onUpdate(selfE);
    if (self->field_27028_b == 2) {
        World_playSoundEffect(selfE->worldObj, selfE->posX, selfE->posY, selfE->posZ,
                              "ambient.weather.thunder", 10000.0F,
                              0.8F + JavaRandom_nextFloat(&selfE->rand) * 0.2F);
        World_playSoundEffect(selfE->worldObj, selfE->posX, selfE->posY, selfE->posZ,
                              "random.explode", 2.0F,
                              0.5F + JavaRandom_nextFloat(&selfE->rand) * 0.2F);
    }
    --self->field_27028_b;
    if (self->field_27028_b < 0) {
        if (self->field_27030_c == 0) {
            Entity_setEntityDead(selfE);
        } else if (self->field_27028_b < -JavaRandom_nextInt(&selfE->rand, 10)) {
            --self->field_27030_c;
            self->field_27028_b = 1;
            self->field_27029_a = JavaRandom_nextLong(&selfE->rand);
            if (World_doChunksNearChunkExist(selfE->worldObj, MathHelper_floor_double(selfE->posX),
                                             MathHelper_floor_double(selfE->posY),
                                             MathHelper_floor_double(selfE->posZ), 10)) {
                int var1 = MathHelper_floor_double(selfE->posX);
                int var2 = MathHelper_floor_double(selfE->posY);
                int var3 = MathHelper_floor_double(selfE->posZ);
                if (World_getBlockId(selfE->worldObj, var1, var2, var3) == 0 &&
                    Block_fire->vtable->canPlaceBlockAt(Block_fire, selfE->worldObj, var1, var2,
                                                        var3)) {
                    World_setBlockWithNotify(selfE->worldObj, var1, var2, var3,
                                             Block_fire->blockID);
                }
            }
        }
    }
    if (self->field_27028_b >= 0) {
        double var6 = 3.0;
        AxisAlignedBB *var7bb = AxisAlignedBB_getBoundingBoxFromPool(
            selfE->posX - var6, selfE->posY - var6, selfE->posZ - var6, selfE->posX + var6,
            selfE->posY + 6.0 + var6, selfE->posZ + var6);
        Entity *var7[64];
        int var7count =
            World_getEntitiesWithinAABBExcludingEntity(selfE->worldObj, selfE, var7bb, var7, 64);
        for (int var4 = 0; var4 < var7count; ++var4) {
            Entity *var5 = var7[var4];
            var5->vtable->onStruckByLightning(var5, self);
        }
        selfE->worldObj->field_27172_i = 2;
    }
}

void EntityLightningBolt_readEntityFromNBT(Entity *self, NBTTagCompound *var1) {
    (void)self;
    (void)var1;
}

void EntityLightningBolt_writeEntityToNBT(Entity *self, NBTTagCompound *var1) {
    (void)self;
    (void)var1;
}

int EntityLightningBolt_isInRangeToRenderVec3D(Entity *selfE, Vec3D *var1) {
    (void)var1;
    EntityLightningBolt *self = (EntityLightningBolt *)selfE;
    return self->field_27028_b >= 0;
}
