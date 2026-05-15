#ifndef ENUM_CREATURE_TYPE_H
#define ENUM_CREATURE_TYPE_H

#include "Material.h"

typedef enum {
    ENTITY_CLASS_IMOB = 0,
    ENTITY_CLASS_ENTITY_ANIMAL = 1,
    ENTITY_CLASS_ENTITY_WATER_MOB = 2
} EntityClassId;

typedef enum EnumCreatureType {
    EnumCreatureType_monster = 0,
    EnumCreatureType_creature = 1,
    EnumCreatureType_waterCreature = 2
} EnumCreatureType;

EntityClassId EnumCreatureType_getCreatureClass(EnumCreatureType self);

int EnumCreatureType_getMaxNumberOfCreature(EnumCreatureType self);

Material *EnumCreatureType_getCreatureMaterial(EnumCreatureType self);

int EnumCreatureType_getPeacefulCreature(EnumCreatureType self);

#define ENUM_CREATURE_TYPE_COUNT 3

#endif
