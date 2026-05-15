#include "EnumCreatureType.h"
#include "Material.h"
#include <stddef.h>

EntityClassId EnumCreatureType_getCreatureClass(EnumCreatureType self) {
    static const EntityClassId t[] = {ENTITY_CLASS_IMOB, ENTITY_CLASS_ENTITY_ANIMAL,
                                      ENTITY_CLASS_ENTITY_WATER_MOB};
    return t[self];
}

int EnumCreatureType_getMaxNumberOfCreature(EnumCreatureType self) {
    static const int t[] = {70, 15, 5};
    return t[self];
}

Material *EnumCreatureType_getCreatureMaterial(EnumCreatureType self) {
    switch (self) {
    case EnumCreatureType_monster:
        return Material_air;
    case EnumCreatureType_creature:
        return Material_air;
    case EnumCreatureType_waterCreature:
        return Material_water;
    default:
        return NULL;
    }
}

int EnumCreatureType_getPeacefulCreature(EnumCreatureType self) {
    static const int t[] = {0, 1, 1};
    return t[self];
}
