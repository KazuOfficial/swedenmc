#include "Block.h"
#include "World.h"
#include "Item.h"
#include "Material.h"
#include "ItemBlock.h"
#include "ItemLog.h"
#include "ItemSlab.h"
#include "ItemSapling.h"
#include "ItemLeaves.h"
#include "ItemPiston.h"
#include "EntityPlayer.h"
#include "EntityItem.h"
#include "ItemStack.h"
#include "StepSoundStone.h"
#include "StepSoundSand.h"
#include "MovingObjectPosition.h"
#include "Vec3D.h"
#include "StatList.h"
#include "StatCollector.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Block *Block_blocksList[BLOCK_COUNT];
int Block_tickOnLoad[BLOCK_COUNT];
int Block_opaqueCubeLookup[BLOCK_COUNT];
int Block_isBlockContainer[BLOCK_COUNT];
int Block_lightOpacity[BLOCK_COUNT];
int Block_canBlockGrass[BLOCK_COUNT];
int Block_lightValue[BLOCK_COUNT];
int Block_field_28032_t[BLOCK_COUNT];

StepSound *Block_soundPowderFootstep;
StepSound *Block_soundWoodFootstep;
StepSound *Block_soundGravelFootstep;
StepSound *Block_soundGrassFootstep;
StepSound *Block_soundStoneFootstep;
StepSound *Block_soundMetalFootstep;
StepSound *Block_soundGlassFootstep;
StepSound *Block_soundClothFootstep;
StepSound *Block_soundSandFootstep;

Block *Block_stone;
Block *Block_grass;
Block *Block_dirt;
Block *Block_cobblestone;
Block *Block_planks;
Block *Block_sapling;
Block *Block_bedrock;
Block *Block_waterMoving;
Block *Block_waterStill;
Block *Block_lavaMoving;
Block *Block_lavaStill;
Block *Block_sand;
Block *Block_gravel;
Block *Block_oreGold;
Block *Block_oreIron;
Block *Block_oreCoal;
Block *Block_wood;
Block *Block_leaves;
Block *Block_sponge;
Block *Block_glass;
Block *Block_oreLapis;
Block *Block_blockLapis;
Block *Block_dispenser;
Block *Block_sandStone;
Block *Block_musicBlock;
Block *Block_blockBed;
Block *Block_railPowered;
Block *Block_railDetector;
Block *Block_pistonStickyBase;
Block *Block_web;
Block *Block_tallGrass;
Block *Block_deadBush;
Block *Block_pistonBase;
Block *Block_pistonExtension;
Block *Block_cloth;
Block *Block_pistonMoving;
Block *Block_plantYellow;
Block *Block_plantRed;
Block *Block_mushroomBrown;
Block *Block_mushroomRed;
Block *Block_blockGold;
Block *Block_blockSteel;
Block *Block_stairDouble;
Block *Block_stairSingle;
Block *Block_brick;
Block *Block_tnt;
Block *Block_bookShelf;
Block *Block_cobblestoneMossy;
Block *Block_obsidian;
Block *Block_torchWood;
Block *Block_fire;
Block *Block_mobSpawner;
Block *Block_stairCompactPlanks;
Block *Block_chest;
Block *Block_redstoneWire;
Block *Block_oreDiamond;
Block *Block_blockDiamond;
Block *Block_workbench;
Block *Block_crops;
Block *Block_tilledField;
Block *Block_stoneOvenIdle;
Block *Block_stoneOvenActive;
Block *Block_signPost;
Block *Block_doorWood;
Block *Block_ladder;
Block *Block_rail;
Block *Block_stairCompactCobblestone;
Block *Block_signWall;
Block *Block_lever;
Block *Block_pressurePlateStone;
Block *Block_doorSteel;
Block *Block_pressurePlatePlanks;
Block *Block_oreRedstone;
Block *Block_oreRedstoneGlowing;
Block *Block_torchRedstoneIdle;
Block *Block_torchRedstoneActive;
Block *Block_button;
Block *Block_snow;
Block *Block_ice;
Block *Block_blockSnow;
Block *Block_cactus;
Block *Block_blockClay;
Block *Block_reed;
Block *Block_jukebox;
Block *Block_fence;
Block *Block_pumpkin;
Block *Block_netherrack;
Block *Block_slowSand;
Block *Block_glowStone;
Block *Block_portal;
Block *Block_pumpkinLantern;
Block *Block_cake;
Block *Block_redstoneRepeaterIdle;
Block *Block_redstoneRepeaterActive;
Block *Block_lockedChest;
Block *Block_trapdoor;

