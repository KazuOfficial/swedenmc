#ifndef RENDER_BLOCKS_H
#define RENDER_BLOCKS_H

#include "IBlockAccess.h"
#include "Block.h"
#include "Tessellator.h"
#include "Minecraft.h"

typedef struct RenderBlocks {
    IBlockAccess *blockAccess;
    int overrideBlockTexture;
    int flipTexture;
    int renderAllFaces;
    int field_31088_b;

    int field_31087_g;
    int field_31086_h;
    int field_31085_i;
    int field_31084_j;
    int field_31083_k;
    int field_31082_l;

    int enableAO;
    float lightValueOwn;
    float aoLightValueXNeg;
    float aoLightValueYNeg;
    float aoLightValueZNeg;
    float aoLightValueXPos;
    float aoLightValueYPos;
    float aoLightValueZPos;

    float field_22377_m, field_22376_n, field_22375_o, field_22374_p;
    float field_22373_q, field_22372_r, field_22371_s, field_22370_t;
    float field_22369_u, field_22368_v, field_22367_w, field_22366_x;
    float field_22365_y, field_22364_z, field_22362_A, field_22360_B;
    float field_22358_C, field_22356_D, field_22354_E, field_22353_F;

    int field_22352_G;

    float colorRedTopLeft, colorRedBottomLeft, colorRedBottomRight, colorRedTopRight;
    float colorGreenTopLeft, colorGreenBottomLeft, colorGreenBottomRight, colorGreenTopRight;
    float colorBlueTopLeft, colorBlueBottomLeft, colorBlueBottomRight, colorBlueTopRight;

    int field_22339_T, field_22338_U, field_22337_V, field_22336_W;
    int field_22335_X, field_22334_Y, field_22333_Z;
    int field_22363_aa, field_22361_ab, field_22359_ac, field_22357_ad, field_22355_ae;

} RenderBlocks;

extern int RenderBlocks_fancyGrass;

void RenderBlocks_init(RenderBlocks *self, IBlockAccess *blockAccess);

int RenderBlocks_renderBlockByRenderType(RenderBlocks *self, Block *block, int x, int y, int z);
void RenderBlocks_renderBlockUsingTexture(RenderBlocks *self, Block *block, int x, int y, int z,
                                          int tex);
void RenderBlocks_func_31075_a(RenderBlocks *self, Block *block, int x, int y, int z);

int RenderBlocks_renderStandardBlock(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderStandardBlockWithColorMultiplier(RenderBlocks *self, Block *block, int x,
                                                        int y, int z, float r, float g, float b);
int RenderBlocks_renderStandardBlockWithAmbientOcclusion(RenderBlocks *self, Block *block, int x,
                                                         int y, int z, float r, float g, float b);

int RenderBlocks_renderBlockFluids(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderBlockCactus(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderBlockReed(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderBlockCrops(RenderBlocks *self, Block *block, int x, int y, int z);

void RenderBlocks_renderBlockFallingSand(RenderBlocks *self, Block *var1, struct World *var2,
                                         int var3, int var4, int var5);
int RenderBlocks_renderBlockTorch(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderBlockFire(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderBlockRedstoneWire(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderBlockLadder(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderBlockDoor(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderBlockStairs(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderBlockFence(RenderBlocks *self, Block *block, int x, int y, int z);
int RenderBlocks_renderBlockLever(RenderBlocks *self, Block *block, int x, int y, int z);

void RenderBlocks_func_31078_d(RenderBlocks *self, Block *block, int x, int y, int z);
void RenderBlocks_func_31079_a(RenderBlocks *self, Block *block, int x, int y, int z, int var5);

void RenderBlocks_renderBottomFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                   int tex);
void RenderBlocks_renderTopFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                int tex);
void RenderBlocks_renderEastFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                 int tex);
void RenderBlocks_renderWestFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                 int tex);
void RenderBlocks_renderNorthFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                  int tex);
void RenderBlocks_renderSouthFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                  int tex);

void RenderBlocks_renderTorchAtAngle(RenderBlocks *self, Block *block, double x, double y, double z,
                                     double ox, double oz);
void RenderBlocks_renderCrossedSquares(RenderBlocks *self, Block *block, int meta, double x,
                                       double y, double z);
void RenderBlocks_func_1245_b(RenderBlocks *self, Block *block, int meta, double x, double y,
                              double z);

void RenderBlocks_renderBlockOnInventory(RenderBlocks *self, Block *block, int meta,
                                         float brightness);

static inline int RenderBlocks_renderItemIn3d(int renderType) {
    return renderType == 0 || renderType == 13 || renderType == 10 || renderType == 11 ||
           renderType == 16;
}

#endif
