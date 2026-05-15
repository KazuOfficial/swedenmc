#ifndef BLOCK_H
#define BLOCK_H

#include "Material.h"
#include "StepSound.h"
#include "AxisAlignedBB.h"
#include "MovingObjectPosition.h"
#include "JavaRandom.h"
#include "GameForward.h"
#include <stdint.h>

#define BLOCK_COUNT 256

typedef struct {
    AxisAlignedBB *arr;
    int count;
    int max;
} CollidingList;

struct Block;
typedef struct Block Block;

typedef struct {
    void (*initializeBlock)(Block *self);
    int (*renderAsNormalBlock)(Block *self);
    int (*getRenderType)(Block *self);
    int (*shouldSideBeRendered)(Block *self, IBlockAccess *world, int x, int y, int z, int side);
    int (*getIsBlockSolid)(Block *self, IBlockAccess *world, int x, int y, int z, int side);
    int (*getBlockTexture)(Block *self, IBlockAccess *world, int x, int y, int z, int side);
    int (*getBlockTextureFromSideAndMetadata)(Block *self, int side, int meta);
    int (*getBlockTextureFromSide)(Block *self, int side);
    AxisAlignedBB *(*getSelectedBoundingBoxFromPool)(Block *self, World *world, int x, int y,
                                                     int z);
    void (*getCollidingBoundingBoxes)(Block *self, World *world, int x, int y, int z,
                                      AxisAlignedBB *bb, void *list);
    AxisAlignedBB *(*getCollisionBoundingBoxFromPool)(Block *self, World *world, int x, int y,
                                                      int z);
    int (*isOpaqueCube)(Block *self);
    int (*canCollideCheck)(Block *self, int meta, int flag);
    int (*isCollidable)(Block *self);
    void (*updateTick)(Block *self, World *world, int x, int y, int z, JavaRandom *rand);
    void (*randomDisplayTick)(Block *self, World *world, int x, int y, int z, JavaRandom *rand);
    void (*onBlockDestroyedByPlayer)(Block *self, World *world, int x, int y, int z, int meta);
    void (*onNeighborBlockChange)(Block *self, World *world, int x, int y, int z, int neighborID);
    int (*tickRate)(Block *self);
    void (*onBlockAdded)(Block *self, World *world, int x, int y, int z);
    void (*onBlockRemoval)(Block *self, World *world, int x, int y, int z);
    int (*quantityDropped)(Block *self, JavaRandom *rand);
    int (*idDropped)(Block *self, int meta, JavaRandom *rand);
    float (*blockStrength)(Block *self, EntityPlayer *player);
    void (*dropBlockAsItemWithChance)(Block *self, World *world, int x, int y, int z, int meta,
                                      float chance);
    void (*dropBlockAsItem_do)(Block *self, World *world, int x, int y, int z, ItemStack *stack);
    int (*damageDropped)(Block *self, int meta);
    float (*getExplosionResistance)(Block *self, struct Entity *entity);
    MovingObjectPosition *(*collisionRayTrace)(Block *self, World *world, int x, int y, int z,
                                               Vec3D *start, Vec3D *end);
    void (*onBlockDestroyedByExplosion)(Block *self, World *world, int x, int y, int z);
    int (*getRenderBlockPass)(Block *self);
    int (*canPlaceBlockOnSide)(Block *self, World *world, int x, int y, int z, int side);
    int (*canPlaceBlockAt)(Block *self, World *world, int x, int y, int z);
    int (*blockActivated)(Block *self, World *world, int x, int y, int z, EntityPlayer *player);
    void (*onEntityWalking)(Block *self, World *world, int x, int y, int z, struct Entity *entity);
    void (*onBlockPlaced)(Block *self, World *world, int x, int y, int z, int side);
    void (*onBlockClicked)(Block *self, World *world, int x, int y, int z, EntityPlayer *player);
    void (*velocityToAddToEntity)(Block *self, World *world, int x, int y, int z,
                                  struct Entity *entity, Vec3D *vel);
    void (*setBlockBoundsBasedOnState)(Block *self, IBlockAccess *world, int x, int y, int z);
    int (*getRenderColor)(Block *self, int meta);
    int (*colorMultiplier)(Block *self, IBlockAccess *world, int x, int y, int z);
    int (*isPoweringTo)(Block *self, IBlockAccess *world, int x, int y, int z, int side);
    int (*canProvidePower)(Block *self);
    void (*onEntityCollidedWithBlock)(Block *self, World *world, int x, int y, int z,
                                      struct Entity *entity);
    int (*isIndirectlyPoweringTo)(Block *self, World *world, int x, int y, int z, int side);
    void (*setBlockBoundsForItemRender)(Block *self);
    void (*harvestBlock)(Block *self, World *world, EntityPlayer *player, int x, int y, int z,
                         int meta);
    int (*canBlockStay)(Block *self, World *world, int x, int y, int z);

    int (*canThisPlantGrowOnThisBlockID)(Block *self, int blockId);
    void (*onBlockPlacedBy)(Block *self, World *world, int x, int y, int z, EntityLiving *entity);
    const char *(*translateBlockName)(Block *self);
    const char *(*getBlockName)(Block *self);
    void (*playBlock)(Block *self, World *world, int x, int y, int z, int p1, int p2);
    int (*getEnableStats)(Block *self);
    int (*getMobilityFlag)(Block *self);
    int (*removeBlockByPlayer)(Block *self, World *world, EntityPlayer *player, int x, int y,
                               int z);
    float (*getBlockBrightness)(Block *self, IBlockAccess *world, int x, int y, int z);
} BlockVtable;

