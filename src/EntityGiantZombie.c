#include "EntityGiantZombie.h"
#include "World.h"
#include <stdlib.h>

static EntityMobVtable EntityGiantZombie_vtable;
static int EntityGiantZombie_vtable_init = 0;

static const char *giantzombie_getEntityString(Entity *self) {
    (void)self;
    return "Giant";
}

static void ensure_vtable(void) {
    if (EntityGiantZombie_vtable_init)
        return;
    EntityMob_initVtable(&EntityGiantZombie_vtable);
    EntityGiantZombie_vtable.base.base.base.getEntityString = giantzombie_getEntityString;
    EntityGiantZombie_vtable.base.getBlockPathWeight = EntityGiantZombie_getBlockPathWeight;
    EntityGiantZombie_vtable_init = 1;
}

void EntityGiantZombie_construct(EntityGiantZombie *self, World *var1) {
    ensure_vtable();
    EntityMob_construct(&self->base, var1);
    self->base.base.base.base.vtable = (const EntityVtable *)&EntityGiantZombie_vtable;
    self->base.base.base.texture = "/mob/zombie.png";
    self->base.base.base.moveSpeed = 0.5F;
    self->base.attackStrength = 50;
    self->base.base.base.health *= 10;
    self->base.base.base.base.yOffset *= 6.0F;
    Entity_setSize(&self->base.base.base.base, self->base.base.base.base.width * 6.0F,
                   self->base.base.base.base.height * 6.0F);
    self->base.base.base.base.isGiantZombie = 1;
}

EntityGiantZombie *EntityGiantZombie_create(World *var1) {
    EntityGiantZombie *self = (EntityGiantZombie *)calloc(1, sizeof(EntityGiantZombie));
    EntityGiantZombie_construct(self, var1);
    return self;
}

float EntityGiantZombie_getBlockPathWeight(EntityCreature *selfC, int var1, int var2, int var3) {
    EntityGiantZombie *self = (EntityGiantZombie *)selfC;
    return self->base.base.base.base.worldObj->vtable->getLightBrightness(
               self->base.base.base.base.worldObj, var1, var2, var3) -
           0.5F;
}
