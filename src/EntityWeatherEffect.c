#include "EntityWeatherEffect.h"

static EntityVtable EntityWeatherEffect_vtable;
static int EntityWeatherEffect_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityWeatherEffect_vtable_init)
        return;
    Entity_initVtable(&EntityWeatherEffect_vtable);
    EntityWeatherEffect_vtable_init = 1;
}

void EntityWeatherEffect_initVtable(EntityVtable *vt) {
    ensure_vtable();
    *vt = EntityWeatherEffect_vtable;
}

void EntityWeatherEffect_construct(EntityWeatherEffect *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityWeatherEffect_vtable;
}