struct Block {
    const BlockVtable *vtable;
    int blockIndexInTexture;
    int blockID;
    float blockHardness;
    float blockResistance;
    int blockConstructorCalled;
    int enableStats;
    double minX, minY, minZ;
    double maxX, maxY, maxZ;
    StepSound *stepSound;
    float blockParticleGravity;
    Material *blockMaterial;
    float slipperiness;
    char blockName[64];
    int fallInstantly;
};

extern Block *Block_blocksList[BLOCK_COUNT];
extern int Block_tickOnLoad[BLOCK_COUNT];
extern int Block_opaqueCubeLookup[BLOCK_COUNT];
extern int Block_isBlockContainer[BLOCK_COUNT];
extern int Block_lightOpacity[BLOCK_COUNT];
extern int Block_canBlockGrass[BLOCK_COUNT];
extern int Block_lightValue[BLOCK_COUNT];
extern int Block_field_28032_t[BLOCK_COUNT];

extern StepSound *Block_soundPowderFootstep;
extern StepSound *Block_soundWoodFootstep;
extern StepSound *Block_soundGravelFootstep;
extern StepSound *Block_soundGrassFootstep;
extern StepSound *Block_soundStoneFootstep;
extern StepSound *Block_soundMetalFootstep;
extern StepSound *Block_soundGlassFootstep;
extern StepSound *Block_soundClothFootstep;
extern StepSound *Block_soundSandFootstep;

extern Block *Block_stone;
extern Block *Block_grass;
extern Block *Block_dirt;
extern Block *Block_cobblestone;
extern Block *Block_planks;
extern Block *Block_sapling;
extern Block *Block_bedrock;
extern Block *Block_waterMoving;
extern Block *Block_waterStill;
extern Block *Block_lavaMoving;
extern Block *Block_lavaStill;
extern Block *Block_sand;
extern Block *Block_gravel;
extern Block *Block_oreGold;
extern Block *Block_oreIron;
extern Block *Block_oreCoal;
extern Block *Block_wood;
extern Block *Block_leaves;
extern Block *Block_sponge;
extern Block *Block_glass;
extern Block *Block_oreLapis;
extern Block *Block_blockLapis;
extern Block *Block_dispenser;
extern Block *Block_sandStone;
extern Block *Block_musicBlock;
extern Block *Block_blockBed;
extern Block *Block_railPowered;
extern Block *Block_railDetector;
extern Block *Block_pistonStickyBase;
extern Block *Block_web;
extern Block *Block_tallGrass;
extern Block *Block_deadBush;
extern Block *Block_pistonBase;
extern Block *Block_pistonExtension;
extern Block *Block_cloth;
extern Block *Block_pistonMoving;
extern Block *Block_plantYellow;
extern Block *Block_plantRed;
extern Block *Block_mushroomBrown;
extern Block *Block_mushroomRed;
extern Block *Block_blockGold;
extern Block *Block_blockSteel;
extern Block *Block_stairDouble;
extern Block *Block_stairSingle;
extern Block *Block_brick;
extern Block *Block_tnt;
extern Block *Block_bookShelf;
extern Block *Block_cobblestoneMossy;
extern Block *Block_obsidian;
extern Block *Block_torchWood;
extern Block *Block_fire;
extern Block *Block_mobSpawner;
extern Block *Block_stairCompactPlanks;
extern Block *Block_chest;
extern Block *Block_redstoneWire;
extern Block *Block_oreDiamond;
extern Block *Block_blockDiamond;
extern Block *Block_workbench;
extern Block *Block_crops;
extern Block *Block_tilledField;
extern Block *Block_stoneOvenIdle;
extern Block *Block_stoneOvenActive;
extern Block *Block_signPost;
extern Block *Block_doorWood;
extern Block *Block_ladder;
extern Block *Block_rail;
extern Block *Block_stairCompactCobblestone;
extern Block *Block_signWall;
extern Block *Block_lever;
extern Block *Block_pressurePlateStone;
extern Block *Block_doorSteel;
extern Block *Block_pressurePlatePlanks;
extern Block *Block_oreRedstone;
extern Block *Block_oreRedstoneGlowing;
extern Block *Block_torchRedstoneIdle;
extern Block *Block_torchRedstoneActive;
extern Block *Block_button;
extern Block *Block_snow;
extern Block *Block_ice;
extern Block *Block_blockSnow;
extern Block *Block_cactus;
extern Block *Block_blockClay;
extern Block *Block_reed;
extern Block *Block_jukebox;
extern Block *Block_fence;
extern Block *Block_pumpkin;
extern Block *Block_netherrack;
extern Block *Block_slowSand;
extern Block *Block_glowStone;
extern Block *Block_portal;
extern Block *Block_pumpkinLantern;
extern Block *Block_cake;
extern Block *Block_redstoneRepeaterIdle;
extern Block *Block_redstoneRepeaterActive;
extern Block *Block_lockedChest;
extern Block *Block_trapdoor;