void Block_construct(Block *self, int id, Material *mat) {
    self->blockConstructorCalled = 1;
    self->enableStats = 1;
    self->stepSound = Block_soundPowderFootstep;
    self->blockParticleGravity = 1.0f;
    self->slipperiness = 0.6f;
    self->blockMaterial = mat;
    self->blockID = id;
    Block_blocksList[id] = self;
    Block_setBlockBounds(self, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    Block_opaqueCubeLookup[id] = self->vtable->isOpaqueCube(self) ? 1 : 0;
    Block_lightOpacity[id] = Block_opaqueCubeLookup[id] ? 255 : 0;
    Block_canBlockGrass[id] = !Material_getCanBlockGrass(mat);
    Block_isBlockContainer[id] = 0;
}

void Block_constructWithTexture(Block *self, int id, int texIndex, Material *mat) {
    Block_construct(self, id, mat);
    self->blockIndexInTexture = texIndex;
}

Block *Block_disableNeighborNotifyOnMetadataChange(Block *self) {
    Block_field_28032_t[self->blockID] = 1;
    return self;
}
Block *Block_setStepSound(Block *self, StepSound *s) {
    self->stepSound = s;
    return self;
}
Block *Block_setLightOpacity(Block *self, int v) {
    Block_lightOpacity[self->blockID] = v;
    return self;
}
Block *Block_setLightValue(Block *self, float v) {
    Block_lightValue[self->blockID] = (int)(15.0f * v);
    return self;
}
Block *Block_setResistance(Block *self, float v) {
    self->blockResistance = v * 3.0f;
    return self;
}
Block *Block_setHardness(Block *self, float v) {
    self->blockHardness = v;
    if (self->blockResistance < v * 5.0f)
        self->blockResistance = v * 5.0f;
    return self;
}
Block *Block_setBlockUnbreakable(Block *self) { return Block_setHardness(self, -1.0f); }
Block *Block_setTickOnLoad(Block *self, int v) {
    Block_tickOnLoad[self->blockID] = v;
    return self;
}
Block *Block_setBlockName(Block *self, const char *name) {
    snprintf(self->blockName, sizeof(self->blockName), "tile.%s", name);
    return self;
}
Block *Block_disableStats(Block *self) {
    self->enableStats = 0;
    return self;
}

void Block_setBlockBounds(Block *self, float x0, float y0, float z0, float x1, float y1, float z1) {
    self->minX = x0;
    self->minY = y0;
    self->minZ = z0;
    self->maxX = x1;
    self->maxY = y1;
    self->maxZ = z1;
}

void Block_dropBlockAsItem(Block *self, World *world, int x, int y, int z, int meta) {
    self->vtable->dropBlockAsItemWithChance(self, world, x, y, z, meta, 1.0F);
}

static void def_initializeBlock(Block *self) { (void)self; }
static int def_renderAsNormalBlock(Block *self) {
    (void)self;
    return 1;
}
static int def_getRenderType(Block *self) {
    (void)self;
    return 0;
}
static int def_shouldSideBeRendered(Block *self, IBlockAccess *w, int x, int y, int z, int side) {
    if (side == 0 && self->minY > 0.0)
        return 1;
    if (side == 1 && self->maxY < 1.0)
        return 1;
    if (side == 2 && self->minZ > 0.0)
        return 1;
    if (side == 3 && self->maxZ < 1.0)
        return 1;
    if (side == 4 && self->minX > 0.0)
        return 1;
    if (side == 5 && self->maxX < 1.0)
        return 1;
    return !w->vtable->isBlockOpaqueCube(w, x, y, z);
}
static int def_getIsBlockSolid(Block *self, IBlockAccess *w, int x, int y, int z, int side) {
    (void)self;
    (void)side;
    Material *mat = w->vtable->getBlockMaterial(w, x, y, z);
    return mat ? mat->isSolid : 0;
}
static int def_getBlockTexture(Block *self, IBlockAccess *w, int x, int y, int z, int side) {
    return self->vtable->getBlockTextureFromSideAndMetadata(
        self, side, w->vtable->getBlockMetadata(w, x, y, z));
}
static int def_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta) {
    (void)meta;
    return self->vtable->getBlockTextureFromSide(self, side);
}
static int def_getBlockTextureFromSide(Block *self, int side) {
    (void)side;
    return self->blockIndexInTexture;
}
static AxisAlignedBB *def_getSelectedBoundingBoxFromPool(Block *self, World *w, int x, int y,
                                                         int z) {
    (void)w;
    return AxisAlignedBB_getBoundingBoxFromPool(x + self->minX, y + self->minY, z + self->minZ,
                                                x + self->maxX, y + self->maxY, z + self->maxZ);
}
static void def_getCollidingBoundingBoxes(Block *self, World *w, int x, int y, int z,
                                          AxisAlignedBB *bb, void *list) {
    CollidingList *cl = (CollidingList *)list;
    AxisAlignedBB *col = self->vtable->getCollisionBoundingBoxFromPool(self, w, x, y, z);
    if (col != NULL && AxisAlignedBB_intersectsWith(bb, col) && cl->count < cl->max)
        cl->arr[cl->count++] = *col;
}
static AxisAlignedBB *def_getCollisionBoundingBoxFromPool(Block *self, World *w, int x, int y,
                                                          int z) {
    (void)w;
    return AxisAlignedBB_getBoundingBoxFromPool(x + self->minX, y + self->minY, z + self->minZ,
                                                x + self->maxX, y + self->maxY, z + self->maxZ);
}
static int def_isOpaqueCube(Block *self) {
    (void)self;
    return 1;
}
static int def_canCollideCheck(Block *self, int m, int f) {
    (void)self;
    (void)m;
    (void)f;
    return self->vtable->isCollidable(self);
}
static int def_isCollidable(Block *self) {
    (void)self;
    return 1;
}
static void def_updateTick(Block *self, World *w, int x, int y, int z, JavaRandom *r) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)r;
}
static void def_randomDisplayTick(Block *self, World *w, int x, int y, int z, JavaRandom *r) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)r;
}
static void def_onBlockDestroyedByPlayer(Block *self, World *w, int x, int y, int z, int m) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)m;
}
static void def_onNeighborBlockChange(Block *self, World *w, int x, int y, int z, int n) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)n;
}
static int def_tickRate(Block *self) {
    (void)self;
    return 10;
}
static void def_onBlockAdded(Block *self, World *w, int x, int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
}
static void def_onBlockRemoval(Block *self, World *w, int x, int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
}
static int def_quantityDropped(Block *self, JavaRandom *r) {
    (void)self;
    (void)r;
    return 1;
}
static int def_idDropped(Block *self, int m, JavaRandom *r) {
    (void)m;
    (void)r;
    return self->blockID;
}
static float def_blockStrength(Block *self, EntityPlayer *p) {
    if (self->blockHardness < 0.0f)
        return 0.0f;
    return !EntityPlayer_canHarvestBlock(p, self)
               ? 1.0f / self->blockHardness / 100.0f
               : EntityPlayer_getCurrentPlayerStrVsBlock(p, self) / self->blockHardness / 30.0f;
}
static void def_dropBlockAsItemWithChance(Block *self, World *w, int x, int y, int z, int m,
                                          float chance) {
    if (!w->multiplayerWorld) {
        int var7 = self->vtable->quantityDropped(self, &w->rand);
        for (int var8 = 0; var8 < var7; ++var8) {
            if (JavaRandom_nextFloat(&w->rand) <= chance) {
                int var9 = self->vtable->idDropped(self, m, &w->rand);
                if (var9 > 0) {
                    ItemStack s =
                        ItemStack_fromItemID(var9, 1, self->vtable->damageDropped(self, m));
                    self->vtable->dropBlockAsItem_do(self, w, x, y, z, &s);
                }
            }
        }
    }
}
static void def_dropBlockAsItem_do(Block *self, World *w, int x, int y, int z, ItemStack *s) {
    (void)self;
    if (!w->multiplayerWorld) {
        float var6 = 0.7f;
        double var7 = (double)(JavaRandom_nextFloat(&w->rand) * var6) + (double)(1.0f - var6) * 0.5;
        double var9 = (double)(JavaRandom_nextFloat(&w->rand) * var6) + (double)(1.0f - var6) * 0.5;
        double var11 =
            (double)(JavaRandom_nextFloat(&w->rand) * var6) + (double)(1.0f - var6) * 0.5;
        EntityItem *var13 =
            EntityItem_create(w, (double)x + var7, (double)y + var9, (double)z + var11, *s);
        var13->delayBeforeCanPickup = 10;
        World_entityJoinedWorld(w, (Entity *)var13);
    }
}
static int def_damageDropped(Block *self, int m) {
    (void)self;
    (void)m;
    return 0;
}
static float def_getExplosionResistance(Block *self, Entity *e) {
    (void)e;
    return self->blockResistance / 5.0f;
}
static MovingObjectPosition *def_collisionRayTrace(Block *self, World *w, int x, int y, int z,
                                                   Vec3D *start, Vec3D *end) {
    self->vtable->setBlockBoundsBasedOnState(self, (IBlockAccess *)w, x, y, z);
    Vec3D *var5 = Vec3D_addVector(start, (double)(-x), (double)(-y), (double)(-z));
    Vec3D *var6 = Vec3D_addVector(end, (double)(-x), (double)(-y), (double)(-z));
    Vec3D *var7 = Vec3D_getIntermediateWithXValue(var5, var6, self->minX);
    Vec3D *var8 = Vec3D_getIntermediateWithXValue(var5, var6, self->maxX);
    Vec3D *var9 = Vec3D_getIntermediateWithYValue(var5, var6, self->minY);
    Vec3D *var10 = Vec3D_getIntermediateWithYValue(var5, var6, self->maxY);
    Vec3D *var11 = Vec3D_getIntermediateWithZValue(var5, var6, self->minZ);
    Vec3D *var12 = Vec3D_getIntermediateWithZValue(var5, var6, self->maxZ);

    if (var7 != NULL && !(var7->yCoord >= self->minY && var7->yCoord <= self->maxY &&
                          var7->zCoord >= self->minZ && var7->zCoord <= self->maxZ))
        var7 = NULL;
    if (var8 != NULL && !(var8->yCoord >= self->minY && var8->yCoord <= self->maxY &&
                          var8->zCoord >= self->minZ && var8->zCoord <= self->maxZ))
        var8 = NULL;

    if (var9 != NULL && !(var9->xCoord >= self->minX && var9->xCoord <= self->maxX &&
                          var9->zCoord >= self->minZ && var9->zCoord <= self->maxZ))
        var9 = NULL;
    if (var10 != NULL && !(var10->xCoord >= self->minX && var10->xCoord <= self->maxX &&
                           var10->zCoord >= self->minZ && var10->zCoord <= self->maxZ))
        var10 = NULL;

    if (var11 != NULL && !(var11->xCoord >= self->minX && var11->xCoord <= self->maxX &&
                           var11->yCoord >= self->minY && var11->yCoord <= self->maxY))
        var11 = NULL;
    if (var12 != NULL && !(var12->xCoord >= self->minX && var12->xCoord <= self->maxX &&
                           var12->yCoord >= self->minY && var12->yCoord <= self->maxY))
        var12 = NULL;
    Vec3D *var13 = NULL;
    if (var7 != NULL &&
        (var13 == NULL || Vec3D_distanceTo(var5, var7) < Vec3D_distanceTo(var5, var13)))
        var13 = var7;
    if (var8 != NULL &&
        (var13 == NULL || Vec3D_distanceTo(var5, var8) < Vec3D_distanceTo(var5, var13)))
        var13 = var8;
    if (var9 != NULL &&
        (var13 == NULL || Vec3D_distanceTo(var5, var9) < Vec3D_distanceTo(var5, var13)))
        var13 = var9;
    if (var10 != NULL &&
        (var13 == NULL || Vec3D_distanceTo(var5, var10) < Vec3D_distanceTo(var5, var13)))
        var13 = var10;
    if (var11 != NULL &&
        (var13 == NULL || Vec3D_distanceTo(var5, var11) < Vec3D_distanceTo(var5, var13)))
        var13 = var11;
    if (var12 != NULL &&
        (var13 == NULL || Vec3D_distanceTo(var5, var12) < Vec3D_distanceTo(var5, var13)))
        var13 = var12;
    if (var13 == NULL)
        return NULL;
    int var14 = -1;
    if (var13 == var7)
        var14 = 4;
    if (var13 == var8)
        var14 = 5;
    if (var13 == var9)
        var14 = 0;
    if (var13 == var10)
        var14 = 1;
    if (var13 == var11)
        var14 = 2;
    if (var13 == var12)
        var14 = 3;
    Vec3D *hitVec = Vec3D_addVector(var13, (double)x, (double)y, (double)z);
    return MovingObjectPosition_create(x, y, z, var14, hitVec);
}
static void def_onBlockDestroyedByExplosion(Block *self, World *w, int x, int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
}
static int def_getRenderBlockPass(Block *self) {
    (void)self;
    return 0;
}
static int def_canPlaceBlockOnSide(Block *self, World *w, int x, int y, int z, int side) {
    (void)side;
    return self->vtable->canPlaceBlockAt(self, w, x, y, z);
}
static int def_canPlaceBlockAt(Block *self, World *w, int x, int y, int z) {
    (void)self;
    int var5 = World_getBlockId(w, x, y, z);
    return var5 == 0 || (Block_blocksList[var5] != NULL &&
                         Material_getIsGroundCover(Block_blocksList[var5]->blockMaterial));
}
static int def_blockActivated(Block *self, World *w, int x, int y, int z, EntityPlayer *p) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)p;
    return 0;
}
static void def_onEntityWalking(Block *self, World *w, int x, int y, int z, Entity *e) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)e;
}
static void def_onBlockPlaced(Block *self, World *w, int x, int y, int z, int side) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)side;
}
static void def_onBlockClicked(Block *self, World *w, int x, int y, int z, EntityPlayer *p) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)p;
}
static void def_velocityToAddToEntity(Block *self, World *w, int x, int y, int z, Entity *e,
                                      Vec3D *v) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)e;
    (void)v;
}
static void def_setBlockBoundsBasedOnState(Block *self, IBlockAccess *w, int x, int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
}
static int def_getRenderColor(Block *self, int m) {
    (void)self;
    (void)m;
    return 16777215;
}
static int def_colorMultiplier(Block *self, IBlockAccess *w, int x, int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    return 16777215;
}
static int def_isPoweringTo(Block *self, IBlockAccess *w, int x, int y, int z, int s) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)s;
    return 0;
}
static int def_canProvidePower(Block *self) {
    (void)self;
    return 0;
}
static void def_onEntityCollidedWithBlock(Block *self, World *w, int x, int y, int z, Entity *e) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)e;
}
static int def_isIndirectlyPoweringTo(Block *self, World *w, int x, int y, int z, int s) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)s;
    return 0;
}
static void def_setBlockBoundsForItemRender(Block *self) { (void)self; }
static void def_harvestBlock(Block *self, World *w, EntityPlayer *p, int x, int y, int z, int m) {
    EntityPlayer_addStat(p, StatList_mineBlockStatArray[self->blockID], 1);
    Block_dropBlockAsItem(self, w, x, y, z, m);
}
static int def_canBlockStay(Block *self, World *w, int x, int y, int z) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    return 1;
}
static int def_canThisPlantGrowOnThisBlockID(Block *self, int blockId) {
    (void)self;
    (void)blockId;
    return 0;
}
static void def_onBlockPlacedBy(Block *self, World *w, int x, int y, int z, EntityLiving *e) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)e;
}
static const char *def_translateBlockName(Block *self) {
    static char key[128];
    snprintf(key, sizeof(key), "%s.name", self->blockName);
    return StatCollector_translateToLocal(key);
}
static const char *def_getBlockName(Block *self) { return self->blockName; }
static void def_playBlock(Block *self, World *w, int x, int y, int z, int p1, int p2) {
    (void)self;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)p1;
    (void)p2;
}
static int def_getEnableStats(Block *self) { return self->enableStats; }
static int def_getMobilityFlag(Block *self) {
    return Material_getMaterialMobility(self->blockMaterial);
}
static int def_removeBlockByPlayer(Block *self, World *world, EntityPlayer *player, int x, int y,
                                   int z) {
    (void)self;
    (void)player;
    return World_setBlockToAir(world, x, y, z);
}
static float def_getBlockBrightness(Block *self, IBlockAccess *var1, int var2, int var3, int var4) {
    return var1->vtable->getBrightness(var1, var2, var3, var4, Block_lightValue[self->blockID]);
}

