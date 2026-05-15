#ifndef MATERIAL_H
#define MATERIAL_H

#include "MapColor.h"

typedef struct {
    MapColor *materialMapColor;
    int canBurn;
    int groundCover;
    int isTranslucent;
    int canHarvest;
    int mobilityFlag;

    int isLiquid;
    int isSolid;
    int canBlockGrass;
} Material;

Material *Material_create(MapColor *color);
Material *MaterialLiquid_create(MapColor *color);
Material *MaterialTransparent_create(MapColor *color);
Material *MaterialLogic_create(MapColor *color);
Material *MaterialPortal_create(MapColor *color);

Material *Material_setBurning(Material *self);
Material *Material_setIsGroundCover(Material *self);
Material *Material_setIsTranslucent(Material *self);
Material *Material_setNoHarvest(Material *self);
Material *Material_setNoPushMobility(Material *self);
Material *Material_setImmovableMobility(Material *self);

int Material_getIsLiquid(Material *self);
int Material_isSolid(Material *self);
int Material_getCanBlockGrass(Material *self);
int Material_getIsSolid(Material *self);
int Material_getIsGroundCover(Material *self);
int Material_getIsTranslucent(Material *self);
int Material_getIsHarvestable(Material *self);
int Material_getBurning(Material *self);
int Material_getMaterialMobility(Material *self);

extern Material *Material_air;
extern Material *Material_grassMaterial;
extern Material *Material_ground;
extern Material *Material_wood;
extern Material *Material_rock;
extern Material *Material_iron;
extern Material *Material_water;
extern Material *Material_lava;
extern Material *Material_leaves;
extern Material *Material_plants;
extern Material *Material_sponge;
extern Material *Material_cloth;
extern Material *Material_fire;
extern Material *Material_sand;
extern Material *Material_circuits;
extern Material *Material_glass;
extern Material *Material_tnt;
extern Material *Material_field_4262_q;
extern Material *Material_ice;
extern Material *Material_snow;
extern Material *Material_builtSnow;
extern Material *Material_cactus;
extern Material *Material_clay;
extern Material *Material_pumpkin;
extern Material *Material_portal;
extern Material *Material_cakeMaterial;
extern Material *Material_field_31068_A;
extern Material *Material_field_31067_B;

void Material_init(void);

#endif