void Block_construct(Block *self, int id, Material *mat);
void Block_constructWithTexture(Block *self, int id, int texIndex, Material *mat);
Block *Block_disableNeighborNotifyOnMetadataChange(Block *self);
Block *Block_setStepSound(Block *self, StepSound *sound);
Block *Block_setLightOpacity(Block *self, int opacity);
Block *Block_setLightValue(Block *self, float value);
Block *Block_setResistance(Block *self, float resistance);
Block *Block_setHardness(Block *self, float hardness);
Block *Block_setBlockUnbreakable(Block *self);
Block *Block_setTickOnLoad(Block *self, int flag);
Block *Block_setBlockName(Block *self, const char *name);
Block *Block_disableStats(Block *self);

void Block_dropBlockAsItem(Block *self, World *world, int x, int y, int z, int meta);
void Block_setBlockBounds(Block *self, float x0, float y0, float z0, float x1, float y1, float z1);

extern const BlockVtable Block_defaultVtable;

void Block_initRegistry(void);

#define Block_renderAsNormalBlock(b) (b)->vtable->renderAsNormalBlock(b)
#define Block_getRenderType(b) (b)->vtable->getRenderType(b)
#define Block_isOpaqueCube(b) (b)->vtable->isOpaqueCube(b)
#define Block_isCollidable(b) (b)->vtable->isCollidable(b)
#define Block_updateTick(b, w, x, y, z, r) (b)->vtable->updateTick(b, w, x, y, z, r)
#define Block_idDropped(b, m, r) (b)->vtable->idDropped(b, m, r)
#define Block_quantityDropped(b, r) (b)->vtable->quantityDropped(b, r)
#define Block_getBlockTexture(b, w, x, y, z, s) (b)->vtable->getBlockTexture(b, w, x, y, z, s)

int BlockSand_canFallBelow(World *var0, int var1, int var2, int var3);
extern int BlockSand_fallInstantly;

void BlockPistonExtension_func_31052_a_(Block *self, int var1);

void BlockPistonExtension_func_31051_a(Block *self);

int BlockPistonBase_func_31040_i(Block *self);

int BlockPistonBase_isPowered(int meta);

int BlockPistonBase_func_31044_d(int meta);

struct TileEntity *BlockPistonMoving_func_31036_a(int var0, int var1, int var2, int var3, int var4);

AxisAlignedBB *BlockPistonMoving_func_31035_a(Block *self, World *var1, int var2, int var3,
                                              int var4, int var5, float var6, int var7);

int BlockPortal_tryToCreatePortal(struct Block *self, struct World *var1, int var2, int var3,
                                  int var4);

void BlockSapling_growTree(struct Block *self, struct World *var1, int var2, int var3, int var4,
                           struct JavaRandom *var5);

#endif