const BlockVtable Block_defaultVtable = {def_initializeBlock,
                                         def_renderAsNormalBlock,
                                         def_getRenderType,
                                         def_shouldSideBeRendered,
                                         def_getIsBlockSolid,
                                         def_getBlockTexture,
                                         def_getBlockTextureFromSideAndMetadata,
                                         def_getBlockTextureFromSide,
                                         def_getSelectedBoundingBoxFromPool,
                                         def_getCollidingBoundingBoxes,
                                         def_getCollisionBoundingBoxFromPool,
                                         def_isOpaqueCube,
                                         def_canCollideCheck,
                                         def_isCollidable,
                                         def_updateTick,
                                         def_randomDisplayTick,
                                         def_onBlockDestroyedByPlayer,
                                         def_onNeighborBlockChange,
                                         def_tickRate,
                                         def_onBlockAdded,
                                         def_onBlockRemoval,
                                         def_quantityDropped,
                                         def_idDropped,
                                         def_blockStrength,
                                         def_dropBlockAsItemWithChance,
                                         def_dropBlockAsItem_do,
                                         def_damageDropped,
                                         def_getExplosionResistance,
                                         def_collisionRayTrace,
                                         def_onBlockDestroyedByExplosion,
                                         def_getRenderBlockPass,
                                         def_canPlaceBlockOnSide,
                                         def_canPlaceBlockAt,
                                         def_blockActivated,
                                         def_onEntityWalking,
                                         def_onBlockPlaced,
                                         def_onBlockClicked,
                                         def_velocityToAddToEntity,
                                         def_setBlockBoundsBasedOnState,
                                         def_getRenderColor,
                                         def_colorMultiplier,
                                         def_isPoweringTo,
                                         def_canProvidePower,
                                         def_onEntityCollidedWithBlock,
                                         def_isIndirectlyPoweringTo,
                                         def_setBlockBoundsForItemRender,
                                         def_harvestBlock,
                                         def_canBlockStay,
                                         def_canThisPlantGrowOnThisBlockID,
                                         def_onBlockPlacedBy,
                                         def_translateBlockName,
                                         def_getBlockName,
                                         def_playBlock,
                                         def_getEnableStats,
                                         def_getMobilityFlag,
                                         def_removeBlockByPlayer,
                                         def_getBlockBrightness};

