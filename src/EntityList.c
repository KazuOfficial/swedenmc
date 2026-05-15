#include "EntityList.h"
#include "Entity.h"
#include "EntityItem.h"
#include "EntityArrow.h"
#include "EntitySnowball.h"
#include "EntityPainting.h"
#include "EntityCreeper.h"
#include "EntitySkeleton.h"
#include "EntitySpider.h"
#include "EntityGiantZombie.h"
#include "EntityZombie.h"
#include "EntitySlime.h"
#include "EntityGhast.h"
#include "EntityPigZombie.h"
#include "EntityPig.h"
#include "EntitySheep.h"
#include "EntityCow.h"
#include "EntityChicken.h"
#include "EntitySquid.h"
#include "EntityWolf.h"
#include "EntityTNTPrimed.h"
#include "EntityFallingSand.h"
#include "EntityMinecart.h"
#include "EntityBoat.h"
#include "NBTTagCompound.h"
#include "ItemStack.h"
#include "World.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef Entity *(*EntityFactory)(World *world);

typedef struct {
    const char *name;
    int id;
    EntityFactory factory;
} EntityMapping;

static Entity *factory_Item(World *w) {

    ItemStack empty;
    memset(&empty, 0, sizeof(empty));
    return (Entity *)EntityItem_create(w, 0.0, 0.0, 0.0, empty);
}
static Entity *factory_Arrow(World *w) { return (Entity *)EntityArrow_create1(w); }
static Entity *factory_Snowball(World *w) {
    EntitySnowball *e = (EntitySnowball *)calloc(1, sizeof(EntitySnowball));
    EntitySnowball_construct1(e, w);
    return (Entity *)e;
}
static Entity *factory_Painting(World *w) { return (Entity *)EntityPainting_create1(w); }
static Entity *factory_Creeper(World *w) { return (Entity *)EntityCreeper_create(w); }
static Entity *factory_Skeleton(World *w) { return (Entity *)EntitySkeleton_create(w); }
static Entity *factory_Spider(World *w) { return (Entity *)EntitySpider_create(w); }
static Entity *factory_GiantZombie(World *w) { return (Entity *)EntityGiantZombie_create(w); }
static Entity *factory_Zombie(World *w) { return (Entity *)EntityZombie_create(w); }
static Entity *factory_Slime(World *w) { return (Entity *)EntitySlime_create(w); }
static Entity *factory_Ghast(World *w) { return (Entity *)EntityGhast_create(w); }
static Entity *factory_PigZombie(World *w) { return (Entity *)EntityPigZombie_create(w); }
static Entity *factory_Pig(World *w) { return (Entity *)EntityPig_create(w); }
static Entity *factory_Sheep(World *w) { return (Entity *)EntitySheep_create(w); }
static Entity *factory_Cow(World *w) { return (Entity *)EntityCow_create(w); }
static Entity *factory_Chicken(World *w) { return (Entity *)EntityChicken_create(w); }
static Entity *factory_Squid(World *w) { return (Entity *)EntitySquid_create(w); }
static Entity *factory_Wolf(World *w) { return (Entity *)EntityWolf_create(w); }
static Entity *factory_TNTPrimed(World *w) { return (Entity *)EntityTNTPrimed_create(w); }
static Entity *factory_FallingSand(World *w) {
    EntityFallingSand *e = (EntityFallingSand *)calloc(1, sizeof(EntityFallingSand));
    EntityFallingSand_construct1(e, w);
    return (Entity *)e;
}
static Entity *factory_Minecart(World *w) {
    EntityMinecart *e = (EntityMinecart *)calloc(1, sizeof(EntityMinecart));
    EntityMinecart_construct1(e, w);
    return (Entity *)e;
}
static Entity *factory_Boat(World *w) {
    EntityBoat *e = (EntityBoat *)calloc(1, sizeof(EntityBoat));
    EntityBoat_construct1(e, w);
    return (Entity *)e;
}

static const EntityMapping entity_mappings[] = {

    {"Arrow", 10, factory_Arrow},
    {"Snowball", 11, factory_Snowball},
    {"Item", 1, factory_Item},
    {"Painting", 9, factory_Painting},
    {"Mob", 48, NULL},
    {"Monster", 49, NULL},
    {"Creeper", 50, factory_Creeper},
    {"Skeleton", 51, factory_Skeleton},
    {"Spider", 52, factory_Spider},
    {"Giant", 53, factory_GiantZombie},
    {"Zombie", 54, factory_Zombie},
    {"Slime", 55, factory_Slime},
    {"Ghast", 56, factory_Ghast},
    {"PigZombie", 57, factory_PigZombie},
    {"Pig", 90, factory_Pig},
    {"Sheep", 91, factory_Sheep},
    {"Cow", 92, factory_Cow},
    {"Chicken", 93, factory_Chicken},
    {"Squid", 94, factory_Squid},
    {"Wolf", 95, factory_Wolf},
    {"PrimedTnt", 20, factory_TNTPrimed},
    {"FallingSand", 21, factory_FallingSand},
    {"Minecart", 40, factory_Minecart},
    {"Boat", 41, factory_Boat},
    {NULL, 0, NULL}};

#define NUM_MAPPINGS ((int)(sizeof(entity_mappings) / sizeof(entity_mappings[0]) - 1))

static const EntityMapping *find_by_name(const char *name) {
    for (int i = 0; i < NUM_MAPPINGS; ++i) {
        if (strcmp(entity_mappings[i].name, name) == 0)
            return &entity_mappings[i];
    }
    return NULL;
}

static const EntityMapping *find_by_id(int id) {
    for (int i = 0; i < NUM_MAPPINGS; ++i) {
        if (entity_mappings[i].id == id)
            return &entity_mappings[i];
    }
    return NULL;
}

Entity *EntityList_createEntityInWorld(const char *name, World *world) {
    const EntityMapping *m = find_by_name(name);
    if (m == NULL || m->factory == NULL)
        return NULL;
    return m->factory(world);
}

Entity *EntityList_createEntityFromNBT(NBTTagCompound *tag, World *world) {
    const char *id = NBTTagCompound_getString(tag, "id");
    Entity *entity = NULL;
    const EntityMapping *m = (id != NULL) ? find_by_name(id) : NULL;
    if (m != NULL && m->factory != NULL) {
        entity = m->factory(world);
    }
    if (entity != NULL) {
        Entity_readFromNBT(entity, tag);
    } else {
        fprintf(stderr, "Skipping Entity with id %s\n", id ? id : "(null)");
    }
    return entity;
}

Entity *EntityList_createEntity(int id, World *world) {
    const EntityMapping *m = find_by_id(id);
    Entity *entity = NULL;
    if (m != NULL && m->factory != NULL) {
        entity = m->factory(world);
    }
    if (entity == NULL) {
        fprintf(stderr, "Skipping Entity with id %d\n", id);
    }
    return entity;
}

int EntityList_getEntityID(Entity *entity) {
    const char *name = entity->vtable->getEntityString(entity);
    if (name == NULL)
        return -1;
    const EntityMapping *m = find_by_name(name);
    return m ? m->id : -1;
}

const char *EntityList_getEntityString(Entity *entity) {
    const char *name = entity->vtable->getEntityString(entity);
    if (name == NULL)
        return NULL;

    return find_by_name(name) ? name : NULL;
}
