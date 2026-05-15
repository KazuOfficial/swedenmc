#include "Material.h"
#include "MaterialLiquid.h"
#include "MaterialTransparent.h"
#include "MaterialLogic.h"
#include "MaterialPortal.h"
#include <stdlib.h>

Material *Material_create(MapColor *color) {
    Material *m = (Material *)calloc(1, sizeof(Material));
    m->materialMapColor = color;
    m->canHarvest = 1;
    m->isSolid = 1;
    m->canBlockGrass = 1;
    return m;
}

Material *Material_setBurning(Material *self) {
    self->canBurn = 1;
    return self;
}
Material *Material_setIsGroundCover(Material *self) {
    self->groundCover = 1;
    return self;
}
Material *Material_setIsTranslucent(Material *self) {
    self->isTranslucent = 1;
    return self;
}
Material *Material_setNoHarvest(Material *self) {
    self->canHarvest = 0;
    return self;
}
Material *Material_setNoPushMobility(Material *self) {
    self->mobilityFlag = 1;
    return self;
}
Material *Material_setImmovableMobility(Material *self) {
    self->mobilityFlag = 2;
    return self;
}

int Material_getIsLiquid(Material *self) { return self->isLiquid; }
int Material_isSolid(Material *self) { return self->isSolid; }
int Material_getCanBlockGrass(Material *self) { return self->canBlockGrass; }
int Material_getIsSolid(Material *self) { return self->isSolid; }
int Material_getIsGroundCover(Material *self) { return self->groundCover; }
int Material_getBurning(Material *self) { return self->canBurn; }
int Material_getIsHarvestable(Material *self) { return self->canHarvest; }
int Material_getMaterialMobility(Material *self) { return self->mobilityFlag; }
int Material_getIsTranslucent(Material *self) { return self->isTranslucent ? 0 : self->isSolid; }

Material *Material_air;
Material *Material_grassMaterial;
Material *Material_ground;
Material *Material_wood;
Material *Material_rock;
Material *Material_iron;
Material *Material_water;
Material *Material_lava;
Material *Material_leaves;
Material *Material_plants;
Material *Material_sponge;
Material *Material_cloth;
Material *Material_fire;
Material *Material_sand;
Material *Material_circuits;
Material *Material_glass;
Material *Material_tnt;
Material *Material_field_4262_q;
Material *Material_ice;
Material *Material_snow;
Material *Material_builtSnow;
Material *Material_cactus;
Material *Material_clay;
Material *Material_pumpkin;
Material *Material_portal;
Material *Material_cakeMaterial;
Material *Material_field_31068_A;
Material *Material_field_31067_B;

void Material_init(void) {
    Material_air = MaterialTransparent_create(MapColor_airColor);
    Material_grassMaterial = Material_create(MapColor_grassColor);
    Material_ground = Material_create(MapColor_dirtColor);
    Material_wood = Material_setBurning(Material_create(MapColor_woodColor));
    Material_rock = Material_setNoHarvest(Material_create(MapColor_stoneColor));
    Material_iron = Material_setNoHarvest(Material_create(MapColor_ironColor));
    Material_water = Material_setNoPushMobility(MaterialLiquid_create(MapColor_waterColor));
    Material_lava = Material_setNoPushMobility(MaterialLiquid_create(MapColor_tntColor));
    Material_leaves = Material_setNoPushMobility(
        Material_setIsTranslucent(Material_setBurning(Material_create(MapColor_foliageColor))));
    Material_plants = Material_setNoPushMobility(MaterialLogic_create(MapColor_foliageColor));
    Material_sponge = Material_create(MapColor_clothColor);
    Material_cloth = Material_setBurning(Material_create(MapColor_clothColor));
    Material_fire = Material_setNoPushMobility(MaterialTransparent_create(MapColor_airColor));
    Material_sand = Material_create(MapColor_sandColor);
    Material_circuits = Material_setNoPushMobility(MaterialLogic_create(MapColor_airColor));
    Material_glass = Material_setIsTranslucent(Material_create(MapColor_airColor));
    Material_tnt =
        Material_setIsTranslucent(Material_setBurning(Material_create(MapColor_tntColor)));
    Material_field_4262_q = Material_setNoPushMobility(Material_create(MapColor_foliageColor));
    Material_ice = Material_setIsTranslucent(Material_create(MapColor_iceColor));
    Material_snow = Material_setNoPushMobility(Material_setNoHarvest(Material_setIsTranslucent(
        Material_setIsGroundCover(MaterialLogic_create(MapColor_snowColor)))));
    Material_builtSnow = Material_setNoHarvest(Material_create(MapColor_snowColor));
    Material_cactus = Material_setNoPushMobility(
        Material_setIsTranslucent(Material_create(MapColor_foliageColor)));
    Material_clay = Material_create(MapColor_clayColor);
    Material_pumpkin = Material_setNoPushMobility(Material_create(MapColor_foliageColor));
    Material_portal = Material_setImmovableMobility(MaterialPortal_create(MapColor_airColor));
    Material_cakeMaterial = Material_setNoPushMobility(Material_create(MapColor_airColor));
    Material_field_31068_A =
        Material_setNoPushMobility(Material_setNoHarvest(Material_create(MapColor_clothColor)));
    Material_field_31067_B = Material_setImmovableMobility(Material_create(MapColor_stoneColor));
}