Block *BlockStone_create(int id, int tex);
Block *BlockGrass_create(int id);
Block *BlockDirt_create(int id, int tex);
Block *BlockSapling_create(int id, int tex);
Block *BlockFlowing_create(int id, Material *mat);
Block *BlockStationary_create(int id, Material *mat);
Block *BlockSand_create(int id, int tex);
Block *BlockGravel_create(int id, int tex);
Block *BlockOre_create(int id, int tex);
Block *BlockLog_create(int id);
Block *BlockLeaves_create(int id, int tex);
Block *BlockSponge_create(int id);
Block *BlockGlass_create(int id, int tex, Material *mat, int seeThrough);
Block *BlockDispenser_create(int id);
Block *BlockSandStone_create(int id);
Block *BlockNote_create(int id);
Block *BlockBed_create(int id);
Block *BlockRail_create(int id, int tex, int booster);
Block *BlockDetectorRail_create(int id, int tex);
Block *BlockPistonBase_create(int id, int tex, int sticky);
Block *BlockWeb_create(int id, int tex);
Block *BlockTallGrass_create(int id, int tex);
Block *BlockDeadBush_create(int id, int tex);
Block *BlockPistonExtension_create(int id, int tex);
Block *BlockCloth_create(void);
Block *BlockPistonMoving_create(int id);
Block *BlockFlower_create(int id, int tex);
Block *BlockMushroom_create(int id, int tex);
Block *BlockOreStorage_create(int id, int tex);
Block *BlockStep_create(int id, int isDouble);
Block *BlockTNT_create(int id, int tex);
Block *BlockBookshelf_create(int id, int tex);
Block *BlockObsidian_create(int id, int tex);
Block *BlockTorch_create(int id, int tex);
Block *BlockFire_create(int id, int tex);
Block *BlockMobSpawner_create(int id, int tex);
Block *BlockStairs_create(int id, Block *modelBlock);
Block *BlockChest_create(int id);
Block *BlockRedstoneWire_create(int id, int tex);
Block *BlockFarmland_create(int id);
Block *BlockFurnace_create(int id, int active);
void BlockFurnace_updateFurnaceBlockState(int var0, World *var1, int var2, int var3, int var4);
Block *BlockSign_create(int id, int isPost);
Block *BlockDoor_create(int id, Material *mat);
Block *BlockLadder_create(int id, int tex);
Block *BlockLever_create(int id, int tex);
Block *BlockPressurePlate_create(int id, int tex, int mobType, Material *mat);
Block *BlockRedstoneOre_create(int id, int tex, int glowing);
Block *BlockRedstoneTorch_create(int id, int tex, int active);
Block *BlockButton_create(int id, int tex);
Block *BlockSnow_create(int id, int tex);
Block *BlockIce_create(int id, int tex);
Block *BlockSnowBlock_create(int id, int tex);
Block *BlockCactus_create(int id, int tex);
Block *BlockClay_create(int id, int tex);
Block *BlockReed_create(int id, int tex);
Block *BlockJukeBox_create(int id, int tex);
Block *BlockFence_create(int id, int tex);
Block *BlockPumpkin_create(int id, int tex, int lantern);
Block *BlockNetherrack_create(int id, int tex);
Block *BlockSoulSand_create(int id, int tex);
Block *BlockGlowStone_create(int id, int tex, Material *mat);
Block *BlockPortal_create(int id, int tex);
Block *BlockCake_create(int id, int tex);
Block *BlockRedstoneRepeater_create(int id, int active);
Block *BlockLockedChest_create(int id);
Block *BlockTrapDoor_create(int id, Material *mat);
Block *BlockWorkbench_create(int id);
Block *BlockCrops_create(int id, int tex);

void Block_initRegistry(void) {
    int i;

    Block_soundPowderFootstep = StepSound_create("stone", 1.0f, 1.0f);
    Block_soundWoodFootstep = StepSound_create("wood", 1.0f, 1.0f);
    Block_soundGravelFootstep = StepSound_create("gravel", 1.0f, 1.0f);
    Block_soundGrassFootstep = StepSound_create("grass", 1.0f, 1.0f);
    Block_soundStoneFootstep = StepSound_create("stone", 1.0f, 1.0f);
    Block_soundMetalFootstep = StepSound_create("stone", 1.0f, 1.5f);
    Block_soundGlassFootstep = StepSoundStone_create("stone", 1.0f, 1.0f);
    Block_soundClothFootstep = StepSound_create("cloth", 1.0f, 1.0f);
    Block_soundSandFootstep = StepSoundSand_create("sand", 1.0f, 1.0f);

    Block_stone = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockStone_create(1, 1), 1.5f), 10.0f),
            Block_soundStoneFootstep),
        "stone");
    Block_grass = Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockGrass_create(2), 0.6f), Block_soundGrassFootstep),
        "grass");
    Block_dirt =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockDirt_create(3, 2), 0.5f),
                                              Block_soundGravelFootstep),
                           "dirt");
    Block_cobblestone = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(({
                                                      Block *_b = (Block *)calloc(1, sizeof(Block));
                                                      _b->vtable = &Block_defaultVtable;
                                                      Block_constructWithTexture(_b, 4, 16,
                                                                                 Material_rock);
                                                      _b;
                                                  }),
                                                  2.0f),
                                10.0f),
            Block_soundStoneFootstep),
        "stonebrick");
    Block_planks = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(({
                                                      Block *_b = (Block *)calloc(1, sizeof(Block));
                                                      _b->vtable = &Block_defaultVtable;
                                                      Block_constructWithTexture(_b, 5, 4,
                                                                                 Material_wood);
                                                      _b;
                                                  }),
                                                  2.0f),
                                5.0f),
            Block_soundWoodFootstep),
        "wood"));
    Block_sapling = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockSapling_create(6, 15), 0.0f),
                                              Block_soundGrassFootstep),
                           "sapling"));
    Block_bedrock = Block_disableStats(Block_setBlockName(
        Block_setStepSound(Block_setResistance(Block_setBlockUnbreakable(({
                                                   Block *_b = (Block *)calloc(1, sizeof(Block));
                                                   _b->vtable = &Block_defaultVtable;
                                                   Block_constructWithTexture(_b, 7, 17,
                                                                              Material_rock);
                                                   _b;
                                               })),
                                               6000000.0f),
                           Block_soundStoneFootstep),
        "bedrock"));
    Block_waterMoving =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setLightOpacity(Block_setHardness(BlockFlowing_create(8, Material_water), 100.0f),
                                  3),
            "water")));
    Block_waterStill =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setLightOpacity(
                Block_setHardness(BlockStationary_create(9, Material_water), 100.0f), 3),
            "water")));
    Block_lavaMoving =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setLightOpacity(
                Block_setLightValue(Block_setHardness(BlockFlowing_create(10, Material_lava), 0.0f),
                                    1.0f),
                255),
            "lava")));
    Block_lavaStill =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setLightOpacity(
                Block_setLightValue(
                    Block_setHardness(BlockStationary_create(11, Material_lava), 100.0f), 1.0f),
                255),
            "lava")));
    Block_sand =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockSand_create(12, 18), 0.5f),
                                              Block_soundSandFootstep),
                           "sand");
    Block_gravel =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockGravel_create(13, 19), 0.6f),
                                              Block_soundGravelFootstep),
                           "gravel");
    Block_oreGold = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockOre_create(14, 32), 3.0f), 5.0f),
            Block_soundStoneFootstep),
        "oreGold");
    Block_oreIron = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockOre_create(15, 33), 3.0f), 5.0f),
            Block_soundStoneFootstep),
        "oreIron");
    Block_oreCoal = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockOre_create(16, 34), 3.0f), 5.0f),
            Block_soundStoneFootstep),
        "oreCoal");
    Block_wood = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockLog_create(17), 2.0f), Block_soundWoodFootstep),
        "log"));
    Block_leaves =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setStepSound(
                Block_setLightOpacity(Block_setHardness(BlockLeaves_create(18, 52), 0.2f), 1),
                Block_soundGrassFootstep),
            "leaves")));
    Block_sponge =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockSponge_create(19), 0.6f),
                                              Block_soundGrassFootstep),
                           "sponge");
    Block_glass = Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockGlass_create(20, 49, Material_glass, 0), 0.3f),
                           Block_soundGlassFootstep),
        "glass");
    Block_oreLapis = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockOre_create(21, 160), 3.0f), 5.0f),
            Block_soundStoneFootstep),
        "oreLapis");
    Block_blockLapis = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(({
                                                      Block *_b = (Block *)calloc(1, sizeof(Block));
                                                      _b->vtable = &Block_defaultVtable;
                                                      Block_constructWithTexture(_b, 22, 144,
                                                                                 Material_rock);
                                                      _b;
                                                  }),
                                                  3.0f),
                                5.0f),
            Block_soundStoneFootstep),
        "blockLapis");
    Block_dispenser = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockDispenser_create(23), 3.5f),
                                              Block_soundStoneFootstep),
                           "dispenser"));
    Block_sandStone =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockSandStone_create(24), 0.8f),
                                              Block_soundStoneFootstep),
                           "sandStone");
    Block_musicBlock = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(Block_setHardness(BlockNote_create(25), 0.8f), "musicBlock"));
    Block_blockBed = Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(
        Block_setBlockName(Block_setHardness(BlockBed_create(26), 0.2f), "bed")));
    Block_railPowered = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockRail_create(27, 179, 1), 0.7f),
                                              Block_soundMetalFootstep),
                           "goldenRail"));
    Block_railDetector = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockDetectorRail_create(28, 195), 0.7f),
                           Block_soundMetalFootstep),
        "detectorRail"));
    Block_pistonStickyBase = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(BlockPistonBase_create(29, 106, 1), "pistonStickyBase"));
    Block_web = Block_setBlockName(
        Block_setLightOpacity(Block_setHardness(BlockWeb_create(30, 11), 4.0f), 1), "web");
    Block_tallGrass = Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockTallGrass_create(31, 39), 0.0f),
                           Block_soundGrassFootstep),
        "tallgrass");
    Block_deadBush =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockDeadBush_create(32, 55), 0.0f),
                                              Block_soundGrassFootstep),
                           "deadbush");
    Block_pistonBase = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(BlockPistonBase_create(33, 107, 0), "pistonBase"));
    Block_pistonExtension =
        Block_disableNeighborNotifyOnMetadataChange(BlockPistonExtension_create(34, 107));
    Block_cloth = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockCloth_create(), 0.8f), Block_soundClothFootstep),
        "cloth"));
    Block_pistonMoving = BlockPistonMoving_create(36);
    Block_plantYellow =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockFlower_create(37, 13), 0.0f),
                                              Block_soundGrassFootstep),
                           "flower");
    Block_plantRed =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockFlower_create(38, 12), 0.0f),
                                              Block_soundGrassFootstep),
                           "rose");
    Block_mushroomBrown = Block_setBlockName(
        Block_setLightValue(
            Block_setStepSound(Block_setHardness(BlockMushroom_create(39, 29), 0.0f),
                               Block_soundGrassFootstep),
            2.0f / 16.0f),
        "mushroom");
    Block_mushroomRed =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockMushroom_create(40, 28), 0.0f),
                                              Block_soundGrassFootstep),
                           "mushroom");
    Block_blockGold = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockOreStorage_create(41, 23), 3.0f), 10.0f),
            Block_soundMetalFootstep),
        "blockGold");
    Block_blockSteel = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockOreStorage_create(42, 22), 5.0f), 10.0f),
            Block_soundMetalFootstep),
        "blockIron");
    Block_stairDouble = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockStep_create(43, 1), 2.0f), 10.0f),
            Block_soundStoneFootstep),
        "stoneSlab");
    Block_stairSingle = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockStep_create(44, 0), 2.0f), 10.0f),
            Block_soundStoneFootstep),
        "stoneSlab");
    Block_brick = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(({
                                                      Block *_b = (Block *)calloc(1, sizeof(Block));
                                                      _b->vtable = &Block_defaultVtable;
                                                      Block_constructWithTexture(_b, 45, 7,
                                                                                 Material_rock);
                                                      _b;
                                                  }),
                                                  2.0f),
                                10.0f),
            Block_soundStoneFootstep),
        "brick");
    Block_tnt =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockTNT_create(46, 8), 0.0f),
                                              Block_soundGrassFootstep),
                           "tnt");
    Block_bookShelf = Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockBookshelf_create(47, 35), 1.5f),
                           Block_soundWoodFootstep),
        "bookshelf");
    Block_cobblestoneMossy = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(({
                                                      Block *_b = (Block *)calloc(1, sizeof(Block));
                                                      _b->vtable = &Block_defaultVtable;
                                                      Block_constructWithTexture(_b, 48, 36,
                                                                                 Material_rock);
                                                      _b;
                                                  }),
                                                  2.0f),
                                10.0f),
            Block_soundStoneFootstep),
        "stoneMoss");
    Block_obsidian = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockObsidian_create(49, 37), 10.0f), 2000.0f),
            Block_soundStoneFootstep),
        "obsidian");
    Block_torchWood = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(
            Block_setLightValue(Block_setHardness(BlockTorch_create(50, 80), 0.0f), 15.0f / 16.0f),
            Block_soundWoodFootstep),
        "torch"));
    Block_fire = Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
        Block_setStepSound(
            Block_setLightValue(Block_setHardness(BlockFire_create(51, 31), 0.0f), 1.0f),
            Block_soundWoodFootstep),
        "fire")));
    Block_mobSpawner = Block_disableStats(Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockMobSpawner_create(52, 65), 5.0f),
                           Block_soundMetalFootstep),
        "mobSpawner"));
    Block_stairCompactPlanks = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(BlockStairs_create(53, Block_planks), "stairsWood"));
    Block_chest = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockChest_create(54), 2.5f), Block_soundWoodFootstep),
        "chest"));
    Block_redstoneWire =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setStepSound(Block_setHardness(BlockRedstoneWire_create(55, 164), 0.0f),
                               Block_soundPowderFootstep),
            "redstoneDust")));
    Block_oreDiamond = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockOre_create(56, 50), 3.0f), 5.0f),
            Block_soundStoneFootstep),
        "oreDiamond");
    Block_blockDiamond = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockOreStorage_create(57, 24), 5.0f), 10.0f),
            Block_soundMetalFootstep),
        "blockDiamond");
    Block_workbench =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockWorkbench_create(58), 2.5f),
                                              Block_soundWoodFootstep),
                           "workbench");
    Block_crops = Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockCrops_create(59, 88), 0.0f),
                                              Block_soundGrassFootstep),
                           "crops")));
    Block_tilledField =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockFarmland_create(60), 0.6f),
                                              Block_soundGravelFootstep),
                           "farmland");
    Block_stoneOvenIdle = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockFurnace_create(61, 0), 3.5f),
                                              Block_soundStoneFootstep),
                           "furnace"));
    Block_stoneOvenActive = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setLightValue(Block_setStepSound(Block_setHardness(BlockFurnace_create(62, 1), 3.5f),
                                               Block_soundStoneFootstep),
                            14.0f / 16.0f),
        "furnace"));
    Block_signPost = Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockSign_create(63, 1), 1.0f),
                                              Block_soundWoodFootstep),
                           "sign")));
    Block_doorWood =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setStepSound(Block_setHardness(BlockDoor_create(64, Material_wood), 3.0f),
                               Block_soundWoodFootstep),
            "doorWood")));
    Block_ladder = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockLadder_create(65, 83), 0.4f),
                                              Block_soundWoodFootstep),
                           "ladder"));
    Block_rail = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockRail_create(66, 128, 0), 0.7f),
                                              Block_soundMetalFootstep),
                           "rail"));
    Block_stairCompactCobblestone = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(BlockStairs_create(67, Block_cobblestone), "stairsStone"));
    Block_signWall = Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockSign_create(68, 0), 1.0f),
                                              Block_soundWoodFootstep),
                           "sign")));
    Block_lever = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockLever_create(69, 96), 0.5f),
                                              Block_soundWoodFootstep),
                           "lever"));
    Block_pressurePlateStone = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(
            Block_setHardness(BlockPressurePlate_create(70, 1, 0, Material_rock), 0.5f),
            Block_soundStoneFootstep),
        "pressurePlate"));
    Block_doorSteel =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setStepSound(Block_setHardness(BlockDoor_create(71, Material_iron), 5.0f),
                               Block_soundMetalFootstep),
            "doorIron")));
    Block_pressurePlatePlanks = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(
            Block_setHardness(BlockPressurePlate_create(72, 4, 1, Material_wood), 0.5f),
            Block_soundWoodFootstep),
        "pressurePlate"));
    Block_oreRedstone = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockRedstoneOre_create(73, 51, 0), 3.0f), 5.0f),
            Block_soundStoneFootstep),
        "oreRedstone"));
    Block_oreRedstoneGlowing = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(
                Block_setLightValue(Block_setHardness(BlockRedstoneOre_create(74, 51, 1), 3.0f),
                                    10.0f / 16.0f),
                5.0f),
            Block_soundStoneFootstep),
        "oreRedstone"));
    Block_torchRedstoneIdle = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockRedstoneTorch_create(75, 115, 0), 0.0f),
                           Block_soundWoodFootstep),
        "notGate"));
    Block_torchRedstoneActive = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setLightValue(
            Block_setStepSound(Block_setHardness(BlockRedstoneTorch_create(76, 99, 1), 0.0f),
                               Block_soundWoodFootstep),
            0.5f),
        "notGate"));
    Block_button = Block_disableNeighborNotifyOnMetadataChange(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockButton_create(77, 1), 0.5f),
                                              Block_soundStoneFootstep),
                           "button"));
    Block_snow =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockSnow_create(78, 66), 0.1f),
                                              Block_soundClothFootstep),
                           "snow");
    Block_ice = Block_setBlockName(
        Block_setStepSound(
            Block_setLightOpacity(Block_setHardness(BlockIce_create(79, 67), 0.5f), 3),
            Block_soundGlassFootstep),
        "ice");
    Block_blockSnow = Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockSnowBlock_create(80, 66), 0.2f),
                           Block_soundClothFootstep),
        "snow");
    Block_cactus =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockCactus_create(81, 70), 0.4f),
                                              Block_soundClothFootstep),
                           "cactus");
    Block_blockClay =
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockClay_create(82, 72), 0.6f),
                                              Block_soundGravelFootstep),
                           "clay");
    Block_reed = Block_disableStats(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockReed_create(83, 73), 0.0f),
                                              Block_soundGrassFootstep),
                           "reeds"));
    Block_jukebox = Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockJukeBox_create(84, 74), 2.0f), 10.0f),
            Block_soundStoneFootstep),
        "jukebox");
    Block_fence = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(
            Block_setResistance(Block_setHardness(BlockFence_create(85, 4), 2.0f), 5.0f),
            Block_soundWoodFootstep),
        "fence"));
    Block_pumpkin = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockPumpkin_create(86, 102, 0), 1.0f),
                           Block_soundWoodFootstep),
        "pumpkin"));
    Block_netherrack = Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockNetherrack_create(87, 103), 0.4f),
                           Block_soundStoneFootstep),
        "hellrock");
    Block_slowSand = Block_setBlockName(
        Block_setStepSound(Block_setHardness(BlockSoulSand_create(88, 104), 0.5f),
                           Block_soundSandFootstep),
        "hellsand");
    Block_glowStone = Block_setBlockName(
        Block_setLightValue(
            Block_setStepSound(
                Block_setHardness(BlockGlowStone_create(89, 105, Material_rock), 0.3f),
                Block_soundGlassFootstep),
            1.0f),
        "lightgem");
    Block_portal = Block_setBlockName(
        Block_setLightValue(Block_setStepSound(Block_setHardness(BlockPortal_create(90, 14), -1.0f),
                                               Block_soundGlassFootstep),
                            12.0f / 16.0f),
        "portal");
    Block_pumpkinLantern = Block_disableNeighborNotifyOnMetadataChange(Block_setBlockName(
        Block_setLightValue(
            Block_setStepSound(Block_setHardness(BlockPumpkin_create(91, 102, 1), 1.0f),
                               Block_soundWoodFootstep),
            1.0f),
        "litpumpkin"));
    Block_cake = Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(
        Block_setBlockName(Block_setStepSound(Block_setHardness(BlockCake_create(92, 121), 0.5f),
                                              Block_soundClothFootstep),
                           "cake")));
    Block_redstoneRepeaterIdle =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setStepSound(Block_setHardness(BlockRedstoneRepeater_create(93, 0), 0.0f),
                               Block_soundWoodFootstep),
            "diode")));
    Block_redstoneRepeaterActive =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setLightValue(
                Block_setStepSound(Block_setHardness(BlockRedstoneRepeater_create(94, 1), 0.0f),
                                   Block_soundWoodFootstep),
                10.0f / 16.0f),
            "diode")));
    Block_lockedChest = Block_disableNeighborNotifyOnMetadataChange(Block_setTickOnLoad(
        Block_setBlockName(
            Block_setLightValue(
                Block_setStepSound(Block_setHardness(BlockLockedChest_create(95), 0.0f),
                                   Block_soundWoodFootstep),
                1.0f),
            "lockedchest"),
        1));
    Block_trapdoor =
        Block_disableNeighborNotifyOnMetadataChange(Block_disableStats(Block_setBlockName(
            Block_setStepSound(Block_setHardness(BlockTrapDoor_create(96, Material_wood), 3.0f),
                               Block_soundWoodFootstep),
            "trapdoor")));

    Item_setItemName(ItemCloth_create(Block_cloth->blockID), "cloth");
    Item_setItemName(ItemLog_create(Block_wood->blockID), "log");
    Item_setItemName(ItemSlab_create(Block_stairSingle->blockID), "stoneSlab");
    Item_setItemName(ItemSapling_create(Block_sapling->blockID), "sapling");
    Item_setItemName(ItemLeaves_create(Block_leaves->blockID), "leaves");
    ItemPiston_create(Block_pistonBase->blockID);
    ItemPiston_create(Block_pistonStickyBase->blockID);

    for (i = 0; i < 256; i++) {
        if (Block_blocksList[i] != NULL && Item_itemsList[i] == NULL) {
            Item_registerBlockItem(i);
            Block_blocksList[i]->vtable->initializeBlock(Block_blocksList[i]);
        }
    }
    Block_canBlockGrass[0] = 1;
    StatList_func_25154_a();
}
