#include "Renderer.h"
#include "RenderBlocks.h"
#include <stdint.h>
#include "Tessellator.h"
#include "Block.h"
#include "EntityRenderer.h"
#include "BlockFire.h"
#include "BlockFluid.h"
#include "BlockDoor.h"
#include "Vec3D.h"
#include "MathHelper.h"
#include <string.h>
#include <math.h>

extern int BlockRedstoneWire_isPowerProviderOrWire(IBlockAccess *ba, int x, int y, int z, int side);

int RenderBlocks_fancyGrass = 1;

void RenderBlocks_init(RenderBlocks *self, IBlockAccess *blockAccess) {
    memset(self, 0, sizeof(RenderBlocks));
    self->blockAccess = blockAccess;
    self->overrideBlockTexture = -1;
    self->field_31088_b = 1;
    self->field_22352_G = 1;
}

void RenderBlocks_renderBottomFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                   int tex) {
    Tessellator *t = &Tessellator_instance;
    if (self->overrideBlockTexture >= 0)
        tex = self->overrideBlockTexture;

    int var10 = (tex & 15) << 4;
    int var11 = tex & 240;

    double u0 = ((double)var10 + block->minX * 16.0) / 256.0;
    double u1 = ((double)var10 + block->maxX * 16.0 - 0.01) / 256.0;
    double v0 = ((double)var11 + block->minZ * 16.0) / 256.0;
    double v1 = ((double)var11 + block->maxZ * 16.0 - 0.01) / 256.0;
    if (block->minX < 0.0 || block->maxX > 1.0) {
        u0 = (float)var10 / 256.0f;
        u1 = ((float)var10 + 15.99f) / 256.0f;
    }
    if (block->minZ < 0.0 || block->maxZ > 1.0) {
        v0 = (float)var11 / 256.0f;
        v1 = ((float)var11 + 15.99f) / 256.0f;
    }

    double var12 = u0, var14 = u1, var16 = v0, var18 = v1;
    double var20 = u1, var22 = u0, var24 = v0, var26 = v1;
    if (self->field_31082_l == 2) {
        var12 = ((double)var10 + block->minZ * 16.0) / 256.0;
        var16 = ((double)(var11 + 16) - block->maxX * 16.0) / 256.0;
        var14 = ((double)var10 + block->maxZ * 16.0) / 256.0;
        var18 = ((double)(var11 + 16) - block->minX * 16.0) / 256.0;
        var24 = var16;
        var26 = var18;
        var20 = var12;
        var22 = var14;
        var16 = var18;
        var18 = var24;
    } else if (self->field_31082_l == 1) {
        var12 = ((double)(var10 + 16) - block->maxZ * 16.0) / 256.0;
        var16 = ((double)var11 + block->minX * 16.0) / 256.0;
        var14 = ((double)(var10 + 16) - block->minZ * 16.0) / 256.0;
        var18 = ((double)var11 + block->maxX * 16.0) / 256.0;
        var20 = var14;
        var22 = var12;
        var12 = var14;
        var14 = var22;
        var24 = var18;
        var26 = var16;
    } else if (self->field_31082_l == 3) {
        var12 = ((double)(var10 + 16) - block->minX * 16.0) / 256.0;
        var14 = ((double)(var10 + 16) - block->maxX * 16.0 - 0.01) / 256.0;
        var16 = ((double)(var11 + 16) - block->minZ * 16.0) / 256.0;
        var18 = ((double)(var11 + 16) - block->maxZ * 16.0 - 0.01) / 256.0;
        var20 = var14;
        var22 = var12;
        var24 = var16;
        var26 = var18;
    }

    double xMin = x + block->minX, xMax = x + block->maxX;
    double yMin = y + block->minY;
    double zMin = z + block->minZ, zMax = z + block->maxZ;

    if (self->enableAO) {
        Tessellator_setColorOpaque_F(t, self->colorRedTopLeft, self->colorGreenTopLeft,
                                     self->colorBlueTopLeft);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMax, var22, var26);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomLeft, self->colorGreenBottomLeft,
                                     self->colorBlueBottomLeft);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMin, var12, var16);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomRight, self->colorGreenBottomRight,
                                     self->colorBlueBottomRight);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMin, var20, var24);
        Tessellator_setColorOpaque_F(t, self->colorRedTopRight, self->colorGreenTopRight,
                                     self->colorBlueTopRight);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMax, var14, var18);
    } else {
        Tessellator_addVertexWithUV(t, xMin, yMin, zMax, var22, var26);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMin, var12, var16);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMin, var20, var24);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMax, var14, var18);
    }
}

void RenderBlocks_renderTopFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                int tex) {
    Tessellator *t = &Tessellator_instance;
    if (self->overrideBlockTexture >= 0)
        tex = self->overrideBlockTexture;

    int var10 = (tex & 15) << 4;
    int var11 = tex & 240;

    double u0 = ((double)var10 + block->minX * 16.0) / 256.0;
    double u1 = ((double)var10 + block->maxX * 16.0 - 0.01) / 256.0;
    double v0 = ((double)var11 + block->minZ * 16.0) / 256.0;
    double v1 = ((double)var11 + block->maxZ * 16.0 - 0.01) / 256.0;
    if (block->minX < 0.0 || block->maxX > 1.0) {
        u0 = (float)var10 / 256.0f;
        u1 = ((float)var10 + 15.99f) / 256.0f;
    }
    if (block->minZ < 0.0 || block->maxZ > 1.0) {
        v0 = (float)var11 / 256.0f;
        v1 = ((float)var11 + 15.99f) / 256.0f;
    }

    double var20 = u1, var22 = u0, var24 = v0, var26 = v1;

    double xMin = x + block->minX, xMax = x + block->maxX;
    double yMax = y + block->maxY;
    double zMin = z + block->minZ, zMax = z + block->maxZ;

    if (self->enableAO) {
        Tessellator_setColorOpaque_F(t, self->colorRedTopLeft, self->colorGreenTopLeft,
                                     self->colorBlueTopLeft);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMax, u1, v1);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomLeft, self->colorGreenBottomLeft,
                                     self->colorBlueBottomLeft);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMin, var20, var24);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomRight, self->colorGreenBottomRight,
                                     self->colorBlueBottomRight);
        Tessellator_addVertexWithUV(t, xMin, yMax, zMin, u0, v0);
        Tessellator_setColorOpaque_F(t, self->colorRedTopRight, self->colorGreenTopRight,
                                     self->colorBlueTopRight);
        Tessellator_addVertexWithUV(t, xMin, yMax, zMax, var22, var26);
    } else {
        Tessellator_addVertexWithUV(t, xMax, yMax, zMax, u1, v1);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMin, var20, var24);
        Tessellator_addVertexWithUV(t, xMin, yMax, zMin, u0, v0);
        Tessellator_addVertexWithUV(t, xMin, yMax, zMax, var22, var26);
    }
}

void RenderBlocks_renderEastFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                 int tex) {
    Tessellator *t = &Tessellator_instance;
    if (self->overrideBlockTexture >= 0)
        tex = self->overrideBlockTexture;

    int var10 = (tex & 15) << 4;
    int var11 = tex & 240;

    double u0 = ((double)var10 + block->minX * 16.0) / 256.0;
    double u1 = ((double)var10 + block->maxX * 16.0 - 0.01) / 256.0;
    double v0 = ((double)(var11 + 16) - block->maxY * 16.0) / 256.0;
    double v1 = ((double)(var11 + 16) - block->minY * 16.0 - 0.01) / 256.0;

    if (self->flipTexture) {
        double tmp = u0;
        u0 = u1;
        u1 = tmp;
    }
    if (block->minX < 0.0 || block->maxX > 1.0) {
        u0 = (float)var10 / 256.0f;
        u1 = ((float)var10 + 15.99f) / 256.0f;
    }
    if (block->minY < 0.0 || block->maxY > 1.0) {
        v0 = (float)var11 / 256.0f;
        v1 = ((float)var11 + 15.99f) / 256.0f;
    }

    double var20 = u1, var22 = u0, var24 = v0, var26 = v1;

    double xMin = x + block->minX, xMax = x + block->maxX;
    double yMin = y + block->minY, yMax = y + block->maxY;
    double zMin = z + block->minZ;

    if (self->enableAO) {
        Tessellator_setColorOpaque_F(t, self->colorRedTopLeft, self->colorGreenTopLeft,
                                     self->colorBlueTopLeft);
        Tessellator_addVertexWithUV(t, xMin, yMax, zMin, var20, var24);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomLeft, self->colorGreenBottomLeft,
                                     self->colorBlueBottomLeft);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMin, u0, v0);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomRight, self->colorGreenBottomRight,
                                     self->colorBlueBottomRight);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMin, var22, var26);
        Tessellator_setColorOpaque_F(t, self->colorRedTopRight, self->colorGreenTopRight,
                                     self->colorBlueTopRight);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMin, u1, v1);
    } else {
        Tessellator_addVertexWithUV(t, xMin, yMax, zMin, var20, var24);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMin, u0, v0);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMin, var22, var26);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMin, u1, v1);
    }
}

void RenderBlocks_renderWestFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                 int tex) {
    Tessellator *t = &Tessellator_instance;
    if (self->overrideBlockTexture >= 0)
        tex = self->overrideBlockTexture;

    int var10 = (tex & 15) << 4;
    int var11 = tex & 240;

    double u0 = ((double)var10 + block->minX * 16.0) / 256.0;
    double u1 = ((double)var10 + block->maxX * 16.0 - 0.01) / 256.0;
    double v0 = ((double)(var11 + 16) - block->maxY * 16.0) / 256.0;
    double v1 = ((double)(var11 + 16) - block->minY * 16.0 - 0.01) / 256.0;

    if (self->flipTexture) {
        double tmp = u0;
        u0 = u1;
        u1 = tmp;
    }
    if (block->minX < 0.0 || block->maxX > 1.0) {
        u0 = (float)var10 / 256.0f;
        u1 = ((float)var10 + 15.99f) / 256.0f;
    }
    if (block->minY < 0.0 || block->maxY > 1.0) {
        v0 = (float)var11 / 256.0f;
        v1 = ((float)var11 + 15.99f) / 256.0f;
    }

    double var20 = u1, var22 = u0, var24 = v0, var26 = v1;

    double xMin = x + block->minX, xMax = x + block->maxX;
    double yMin = y + block->minY, yMax = y + block->maxY;
    double zMax = z + block->maxZ;

    if (self->enableAO) {
        Tessellator_setColorOpaque_F(t, self->colorRedTopLeft, self->colorGreenTopLeft,
                                     self->colorBlueTopLeft);
        Tessellator_addVertexWithUV(t, xMin, yMax, zMax, u0, v0);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomLeft, self->colorGreenBottomLeft,
                                     self->colorBlueBottomLeft);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMax, var22, var26);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomRight, self->colorGreenBottomRight,
                                     self->colorBlueBottomRight);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMax, u1, v1);
        Tessellator_setColorOpaque_F(t, self->colorRedTopRight, self->colorGreenTopRight,
                                     self->colorBlueTopRight);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMax, var20, var24);
    } else {
        Tessellator_addVertexWithUV(t, xMin, yMax, zMax, u0, v0);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMax, var22, var26);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMax, u1, v1);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMax, var20, var24);
    }
}

void RenderBlocks_renderNorthFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                  int tex) {
    Tessellator *t = &Tessellator_instance;
    if (self->overrideBlockTexture >= 0)
        tex = self->overrideBlockTexture;

    int var10 = (tex & 15) << 4;
    int var11 = tex & 240;

    double u0 = ((double)var10 + block->minZ * 16.0) / 256.0;
    double u1 = ((double)var10 + block->maxZ * 16.0 - 0.01) / 256.0;
    double v0 = ((double)(var11 + 16) - block->maxY * 16.0) / 256.0;
    double v1 = ((double)(var11 + 16) - block->minY * 16.0 - 0.01) / 256.0;

    if (self->flipTexture) {
        double tmp = u0;
        u0 = u1;
        u1 = tmp;
    }
    if (block->minZ < 0.0 || block->maxZ > 1.0) {
        u0 = (float)var10 / 256.0f;
        u1 = ((float)var10 + 15.99f) / 256.0f;
    }
    if (block->minY < 0.0 || block->maxY > 1.0) {
        v0 = (float)var11 / 256.0f;
        v1 = ((float)var11 + 15.99f) / 256.0f;
    }

    double var20 = u1, var22 = u0, var24 = v0, var26 = v1;

    double xMin = x + block->minX;
    double yMin = y + block->minY, yMax = y + block->maxY;
    double zMin = z + block->minZ, zMax = z + block->maxZ;

    if (self->enableAO) {
        Tessellator_setColorOpaque_F(t, self->colorRedTopLeft, self->colorGreenTopLeft,
                                     self->colorBlueTopLeft);
        Tessellator_addVertexWithUV(t, xMin, yMax, zMax, var20, var24);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomLeft, self->colorGreenBottomLeft,
                                     self->colorBlueBottomLeft);
        Tessellator_addVertexWithUV(t, xMin, yMax, zMin, u0, v0);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomRight, self->colorGreenBottomRight,
                                     self->colorBlueBottomRight);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMin, var22, var26);
        Tessellator_setColorOpaque_F(t, self->colorRedTopRight, self->colorGreenTopRight,
                                     self->colorBlueTopRight);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMax, u1, v1);
    } else {
        Tessellator_addVertexWithUV(t, xMin, yMax, zMax, var20, var24);
        Tessellator_addVertexWithUV(t, xMin, yMax, zMin, u0, v0);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMin, var22, var26);
        Tessellator_addVertexWithUV(t, xMin, yMin, zMax, u1, v1);
    }
}

void RenderBlocks_renderSouthFace(RenderBlocks *self, Block *block, double x, double y, double z,
                                  int tex) {
    Tessellator *t = &Tessellator_instance;
    if (self->overrideBlockTexture >= 0)
        tex = self->overrideBlockTexture;

    int var10 = (tex & 15) << 4;
    int var11 = tex & 240;

    double u0 = ((double)var10 + block->minZ * 16.0) / 256.0;
    double u1 = ((double)var10 + block->maxZ * 16.0 - 0.01) / 256.0;
    double v0 = ((double)(var11 + 16) - block->maxY * 16.0) / 256.0;
    double v1 = ((double)(var11 + 16) - block->minY * 16.0 - 0.01) / 256.0;

    if (self->flipTexture) {
        double tmp = u0;
        u0 = u1;
        u1 = tmp;
    }
    if (block->minZ < 0.0 || block->maxZ > 1.0) {
        u0 = (float)var10 / 256.0f;
        u1 = ((float)var10 + 15.99f) / 256.0f;
    }
    if (block->minY < 0.0 || block->maxY > 1.0) {
        v0 = (float)var11 / 256.0f;
        v1 = ((float)var11 + 15.99f) / 256.0f;
    }

    double var20 = u1, var22 = u0, var24 = v0, var26 = v1;

    double xMax = x + block->maxX;
    double yMin = y + block->minY, yMax = y + block->maxY;
    double zMin = z + block->minZ, zMax = z + block->maxZ;

    if (self->enableAO) {
        Tessellator_setColorOpaque_F(t, self->colorRedTopLeft, self->colorGreenTopLeft,
                                     self->colorBlueTopLeft);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMax, var22, var26);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomLeft, self->colorGreenBottomLeft,
                                     self->colorBlueBottomLeft);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMin, u1, v1);
        Tessellator_setColorOpaque_F(t, self->colorRedBottomRight, self->colorGreenBottomRight,
                                     self->colorBlueBottomRight);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMin, var20, var24);
        Tessellator_setColorOpaque_F(t, self->colorRedTopRight, self->colorGreenTopRight,
                                     self->colorBlueTopRight);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMax, u0, v0);
    } else {
        Tessellator_addVertexWithUV(t, xMax, yMin, zMax, var22, var26);
        Tessellator_addVertexWithUV(t, xMax, yMin, zMin, u1, v1);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMin, var20, var24);
        Tessellator_addVertexWithUV(t, xMax, yMax, zMax, u0, v0);
    }
}

int RenderBlocks_renderStandardBlockWithColorMultiplier(RenderBlocks *self, Block *block, int x,
                                                        int y, int z, float var5, float var6,
                                                        float var7) {
    Tessellator *var8 = &Tessellator_instance;
    self->enableAO = 0;
    int var9 = 0;
    float var10 = 0.5f, var11 = 1.0f, var12 = 0.8f, var13 = 0.6f;
    float var14 = var11 * var5, var15 = var11 * var6, var16 = var11 * var7;
    float var17 = var10, var18 = var12, var19 = var13;
    float var20 = var10, var21 = var12, var22 = var13;
    float var23 = var10, var24 = var12, var25 = var13;
    if (block != Block_grass) {
        var17 = var10 * var5;
        var18 = var12 * var5;
        var19 = var13 * var5;
        var20 = var10 * var6;
        var21 = var12 * var6;
        var22 = var13 * var6;
        var23 = var10 * var7;
        var24 = var12 * var7;
        var25 = var13 * var7;
    }
    float var26 = block->vtable->getBlockBrightness(block, self->blockAccess, x, y, z);
    float var27;
    int var28;
    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x, y - 1, z, 0)) {
        var27 = block->vtable->getBlockBrightness(block, self->blockAccess, x, y - 1, z);
        Tessellator_setColorOpaque_F(var8, var17 * var27, var20 * var27, var23 * var27);
        RenderBlocks_renderBottomFace(
            self, block, (double)x, (double)y, (double)z,
            block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 0));
        var9 = 1;
    }
    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x, y + 1, z, 1)) {
        var27 = block->vtable->getBlockBrightness(block, self->blockAccess, x, y + 1, z);
        if (block->maxY != 1.0 && !Material_getIsLiquid(block->blockMaterial))
            var27 = var26;
        Tessellator_setColorOpaque_F(var8, var14 * var27, var15 * var27, var16 * var27);
        RenderBlocks_renderTopFace(
            self, block, (double)x, (double)y, (double)z,
            block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 1));
        var9 = 1;
    }
    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x, y, z - 1, 2)) {
        var27 = block->vtable->getBlockBrightness(block, self->blockAccess, x, y, z - 1);
        if (block->minZ > 0.0)
            var27 = var26;
        Tessellator_setColorOpaque_F(var8, var18 * var27, var21 * var27, var24 * var27);
        var28 = block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 2);
        RenderBlocks_renderEastFace(self, block, (double)x, (double)y, (double)z, var28);
        if (RenderBlocks_fancyGrass && var28 == 3 && self->overrideBlockTexture < 0) {
            Tessellator_setColorOpaque_F(var8, var18 * var27 * var5, var21 * var27 * var6,
                                         var24 * var27 * var7);
            RenderBlocks_renderEastFace(self, block, (double)x, (double)y, (double)z, 38);
        }
        var9 = 1;
    }
    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x, y, z + 1, 3)) {
        var27 = block->vtable->getBlockBrightness(block, self->blockAccess, x, y, z + 1);
        if (block->maxZ < 1.0)
            var27 = var26;
        Tessellator_setColorOpaque_F(var8, var18 * var27, var21 * var27, var24 * var27);
        var28 = block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 3);
        RenderBlocks_renderWestFace(self, block, (double)x, (double)y, (double)z, var28);
        if (RenderBlocks_fancyGrass && var28 == 3 && self->overrideBlockTexture < 0) {
            Tessellator_setColorOpaque_F(var8, var18 * var27 * var5, var21 * var27 * var6,
                                         var24 * var27 * var7);
            RenderBlocks_renderWestFace(self, block, (double)x, (double)y, (double)z, 38);
        }
        var9 = 1;
    }
    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x - 1, y, z, 4)) {
        var27 = block->vtable->getBlockBrightness(block, self->blockAccess, x - 1, y, z);
        if (block->minX > 0.0)
            var27 = var26;
        Tessellator_setColorOpaque_F(var8, var19 * var27, var22 * var27, var25 * var27);
        var28 = block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 4);
        RenderBlocks_renderNorthFace(self, block, (double)x, (double)y, (double)z, var28);
        if (RenderBlocks_fancyGrass && var28 == 3 && self->overrideBlockTexture < 0) {
            Tessellator_setColorOpaque_F(var8, var19 * var27 * var5, var22 * var27 * var6,
                                         var25 * var27 * var7);
            RenderBlocks_renderNorthFace(self, block, (double)x, (double)y, (double)z, 38);
        }
        var9 = 1;
    }
    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x + 1, y, z, 5)) {
        var27 = block->vtable->getBlockBrightness(block, self->blockAccess, x + 1, y, z);
        if (block->maxX < 1.0)
            var27 = var26;
        Tessellator_setColorOpaque_F(var8, var19 * var27, var22 * var27, var25 * var27);
        var28 = block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 5);
        RenderBlocks_renderSouthFace(self, block, (double)x, (double)y, (double)z, var28);
        if (RenderBlocks_fancyGrass && var28 == 3 && self->overrideBlockTexture < 0) {
            Tessellator_setColorOpaque_F(var8, var19 * var27 * var5, var22 * var27 * var6,
                                         var25 * var27 * var7);
            RenderBlocks_renderSouthFace(self, block, (double)x, (double)y, (double)z, 38);
        }
        var9 = 1;
    }
    return var9;
}

#define AO_SET_COLOR(scale)                                                                        \
    self->colorRedTopLeft = self->colorRedBottomLeft = self->colorRedBottomRight =                 \
        self->colorRedTopRight = cr * (scale);                                                     \
    self->colorGreenTopLeft = self->colorGreenBottomLeft = self->colorGreenBottomRight =           \
        self->colorGreenTopRight = cg * (scale);                                                   \
    self->colorBlueTopLeft = self->colorBlueBottomLeft = self->colorBlueBottomRight =              \
        self->colorBlueTopRight = cb * (scale);
#define AO_APPLY_VERTEX(tl, bl, br, tr)                                                            \
    self->colorRedTopLeft *= (tl);                                                                 \
    self->colorGreenTopLeft *= (tl);                                                               \
    self->colorBlueTopLeft *= (tl);                                                                \
    self->colorRedBottomLeft *= (bl);                                                              \
    self->colorGreenBottomLeft *= (bl);                                                            \
    self->colorBlueBottomLeft *= (bl);                                                             \
    self->colorRedBottomRight *= (br);                                                             \
    self->colorGreenBottomRight *= (br);                                                           \
    self->colorBlueBottomRight *= (br);                                                            \
    self->colorRedTopRight *= (tr);                                                                \
    self->colorGreenTopRight *= (tr);                                                              \
    self->colorBlueTopRight *= (tr);
#define AO_GRASS_OVERLAY(face_fn, tex_id)                                                          \
    if (RenderBlocks_fancyGrass && (tex_id) == 3 && self->overrideBlockTexture < 0) {              \
        self->colorRedTopLeft *= r;                                                                \
        self->colorRedBottomLeft *= r;                                                             \
        self->colorRedBottomRight *= r;                                                            \
        self->colorRedTopRight *= r;                                                               \
        self->colorGreenTopLeft *= g;                                                              \
        self->colorGreenBottomLeft *= g;                                                           \
        self->colorGreenBottomRight *= g;                                                          \
        self->colorGreenTopRight *= g;                                                             \
        self->colorBlueTopLeft *= b;                                                               \
        self->colorBlueBottomLeft *= b;                                                            \
        self->colorBlueBottomRight *= b;                                                           \
        self->colorBlueTopRight *= b;                                                              \
        face_fn;                                                                                   \
    }
#define BLK_ID(bx, by, bz)                                                                         \
    self->blockAccess->vtable->getBlockId(self->blockAccess, (bx), (by), (bz))
#define CAN_GRASS(bx, by, bz) Block_canBlockGrass[BLK_ID(bx, by, bz)]
#define BBR(bx, by, bz)                                                                            \
    block->vtable->getBlockBrightness(block, self->blockAccess, (bx), (by), (bz))

int RenderBlocks_renderStandardBlockWithAmbientOcclusion(RenderBlocks *self, Block *block, int x,
                                                         int y, int z, float r, float g, float b) {
    self->enableAO = 1;
    int var8 = 0;
    int var13 = 1, var14 = 1, var15 = 1, var16 = 1, var17 = 1, var18 = 1;
    self->lightValueOwn = BBR(x, y, z);
    self->aoLightValueXNeg = BBR(x - 1, y, z);
    self->aoLightValueYNeg = BBR(x, y - 1, z);
    self->aoLightValueZNeg = BBR(x, y, z - 1);
    self->aoLightValueXPos = BBR(x + 1, y, z);
    self->aoLightValueYPos = BBR(x, y + 1, z);
    self->aoLightValueZPos = BBR(x, y, z + 1);
    self->field_22338_U = CAN_GRASS(x + 1, y + 1, z);
    self->field_22359_ac = CAN_GRASS(x + 1, y - 1, z);
    self->field_22334_Y = CAN_GRASS(x + 1, y, z + 1);
    self->field_22363_aa = CAN_GRASS(x + 1, y, z - 1);
    self->field_22337_V = CAN_GRASS(x - 1, y + 1, z);
    self->field_22357_ad = CAN_GRASS(x - 1, y - 1, z);
    self->field_22335_X = CAN_GRASS(x - 1, y, z - 1);
    self->field_22333_Z = CAN_GRASS(x - 1, y, z + 1);
    self->field_22336_W = CAN_GRASS(x, y + 1, z + 1);
    self->field_22339_T = CAN_GRASS(x, y + 1, z - 1);
    self->field_22355_ae = CAN_GRASS(x, y - 1, z + 1);
    self->field_22361_ab = CAN_GRASS(x, y - 1, z - 1);
    if (block->blockIndexInTexture == 3) {
        var18 = 0;
        var17 = 0;
        var16 = 0;
        var15 = 0;
        var13 = 0;
    }
    if (self->overrideBlockTexture >= 0) {
        var18 = 0;
        var17 = 0;
        var16 = 0;
        var15 = 0;
        var13 = 0;
    }
    float cr, cg, cb, var9, var10, var11, var12;

    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x, y - 1, z, 0)) {
        if (self->field_22352_G <= 0) {
            var9 = var10 = var11 = var12 = self->aoLightValueYNeg;
        } else {
            self->field_22376_n = BBR(x - 1, y - 1, z);
            self->field_22374_p = BBR(x, y - 1, z - 1);
            self->field_22373_q = BBR(x, y - 1, z + 1);
            self->field_22371_s = BBR(x + 1, y - 1, z);
            self->field_22377_m = (!self->field_22361_ab && !self->field_22357_ad)
                                      ? self->field_22376_n
                                      : BBR(x - 1, y - 1, z - 1);
            self->field_22375_o = (!self->field_22355_ae && !self->field_22357_ad)
                                      ? self->field_22376_n
                                      : BBR(x - 1, y - 1, z + 1);
            self->field_22372_r = (!self->field_22361_ab && !self->field_22359_ac)
                                      ? self->field_22371_s
                                      : BBR(x + 1, y - 1, z - 1);
            self->field_22370_t = (!self->field_22355_ae && !self->field_22359_ac)
                                      ? self->field_22371_s
                                      : BBR(x + 1, y - 1, z + 1);
            var9 = (self->field_22375_o + self->field_22376_n + self->field_22373_q +
                    self->aoLightValueYNeg) /
                   4.0f;
            var12 = (self->field_22373_q + self->aoLightValueYNeg + self->field_22370_t +
                     self->field_22371_s) /
                    4.0f;
            var11 = (self->aoLightValueYNeg + self->field_22374_p + self->field_22371_s +
                     self->field_22372_r) /
                    4.0f;
            var10 = (self->field_22376_n + self->field_22377_m + self->aoLightValueYNeg +
                     self->field_22374_p) /
                    4.0f;
        }
        cr = (var13 ? r : 1.0f) * 0.5f;
        cg = (var13 ? g : 1.0f) * 0.5f;
        cb = (var13 ? b : 1.0f) * 0.5f;
        AO_SET_COLOR(1.0f);
        AO_APPLY_VERTEX(var9, var10, var11, var12);
        RenderBlocks_renderBottomFace(
            self, block, (double)x, (double)y, (double)z,
            block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 0));
        var8 = 1;
    }

    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x, y + 1, z, 1)) {
        if (self->field_22352_G <= 0) {
            var9 = var10 = var11 = var12 = self->aoLightValueYPos;
        } else {
            self->field_22368_v = BBR(x - 1, y + 1, z);
            self->field_22364_z = BBR(x + 1, y + 1, z);
            self->field_22366_x = BBR(x, y + 1, z - 1);
            self->field_22362_A = BBR(x, y + 1, z + 1);
            self->field_22369_u = (!self->field_22339_T && !self->field_22337_V)
                                      ? self->field_22368_v
                                      : BBR(x - 1, y + 1, z - 1);
            self->field_22365_y = (!self->field_22339_T && !self->field_22338_U)
                                      ? self->field_22364_z
                                      : BBR(x + 1, y + 1, z - 1);
            self->field_22367_w = (!self->field_22336_W && !self->field_22337_V)
                                      ? self->field_22368_v
                                      : BBR(x - 1, y + 1, z + 1);
            self->field_22360_B = (!self->field_22336_W && !self->field_22338_U)
                                      ? self->field_22364_z
                                      : BBR(x + 1, y + 1, z + 1);
            var12 = (self->field_22367_w + self->field_22368_v + self->field_22362_A +
                     self->aoLightValueYPos) /
                    4.0f;
            var9 = (self->field_22362_A + self->aoLightValueYPos + self->field_22360_B +
                    self->field_22364_z) /
                   4.0f;
            var10 = (self->aoLightValueYPos + self->field_22366_x + self->field_22364_z +
                     self->field_22365_y) /
                    4.0f;
            var11 = (self->field_22368_v + self->field_22369_u + self->aoLightValueYPos +
                     self->field_22366_x) /
                    4.0f;
        }
        cr = var14 ? r : 1.0f;
        cg = var14 ? g : 1.0f;
        cb = var14 ? b : 1.0f;
        AO_SET_COLOR(1.0f);
        AO_APPLY_VERTEX(var9, var10, var11, var12);
        RenderBlocks_renderTopFace(
            self, block, (double)x, (double)y, (double)z,
            block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 1));
        var8 = 1;
    }

    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x, y, z - 1, 2)) {
        if (self->field_22352_G <= 0) {
            var9 = var10 = var11 = var12 = self->aoLightValueZNeg;
        } else {
            self->field_22358_C = BBR(x - 1, y, z - 1);
            self->field_22374_p = BBR(x, y - 1, z - 1);
            self->field_22366_x = BBR(x, y + 1, z - 1);
            self->field_22356_D = BBR(x + 1, y, z - 1);
            self->field_22377_m = (!self->field_22335_X && !self->field_22361_ab)
                                      ? self->field_22358_C
                                      : BBR(x - 1, y - 1, z - 1);
            self->field_22369_u = (!self->field_22335_X && !self->field_22339_T)
                                      ? self->field_22358_C
                                      : BBR(x - 1, y + 1, z - 1);
            self->field_22372_r = (!self->field_22363_aa && !self->field_22361_ab)
                                      ? self->field_22356_D
                                      : BBR(x + 1, y - 1, z - 1);
            self->field_22365_y = (!self->field_22363_aa && !self->field_22339_T)
                                      ? self->field_22356_D
                                      : BBR(x + 1, y + 1, z - 1);
            var9 = (self->field_22358_C + self->field_22369_u + self->aoLightValueZNeg +
                    self->field_22366_x) /
                   4.0f;
            var10 = (self->aoLightValueZNeg + self->field_22366_x + self->field_22356_D +
                     self->field_22365_y) /
                    4.0f;
            var11 = (self->field_22374_p + self->aoLightValueZNeg + self->field_22372_r +
                     self->field_22356_D) /
                    4.0f;
            var12 = (self->field_22377_m + self->field_22358_C + self->field_22374_p +
                     self->aoLightValueZNeg) /
                    4.0f;
        }
        cr = (var15 ? r : 1.0f) * 0.8f;
        cg = (var15 ? g : 1.0f) * 0.8f;
        cb = (var15 ? b : 1.0f) * 0.8f;
        AO_SET_COLOR(1.0f);
        AO_APPLY_VERTEX(var9, var10, var11, var12);
        int tx2 = block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 2);
        RenderBlocks_renderEastFace(self, block, (double)x, (double)y, (double)z, tx2);
        AO_GRASS_OVERLAY(
            RenderBlocks_renderEastFace(self, block, (double)x, (double)y, (double)z, 38), tx2);
        var8 = 1;
    }

    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x, y, z + 1, 3)) {
        if (self->field_22352_G <= 0) {
            var9 = var10 = var11 = var12 = self->aoLightValueZPos;
        } else {
            self->field_22354_E = BBR(x - 1, y, z + 1);
            self->field_22353_F = BBR(x + 1, y, z + 1);
            self->field_22373_q = BBR(x, y - 1, z + 1);
            self->field_22362_A = BBR(x, y + 1, z + 1);
            self->field_22375_o = (!self->field_22333_Z && !self->field_22355_ae)
                                      ? self->field_22354_E
                                      : BBR(x - 1, y - 1, z + 1);
            self->field_22367_w = (!self->field_22333_Z && !self->field_22336_W)
                                      ? self->field_22354_E
                                      : BBR(x - 1, y + 1, z + 1);
            self->field_22370_t = (!self->field_22334_Y && !self->field_22355_ae)
                                      ? self->field_22353_F
                                      : BBR(x + 1, y - 1, z + 1);
            self->field_22360_B = (!self->field_22334_Y && !self->field_22336_W)
                                      ? self->field_22353_F
                                      : BBR(x + 1, y + 1, z + 1);
            var9 = (self->field_22354_E + self->field_22367_w + self->aoLightValueZPos +
                    self->field_22362_A) /
                   4.0f;
            var12 = (self->aoLightValueZPos + self->field_22362_A + self->field_22353_F +
                     self->field_22360_B) /
                    4.0f;
            var11 = (self->field_22373_q + self->aoLightValueZPos + self->field_22370_t +
                     self->field_22353_F) /
                    4.0f;
            var10 = (self->field_22375_o + self->field_22354_E + self->field_22373_q +
                     self->aoLightValueZPos) /
                    4.0f;
        }
        cr = (var16 ? r : 1.0f) * 0.8f;
        cg = (var16 ? g : 1.0f) * 0.8f;
        cb = (var16 ? b : 1.0f) * 0.8f;
        AO_SET_COLOR(1.0f);
        AO_APPLY_VERTEX(var9, var10, var11, var12);
        int tx3 = block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 3);
        RenderBlocks_renderWestFace(self, block, (double)x, (double)y, (double)z, tx3);
        AO_GRASS_OVERLAY(
            RenderBlocks_renderWestFace(self, block, (double)x, (double)y, (double)z, 38), tx3);
        var8 = 1;
    }

    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x - 1, y, z, 4)) {
        if (self->field_22352_G <= 0) {
            var9 = var10 = var11 = var12 = self->aoLightValueXNeg;
        } else {
            self->field_22376_n = BBR(x - 1, y - 1, z);
            self->field_22358_C = BBR(x - 1, y, z - 1);
            self->field_22354_E = BBR(x - 1, y, z + 1);
            self->field_22368_v = BBR(x - 1, y + 1, z);
            self->field_22377_m = (!self->field_22335_X && !self->field_22357_ad)
                                      ? self->field_22358_C
                                      : BBR(x - 1, y - 1, z - 1);
            self->field_22375_o = (!self->field_22333_Z && !self->field_22357_ad)
                                      ? self->field_22354_E
                                      : BBR(x - 1, y - 1, z + 1);
            self->field_22369_u = (!self->field_22335_X && !self->field_22337_V)
                                      ? self->field_22358_C
                                      : BBR(x - 1, y + 1, z - 1);
            self->field_22367_w = (!self->field_22333_Z && !self->field_22337_V)
                                      ? self->field_22354_E
                                      : BBR(x - 1, y + 1, z + 1);
            var12 = (self->field_22376_n + self->field_22375_o + self->aoLightValueXNeg +
                     self->field_22354_E) /
                    4.0f;
            var9 = (self->aoLightValueXNeg + self->field_22354_E + self->field_22368_v +
                    self->field_22367_w) /
                   4.0f;
            var10 = (self->field_22358_C + self->aoLightValueXNeg + self->field_22369_u +
                     self->field_22368_v) /
                    4.0f;
            var11 = (self->field_22377_m + self->field_22376_n + self->field_22358_C +
                     self->aoLightValueXNeg) /
                    4.0f;
        }
        cr = (var17 ? r : 1.0f) * 0.6f;
        cg = (var17 ? g : 1.0f) * 0.6f;
        cb = (var17 ? b : 1.0f) * 0.6f;
        AO_SET_COLOR(1.0f);
        AO_APPLY_VERTEX(var9, var10, var11, var12);
        int tx4 = block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 4);
        RenderBlocks_renderNorthFace(self, block, (double)x, (double)y, (double)z, tx4);
        AO_GRASS_OVERLAY(
            RenderBlocks_renderNorthFace(self, block, (double)x, (double)y, (double)z, 38), tx4);
        var8 = 1;
    }

    if (self->renderAllFaces ||
        block->vtable->shouldSideBeRendered(block, self->blockAccess, x + 1, y, z, 5)) {
        if (self->field_22352_G <= 0) {
            var9 = var10 = var11 = var12 = self->aoLightValueXPos;
        } else {
            self->field_22371_s = BBR(x + 1, y - 1, z);
            self->field_22356_D = BBR(x + 1, y, z - 1);
            self->field_22353_F = BBR(x + 1, y, z + 1);
            self->field_22364_z = BBR(x + 1, y + 1, z);
            self->field_22372_r = (!self->field_22359_ac && !self->field_22363_aa)
                                      ? self->field_22356_D
                                      : BBR(x + 1, y - 1, z - 1);
            self->field_22370_t = (!self->field_22359_ac && !self->field_22334_Y)
                                      ? self->field_22353_F
                                      : BBR(x + 1, y - 1, z + 1);
            self->field_22365_y = (!self->field_22338_U && !self->field_22363_aa)
                                      ? self->field_22356_D
                                      : BBR(x + 1, y + 1, z - 1);
            self->field_22360_B = (!self->field_22338_U && !self->field_22334_Y)
                                      ? self->field_22353_F
                                      : BBR(x + 1, y + 1, z + 1);
            var9 = (self->field_22371_s + self->field_22370_t + self->aoLightValueXPos +
                    self->field_22353_F) /
                   4.0f;
            var12 = (self->aoLightValueXPos + self->field_22353_F + self->field_22364_z +
                     self->field_22360_B) /
                    4.0f;
            var11 = (self->field_22356_D + self->aoLightValueXPos + self->field_22365_y +
                     self->field_22364_z) /
                    4.0f;
            var10 = (self->field_22372_r + self->field_22371_s + self->field_22356_D +
                     self->aoLightValueXPos) /
                    4.0f;
        }
        cr = (var18 ? r : 1.0f) * 0.6f;
        cg = (var18 ? g : 1.0f) * 0.6f;
        cb = (var18 ? b : 1.0f) * 0.6f;
        AO_SET_COLOR(1.0f);
        AO_APPLY_VERTEX(var9, var10, var11, var12);
        int tx5 = block->vtable->getBlockTexture(block, self->blockAccess, x, y, z, 5);
        RenderBlocks_renderSouthFace(self, block, (double)x, (double)y, (double)z, tx5);
        AO_GRASS_OVERLAY(
            RenderBlocks_renderSouthFace(self, block, (double)x, (double)y, (double)z, 38), tx5);
        var8 = 1;
    }
    self->enableAO = 0;
#undef AO_SET_COLOR
#undef AO_APPLY_VERTEX
#undef AO_GRASS_OVERLAY
#undef BLK_ID
#undef CAN_GRASS
#undef BBR
    return var8;
}

int RenderBlocks_renderStandardBlock(RenderBlocks *self, Block *block, int x, int y, int z) {
    int colorMult = block->vtable->colorMultiplier(block, self->blockAccess, x, y, z);
    float r = (float)((colorMult >> 16) & 255) / 255.0f;
    float g = (float)((colorMult >> 8) & 255) / 255.0f;
    float b = (float)(colorMult & 255) / 255.0f;
    if (EntityRenderer_field_28135_a) {
        float var9 = (r * 30.0f + g * 59.0f + b * 11.0f) / 100.0f;
        float var10 = (r * 30.0f + g * 70.0f) / 100.0f;
        float var11 = (r * 30.0f + b * 70.0f) / 100.0f;
        r = var9;
        g = var10;
        b = var11;
    }
    if (Minecraft_isAmbientOcclusionEnabled())
        return RenderBlocks_renderStandardBlockWithAmbientOcclusion(self, block, x, y, z, r, g, b);
    else
        return RenderBlocks_renderStandardBlockWithColorMultiplier(self, block, x, y, z, r, g, b);
}

void RenderBlocks_renderBlockUsingTexture(RenderBlocks *self, Block *block, int x, int y, int z,
                                          int tex) {
    self->overrideBlockTexture = tex;
    RenderBlocks_renderBlockByRenderType(self, block, x, y, z);
    self->overrideBlockTexture = -1;
}

void RenderBlocks_func_31075_a(RenderBlocks *self, Block *block, int x, int y, int z) {
    self->renderAllFaces = 1;
    RenderBlocks_renderBlockByRenderType(self, block, x, y, z);
    self->renderAllFaces = 0;
}

void RenderBlocks_renderCrossedSquares(RenderBlocks *self, Block *block, int meta, double x,
                                       double y, double z) {
    Tessellator *t = &Tessellator_instance;
    int tex = block->vtable->getBlockTextureFromSideAndMetadata(block, 0, meta);
    if (self->overrideBlockTexture >= 0)
        tex = self->overrideBlockTexture;

    int var10 = (tex & 15) << 4;
    int var11 = tex & 240;
    double u0 = (float)var10 / 256.0f;
    double u1 = ((float)var10 + 15.99f) / 256.0f;
    double v0 = (float)var11 / 256.0f;
    double v1 = ((float)var11 + 15.99f) / 256.0f;

    double x0 = x + 0.5 - 0.45, x1 = x + 0.5 + 0.45;
    double z0 = z + 0.5 - 0.45, z1 = z + 0.5 + 0.45;
    double yMin = y, yMax = y + 1.0;

    Tessellator_addVertexWithUV(t, x0, yMax, z0, u0, v0);
    Tessellator_addVertexWithUV(t, x0, yMin, z0, u0, v1);
    Tessellator_addVertexWithUV(t, x1, yMin, z1, u1, v1);
    Tessellator_addVertexWithUV(t, x1, yMax, z1, u1, v0);

    Tessellator_addVertexWithUV(t, x1, yMax, z1, u0, v0);
    Tessellator_addVertexWithUV(t, x1, yMin, z1, u0, v1);
    Tessellator_addVertexWithUV(t, x0, yMin, z0, u1, v1);
    Tessellator_addVertexWithUV(t, x0, yMax, z0, u1, v0);

    Tessellator_addVertexWithUV(t, x0, yMax, z1, u0, v0);
    Tessellator_addVertexWithUV(t, x0, yMin, z1, u0, v1);
    Tessellator_addVertexWithUV(t, x1, yMin, z0, u1, v1);
    Tessellator_addVertexWithUV(t, x1, yMax, z0, u1, v0);

    Tessellator_addVertexWithUV(t, x1, yMax, z0, u0, v0);
    Tessellator_addVertexWithUV(t, x1, yMin, z0, u0, v1);
    Tessellator_addVertexWithUV(t, x0, yMin, z1, u1, v1);
    Tessellator_addVertexWithUV(t, x0, yMax, z1, u1, v0);
}

void RenderBlocks_renderTorchAtAngle(RenderBlocks *self, Block *var1, double var2, double var4,
                                     double var6, double var8, double var10) {
    Tessellator *var12 = &Tessellator_instance;
    int var13 = var1->vtable->getBlockTextureFromSide(var1, 0);
    if (self->overrideBlockTexture >= 0)
        var13 = self->overrideBlockTexture;

    int var14 = (var13 & 15) << 4;
    int var15 = var13 & 240;
    float var16 = (float)var14 / 256.0f;
    float var17 = ((float)var14 + 15.99f) / 256.0f;
    float var18 = (float)var15 / 256.0f;
    float var19 = ((float)var15 + 15.99f) / 256.0f;
    double var20 = (double)var16 + 1.75 / 64.0;
    double var22 = (double)var18 + 6.0 / 256.0;
    double var24 = (double)var16 + 9.0 / 256.0;
    double var26 = (double)var18 + 1.0 / 32.0;
    var2 += 0.5;
    var6 += 0.5;
    double var28 = var2 - 0.5;
    double var30 = var2 + 0.5;
    double var32 = var6 - 0.5;
    double var34 = var6 + 0.5;
    double var36 = 1.0 / 16.0;
    double var38 = 0.625;
    Tessellator_addVertexWithUV(var12, var2 + var8 * (1.0 - var38) - var36, var4 + var38,
                                var6 + var10 * (1.0 - var38) - var36, var20, var22);
    Tessellator_addVertexWithUV(var12, var2 + var8 * (1.0 - var38) - var36, var4 + var38,
                                var6 + var10 * (1.0 - var38) + var36, var20, var26);
    Tessellator_addVertexWithUV(var12, var2 + var8 * (1.0 - var38) + var36, var4 + var38,
                                var6 + var10 * (1.0 - var38) + var36, var24, var26);
    Tessellator_addVertexWithUV(var12, var2 + var8 * (1.0 - var38) + var36, var4 + var38,
                                var6 + var10 * (1.0 - var38) - var36, var24, var22);
    Tessellator_addVertexWithUV(var12, var2 - var36, var4 + 1.0, var32, var16, var18);
    Tessellator_addVertexWithUV(var12, var2 - var36 + var8, var4 + 0.0, var32 + var10, var16,
                                var19);
    Tessellator_addVertexWithUV(var12, var2 - var36 + var8, var4 + 0.0, var34 + var10, var17,
                                var19);
    Tessellator_addVertexWithUV(var12, var2 - var36, var4 + 1.0, var34, var17, var18);
    Tessellator_addVertexWithUV(var12, var2 + var36, var4 + 1.0, var34, var16, var18);
    Tessellator_addVertexWithUV(var12, var2 + var8 + var36, var4 + 0.0, var34 + var10, var16,
                                var19);
    Tessellator_addVertexWithUV(var12, var2 + var8 + var36, var4 + 0.0, var32 + var10, var17,
                                var19);
    Tessellator_addVertexWithUV(var12, var2 + var36, var4 + 1.0, var32, var17, var18);
    Tessellator_addVertexWithUV(var12, var28, var4 + 1.0, var6 + var36, var16, var18);
    Tessellator_addVertexWithUV(var12, var28 + var8, var4 + 0.0, var6 + var36 + var10, var16,
                                var19);
    Tessellator_addVertexWithUV(var12, var30 + var8, var4 + 0.0, var6 + var36 + var10, var17,
                                var19);
    Tessellator_addVertexWithUV(var12, var30, var4 + 1.0, var6 + var36, var17, var18);
    Tessellator_addVertexWithUV(var12, var30, var4 + 1.0, var6 - var36, var16, var18);
    Tessellator_addVertexWithUV(var12, var30 + var8, var4 + 0.0, var6 - var36 + var10, var16,
                                var19);
    Tessellator_addVertexWithUV(var12, var28 + var8, var4 + 0.0, var6 - var36 + var10, var17,
                                var19);
    Tessellator_addVertexWithUV(var12, var28, var4 + 1.0, var6 - var36, var17, var18);
}

static float RenderBlocks_func_1224_a(RenderBlocks *self, int var1, int var2, int var3,
                                      Material *var4) {
    int var5 = 0;
    float var6 = 0.0f;
    for (int var7 = 0; var7 < 4; ++var7) {
        int var8 = var1 - (var7 & 1);
        int var10 = var3 - (var7 >> 1 & 1);
        if (self->blockAccess->vtable->getBlockMaterial(self->blockAccess, var8, var2 + 1, var10) ==
            var4) {
            return 1.0f;
        }
        Material *var11 =
            self->blockAccess->vtable->getBlockMaterial(self->blockAccess, var8, var2, var10);
        if (var11 != var4) {
            if (!Material_isSolid(var11)) {
                ++var6;
                ++var5;
            }
        } else {
            int var12 =
                self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var8, var2, var10);
            if (var12 >= 8 || var12 == 0) {
                var6 += BlockFluid_getPercentAir(var12) * 10.0f;
                var5 += 10;
            }
            var6 += BlockFluid_getPercentAir(var12);
            ++var5;
        }
    }
    return 1.0f - var6 / (float)var5;
}

int RenderBlocks_renderBlockFluids(RenderBlocks *self, Block *var1, int var2, int var3, int var4) {
    Tessellator *var5 = &Tessellator_instance;
    int var6 = var1->vtable->colorMultiplier(var1, self->blockAccess, var2, var3, var4);
    float var7 = (float)(var6 >> 16 & 255) / 255.0f;
    float var8 = (float)(var6 >> 8 & 255) / 255.0f;
    float var9 = (float)(var6 & 255) / 255.0f;
    int var10 =
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2, var3 + 1, var4, 1);
    int var11 =
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2, var3 - 1, var4, 0);
    int var12[4] = {
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2, var3, var4 - 1, 2),
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2, var3, var4 + 1, 3),
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2 - 1, var3, var4, 4),
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2 + 1, var3, var4, 5)};
    if (!var10 && !var11 && !var12[0] && !var12[1] && !var12[2] && !var12[3]) {
        return 0;
    }
    int var13 = 0;
    float var14 = 0.5f;
    float var15 = 1.0f;
    float var16 = 0.8f;
    float var17 = 0.6f;
    double var18 = 0.0;
    double var20 = 1.0;
    Material *var22 = var1->blockMaterial;
    int var23 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    float var24 = RenderBlocks_func_1224_a(self, var2, var3, var4, var22);
    float var25 = RenderBlocks_func_1224_a(self, var2, var3, var4 + 1, var22);
    float var26 = RenderBlocks_func_1224_a(self, var2 + 1, var3, var4 + 1, var22);
    float var27 = RenderBlocks_func_1224_a(self, var2 + 1, var3, var4, var22);
    int var28, var31;
    float var36, var37, var38;
    if (self->renderAllFaces || var10) {
        var13 = 1;
        var28 = var1->vtable->getBlockTextureFromSideAndMetadata(var1, 1, var23);
        float var29 = (float)BlockFluid_func_293_a(self->blockAccess, var2, var3, var4, var22);
        if (var29 > -999.0f) {
            var28 = var1->vtable->getBlockTextureFromSideAndMetadata(var1, 2, var23);
        }
        int var30 = (var28 & 15) << 4;
        var31 = var28 & 240;
        double var32 = ((double)var30 + 8.0) / 256.0;
        double var34 = ((double)var31 + 8.0) / 256.0;
        if (var29 < -999.0f) {
            var29 = 0.0f;
        } else {
            var32 = (double)((float)(var30 + 16) / 256.0f);
            var34 = (double)((float)(var31 + 16) / 256.0f);
        }
        var36 = MathHelper_sin(var29) * 8.0f / 256.0f;
        var37 = MathHelper_cos(var29) * 8.0f / 256.0f;
        var38 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
        Tessellator_setColorOpaque_F(var5, var15 * var38 * var7, var15 * var38 * var8,
                                     var15 * var38 * var9);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)((float)var3 + var24),
                                    (double)(var4 + 0), var32 - (double)var37 - (double)var36,
                                    var34 - (double)var37 + (double)var36);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)((float)var3 + var25),
                                    (double)(var4 + 1), var32 - (double)var37 + (double)var36,
                                    var34 + (double)var37 + (double)var36);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)((float)var3 + var26),
                                    (double)(var4 + 1), var32 + (double)var37 + (double)var36,
                                    var34 + (double)var37 - (double)var36);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)((float)var3 + var27),
                                    (double)(var4 + 0), var32 + (double)var37 - (double)var36,
                                    var34 - (double)var37 - (double)var36);
    }
    if (self->renderAllFaces || var11) {
        float var52 =
            var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3 - 1, var4);
        Tessellator_setColorOpaque_F(var5, var14 * var52, var14 * var52, var14 * var52);
        RenderBlocks_renderBottomFace(self, var1, (double)var2, (double)var3, (double)var4,
                                      var1->vtable->getBlockTextureFromSide(var1, 0));
        var13 = 1;
    }
    for (var28 = 0; var28 < 4; ++var28) {
        int var53 = var2;
        var31 = var4;
        if (var28 == 0)
            var31 = var4 - 1;
        if (var28 == 1)
            ++var31;
        if (var28 == 2)
            var53 = var2 - 1;
        if (var28 == 3)
            ++var53;
        int var54 = var1->vtable->getBlockTextureFromSideAndMetadata(var1, var28 + 2, var23);
        int var33 = (var54 & 15) << 4;
        int var55 = var54 & 240;
        if (self->renderAllFaces || var12[var28]) {
            float var35, var39, var40;
            if (var28 == 0) {
                var35 = var24;
                var36 = var27;
                var37 = (float)var2;
                var39 = (float)(var2 + 1);
                var38 = (float)var4;
                var40 = (float)var4;
            } else if (var28 == 1) {
                var35 = var26;
                var36 = var25;
                var37 = (float)(var2 + 1);
                var39 = (float)var2;
                var38 = (float)(var4 + 1);
                var40 = (float)(var4 + 1);
            } else if (var28 == 2) {
                var35 = var25;
                var36 = var24;
                var37 = (float)var2;
                var39 = (float)var2;
                var38 = (float)(var4 + 1);
                var40 = (float)var4;
            } else {
                var35 = var27;
                var36 = var26;
                var37 = (float)(var2 + 1);
                var39 = (float)(var2 + 1);
                var38 = (float)var4;
                var40 = (float)(var4 + 1);
            }
            var13 = 1;
            double var41 = (double)((float)(var33 + 0) / 256.0f);
            double var43 = ((double)(var33 + 16) - 0.01) / 256.0;
            double var45 = (double)(((float)var55 + (1.0f - var35) * 16.0f) / 256.0f);
            double var47 = (double)(((float)var55 + (1.0f - var36) * 16.0f) / 256.0f);
            double var49 = ((double)(var55 + 16) - 0.01) / 256.0;
            float var51 =
                var1->vtable->getBlockBrightness(var1, self->blockAccess, var53, var3, var31);
            if (var28 < 2)
                var51 *= var16;
            else
                var51 *= var17;
            Tessellator_setColorOpaque_F(var5, var15 * var51 * var7, var15 * var51 * var8,
                                         var15 * var51 * var9);
            Tessellator_addVertexWithUV(var5, (double)var37, (double)((float)var3 + var35),
                                        (double)var38, var41, var45);
            Tessellator_addVertexWithUV(var5, (double)var39, (double)((float)var3 + var36),
                                        (double)var40, var43, var47);
            Tessellator_addVertexWithUV(var5, (double)var39, (double)(var3 + 0), (double)var40,
                                        var43, var49);
            Tessellator_addVertexWithUV(var5, (double)var37, (double)(var3 + 0), (double)var38,
                                        var41, var49);
        }
    }
    var1->minY = var18;
    var1->maxY = var20;
    return var13;
}

static int RenderBlocks_func_1230_b(RenderBlocks *self, Block *var1, int var2, int var3, int var4,
                                    float var5, float var6, float var7) {
    Tessellator *var8 = &Tessellator_instance;
    int var9 = 0;
    float var10 = 0.5f, var11 = 1.0f, var12 = 0.8f, var13 = 0.6f;
    float var14 = var10 * var5, var15 = var11 * var5, var16 = var12 * var5, var17 = var13 * var5;
    float var18 = var10 * var6, var19 = var11 * var6, var20 = var12 * var6, var21 = var13 * var6;
    float var22 = var10 * var7, var23 = var11 * var7, var24 = var12 * var7, var25 = var13 * var7;
    float var26 = 1.0f / 16.0f;
    float var27 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    float var28;
    if (self->renderAllFaces ||
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2, var3 - 1, var4, 0)) {
        var28 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3 - 1, var4);
        Tessellator_setColorOpaque_F(var8, var14 * var28, var18 * var28, var22 * var28);
        RenderBlocks_renderBottomFace(
            self, var1, (double)var2, (double)var3, (double)var4,
            var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 0));
        var9 = 1;
    }
    if (self->renderAllFaces ||
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2, var3 + 1, var4, 1)) {
        var28 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3 + 1, var4);
        if (var1->maxY != 1.0 && !Material_getIsLiquid(var1->blockMaterial))
            var28 = var27;
        Tessellator_setColorOpaque_F(var8, var15 * var28, var19 * var28, var23 * var28);
        RenderBlocks_renderTopFace(
            self, var1, (double)var2, (double)var3, (double)var4,
            var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 1));
        var9 = 1;
    }
    if (self->renderAllFaces ||
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2, var3, var4 - 1, 2)) {
        var28 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4 - 1);
        if (var1->minZ > 0.0)
            var28 = var27;
        Tessellator_setColorOpaque_F(var8, var16 * var28, var20 * var28, var24 * var28);
        Tessellator_setTranslationF(var8, 0.0f, 0.0f, var26);
        RenderBlocks_renderEastFace(
            self, var1, (double)var2, (double)var3, (double)var4,
            var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 2));
        Tessellator_setTranslationF(var8, 0.0f, 0.0f, -var26);
        var9 = 1;
    }
    if (self->renderAllFaces ||
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2, var3, var4 + 1, 3)) {
        var28 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4 + 1);
        if (var1->maxZ < 1.0)
            var28 = var27;
        Tessellator_setColorOpaque_F(var8, var16 * var28, var20 * var28, var24 * var28);
        Tessellator_setTranslationF(var8, 0.0f, 0.0f, -var26);
        RenderBlocks_renderWestFace(
            self, var1, (double)var2, (double)var3, (double)var4,
            var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 3));
        Tessellator_setTranslationF(var8, 0.0f, 0.0f, var26);
        var9 = 1;
    }
    if (self->renderAllFaces ||
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2 - 1, var3, var4, 4)) {
        var28 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2 - 1, var3, var4);
        if (var1->minX > 0.0)
            var28 = var27;
        Tessellator_setColorOpaque_F(var8, var17 * var28, var21 * var28, var25 * var28);
        Tessellator_setTranslationF(var8, var26, 0.0f, 0.0f);
        RenderBlocks_renderNorthFace(
            self, var1, (double)var2, (double)var3, (double)var4,
            var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 4));
        Tessellator_setTranslationF(var8, -var26, 0.0f, 0.0f);
        var9 = 1;
    }
    if (self->renderAllFaces ||
        var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2 + 1, var3, var4, 5)) {
        var28 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2 + 1, var3, var4);
        if (var1->maxX < 1.0)
            var28 = var27;
        Tessellator_setColorOpaque_F(var8, var17 * var28, var21 * var28, var25 * var28);
        Tessellator_setTranslationF(var8, -var26, 0.0f, 0.0f);
        RenderBlocks_renderSouthFace(
            self, var1, (double)var2, (double)var3, (double)var4,
            var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 5));
        Tessellator_setTranslationF(var8, var26, 0.0f, 0.0f);
        var9 = 1;
    }
    return var9;
}

int RenderBlocks_renderBlockCactus(RenderBlocks *self, Block *var1, int var2, int var3, int var4) {
    int var5 = var1->vtable->colorMultiplier(var1, self->blockAccess, var2, var3, var4);
    float var6 = (float)(var5 >> 16 & 255) / 255.0f;
    float var7 = (float)(var5 >> 8 & 255) / 255.0f;
    float var8 = (float)(var5 & 255) / 255.0f;
    if (EntityRenderer_field_28135_a) {
        float var9 = (var6 * 30.0f + var7 * 59.0f + var8 * 11.0f) / 100.0f;
        float var10 = (var6 * 30.0f + var7 * 70.0f) / 100.0f;
        float var11 = (var6 * 30.0f + var8 * 70.0f) / 100.0f;
        var6 = var9;
        var7 = var10;
        var8 = var11;
    }
    return RenderBlocks_func_1230_b(self, var1, var2, var3, var4, var6, var7, var8);
}
int RenderBlocks_renderBlockReed(RenderBlocks *self, Block *block, int x, int y, int z) {
    Tessellator *var5 = &Tessellator_instance;
    float var6 = block->vtable->getBlockBrightness(block, self->blockAccess, x, y, z);
    int var7 = block->vtable->colorMultiplier(block, self->blockAccess, x, y, z);
    float var8 = (float)(var7 >> 16 & 255) / 255.0f;
    float var9 = (float)(var7 >> 8 & 255) / 255.0f;
    float var10 = (float)(var7 & 255) / 255.0f;
    if (EntityRenderer_field_28135_a) {
        float var11 = (var8 * 30.0f + var9 * 59.0f + var10 * 11.0f) / 100.0f;
        float var12 = (var8 * 30.0f + var9 * 70.0f) / 100.0f;
        float var13 = (var8 * 30.0f + var10 * 70.0f) / 100.0f;
        var8 = var11;
        var9 = var12;
        var10 = var13;
    }
    Tessellator_setColorOpaque_F(var5, var6 * var8, var6 * var9, var6 * var10);
    double var19 = (double)x;
    double var20 = (double)y;
    double var15 = (double)z;
    if (block == Block_tallGrass) {
        long var17 = (long)(int)((unsigned int)x * 3129871U) ^ (long)z * 116129781L ^ (long)y;
        var17 =
            (long)(((uint64_t)var17 * (uint64_t)var17 * 42317861ULL) + ((uint64_t)var17 * 11ULL));
        var19 += ((double)((float)((var17 >> 16) & 15L) / 15.0f) - 0.5) * 0.5;
        var20 += ((double)((float)((var17 >> 20) & 15L) / 15.0f) - 1.0) * 0.2;
        var15 += ((double)((float)((var17 >> 24) & 15L) / 15.0f) - 0.5) * 0.5;
    }
    RenderBlocks_renderCrossedSquares(
        self, block, self->blockAccess->vtable->getBlockMetadata(self->blockAccess, x, y, z), var19,
        var20, var15);
    return 1;
}
int RenderBlocks_renderBlockCrops(RenderBlocks *self, Block *block, int x, int y, int z) {
    Tessellator *t = &Tessellator_instance;
    int meta = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, x, y, z);
    float br = block->vtable->getBlockBrightness(block, self->blockAccess, x, y, z);
    Tessellator_setColorOpaque_F(t, br, br, br);
    RenderBlocks_func_1245_b(self, block, meta, (double)x, (double)((float)y - 1.0f / 16.0f),
                             (double)z);
    return 1;
}
int RenderBlocks_renderBlockTorch(RenderBlocks *self, Block *var1, int var2, int var3, int var4) {
    int var5 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    Tessellator *var6 = &Tessellator_instance;
    float var7 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    if (Block_lightValue[var1->blockID] > 0)
        var7 = 1.0f;
    Tessellator_setColorOpaque_F(var6, var7, var7, var7);
    double var8 = (double)0.4f;
    double var10 = 0.5 - var8;
    double var12 = (double)0.2f;
    if (var5 == 1) {
        RenderBlocks_renderTorchAtAngle(self, var1, (double)var2 - var10, (double)var3 + var12,
                                        (double)var4, -var8, 0.0);
    } else if (var5 == 2) {
        RenderBlocks_renderTorchAtAngle(self, var1, (double)var2 + var10, (double)var3 + var12,
                                        (double)var4, var8, 0.0);
    } else if (var5 == 3) {
        RenderBlocks_renderTorchAtAngle(self, var1, (double)var2, (double)var3 + var12,
                                        (double)var4 - var10, 0.0, -var8);
    } else if (var5 == 4) {
        RenderBlocks_renderTorchAtAngle(self, var1, (double)var2, (double)var3 + var12,
                                        (double)var4 + var10, 0.0, var8);
    } else {
        RenderBlocks_renderTorchAtAngle(self, var1, (double)var2, (double)var3, (double)var4, 0.0,
                                        0.0);
    }
    return 1;
}
int RenderBlocks_renderBlockFire(RenderBlocks *self, Block *var1, int var2, int var3, int var4) {
    Tessellator *var5 = &Tessellator_instance;
    int var6 = var1->vtable->getBlockTextureFromSide(var1, 0);
    if (self->overrideBlockTexture >= 0)
        var6 = self->overrideBlockTexture;
    float var7 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    Tessellator_setColorOpaque_F(var5, var7, var7, var7);
    int var8 = (var6 & 15) << 4;
    int var9 = var6 & 240;
    double var10 = (double)((float)var8 / 256.0f);
    double var12 = (double)(((float)var8 + 15.99f) / 256.0f);
    double var14 = (double)((float)var9 / 256.0f);
    double var16 = (double)(((float)var9 + 15.99f) / 256.0f);
    float var18 = 1.4f;
    double var21, var23, var25, var27, var29, var31, var33;
    if (!self->blockAccess->vtable->isBlockNormalCube(self->blockAccess, var2, var3 - 1, var4) &&
        !BlockFire_canBlockCatchFire(Block_fire, self->blockAccess, var2, var3 - 1, var4)) {
        float var37 = 0.2f;
        float var20 = 1.0f / 16.0f;
        if ((var2 + var3 + var4 & 1) == 1) {
            var10 = (double)((float)var8 / 256.0f);
            var12 = (double)(((float)var8 + 15.99f) / 256.0f);
            var14 = (double)((float)(var9 + 16) / 256.0f);
            var16 = (double)(((float)var9 + 15.99f + 16.0f) / 256.0f);
        }
        if ((var2 / 2 + var3 / 2 + var4 / 2 & 1) == 1) {
            double tmp = var12;
            var12 = var10;
            var10 = tmp;
        }
        if (BlockFire_canBlockCatchFire(Block_fire, self->blockAccess, var2 - 1, var3, var4)) {
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + var37),
                                        (double)((float)var3 + var18 + var20), (double)(var4 + 1),
                                        var12, var14);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 1),
                                        var12, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 0),
                                        var10, var16);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + var37),
                                        (double)((float)var3 + var18 + var20), (double)(var4 + 0),
                                        var10, var14);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + var37),
                                        (double)((float)var3 + var18 + var20), (double)(var4 + 0),
                                        var10, var14);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 0),
                                        var10, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 1),
                                        var12, var16);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + var37),
                                        (double)((float)var3 + var18 + var20), (double)(var4 + 1),
                                        var12, var14);
        }
        if (BlockFire_canBlockCatchFire(Block_fire, self->blockAccess, var2 + 1, var3, var4)) {
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - var37),
                                        (double)((float)var3 + var18 + var20), (double)(var4 + 0),
                                        var10, var14);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1 - 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 0),
                                        var10, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1 - 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 1),
                                        var12, var16);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - var37),
                                        (double)((float)var3 + var18 + var20), (double)(var4 + 1),
                                        var12, var14);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - var37),
                                        (double)((float)var3 + var18 + var20), (double)(var4 + 1),
                                        var12, var14);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1 - 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 1),
                                        var12, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1 - 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 0),
                                        var10, var16);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - var37),
                                        (double)((float)var3 + var18 + var20), (double)(var4 + 0),
                                        var10, var14);
        }
        if (BlockFire_canBlockCatchFire(Block_fire, self->blockAccess, var2, var3, var4 - 1)) {
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)var3 + var18 + var20),
                                        (double)((float)var4 + var37), var12, var14);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 0),
                                        var12, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 0),
                                        var10, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1),
                                        (double)((float)var3 + var18 + var20),
                                        (double)((float)var4 + var37), var10, var14);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1),
                                        (double)((float)var3 + var18 + var20),
                                        (double)((float)var4 + var37), var10, var14);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 0),
                                        var10, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 0),
                                        var12, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)var3 + var18 + var20),
                                        (double)((float)var4 + var37), var12, var14);
        }
        if (BlockFire_canBlockCatchFire(Block_fire, self->blockAccess, var2, var3, var4 + 1)) {
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1),
                                        (double)((float)var3 + var18 + var20),
                                        (double)((float)(var4 + 1) - var37), var10, var14);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 1 - 0),
                                        var10, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 1 - 0),
                                        var12, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)var3 + var18 + var20),
                                        (double)((float)(var4 + 1) - var37), var12, var14);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)var3 + var18 + var20),
                                        (double)((float)(var4 + 1) - var37), var12, var14);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 1 - 0),
                                        var12, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1),
                                        (double)((float)(var3 + 0) + var20), (double)(var4 + 1 - 0),
                                        var10, var16);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1),
                                        (double)((float)var3 + var18 + var20),
                                        (double)((float)(var4 + 1) - var37), var10, var14);
        }
        if (BlockFire_canBlockCatchFire(Block_fire, self->blockAccess, var2, var3 + 1, var4)) {
            var21 = (double)var2 + 0.5 + 0.5;
            var23 = (double)var2 + 0.5 - 0.5;
            var25 = (double)var4 + 0.5 + 0.5;
            var27 = (double)var4 + 0.5 - 0.5;
            var29 = (double)var2 + 0.5 - 0.5;
            var31 = (double)var2 + 0.5 + 0.5;
            var33 = (double)var4 + 0.5 - 0.5;
            double var35 = (double)var4 + 0.5 + 0.5;
            var10 = (double)((float)var8 / 256.0f);
            var12 = (double)(((float)var8 + 15.99f) / 256.0f);
            var14 = (double)((float)var9 / 256.0f);
            var16 = (double)(((float)var9 + 15.99f) / 256.0f);
            ++var3;
            var18 = -0.2f;
            if ((var2 + var3 + var4 & 1) == 0) {
                Tessellator_addVertexWithUV(var5, var29, (double)((float)var3 + var18),
                                            (double)(var4 + 0), var12, var14);
                Tessellator_addVertexWithUV(var5, var21, (double)(var3 + 0), (double)(var4 + 0),
                                            var12, var16);
                Tessellator_addVertexWithUV(var5, var21, (double)(var3 + 0), (double)(var4 + 1),
                                            var10, var16);
                Tessellator_addVertexWithUV(var5, var29, (double)((float)var3 + var18),
                                            (double)(var4 + 1), var10, var14);
                var10 = (double)((float)var8 / 256.0f);
                var12 = (double)(((float)var8 + 15.99f) / 256.0f);
                var14 = (double)((float)(var9 + 16) / 256.0f);
                var16 = (double)(((float)var9 + 15.99f + 16.0f) / 256.0f);
                Tessellator_addVertexWithUV(var5, var31, (double)((float)var3 + var18),
                                            (double)(var4 + 1), var12, var14);
                Tessellator_addVertexWithUV(var5, var23, (double)(var3 + 0), (double)(var4 + 1),
                                            var12, var16);
                Tessellator_addVertexWithUV(var5, var23, (double)(var3 + 0), (double)(var4 + 0),
                                            var10, var16);
                Tessellator_addVertexWithUV(var5, var31, (double)((float)var3 + var18),
                                            (double)(var4 + 0), var10, var14);
            } else {
                Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)((float)var3 + var18),
                                            var35, var12, var14);
                Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)(var3 + 0), var27,
                                            var12, var16);
                Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)(var3 + 0), var27,
                                            var10, var16);
                Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)((float)var3 + var18),
                                            var35, var10, var14);
                var10 = (double)((float)var8 / 256.0f);
                var12 = (double)(((float)var8 + 15.99f) / 256.0f);
                var14 = (double)((float)(var9 + 16) / 256.0f);
                var16 = (double)(((float)var9 + 15.99f + 16.0f) / 256.0f);
                Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)((float)var3 + var18),
                                            var33, var12, var14);
                Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)(var3 + 0), var25,
                                            var12, var16);
                Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)(var3 + 0), var25,
                                            var10, var16);
                Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)((float)var3 + var18),
                                            var33, var10, var14);
            }
        }
    } else {
        double var19 = (double)var2 + 0.5 + 0.2;
        var21 = (double)var2 + 0.5 - 0.2;
        var23 = (double)var4 + 0.5 + 0.2;
        var25 = (double)var4 + 0.5 - 0.2;
        var27 = (double)var2 + 0.5 - 0.3;
        var29 = (double)var2 + 0.5 + 0.3;
        var31 = (double)var4 + 0.5 - 0.3;
        var33 = (double)var4 + 0.5 + 0.3;
        Tessellator_addVertexWithUV(var5, var27, (double)((float)var3 + var18), (double)(var4 + 1),
                                    var12, var14);
        Tessellator_addVertexWithUV(var5, var19, (double)(var3 + 0), (double)(var4 + 1), var12,
                                    var16);
        Tessellator_addVertexWithUV(var5, var19, (double)(var3 + 0), (double)(var4 + 0), var10,
                                    var16);
        Tessellator_addVertexWithUV(var5, var27, (double)((float)var3 + var18), (double)(var4 + 0),
                                    var10, var14);
        Tessellator_addVertexWithUV(var5, var29, (double)((float)var3 + var18), (double)(var4 + 0),
                                    var12, var14);
        Tessellator_addVertexWithUV(var5, var21, (double)(var3 + 0), (double)(var4 + 0), var12,
                                    var16);
        Tessellator_addVertexWithUV(var5, var21, (double)(var3 + 0), (double)(var4 + 1), var10,
                                    var16);
        Tessellator_addVertexWithUV(var5, var29, (double)((float)var3 + var18), (double)(var4 + 1),
                                    var10, var14);
        var10 = (double)((float)var8 / 256.0f);
        var12 = (double)(((float)var8 + 15.99f) / 256.0f);
        var14 = (double)((float)(var9 + 16) / 256.0f);
        var16 = (double)(((float)var9 + 15.99f + 16.0f) / 256.0f);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)((float)var3 + var18), var33,
                                    var12, var14);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)(var3 + 0), var25, var12,
                                    var16);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)(var3 + 0), var25, var10,
                                    var16);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)((float)var3 + var18), var33,
                                    var10, var14);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)((float)var3 + var18), var31,
                                    var12, var14);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)(var3 + 0), var23, var12,
                                    var16);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)(var3 + 0), var23, var10,
                                    var16);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)((float)var3 + var18), var31,
                                    var10, var14);
        var19 = (double)var2 + 0.5 - 0.5;
        var21 = (double)var2 + 0.5 + 0.5;
        var23 = (double)var4 + 0.5 - 0.5;
        var25 = (double)var4 + 0.5 + 0.5;
        var27 = (double)var2 + 0.5 - 0.4;
        var29 = (double)var2 + 0.5 + 0.4;
        var31 = (double)var4 + 0.5 - 0.4;
        var33 = (double)var4 + 0.5 + 0.4;
        Tessellator_addVertexWithUV(var5, var27, (double)((float)var3 + var18), (double)(var4 + 0),
                                    var10, var14);
        Tessellator_addVertexWithUV(var5, var19, (double)(var3 + 0), (double)(var4 + 0), var10,
                                    var16);
        Tessellator_addVertexWithUV(var5, var19, (double)(var3 + 0), (double)(var4 + 1), var12,
                                    var16);
        Tessellator_addVertexWithUV(var5, var27, (double)((float)var3 + var18), (double)(var4 + 1),
                                    var12, var14);
        Tessellator_addVertexWithUV(var5, var29, (double)((float)var3 + var18), (double)(var4 + 1),
                                    var10, var14);
        Tessellator_addVertexWithUV(var5, var21, (double)(var3 + 0), (double)(var4 + 1), var10,
                                    var16);
        Tessellator_addVertexWithUV(var5, var21, (double)(var3 + 0), (double)(var4 + 0), var12,
                                    var16);
        Tessellator_addVertexWithUV(var5, var29, (double)((float)var3 + var18), (double)(var4 + 0),
                                    var12, var14);
        var10 = (double)((float)var8 / 256.0f);
        var12 = (double)(((float)var8 + 15.99f) / 256.0f);
        var14 = (double)((float)var9 / 256.0f);
        var16 = (double)(((float)var9 + 15.99f) / 256.0f);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)((float)var3 + var18), var33,
                                    var10, var14);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)(var3 + 0), var25, var10,
                                    var16);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)(var3 + 0), var25, var12,
                                    var16);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)((float)var3 + var18), var33,
                                    var12, var14);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)((float)var3 + var18), var31,
                                    var10, var14);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)(var3 + 0), var23, var10,
                                    var16);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)(var3 + 0), var23, var12,
                                    var16);
        Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)((float)var3 + var18), var31,
                                    var12, var14);
    }
    return 1;
}
int RenderBlocks_renderBlockRedstoneWire(RenderBlocks *self, Block *var1, int var2, int var3,
                                         int var4) {
    Tessellator *var5 = &Tessellator_instance;
    int var6 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    int var7 = var1->vtable->getBlockTextureFromSideAndMetadata(var1, 1, var6);
    if (self->overrideBlockTexture >= 0)
        var7 = self->overrideBlockTexture;
    float var8 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    float var9 = (float)var6 / 15.0f;
    float var10 = var9 * 0.6f + 0.4f;
    if (var6 == 0)
        var10 = 0.3f;
    float var11 = var9 * var9 * 0.7f - 0.5f;
    float var12 = var9 * var9 * 0.6f - 0.7f;
    if (var11 < 0.0f)
        var11 = 0.0f;
    if (var12 < 0.0f)
        var12 = 0.0f;
    Tessellator_setColorOpaque_F(var5, var8 * var10, var8 * var11, var8 * var12);
    int var13 = (var7 & 15) << 4;
    int var14 = var7 & 240;
    double var15 = (double)((float)var13 / 256.0f);
    double var17 = (double)(((float)var13 + 15.99f) / 256.0f);
    double var19 = (double)((float)var14 / 256.0f);
    double var21 = (double)(((float)var14 + 15.99f) / 256.0f);
    IBlockAccess *ba = self->blockAccess;
    int var26 = BlockRedstoneWire_isPowerProviderOrWire(ba, var2 - 1, var3, var4, 1) ||
                (!ba->vtable->isBlockNormalCube(ba, var2 - 1, var3, var4) &&
                 BlockRedstoneWire_isPowerProviderOrWire(ba, var2 - 1, var3 - 1, var4, -1));
    int var27 = BlockRedstoneWire_isPowerProviderOrWire(ba, var2 + 1, var3, var4, 3) ||
                (!ba->vtable->isBlockNormalCube(ba, var2 + 1, var3, var4) &&
                 BlockRedstoneWire_isPowerProviderOrWire(ba, var2 + 1, var3 - 1, var4, -1));
    int var28 = BlockRedstoneWire_isPowerProviderOrWire(ba, var2, var3, var4 - 1, 2) ||
                (!ba->vtable->isBlockNormalCube(ba, var2, var3, var4 - 1) &&
                 BlockRedstoneWire_isPowerProviderOrWire(ba, var2, var3 - 1, var4 - 1, -1));
    int var29 = BlockRedstoneWire_isPowerProviderOrWire(ba, var2, var3, var4 + 1, 0) ||
                (!ba->vtable->isBlockNormalCube(ba, var2, var3, var4 + 1) &&
                 BlockRedstoneWire_isPowerProviderOrWire(ba, var2, var3 - 1, var4 + 1, -1));
    if (!ba->vtable->isBlockNormalCube(ba, var2, var3 + 1, var4)) {
        if (ba->vtable->isBlockNormalCube(ba, var2 - 1, var3, var4) &&
            BlockRedstoneWire_isPowerProviderOrWire(ba, var2 - 1, var3 + 1, var4, -1))
            var26 = 1;
        if (ba->vtable->isBlockNormalCube(ba, var2 + 1, var3, var4) &&
            BlockRedstoneWire_isPowerProviderOrWire(ba, var2 + 1, var3 + 1, var4, -1))
            var27 = 1;
        if (ba->vtable->isBlockNormalCube(ba, var2, var3, var4 - 1) &&
            BlockRedstoneWire_isPowerProviderOrWire(ba, var2, var3 + 1, var4 - 1, -1))
            var28 = 1;
        if (ba->vtable->isBlockNormalCube(ba, var2, var3, var4 + 1) &&
            BlockRedstoneWire_isPowerProviderOrWire(ba, var2, var3 + 1, var4 + 1, -1))
            var29 = 1;
    }
    float var31 = (float)(var2 + 0);
    float var32 = (float)(var2 + 1);
    float var33 = (float)(var4 + 0);
    float var34 = (float)(var4 + 1);
    int var35 = 0;
    if ((var26 || var27) && !var28 && !var29)
        var35 = 1;
    if ((var28 || var29) && !var27 && !var26)
        var35 = 2;
    if (var35 != 0) {
        var15 = (double)((float)(var13 + 16) / 256.0f);
        var17 = (double)(((float)(var13 + 16) + 15.99f) / 256.0f);
        var19 = (double)((float)var14 / 256.0f);
        var21 = (double)(((float)var14 + 15.99f) / 256.0f);
    }
    if (var35 == 0) {
        if (var27 || var28 || var29 || var26) {
            if (!var26) {
                var31 += 5.0f / 16.0f;
                var15 += 1.25 / 64.0;
            }
            if (!var27) {
                var32 -= 5.0f / 16.0f;
                var17 -= 1.25 / 64.0;
            }
            if (!var28) {
                var33 += 5.0f / 16.0f;
                var19 += 1.25 / 64.0;
            }
            if (!var29) {
                var34 -= 5.0f / 16.0f;
                var21 -= 1.25 / 64.0;
            }
        }
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var34, var17, var21);
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var33, var17, var19);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var33, var15, var19);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var34, var15, var21);
        Tessellator_setColorOpaque_F(var5, var8, var8, var8);
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var34, var17, var21 + 1.0 / 16.0);
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var33, var17, var19 + 1.0 / 16.0);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var33, var15, var19 + 1.0 / 16.0);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var34, var15, var21 + 1.0 / 16.0);
    } else if (var35 == 1) {
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var34, var17, var21);
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var33, var17, var19);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var33, var15, var19);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var34, var15, var21);
        Tessellator_setColorOpaque_F(var5, var8, var8, var8);
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var34, var17, var21 + 1.0 / 16.0);
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var33, var17, var19 + 1.0 / 16.0);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var33, var15, var19 + 1.0 / 16.0);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var34, var15, var21 + 1.0 / 16.0);
    } else if (var35 == 2) {
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var34, var17, var21);
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var33, var15, var21);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var33, var15, var19);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var34, var17, var19);
        Tessellator_setColorOpaque_F(var5, var8, var8, var8);
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var34, var17, var21 + 1.0 / 16.0);
        Tessellator_addVertexWithUV(var5, (double)var32, (double)((float)var3 + 0.015625f),
                                    (double)var33, var15, var21 + 1.0 / 16.0);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var33, var15, var19 + 1.0 / 16.0);
        Tessellator_addVertexWithUV(var5, (double)var31, (double)((float)var3 + 0.015625f),
                                    (double)var34, var17, var19 + 1.0 / 16.0);
    }
    if (!ba->vtable->isBlockNormalCube(ba, var2, var3 + 1, var4)) {
        var15 = (double)((float)(var13 + 16) / 256.0f);
        var17 = (double)(((float)(var13 + 16) + 15.99f) / 256.0f);
        var19 = (double)((float)var14 / 256.0f);
        var21 = (double)(((float)var14 + 15.99f) / 256.0f);
        if (ba->vtable->isBlockNormalCube(ba, var2 - 1, var3, var4) &&
            ba->vtable->getBlockId(ba, var2 - 1, var3 + 1, var4) == Block_redstoneWire->blockID) {
            Tessellator_setColorOpaque_F(var5, var8 * var10, var8 * var11, var8 * var12);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + 0.015625f),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)(var4 + 1), var17, var19);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + 0.015625f), (double)(var3 + 0),
                                        (double)(var4 + 1), var15, var19);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + 0.015625f), (double)(var3 + 0),
                                        (double)(var4 + 0), var15, var21);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + 0.015625f),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)(var4 + 0), var17, var21);
            Tessellator_setColorOpaque_F(var5, var8, var8, var8);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + 0.015625f),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)(var4 + 1), var17, var19 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + 0.015625f), (double)(var3 + 0),
                                        (double)(var4 + 1), var15, var19 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + 0.015625f), (double)(var3 + 0),
                                        (double)(var4 + 0), var15, var21 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(var5, (double)((float)var2 + 0.015625f),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)(var4 + 0), var17, var21 + 1.0 / 16.0);
        }
        if (ba->vtable->isBlockNormalCube(ba, var2 + 1, var3, var4) &&
            ba->vtable->getBlockId(ba, var2 + 1, var3 + 1, var4) == Block_redstoneWire->blockID) {
            Tessellator_setColorOpaque_F(var5, var8 * var10, var8 * var11, var8 * var12);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - 0.015625f),
                                        (double)(var3 + 0), (double)(var4 + 1), var15, var21);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - 0.015625f),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)(var4 + 1), var17, var21);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - 0.015625f),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)(var4 + 0), var17, var19);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - 0.015625f),
                                        (double)(var3 + 0), (double)(var4 + 0), var15, var19);
            Tessellator_setColorOpaque_F(var5, var8, var8, var8);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - 0.015625f),
                                        (double)(var3 + 0), (double)(var4 + 1), var15,
                                        var21 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - 0.015625f),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)(var4 + 1), var17, var21 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - 0.015625f),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)(var4 + 0), var17, var19 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - 0.015625f),
                                        (double)(var3 + 0), (double)(var4 + 0), var15,
                                        var19 + 1.0 / 16.0);
        }
        if (ba->vtable->isBlockNormalCube(ba, var2, var3, var4 - 1) &&
            ba->vtable->getBlockId(ba, var2, var3 + 1, var4 - 1) == Block_redstoneWire->blockID) {
            Tessellator_setColorOpaque_F(var5, var8 * var10, var8 * var11, var8 * var12);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)(var3 + 0),
                                        (double)((float)var4 + 0.015625f), var15, var21);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)((float)var4 + 0.015625f), var17, var21);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)((float)var4 + 0.015625f), var17, var19);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)(var3 + 0),
                                        (double)((float)var4 + 0.015625f), var15, var19);
            Tessellator_setColorOpaque_F(var5, var8, var8, var8);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)(var3 + 0),
                                        (double)((float)var4 + 0.015625f), var15,
                                        var21 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(
                var5, (double)(var2 + 1), (double)((float)(var3 + 1) + 7.0f / 320.0f),
                (double)((float)var4 + 0.015625f), var17, var21 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(
                var5, (double)(var2 + 0), (double)((float)(var3 + 1) + 7.0f / 320.0f),
                (double)((float)var4 + 0.015625f), var17, var19 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)(var3 + 0),
                                        (double)((float)var4 + 0.015625f), var15,
                                        var19 + 1.0 / 16.0);
        }
        if (ba->vtable->isBlockNormalCube(ba, var2, var3, var4 + 1) &&
            ba->vtable->getBlockId(ba, var2, var3 + 1, var4 + 1) == Block_redstoneWire->blockID) {
            Tessellator_setColorOpaque_F(var5, var8 * var10, var8 * var11, var8 * var12);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)((float)(var4 + 1) - 0.015625f), var17, var19);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)(var3 + 0),
                                        (double)((float)(var4 + 1) - 0.015625f), var15, var19);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)(var3 + 0),
                                        (double)((float)(var4 + 1) - 0.015625f), var15, var21);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0),
                                        (double)((float)(var3 + 1) + 7.0f / 320.0f),
                                        (double)((float)(var4 + 1) - 0.015625f), var17, var21);
            Tessellator_setColorOpaque_F(var5, var8, var8, var8);
            Tessellator_addVertexWithUV(
                var5, (double)(var2 + 1), (double)((float)(var3 + 1) + 7.0f / 320.0f),
                (double)((float)(var4 + 1) - 0.015625f), var17, var19 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 1), (double)(var3 + 0),
                                        (double)((float)(var4 + 1) - 0.015625f), var15,
                                        var19 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(var5, (double)(var2 + 0), (double)(var3 + 0),
                                        (double)((float)(var4 + 1) - 0.015625f), var15,
                                        var21 + 1.0 / 16.0);
            Tessellator_addVertexWithUV(
                var5, (double)(var2 + 0), (double)((float)(var3 + 1) + 7.0f / 320.0f),
                (double)((float)(var4 + 1) - 0.015625f), var17, var21 + 1.0 / 16.0);
        }
    }
    return 1;
}
int RenderBlocks_renderBlockLadder(RenderBlocks *self, Block *var1, int var2, int var3, int var4) {
    Tessellator *var5 = &Tessellator_instance;
    int var6 = var1->vtable->getBlockTextureFromSide(var1, 0);
    if (self->overrideBlockTexture >= 0)
        var6 = self->overrideBlockTexture;
    float var7 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    Tessellator_setColorOpaque_F(var5, var7, var7, var7);
    int var8 = (var6 & 15) << 4;
    int var9 = var6 & 240;
    double var10 = (double)((float)var8 / 256.0f);
    double var12 = (double)(((float)var8 + 15.99f) / 256.0f);
    double var14 = (double)((float)var9 / 256.0f);
    double var16 = (double)(((float)var9 + 15.99f) / 256.0f);
    int var18 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    float var19 = 0.0f;
    float var20 = 0.05f;
    if (var18 == 5) {
        Tessellator_addVertexWithUV(var5, (double)((float)var2 + var20),
                                    (double)((float)(var3 + 1) + var19),
                                    (double)((float)(var4 + 1) + var19), var10, var14);
        Tessellator_addVertexWithUV(var5, (double)((float)var2 + var20),
                                    (double)((float)(var3 + 0) - var19),
                                    (double)((float)(var4 + 1) + var19), var10, var16);
        Tessellator_addVertexWithUV(var5, (double)((float)var2 + var20),
                                    (double)((float)(var3 + 0) - var19),
                                    (double)((float)(var4 + 0) - var19), var12, var16);
        Tessellator_addVertexWithUV(var5, (double)((float)var2 + var20),
                                    (double)((float)(var3 + 1) + var19),
                                    (double)((float)(var4 + 0) - var19), var12, var14);
    }
    if (var18 == 4) {
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - var20),
                                    (double)((float)(var3 + 0) - var19),
                                    (double)((float)(var4 + 1) + var19), var12, var16);
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - var20),
                                    (double)((float)(var3 + 1) + var19),
                                    (double)((float)(var4 + 1) + var19), var12, var14);
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - var20),
                                    (double)((float)(var3 + 1) + var19),
                                    (double)((float)(var4 + 0) - var19), var10, var14);
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) - var20),
                                    (double)((float)(var3 + 0) - var19),
                                    (double)((float)(var4 + 0) - var19), var10, var16);
    }
    if (var18 == 3) {
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) + var19),
                                    (double)((float)(var3 + 0) - var19),
                                    (double)((float)var4 + var20), var12, var16);
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) + var19),
                                    (double)((float)(var3 + 1) + var19),
                                    (double)((float)var4 + var20), var12, var14);
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 0) - var19),
                                    (double)((float)(var3 + 1) + var19),
                                    (double)((float)var4 + var20), var10, var14);
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 0) - var19),
                                    (double)((float)(var3 + 0) - var19),
                                    (double)((float)var4 + var20), var10, var16);
    }
    if (var18 == 2) {
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) + var19),
                                    (double)((float)(var3 + 1) + var19),
                                    (double)((float)(var4 + 1) - var20), var10, var14);
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 1) + var19),
                                    (double)((float)(var3 + 0) - var19),
                                    (double)((float)(var4 + 1) - var20), var10, var16);
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 0) - var19),
                                    (double)((float)(var3 + 0) - var19),
                                    (double)((float)(var4 + 1) - var20), var12, var16);
        Tessellator_addVertexWithUV(var5, (double)((float)(var2 + 0) - var19),
                                    (double)((float)(var3 + 1) + var19),
                                    (double)((float)(var4 + 1) - var20), var12, var14);
    }
    return 1;
}
int RenderBlocks_renderBlockDoor(RenderBlocks *self, Block *var1, int var2, int var3, int var4) {
    Tessellator *var5 = &Tessellator_instance;
    BlockDoor *var6 = (BlockDoor *)var1;
    int var7 = 0;
    float var8 = 0.5f, var9 = 1.0f, var10 = 0.8f, var11 = 0.6f;
    float var12 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    float var13 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3 - 1, var4);
    if (var6->base.minY > 0.0)
        var13 = var12;
    if (Block_lightValue[var1->blockID] > 0)
        var13 = 1.0f;
    Tessellator_setColorOpaque_F(var5, var8 * var13, var8 * var13, var8 * var13);
    RenderBlocks_renderBottomFace(
        self, var1, (double)var2, (double)var3, (double)var4,
        var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 0));
    var7 = 1;
    var13 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3 + 1, var4);
    if (var6->base.maxY < 1.0)
        var13 = var12;
    if (Block_lightValue[var1->blockID] > 0)
        var13 = 1.0f;
    Tessellator_setColorOpaque_F(var5, var9 * var13, var9 * var13, var9 * var13);
    RenderBlocks_renderTopFace(
        self, var1, (double)var2, (double)var3, (double)var4,
        var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 1));
    var7 = 1;
    var13 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4 - 1);
    if (var6->base.minZ > 0.0)
        var13 = var12;
    if (Block_lightValue[var1->blockID] > 0)
        var13 = 1.0f;
    Tessellator_setColorOpaque_F(var5, var10 * var13, var10 * var13, var10 * var13);
    int var14 = var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 2);
    if (var14 < 0) {
        self->flipTexture = 1;
        var14 = -var14;
    }
    RenderBlocks_renderEastFace(self, var1, (double)var2, (double)var3, (double)var4, var14);
    var7 = 1;
    self->flipTexture = 0;
    var13 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4 + 1);
    if (var6->base.maxZ < 1.0)
        var13 = var12;
    if (Block_lightValue[var1->blockID] > 0)
        var13 = 1.0f;
    Tessellator_setColorOpaque_F(var5, var10 * var13, var10 * var13, var10 * var13);
    var14 = var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 3);
    if (var14 < 0) {
        self->flipTexture = 1;
        var14 = -var14;
    }
    RenderBlocks_renderWestFace(self, var1, (double)var2, (double)var3, (double)var4, var14);
    var7 = 1;
    self->flipTexture = 0;
    var13 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2 - 1, var3, var4);
    if (var6->base.minX > 0.0)
        var13 = var12;
    if (Block_lightValue[var1->blockID] > 0)
        var13 = 1.0f;
    Tessellator_setColorOpaque_F(var5, var11 * var13, var11 * var13, var11 * var13);
    var14 = var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 4);
    if (var14 < 0) {
        self->flipTexture = 1;
        var14 = -var14;
    }
    RenderBlocks_renderNorthFace(self, var1, (double)var2, (double)var3, (double)var4, var14);
    var7 = 1;
    self->flipTexture = 0;
    var13 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2 + 1, var3, var4);
    if (var6->base.maxX < 1.0)
        var13 = var12;
    if (Block_lightValue[var1->blockID] > 0)
        var13 = 1.0f;
    Tessellator_setColorOpaque_F(var5, var11 * var13, var11 * var13, var11 * var13);
    var14 = var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 5);
    if (var14 < 0) {
        self->flipTexture = 1;
        var14 = -var14;
    }
    RenderBlocks_renderSouthFace(self, var1, (double)var2, (double)var3, (double)var4, var14);
    var7 = 1;
    self->flipTexture = 0;
    return var7;
}
int RenderBlocks_renderBlockStairs(RenderBlocks *self, Block *var1, int var2, int var3, int var4) {
    int var5 = 0;
    int var6 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    if (var6 == 0) {
        Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        Block_setBlockBounds(var1, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        var5 = 1;
    } else if (var6 == 1) {
        Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        Block_setBlockBounds(var1, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        var5 = 1;
    } else if (var6 == 2) {
        Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        Block_setBlockBounds(var1, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        var5 = 1;
    } else if (var6 == 3) {
        Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        Block_setBlockBounds(var1, 0.0f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        var5 = 1;
    }
    Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    return var5;
}
int RenderBlocks_renderBlockFence(RenderBlocks *self, Block *var1, int var2, int var3, int var4) {
    int var5 = 0;
    float var6 = 6.0f / 16.0f;
    float var7 = 10.0f / 16.0f;
    Block_setBlockBounds(var1, var6, 0.0f, var6, var7, 1.0f, var7);
    RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
    var5 = 1;
    IBlockAccess *ba = self->blockAccess;
    int var8 = 0, var9 = 0;
    if (ba->vtable->getBlockId(ba, var2 - 1, var3, var4) == var1->blockID ||
        ba->vtable->getBlockId(ba, var2 + 1, var3, var4) == var1->blockID)
        var8 = 1;
    if (ba->vtable->getBlockId(ba, var2, var3, var4 - 1) == var1->blockID ||
        ba->vtable->getBlockId(ba, var2, var3, var4 + 1) == var1->blockID)
        var9 = 1;
    int var10 = ba->vtable->getBlockId(ba, var2 - 1, var3, var4) == var1->blockID;
    int var11 = ba->vtable->getBlockId(ba, var2 + 1, var3, var4) == var1->blockID;
    int var12 = ba->vtable->getBlockId(ba, var2, var3, var4 - 1) == var1->blockID;
    int var13 = ba->vtable->getBlockId(ba, var2, var3, var4 + 1) == var1->blockID;
    if (!var8 && !var9)
        var8 = 1;
    var6 = 7.0f / 16.0f;
    var7 = 9.0f / 16.0f;
    float var14 = 12.0f / 16.0f;
    float var15 = 15.0f / 16.0f;
    float var16 = var10 ? 0.0f : var6;
    float var17 = var11 ? 1.0f : var7;
    float var18 = var12 ? 0.0f : var6;
    float var19 = var13 ? 1.0f : var7;
    if (var8) {
        Block_setBlockBounds(var1, var16, var14, var6, var17, var15, var7);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        var5 = 1;
    }
    if (var9) {
        Block_setBlockBounds(var1, var6, var14, var18, var7, var15, var19);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        var5 = 1;
    }
    var14 = 6.0f / 16.0f;
    var15 = 9.0f / 16.0f;
    if (var8) {
        Block_setBlockBounds(var1, var16, var14, var6, var17, var15, var7);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        var5 = 1;
    }
    if (var9) {
        Block_setBlockBounds(var1, var6, var14, var18, var7, var15, var19);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        var5 = 1;
    }
    Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    return var5;
}
int RenderBlocks_renderBlockLever(RenderBlocks *self, Block *var1, int var2, int var3, int var4) {
    int var5 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    int var6 = var5 & 7;
    int var7 = (var5 & 8) > 0;
    Tessellator *var8 = &Tessellator_instance;
    int var9 = self->overrideBlockTexture >= 0;
    if (!var9)
        self->overrideBlockTexture = Block_cobblestone->blockIndexInTexture;

    float var10 = 0.25f;
    float var11 = 3.0f / 16.0f;
    float var12 = 3.0f / 16.0f;
    if (var6 == 5)
        Block_setBlockBounds(var1, 0.5f - var11, 0.0f, 0.5f - var10, 0.5f + var11, var12,
                             0.5f + var10);
    else if (var6 == 6)
        Block_setBlockBounds(var1, 0.5f - var10, 0.0f, 0.5f - var11, 0.5f + var10, var12,
                             0.5f + var11);
    else if (var6 == 4)
        Block_setBlockBounds(var1, 0.5f - var11, 0.5f - var10, 1.0f - var12, 0.5f + var11,
                             0.5f + var10, 1.0f);
    else if (var6 == 3)
        Block_setBlockBounds(var1, 0.5f - var11, 0.5f - var10, 0.0f, 0.5f + var11, 0.5f + var10,
                             var12);
    else if (var6 == 2)
        Block_setBlockBounds(var1, 1.0f - var12, 0.5f - var10, 0.5f - var11, 1.0f, 0.5f + var10,
                             0.5f + var11);
    else if (var6 == 1)
        Block_setBlockBounds(var1, 0.0f, 0.5f - var10, 0.5f - var11, var12, 0.5f + var10,
                             0.5f + var11);

    RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
    if (!var9)
        self->overrideBlockTexture = -1;

    float var13 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    if (Block_lightValue[var1->blockID] > 0)
        var13 = 1.0f;
    Tessellator_setColorOpaque_F(var8, var13, var13, var13);

    int var14 = var1->vtable->getBlockTextureFromSide(var1, 0);
    if (self->overrideBlockTexture >= 0)
        var14 = self->overrideBlockTexture;
    int var15 = (var14 & 15) << 4;
    int var16 = var14 & 240;
    float var17 = (float)var15 / 256.0f;
    float var18 = ((float)var15 + 15.99f) / 256.0f;
    float var19 = (float)var16 / 256.0f;
    float var20 = ((float)var16 + 15.99f) / 256.0f;

    Vec3D var21[8];
    float var22 = 1.0f / 16.0f;
    float var23 = 1.0f / 16.0f;
    float var24 = 10.0f / 16.0f;
    var21[0] = (Vec3D){(double)(-var22), 0.0, (double)(-var23)};
    var21[1] = (Vec3D){(double)var22, 0.0, (double)(-var23)};
    var21[2] = (Vec3D){(double)var22, 0.0, (double)var23};
    var21[3] = (Vec3D){(double)(-var22), 0.0, (double)var23};
    var21[4] = (Vec3D){(double)(-var22), (double)var24, (double)(-var23)};
    var21[5] = (Vec3D){(double)var22, (double)var24, (double)(-var23)};
    var21[6] = (Vec3D){(double)var22, (double)var24, (double)var23};
    var21[7] = (Vec3D){(double)(-var22), (double)var24, (double)var23};

    for (int var25 = 0; var25 < 8; ++var25) {
        if (var7) {
            var21[var25].zCoord -= 1.0 / 16.0;
            Vec3D_rotateAroundX(&var21[var25], (float)M_PI * 2.0f / 9.0f);
        } else {
            var21[var25].zCoord += 1.0 / 16.0;
            Vec3D_rotateAroundX(&var21[var25], -((float)M_PI * 2.0f / 9.0f));
        }
        if (var6 == 6)
            Vec3D_rotateAroundY(&var21[var25], (float)M_PI * 0.5f);
        if (var6 < 5) {
            var21[var25].yCoord -= 0.375;
            Vec3D_rotateAroundX(&var21[var25], (float)M_PI * 0.5f);
            if (var6 == 4)
                Vec3D_rotateAroundY(&var21[var25], 0.0f);
            if (var6 == 3)
                Vec3D_rotateAroundY(&var21[var25], (float)M_PI);
            if (var6 == 2)
                Vec3D_rotateAroundY(&var21[var25], (float)M_PI * 0.5f);
            if (var6 == 1)
                Vec3D_rotateAroundY(&var21[var25], (float)M_PI * -0.5f);
            var21[var25].xCoord += (double)var2 + 0.5;
            var21[var25].yCoord += (double)((float)var3 + 0.5f);
            var21[var25].zCoord += (double)var4 + 0.5;
        } else {
            var21[var25].xCoord += (double)var2 + 0.5;
            var21[var25].yCoord += (double)((float)var3 + 2.0f / 16.0f);
            var21[var25].zCoord += (double)var4 + 0.5;
        }
    }

    Vec3D *var30, *var26v, *var27v, *var28v;
    for (int var29 = 0; var29 < 6; ++var29) {
        if (var29 == 0) {
            var17 = (float)(var15 + 7) / 256.0f;
            var18 = ((float)(var15 + 9) - 0.01f) / 256.0f;
            var19 = (float)(var16 + 6) / 256.0f;
            var20 = ((float)(var16 + 8) - 0.01f) / 256.0f;
        } else if (var29 == 2) {
            var17 = (float)(var15 + 7) / 256.0f;
            var18 = ((float)(var15 + 9) - 0.01f) / 256.0f;
            var19 = (float)(var16 + 6) / 256.0f;
            var20 = ((float)(var16 + 16) - 0.01f) / 256.0f;
        }
        if (var29 == 0) {
            var30 = &var21[0];
            var26v = &var21[1];
            var27v = &var21[2];
            var28v = &var21[3];
        } else if (var29 == 1) {
            var30 = &var21[7];
            var26v = &var21[6];
            var27v = &var21[5];
            var28v = &var21[4];
        } else if (var29 == 2) {
            var30 = &var21[1];
            var26v = &var21[0];
            var27v = &var21[4];
            var28v = &var21[5];
        } else if (var29 == 3) {
            var30 = &var21[2];
            var26v = &var21[1];
            var27v = &var21[5];
            var28v = &var21[6];
        } else if (var29 == 4) {
            var30 = &var21[3];
            var26v = &var21[2];
            var27v = &var21[6];
            var28v = &var21[7];
        } else {
            var30 = &var21[0];
            var26v = &var21[3];
            var27v = &var21[7];
            var28v = &var21[4];
        }
        Tessellator_addVertexWithUV(var8, var30->xCoord, var30->yCoord, var30->zCoord,
                                    (double)var17, (double)var20);
        Tessellator_addVertexWithUV(var8, var26v->xCoord, var26v->yCoord, var26v->zCoord,
                                    (double)var18, (double)var20);
        Tessellator_addVertexWithUV(var8, var27v->xCoord, var27v->yCoord, var27v->zCoord,
                                    (double)var18, (double)var19);
        Tessellator_addVertexWithUV(var8, var28v->xCoord, var28v->yCoord, var28v->zCoord,
                                    (double)var17, (double)var19);
    }
    return 1;
}

extern int BlockPistonBase_func_31044_d(int meta);
extern int BlockPistonExtension_func_31050_c(int meta);

static void RenderBlocks_func_31076_a(RenderBlocks *self, double var1, double var3, double var5,
                                      double var7, double var9, double var11, float var13,
                                      double var14) {
    int var16 = 108;
    if (self->overrideBlockTexture >= 0)
        var16 = self->overrideBlockTexture;
    int var17 = (var16 & 15) << 4;
    int var18 = var16 & 240;
    Tessellator *var19 = &Tessellator_instance;
    double var20 = (double)((float)(var17 + 0) / 256.0f);
    double var22 = (double)((float)(var18 + 0) / 256.0f);
    double var24 = ((double)var17 + var14 - 0.01) / 256.0;
    double var26 = ((double)((float)var18 + 4.0f) - 0.01) / 256.0;
    Tessellator_setColorOpaque_F(var19, var13, var13, var13);
    Tessellator_addVertexWithUV(var19, var1, var7, var9, var24, var22);
    Tessellator_addVertexWithUV(var19, var1, var5, var9, var20, var22);
    Tessellator_addVertexWithUV(var19, var3, var5, var11, var20, var26);
    Tessellator_addVertexWithUV(var19, var3, var7, var11, var24, var26);
}

static void RenderBlocks_func_31081_b(RenderBlocks *self, double var1, double var3, double var5,
                                      double var7, double var9, double var11, float var13,
                                      double var14) {
    int var16 = 108;
    if (self->overrideBlockTexture >= 0)
        var16 = self->overrideBlockTexture;
    int var17 = (var16 & 15) << 4;
    int var18 = var16 & 240;
    Tessellator *var19 = &Tessellator_instance;
    double var20 = (double)((float)(var17 + 0) / 256.0f);
    double var22 = (double)((float)(var18 + 0) / 256.0f);
    double var24 = ((double)var17 + var14 - 0.01) / 256.0;
    double var26 = ((double)((float)var18 + 4.0f) - 0.01) / 256.0;
    Tessellator_setColorOpaque_F(var19, var13, var13, var13);
    Tessellator_addVertexWithUV(var19, var1, var5, var11, var24, var22);
    Tessellator_addVertexWithUV(var19, var1, var5, var9, var20, var22);
    Tessellator_addVertexWithUV(var19, var3, var7, var9, var20, var26);
    Tessellator_addVertexWithUV(var19, var3, var7, var11, var24, var26);
}

static void RenderBlocks_func_31077_c(RenderBlocks *self, double var1, double var3, double var5,
                                      double var7, double var9, double var11, float var13,
                                      double var14) {
    int var16 = 108;
    if (self->overrideBlockTexture >= 0)
        var16 = self->overrideBlockTexture;
    int var17 = (var16 & 15) << 4;
    int var18 = var16 & 240;
    Tessellator *var19 = &Tessellator_instance;
    double var20 = (double)((float)(var17 + 0) / 256.0f);
    double var22 = (double)((float)(var18 + 0) / 256.0f);
    double var24 = ((double)var17 + var14 - 0.01) / 256.0;
    double var26 = ((double)((float)var18 + 4.0f) - 0.01) / 256.0;
    Tessellator_setColorOpaque_F(var19, var13, var13, var13);
    Tessellator_addVertexWithUV(var19, var3, var5, var9, var24, var22);
    Tessellator_addVertexWithUV(var19, var1, var5, var9, var20, var22);
    Tessellator_addVertexWithUV(var19, var1, var7, var11, var20, var26);
    Tessellator_addVertexWithUV(var19, var3, var7, var11, var24, var26);
}

static int RenderBlocks_func_31074_b(RenderBlocks *self, Block *var1, int var2, int var3, int var4,
                                     int var5) {
    int var6 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    int var7 = var5 || (var6 & 8) != 0;
    int var8 = BlockPistonBase_func_31044_d(var6);
    if (var7) {
        switch (var8) {
        case 0:
            self->field_31087_g = 3;
            self->field_31086_h = 3;
            self->field_31085_i = 3;
            self->field_31084_j = 3;
            Block_setBlockBounds(var1, 0.0f, 0.25f, 0.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 1:
            Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 1.0f, 12.0f / 16.0f, 1.0f);
            break;
        case 2:
            self->field_31085_i = 1;
            self->field_31084_j = 2;
            Block_setBlockBounds(var1, 0.0f, 0.0f, 0.25f, 1.0f, 1.0f, 1.0f);
            break;
        case 3:
            self->field_31085_i = 2;
            self->field_31084_j = 1;
            self->field_31083_k = 3;
            self->field_31082_l = 3;
            Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 12.0f / 16.0f);
            break;
        case 4:
            self->field_31087_g = 1;
            self->field_31086_h = 2;
            self->field_31083_k = 2;
            self->field_31082_l = 1;
            Block_setBlockBounds(var1, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 5:
            self->field_31087_g = 2;
            self->field_31086_h = 1;
            self->field_31083_k = 1;
            self->field_31082_l = 2;
            Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 12.0f / 16.0f, 1.0f, 1.0f);
            break;
        }
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        self->field_31087_g = 0;
        self->field_31086_h = 0;
        self->field_31085_i = 0;
        self->field_31084_j = 0;
        self->field_31083_k = 0;
        self->field_31082_l = 0;
        Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    } else {
        switch (var8) {
        case 0:
            self->field_31087_g = 3;
            self->field_31086_h = 3;
            self->field_31085_i = 3;
            self->field_31084_j = 3;

        case 1:
        default:
            break;
        case 2:
            self->field_31085_i = 1;
            self->field_31084_j = 2;
            break;
        case 3:
            self->field_31085_i = 2;
            self->field_31084_j = 1;
            self->field_31083_k = 3;
            self->field_31082_l = 3;
            break;
        case 4:
            self->field_31087_g = 1;
            self->field_31086_h = 2;
            self->field_31083_k = 2;
            self->field_31082_l = 1;
            break;
        case 5:
            self->field_31087_g = 2;
            self->field_31086_h = 1;
            self->field_31083_k = 1;
            self->field_31082_l = 2;
            break;
        }
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        self->field_31087_g = 0;
        self->field_31086_h = 0;
        self->field_31085_i = 0;
        self->field_31084_j = 0;
        self->field_31083_k = 0;
        self->field_31082_l = 0;
    }
    return 1;
}

void RenderBlocks_func_31078_d(RenderBlocks *self, Block *block, int x, int y, int z) {
    self->renderAllFaces = 1;
    RenderBlocks_func_31074_b(self, block, x, y, z, 1);
    self->renderAllFaces = 0;
}

static int RenderBlocks_func_31080_c(RenderBlocks *self, Block *var1, int var2, int var3, int var4,
                                     int var5) {
    int var6 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    int var7 = BlockPistonExtension_func_31050_c(var6);
    float var11 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    float var12 = var5 ? 1.0f : 0.5f;
    double var13 = var5 ? 16.0 : 8.0;
    switch (var7) {
    case 0:
        self->field_31087_g = 3;
        self->field_31086_h = 3;
        self->field_31085_i = 3;
        self->field_31084_j = 3;
        Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        RenderBlocks_func_31076_a(
            self, (double)((float)var2 + 6.0f / 16.0f), (double)((float)var2 + 10.0f / 16.0f),
            (double)((float)var3 + 0.25f), (double)((float)var3 + 0.25f + var12),
            (double)((float)var4 + 10.0f / 16.0f), (double)((float)var4 + 10.0f / 16.0f),
            var11 * 0.8f, var13);
        RenderBlocks_func_31076_a(
            self, (double)((float)var2 + 10.0f / 16.0f), (double)((float)var2 + 6.0f / 16.0f),
            (double)((float)var3 + 0.25f), (double)((float)var3 + 0.25f + var12),
            (double)((float)var4 + 6.0f / 16.0f), (double)((float)var4 + 6.0f / 16.0f),
            var11 * 0.8f, var13);
        RenderBlocks_func_31076_a(
            self, (double)((float)var2 + 6.0f / 16.0f), (double)((float)var2 + 6.0f / 16.0f),
            (double)((float)var3 + 0.25f), (double)((float)var3 + 0.25f + var12),
            (double)((float)var4 + 6.0f / 16.0f), (double)((float)var4 + 10.0f / 16.0f),
            var11 * 0.6f, var13);
        RenderBlocks_func_31076_a(
            self, (double)((float)var2 + 10.0f / 16.0f), (double)((float)var2 + 10.0f / 16.0f),
            (double)((float)var3 + 0.25f), (double)((float)var3 + 0.25f + var12),
            (double)((float)var4 + 10.0f / 16.0f), (double)((float)var4 + 6.0f / 16.0f),
            var11 * 0.6f, var13);
        break;
    case 1:
        Block_setBlockBounds(var1, 0.0f, 12.0f / 16.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        RenderBlocks_func_31076_a(
            self, (double)((float)var2 + 6.0f / 16.0f), (double)((float)var2 + 10.0f / 16.0f),
            (double)((float)var3 - 0.25f + 1.0f - var12), (double)((float)var3 - 0.25f + 1.0f),
            (double)((float)var4 + 10.0f / 16.0f), (double)((float)var4 + 10.0f / 16.0f),
            var11 * 0.8f, var13);
        RenderBlocks_func_31076_a(
            self, (double)((float)var2 + 10.0f / 16.0f), (double)((float)var2 + 6.0f / 16.0f),
            (double)((float)var3 - 0.25f + 1.0f - var12), (double)((float)var3 - 0.25f + 1.0f),
            (double)((float)var4 + 6.0f / 16.0f), (double)((float)var4 + 6.0f / 16.0f),
            var11 * 0.8f, var13);
        RenderBlocks_func_31076_a(
            self, (double)((float)var2 + 6.0f / 16.0f), (double)((float)var2 + 6.0f / 16.0f),
            (double)((float)var3 - 0.25f + 1.0f - var12), (double)((float)var3 - 0.25f + 1.0f),
            (double)((float)var4 + 6.0f / 16.0f), (double)((float)var4 + 10.0f / 16.0f),
            var11 * 0.6f, var13);
        RenderBlocks_func_31076_a(
            self, (double)((float)var2 + 10.0f / 16.0f), (double)((float)var2 + 10.0f / 16.0f),
            (double)((float)var3 - 0.25f + 1.0f - var12), (double)((float)var3 - 0.25f + 1.0f),
            (double)((float)var4 + 10.0f / 16.0f), (double)((float)var4 + 6.0f / 16.0f),
            var11 * 0.6f, var13);
        break;
    case 2:
        self->field_31085_i = 1;
        self->field_31084_j = 2;
        Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.25f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        RenderBlocks_func_31081_b(
            self, (double)((float)var2 + 6.0f / 16.0f), (double)((float)var2 + 6.0f / 16.0f),
            (double)((float)var3 + 10.0f / 16.0f), (double)((float)var3 + 6.0f / 16.0f),
            (double)((float)var4 + 0.25f), (double)((float)var4 + 0.25f + var12), var11 * 0.6f,
            var13);
        RenderBlocks_func_31081_b(
            self, (double)((float)var2 + 10.0f / 16.0f), (double)((float)var2 + 10.0f / 16.0f),
            (double)((float)var3 + 6.0f / 16.0f), (double)((float)var3 + 10.0f / 16.0f),
            (double)((float)var4 + 0.25f), (double)((float)var4 + 0.25f + var12), var11 * 0.6f,
            var13);
        RenderBlocks_func_31081_b(
            self, (double)((float)var2 + 6.0f / 16.0f), (double)((float)var2 + 10.0f / 16.0f),
            (double)((float)var3 + 6.0f / 16.0f), (double)((float)var3 + 6.0f / 16.0f),
            (double)((float)var4 + 0.25f), (double)((float)var4 + 0.25f + var12), var11 * 0.5f,
            var13);
        RenderBlocks_func_31081_b(
            self, (double)((float)var2 + 10.0f / 16.0f), (double)((float)var2 + 6.0f / 16.0f),
            (double)((float)var3 + 10.0f / 16.0f), (double)((float)var3 + 10.0f / 16.0f),
            (double)((float)var4 + 0.25f), (double)((float)var4 + 0.25f + var12), var11, var13);
        break;
    case 3:
        self->field_31085_i = 2;
        self->field_31084_j = 1;
        self->field_31083_k = 3;
        self->field_31082_l = 3;
        Block_setBlockBounds(var1, 0.0f, 0.0f, 12.0f / 16.0f, 1.0f, 1.0f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        RenderBlocks_func_31081_b(
            self, (double)((float)var2 + 6.0f / 16.0f), (double)((float)var2 + 6.0f / 16.0f),
            (double)((float)var3 + 10.0f / 16.0f), (double)((float)var3 + 6.0f / 16.0f),
            (double)((float)var4 - 0.25f + 1.0f - var12), (double)((float)var4 - 0.25f + 1.0f),
            var11 * 0.6f, var13);
        RenderBlocks_func_31081_b(
            self, (double)((float)var2 + 10.0f / 16.0f), (double)((float)var2 + 10.0f / 16.0f),
            (double)((float)var3 + 6.0f / 16.0f), (double)((float)var3 + 10.0f / 16.0f),
            (double)((float)var4 - 0.25f + 1.0f - var12), (double)((float)var4 - 0.25f + 1.0f),
            var11 * 0.6f, var13);
        RenderBlocks_func_31081_b(
            self, (double)((float)var2 + 6.0f / 16.0f), (double)((float)var2 + 10.0f / 16.0f),
            (double)((float)var3 + 6.0f / 16.0f), (double)((float)var3 + 6.0f / 16.0f),
            (double)((float)var4 - 0.25f + 1.0f - var12), (double)((float)var4 - 0.25f + 1.0f),
            var11 * 0.5f, var13);
        RenderBlocks_func_31081_b(
            self, (double)((float)var2 + 10.0f / 16.0f), (double)((float)var2 + 6.0f / 16.0f),
            (double)((float)var3 + 10.0f / 16.0f), (double)((float)var3 + 10.0f / 16.0f),
            (double)((float)var4 - 0.25f + 1.0f - var12), (double)((float)var4 - 0.25f + 1.0f),
            var11, var13);
        break;
    case 4:
        self->field_31087_g = 1;
        self->field_31086_h = 2;
        self->field_31083_k = 2;
        self->field_31082_l = 1;
        Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 0.25f, 1.0f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        RenderBlocks_func_31077_c(
            self, (double)((float)var2 + 0.25f), (double)((float)var2 + 0.25f + var12),
            (double)((float)var3 + 6.0f / 16.0f), (double)((float)var3 + 6.0f / 16.0f),
            (double)((float)var4 + 10.0f / 16.0f), (double)((float)var4 + 6.0f / 16.0f),
            var11 * 0.5f, var13);
        RenderBlocks_func_31077_c(
            self, (double)((float)var2 + 0.25f), (double)((float)var2 + 0.25f + var12),
            (double)((float)var3 + 10.0f / 16.0f), (double)((float)var3 + 10.0f / 16.0f),
            (double)((float)var4 + 6.0f / 16.0f), (double)((float)var4 + 10.0f / 16.0f), var11,
            var13);
        RenderBlocks_func_31077_c(
            self, (double)((float)var2 + 0.25f), (double)((float)var2 + 0.25f + var12),
            (double)((float)var3 + 6.0f / 16.0f), (double)((float)var3 + 10.0f / 16.0f),
            (double)((float)var4 + 6.0f / 16.0f), (double)((float)var4 + 6.0f / 16.0f),
            var11 * 0.6f, var13);
        RenderBlocks_func_31077_c(
            self, (double)((float)var2 + 0.25f), (double)((float)var2 + 0.25f + var12),
            (double)((float)var3 + 10.0f / 16.0f), (double)((float)var3 + 6.0f / 16.0f),
            (double)((float)var4 + 10.0f / 16.0f), (double)((float)var4 + 10.0f / 16.0f),
            var11 * 0.6f, var13);
        break;
    case 5:
        self->field_31087_g = 2;
        self->field_31086_h = 1;
        self->field_31083_k = 1;
        self->field_31082_l = 2;
        Block_setBlockBounds(var1, 12.0f / 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
        RenderBlocks_func_31077_c(
            self, (double)((float)var2 - 0.25f + 1.0f - var12),
            (double)((float)var2 - 0.25f + 1.0f), (double)((float)var3 + 6.0f / 16.0f),
            (double)((float)var3 + 6.0f / 16.0f), (double)((float)var4 + 10.0f / 16.0f),
            (double)((float)var4 + 6.0f / 16.0f), var11 * 0.5f, var13);
        RenderBlocks_func_31077_c(
            self, (double)((float)var2 - 0.25f + 1.0f - var12),
            (double)((float)var2 - 0.25f + 1.0f), (double)((float)var3 + 10.0f / 16.0f),
            (double)((float)var3 + 10.0f / 16.0f), (double)((float)var4 + 6.0f / 16.0f),
            (double)((float)var4 + 10.0f / 16.0f), var11, var13);
        RenderBlocks_func_31077_c(
            self, (double)((float)var2 - 0.25f + 1.0f - var12),
            (double)((float)var2 - 0.25f + 1.0f), (double)((float)var3 + 6.0f / 16.0f),
            (double)((float)var3 + 10.0f / 16.0f), (double)((float)var4 + 6.0f / 16.0f),
            (double)((float)var4 + 6.0f / 16.0f), var11 * 0.6f, var13);
        RenderBlocks_func_31077_c(
            self, (double)((float)var2 - 0.25f + 1.0f - var12),
            (double)((float)var2 - 0.25f + 1.0f), (double)((float)var3 + 10.0f / 16.0f),
            (double)((float)var3 + 6.0f / 16.0f), (double)((float)var4 + 10.0f / 16.0f),
            (double)((float)var4 + 10.0f / 16.0f), var11 * 0.6f, var13);
        break;
    }
    self->field_31087_g = 0;
    self->field_31086_h = 0;
    self->field_31085_i = 0;
    self->field_31084_j = 0;
    self->field_31083_k = 0;
    self->field_31082_l = 0;
    Block_setBlockBounds(var1, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    return 1;
}

void RenderBlocks_func_31079_a(RenderBlocks *self, Block *block, int x, int y, int z, int var5) {
    self->renderAllFaces = 1;
    RenderBlocks_func_31080_c(self, block, x, y, z, var5);
    self->renderAllFaces = 0;
}

extern int BlockRail_getIsPowered(Block *self);
int RenderBlocks_renderBlockMinecartTrack(RenderBlocks *self, Block *var1, int var2, int var3,
                                          int var4) {
    Tessellator *var5 = &Tessellator_instance;
    int var6 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    int var7 = var1->vtable->getBlockTextureFromSideAndMetadata(var1, 0, var6);
    if (self->overrideBlockTexture >= 0)
        var7 = self->overrideBlockTexture;
    if (BlockRail_getIsPowered(var1))
        var6 &= 7;
    float var8 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    Tessellator_setColorOpaque_F(var5, var8, var8, var8);
    int var9 = (var7 & 15) << 4;
    int var10 = var7 & 240;
    double var11 = (double)((float)var9 / 256.0f);
    double var13 = (double)(((float)var9 + 15.99f) / 256.0f);
    double var15 = (double)((float)var10 / 256.0f);
    double var17 = (double)(((float)var10 + 15.99f) / 256.0f);
    float var19 = 1.0f / 16.0f;
    float var20 = (float)(var2 + 1);
    float var21 = (float)(var2 + 1);
    float var22 = (float)(var2 + 0);
    float var23 = (float)(var2 + 0);
    float var24 = (float)(var4 + 0);
    float var25 = (float)(var4 + 1);
    float var26 = (float)(var4 + 1);
    float var27 = (float)(var4 + 0);
    float var28 = (float)var3 + var19;
    float var29 = (float)var3 + var19;
    float var30 = (float)var3 + var19;
    float var31 = (float)var3 + var19;
    if (var6 != 1 && var6 != 2 && var6 != 3 && var6 != 7) {
        if (var6 == 8) {
            var21 = (float)(var2 + 0);
            var20 = var21;
            var23 = (float)(var2 + 1);
            var22 = var23;
            var27 = (float)(var4 + 1);
            var24 = var27;
            var26 = (float)(var4 + 0);
            var25 = var26;
        } else if (var6 == 9) {
            var23 = (float)(var2 + 0);
            var20 = var23;
            var22 = (float)(var2 + 1);
            var21 = var22;
            var25 = (float)(var4 + 0);
            var24 = var25;
            var27 = (float)(var4 + 1);
            var26 = var27;
        }
    } else {
        var23 = (float)(var2 + 1);
        var20 = var23;
        var22 = (float)(var2 + 0);
        var21 = var22;
        var25 = (float)(var4 + 1);
        var24 = var25;
        var27 = (float)(var4 + 0);
        var26 = var27;
    }
    if (var6 != 2 && var6 != 4) {
        if (var6 == 3 || var6 == 5) {
            ++var29;
            ++var30;
        }
    } else {
        ++var28;
        ++var31;
    }
    Tessellator_addVertexWithUV(var5, (double)var20, (double)var28, (double)var24, var13, var15);
    Tessellator_addVertexWithUV(var5, (double)var21, (double)var29, (double)var25, var13, var17);
    Tessellator_addVertexWithUV(var5, (double)var22, (double)var30, (double)var26, var11, var17);
    Tessellator_addVertexWithUV(var5, (double)var23, (double)var31, (double)var27, var11, var15);
    Tessellator_addVertexWithUV(var5, (double)var23, (double)var31, (double)var27, var11, var15);
    Tessellator_addVertexWithUV(var5, (double)var22, (double)var30, (double)var26, var11, var17);
    Tessellator_addVertexWithUV(var5, (double)var21, (double)var29, (double)var25, var13, var17);
    Tessellator_addVertexWithUV(var5, (double)var20, (double)var28, (double)var24, var13, var15);
    return 1;
}

extern int BlockBed_getDirectionFromMetadata(int meta);
extern int BlockBed_isBlockFootOfBed(int meta);
extern const int ModelBed_field_22280_a[4];
extern const int ModelBed_field_22279_b[4];
static int RenderBlocks_renderBlockBed(RenderBlocks *self, Block *var1, int var2, int var3,
                                       int var4) {
    Tessellator *var5 = &Tessellator_instance;
    int var6 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    int var7 = BlockBed_getDirectionFromMetadata(var6);
    int var8 = BlockBed_isBlockFootOfBed(var6);
    float var9 = 0.5f;
    float var10 = 1.0f;
    float var11 = 0.8f;
    float var12 = 0.6f;
    float var25 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    Tessellator_setColorOpaque_F(var5, var9 * var25, var9 * var25, var9 * var25);
    int var26 = var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 0);
    int var27 = (var26 & 15) << 4;
    int var28 = var26 & 240;
    double var29 = (double)((float)var27 / 256.0f);
    double var31 = ((double)(var27 + 16) - 0.01) / 256.0;
    double var33 = (double)((float)var28 / 256.0f);
    double var35 = ((double)(var28 + 16) - 0.01) / 256.0;
    double var37 = (double)var2 + var1->minX;
    double var39 = (double)var2 + var1->maxX;
    double var41 = (double)var3 + var1->minY + 0.1875;
    double var43 = (double)var4 + var1->minZ;
    double var45 = (double)var4 + var1->maxZ;
    Tessellator_addVertexWithUV(var5, var37, var41, var45, var29, var35);
    Tessellator_addVertexWithUV(var5, var37, var41, var43, var29, var33);
    Tessellator_addVertexWithUV(var5, var39, var41, var43, var31, var33);
    Tessellator_addVertexWithUV(var5, var39, var41, var45, var31, var35);
    float var64 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3 + 1, var4);
    Tessellator_setColorOpaque_F(var5, var10 * var64, var10 * var64, var10 * var64);
    var27 = var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 1);
    var28 = (var27 & 15) << 4;
    int var67 = var27 & 240;
    double var30 = (double)((float)var28 / 256.0f);
    double var32 = ((double)(var28 + 16) - 0.01) / 256.0;
    double var34 = (double)((float)var67 / 256.0f);
    double var36 = ((double)(var67 + 16) - 0.01) / 256.0;
    double var38 = var30, var40 = var32, var42 = var34, var44 = var34;
    double var46 = var30, var48 = var32, var50 = var36, var52 = var36;
    if (var7 == 0) {
        var40 = var30;
        var42 = var36;
        var46 = var32;
        var52 = var34;
    } else if (var7 == 2) {
        var38 = var32;
        var44 = var36;
        var48 = var30;
        var50 = var34;
    } else if (var7 == 3) {
        var38 = var32;
        var44 = var36;
        var48 = var30;
        var50 = var34;
        var40 = var30;
        var42 = var36;
        var46 = var32;
        var52 = var34;
    }
    double var54 = (double)var2 + var1->minX;
    double var56 = (double)var2 + var1->maxX;
    double var58 = (double)var3 + var1->maxY;
    double var60 = (double)var4 + var1->minZ;
    double var62 = (double)var4 + var1->maxZ;
    Tessellator_addVertexWithUV(var5, var56, var58, var62, var46, var50);
    Tessellator_addVertexWithUV(var5, var56, var58, var60, var38, var42);
    Tessellator_addVertexWithUV(var5, var54, var58, var60, var40, var44);
    Tessellator_addVertexWithUV(var5, var54, var58, var62, var48, var52);
    var26 = ModelBed_field_22280_a[var7];
    if (var8)
        var26 = ModelBed_field_22280_a[ModelBed_field_22279_b[var7]];
    int var65 = 4;
    switch (var7) {
    case 0:
        var65 = 5;
        break;
    case 1:
        var65 = 3;
        break;
    case 3:
        var65 = 2;
        break;
    default:
        break;
    }
    float var66;
    if (var26 != 2 &&
        (self->renderAllFaces ||
         var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2, var3, var4 - 1, 2))) {
        var66 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4 - 1);
        if (var1->minZ > 0.0)
            var66 = var25;
        Tessellator_setColorOpaque_F(var5, var11 * var66, var11 * var66, var11 * var66);
        self->flipTexture = (var65 == 2);
        RenderBlocks_renderEastFace(
            self, var1, (double)var2, (double)var3, (double)var4,
            var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 2));
    }
    if (var26 != 3 &&
        (self->renderAllFaces ||
         var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2, var3, var4 + 1, 3))) {
        var66 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4 + 1);
        if (var1->maxZ < 1.0)
            var66 = var25;
        Tessellator_setColorOpaque_F(var5, var11 * var66, var11 * var66, var11 * var66);
        self->flipTexture = (var65 == 3);
        RenderBlocks_renderWestFace(
            self, var1, (double)var2, (double)var3, (double)var4,
            var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 3));
    }
    if (var26 != 4 &&
        (self->renderAllFaces ||
         var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2 - 1, var3, var4, 4))) {
        var66 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2 - 1, var3, var4);
        if (var1->minX > 0.0)
            var66 = var25;
        Tessellator_setColorOpaque_F(var5, var12 * var66, var12 * var66, var12 * var66);
        self->flipTexture = (var65 == 4);
        RenderBlocks_renderNorthFace(
            self, var1, (double)var2, (double)var3, (double)var4,
            var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 4));
    }
    if (var26 != 5 &&
        (self->renderAllFaces ||
         var1->vtable->shouldSideBeRendered(var1, self->blockAccess, var2 + 1, var3, var4, 5))) {
        var66 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2 + 1, var3, var4);
        if (var1->maxX < 1.0)
            var66 = var25;
        Tessellator_setColorOpaque_F(var5, var12 * var66, var12 * var66, var12 * var66);
        self->flipTexture = (var65 == 5);
        RenderBlocks_renderSouthFace(
            self, var1, (double)var2, (double)var3, (double)var4,
            var1->vtable->getBlockTexture(var1, self->blockAccess, var2, var3, var4, 5));
    }
    self->flipTexture = 0;
    return 1;
}

extern const double BlockRedstoneRepeater_field_22024_a[4];
static int RenderBlocks_renderBlockRepeater(RenderBlocks *self, Block *var1, int var2, int var3,
                                            int var4) {
    int var5 = self->blockAccess->vtable->getBlockMetadata(self->blockAccess, var2, var3, var4);
    int var6 = var5 & 3;
    int var7 = (var5 & 12) >> 2;
    RenderBlocks_renderStandardBlock(self, var1, var2, var3, var4);
    Tessellator *var8 = &Tessellator_instance;
    float var9 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var2, var3, var4);
    if (Block_lightValue[var1->blockID] > 0)
        var9 = (var9 + 1.0f) * 0.5f;
    Tessellator_setColorOpaque_F(var8, var9, var9, var9);
    double var10 = -0.1875;
    double var12 = 0.0, var14 = 0.0, var16 = 0.0, var18 = 0.0;
    switch (var6) {
    case 0:
        var18 = -0.3125;
        var14 = BlockRedstoneRepeater_field_22024_a[var7];
        break;
    case 1:
        var16 = 0.3125;
        var12 = -BlockRedstoneRepeater_field_22024_a[var7];
        break;
    case 2:
        var18 = 0.3125;
        var14 = -BlockRedstoneRepeater_field_22024_a[var7];
        break;
    case 3:
        var16 = -0.3125;
        var12 = BlockRedstoneRepeater_field_22024_a[var7];
        break;
    }
    RenderBlocks_renderTorchAtAngle(self, var1, (double)var2 + var12, (double)var3 + var10,
                                    (double)var4 + var14, 0.0, 0.0);
    RenderBlocks_renderTorchAtAngle(self, var1, (double)var2 + var16, (double)var3 + var10,
                                    (double)var4 + var18, 0.0, 0.0);
    int var20 = var1->vtable->getBlockTextureFromSide(var1, 1);
    int var21 = (var20 & 15) << 4;
    int var22 = var20 & 240;
    double var23 = (double)((float)var21 / 256.0f);
    double var25 = (double)(((float)var21 + 15.99f) / 256.0f);
    double var27 = (double)((float)var22 / 256.0f);
    double var29 = (double)(((float)var22 + 15.99f) / 256.0f);
    float var31 = 2.0f / 16.0f;
    float var32 = (float)(var2 + 1), var33 = (float)(var2 + 1);
    float var34 = (float)(var2 + 0), var35 = (float)(var2 + 0);
    float var36 = (float)(var4 + 0), var37 = (float)(var4 + 1);
    float var38 = (float)(var4 + 1), var39 = (float)(var4 + 0);
    float var40 = (float)var3 + var31;
    if (var6 == 2) {
        var33 = (float)(var2 + 0);
        var32 = var33;
        var35 = (float)(var2 + 1);
        var34 = var35;
        var39 = (float)(var4 + 1);
        var36 = var39;
        var38 = (float)(var4 + 0);
        var37 = var38;
    } else if (var6 == 3) {
        var35 = (float)(var2 + 0);
        var32 = var35;
        var34 = (float)(var2 + 1);
        var33 = var34;
        var37 = (float)(var4 + 0);
        var36 = var37;
        var39 = (float)(var4 + 1);
        var38 = var39;
    } else if (var6 == 1) {
        var35 = (float)(var2 + 1);
        var32 = var35;
        var34 = (float)(var2 + 0);
        var33 = var34;
        var37 = (float)(var4 + 1);
        var36 = var37;
        var39 = (float)(var4 + 0);
        var38 = var39;
    }
    Tessellator_addVertexWithUV(var8, (double)var35, (double)var40, (double)var39, var23, var27);
    Tessellator_addVertexWithUV(var8, (double)var34, (double)var40, (double)var38, var23, var29);
    Tessellator_addVertexWithUV(var8, (double)var33, (double)var40, (double)var37, var25, var29);
    Tessellator_addVertexWithUV(var8, (double)var32, (double)var40, (double)var36, var25, var27);
    return 1;
}

int RenderBlocks_renderBlockByRenderType(RenderBlocks *self, Block *block, int x, int y, int z) {
    int rt = block->vtable->getRenderType(block);
    block->vtable->setBlockBoundsBasedOnState(block, self->blockAccess, x, y, z);
    switch (rt) {
    case 0:
        return RenderBlocks_renderStandardBlock(self, block, x, y, z);
    case 4:
        return RenderBlocks_renderBlockFluids(self, block, x, y, z);
    case 13:
        return RenderBlocks_renderBlockCactus(self, block, x, y, z);
    case 1:
        return RenderBlocks_renderBlockReed(self, block, x, y, z);
    case 6:
        return RenderBlocks_renderBlockCrops(self, block, x, y, z);
    case 2:
        return RenderBlocks_renderBlockTorch(self, block, x, y, z);
    case 3:
        return RenderBlocks_renderBlockFire(self, block, x, y, z);
    case 5:
        return RenderBlocks_renderBlockRedstoneWire(self, block, x, y, z);
    case 8:
        return RenderBlocks_renderBlockLadder(self, block, x, y, z);
    case 7:
        return RenderBlocks_renderBlockDoor(self, block, x, y, z);
    case 9:
        return RenderBlocks_renderBlockMinecartTrack(self, block, x, y, z);
    case 10:
        return RenderBlocks_renderBlockStairs(self, block, x, y, z);
    case 11:
        return RenderBlocks_renderBlockFence(self, block, x, y, z);
    case 12:
        return RenderBlocks_renderBlockLever(self, block, x, y, z);
    case 14:
        return RenderBlocks_renderBlockBed(self, block, x, y, z);
    case 15:
        return RenderBlocks_renderBlockRepeater(self, block, x, y, z);
    case 16:
        return RenderBlocks_func_31074_b(self, block, x, y, z, 0);
    case 17:
        return RenderBlocks_func_31080_c(self, block, x, y, z, 1);
    default:
        return 0;
    }
}

void RenderBlocks_func_1245_b(RenderBlocks *self, Block *block, int meta, double var3, double var5,
                              double var7) {
    Tessellator *var9 = &Tessellator_instance;
    int var10 = block->vtable->getBlockTextureFromSideAndMetadata(block, 0, meta);
    if (self->overrideBlockTexture >= 0) {
        var10 = self->overrideBlockTexture;
    }
    int var11 = (var10 & 15) << 4;
    int var12 = var10 & 240;
    double var13 = (double)((float)var11 / 256.0f);
    double var15 = (double)(((float)var11 + 15.99f) / 256.0f);
    double var17 = (double)((float)var12 / 256.0f);
    double var19 = (double)(((float)var12 + 15.99f) / 256.0f);
    double var21 = var3 + 0.5 - 0.25;
    double var23 = var3 + 0.5 + 0.25;
    double var25 = var7 + 0.5 - 0.5;
    double var27 = var7 + 0.5 + 0.5;
    Tessellator_addVertexWithUV(var9, var21, var5 + 1.0, var25, var13, var17);
    Tessellator_addVertexWithUV(var9, var21, var5 + 0.0, var25, var13, var19);
    Tessellator_addVertexWithUV(var9, var21, var5 + 0.0, var27, var15, var19);
    Tessellator_addVertexWithUV(var9, var21, var5 + 1.0, var27, var15, var17);
    Tessellator_addVertexWithUV(var9, var21, var5 + 1.0, var27, var13, var17);
    Tessellator_addVertexWithUV(var9, var21, var5 + 0.0, var27, var13, var19);
    Tessellator_addVertexWithUV(var9, var21, var5 + 0.0, var25, var15, var19);
    Tessellator_addVertexWithUV(var9, var21, var5 + 1.0, var25, var15, var17);
    Tessellator_addVertexWithUV(var9, var23, var5 + 1.0, var27, var13, var17);
    Tessellator_addVertexWithUV(var9, var23, var5 + 0.0, var27, var13, var19);
    Tessellator_addVertexWithUV(var9, var23, var5 + 0.0, var25, var15, var19);
    Tessellator_addVertexWithUV(var9, var23, var5 + 1.0, var25, var15, var17);
    Tessellator_addVertexWithUV(var9, var23, var5 + 1.0, var25, var13, var17);
    Tessellator_addVertexWithUV(var9, var23, var5 + 0.0, var25, var13, var19);
    Tessellator_addVertexWithUV(var9, var23, var5 + 0.0, var27, var15, var19);
    Tessellator_addVertexWithUV(var9, var23, var5 + 1.0, var27, var15, var17);
    var21 = var3 + 0.5 - 0.5;
    var23 = var3 + 0.5 + 0.5;
    var25 = var7 + 0.5 - 0.25;
    var27 = var7 + 0.5 + 0.25;
    Tessellator_addVertexWithUV(var9, var21, var5 + 1.0, var25, var13, var17);
    Tessellator_addVertexWithUV(var9, var21, var5 + 0.0, var25, var13, var19);
    Tessellator_addVertexWithUV(var9, var23, var5 + 0.0, var25, var15, var19);
    Tessellator_addVertexWithUV(var9, var23, var5 + 1.0, var25, var15, var17);
    Tessellator_addVertexWithUV(var9, var23, var5 + 1.0, var25, var13, var17);
    Tessellator_addVertexWithUV(var9, var23, var5 + 0.0, var25, var13, var19);
    Tessellator_addVertexWithUV(var9, var21, var5 + 0.0, var25, var15, var19);
    Tessellator_addVertexWithUV(var9, var21, var5 + 1.0, var25, var15, var17);
    Tessellator_addVertexWithUV(var9, var23, var5 + 1.0, var27, var13, var17);
    Tessellator_addVertexWithUV(var9, var23, var5 + 0.0, var27, var13, var19);
    Tessellator_addVertexWithUV(var9, var21, var5 + 0.0, var27, var15, var19);
    Tessellator_addVertexWithUV(var9, var21, var5 + 1.0, var27, var15, var17);
    Tessellator_addVertexWithUV(var9, var21, var5 + 1.0, var27, var13, var17);
    Tessellator_addVertexWithUV(var9, var21, var5 + 0.0, var27, var13, var19);
    Tessellator_addVertexWithUV(var9, var23, var5 + 0.0, var27, var15, var19);
    Tessellator_addVertexWithUV(var9, var23, var5 + 1.0, var27, var15, var17);
}

void RenderBlocks_renderBlockOnInventory(RenderBlocks *self, Block *block, int meta,
                                         float brightness) {
    Tessellator *t = &Tessellator_instance;
    int var5;
    float var6, var7;

    if (self->field_31088_b) {
        var5 = block->vtable->getRenderColor(block, meta);
        var6 = (float)(var5 >> 16 & 255) / 255.0f;
        var7 = (float)(var5 >> 8 & 255) / 255.0f;
        float var8 = (float)(var5 & 255) / 255.0f;
        R_color4f(var6 * brightness, var7 * brightness, var8 * brightness, 1.0f);
    }

    var5 = block->vtable->getRenderType(block);
    if (var5 != 0 && var5 != 16) {
        if (var5 == 1) {

            Tessellator_startDrawingQuads(t);
            Tessellator_setNormal(t, 0.0f, -1.0f, 0.0f);
            RenderBlocks_renderCrossedSquares(self, block, meta, -0.5, -0.5, -0.5);
            Tessellator_draw(t);
        } else if (var5 == 13) {

            block->vtable->setBlockBoundsForItemRender(block);
            R_translatef(-0.5f, -0.5f, -0.5f);
            var6 = 1.0f / 16.0f;
            Tessellator_startDrawingQuads(t);
            Tessellator_setNormal(t, 0.0f, -1.0f, 0.0f);
            RenderBlocks_renderBottomFace(self, block, 0.0, 0.0, 0.0,
                                          block->vtable->getBlockTextureFromSide(block, 0));
            Tessellator_draw(t);
            Tessellator_startDrawingQuads(t);
            Tessellator_setNormal(t, 0.0f, 1.0f, 0.0f);
            RenderBlocks_renderTopFace(self, block, 0.0, 0.0, 0.0,
                                       block->vtable->getBlockTextureFromSide(block, 1));
            Tessellator_draw(t);
            Tessellator_startDrawingQuads(t);
            Tessellator_setNormal(t, 0.0f, 0.0f, -1.0f);
            Tessellator_setTranslationF(t, 0.0f, 0.0f, var6);
            RenderBlocks_renderEastFace(self, block, 0.0, 0.0, 0.0,
                                        block->vtable->getBlockTextureFromSide(block, 2));
            Tessellator_setTranslationF(t, 0.0f, 0.0f, -var6);
            Tessellator_draw(t);
            Tessellator_startDrawingQuads(t);
            Tessellator_setNormal(t, 0.0f, 0.0f, 1.0f);
            Tessellator_setTranslationF(t, 0.0f, 0.0f, -var6);
            RenderBlocks_renderWestFace(self, block, 0.0, 0.0, 0.0,
                                        block->vtable->getBlockTextureFromSide(block, 3));
            Tessellator_setTranslationF(t, 0.0f, 0.0f, var6);
            Tessellator_draw(t);
            Tessellator_startDrawingQuads(t);
            Tessellator_setNormal(t, -1.0f, 0.0f, 0.0f);
            Tessellator_setTranslationF(t, var6, 0.0f, 0.0f);
            RenderBlocks_renderNorthFace(self, block, 0.0, 0.0, 0.0,
                                         block->vtable->getBlockTextureFromSide(block, 4));
            Tessellator_setTranslationF(t, -var6, 0.0f, 0.0f);
            Tessellator_draw(t);
            Tessellator_startDrawingQuads(t);
            Tessellator_setNormal(t, 1.0f, 0.0f, 0.0f);
            Tessellator_setTranslationF(t, -var6, 0.0f, 0.0f);
            RenderBlocks_renderSouthFace(self, block, 0.0, 0.0, 0.0,
                                         block->vtable->getBlockTextureFromSide(block, 5));
            Tessellator_setTranslationF(t, var6, 0.0f, 0.0f);
            Tessellator_draw(t);
            R_translatef(0.5f, 0.5f, 0.5f);
        } else if (var5 == 6) {

            Tessellator_startDrawingQuads(t);
            Tessellator_setNormal(t, 0.0f, -1.0f, 0.0f);
            RenderBlocks_func_1245_b(self, block, meta, -0.5, -0.5, -0.5);
            Tessellator_draw(t);
        } else if (var5 == 2) {

            Tessellator_startDrawingQuads(t);
            Tessellator_setNormal(t, 0.0f, -1.0f, 0.0f);
            RenderBlocks_renderTorchAtAngle(self, block, -0.5, -0.5, -0.5, 0.0, 0.0);
            Tessellator_draw(t);
        } else {
            int var9;
            if (var5 == 10) {

                for (var9 = 0; var9 < 2; ++var9) {
                    if (var9 == 0) {
                        Block_setBlockBounds(block, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f);
                    }
                    if (var9 == 1) {
                        Block_setBlockBounds(block, 0.0f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f);
                    }
                    R_translatef(-0.5f, -0.5f, -0.5f);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, 0.0f, -1.0f, 0.0f);
                    RenderBlocks_renderBottomFace(self, block, 0.0, 0.0, 0.0,
                                                  block->vtable->getBlockTextureFromSide(block, 0));
                    Tessellator_draw(t);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, 0.0f, 1.0f, 0.0f);
                    RenderBlocks_renderTopFace(self, block, 0.0, 0.0, 0.0,
                                               block->vtable->getBlockTextureFromSide(block, 1));
                    Tessellator_draw(t);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, 0.0f, 0.0f, -1.0f);
                    RenderBlocks_renderEastFace(self, block, 0.0, 0.0, 0.0,
                                                block->vtable->getBlockTextureFromSide(block, 2));
                    Tessellator_draw(t);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, 0.0f, 0.0f, 1.0f);
                    RenderBlocks_renderWestFace(self, block, 0.0, 0.0, 0.0,
                                                block->vtable->getBlockTextureFromSide(block, 3));
                    Tessellator_draw(t);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, -1.0f, 0.0f, 0.0f);
                    RenderBlocks_renderNorthFace(self, block, 0.0, 0.0, 0.0,
                                                 block->vtable->getBlockTextureFromSide(block, 4));
                    Tessellator_draw(t);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, 1.0f, 0.0f, 0.0f);
                    RenderBlocks_renderSouthFace(self, block, 0.0, 0.0, 0.0,
                                                 block->vtable->getBlockTextureFromSide(block, 5));
                    Tessellator_draw(t);
                    R_translatef(0.5f, 0.5f, 0.5f);
                }
            } else if (var5 == 11) {

                for (var9 = 0; var9 < 4; ++var9) {
                    var7 = 2.0f / 16.0f;
                    if (var9 == 0) {
                        Block_setBlockBounds(block, 0.5f - var7, 0.0f, 0.0f, 0.5f + var7, 1.0f,
                                             var7 * 2.0f);
                    }
                    if (var9 == 1) {
                        Block_setBlockBounds(block, 0.5f - var7, 0.0f, 1.0f - var7 * 2.0f,
                                             0.5f + var7, 1.0f, 1.0f);
                    }
                    var7 = 1.0f / 16.0f;
                    if (var9 == 2) {
                        Block_setBlockBounds(block, 0.5f - var7, 1.0f - var7 * 3.0f, -var7 * 2.0f,
                                             0.5f + var7, 1.0f - var7, 1.0f + var7 * 2.0f);
                    }
                    if (var9 == 3) {
                        Block_setBlockBounds(block, 0.5f - var7, 0.5f - var7 * 3.0f, -var7 * 2.0f,
                                             0.5f + var7, 0.5f - var7, 1.0f + var7 * 2.0f);
                    }
                    R_translatef(-0.5f, -0.5f, -0.5f);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, 0.0f, -1.0f, 0.0f);
                    RenderBlocks_renderBottomFace(self, block, 0.0, 0.0, 0.0,
                                                  block->vtable->getBlockTextureFromSide(block, 0));
                    Tessellator_draw(t);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, 0.0f, 1.0f, 0.0f);
                    RenderBlocks_renderTopFace(self, block, 0.0, 0.0, 0.0,
                                               block->vtable->getBlockTextureFromSide(block, 1));
                    Tessellator_draw(t);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, 0.0f, 0.0f, -1.0f);
                    RenderBlocks_renderEastFace(self, block, 0.0, 0.0, 0.0,
                                                block->vtable->getBlockTextureFromSide(block, 2));
                    Tessellator_draw(t);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, 0.0f, 0.0f, 1.0f);
                    RenderBlocks_renderWestFace(self, block, 0.0, 0.0, 0.0,
                                                block->vtable->getBlockTextureFromSide(block, 3));
                    Tessellator_draw(t);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, -1.0f, 0.0f, 0.0f);
                    RenderBlocks_renderNorthFace(self, block, 0.0, 0.0, 0.0,
                                                 block->vtable->getBlockTextureFromSide(block, 4));
                    Tessellator_draw(t);
                    Tessellator_startDrawingQuads(t);
                    Tessellator_setNormal(t, 1.0f, 0.0f, 0.0f);
                    RenderBlocks_renderSouthFace(self, block, 0.0, 0.0, 0.0,
                                                 block->vtable->getBlockTextureFromSide(block, 5));
                    Tessellator_draw(t);
                    R_translatef(0.5f, 0.5f, 0.5f);
                }
                Block_setBlockBounds(block, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
            }
        }
    } else {

        if (var5 == 16) {
            meta = 1;
        }
        block->vtable->setBlockBoundsForItemRender(block);
        R_translatef(-0.5f, -0.5f, -0.5f);
        Tessellator_startDrawingQuads(t);
        Tessellator_setNormal(t, 0.0f, -1.0f, 0.0f);
        RenderBlocks_renderBottomFace(
            self, block, 0.0, 0.0, 0.0,
            block->vtable->getBlockTextureFromSideAndMetadata(block, 0, meta));
        Tessellator_draw(t);
        Tessellator_startDrawingQuads(t);
        Tessellator_setNormal(t, 0.0f, 1.0f, 0.0f);
        RenderBlocks_renderTopFace(
            self, block, 0.0, 0.0, 0.0,
            block->vtable->getBlockTextureFromSideAndMetadata(block, 1, meta));
        Tessellator_draw(t);
        Tessellator_startDrawingQuads(t);
        Tessellator_setNormal(t, 0.0f, 0.0f, -1.0f);
        RenderBlocks_renderEastFace(
            self, block, 0.0, 0.0, 0.0,
            block->vtable->getBlockTextureFromSideAndMetadata(block, 2, meta));
        Tessellator_draw(t);
        Tessellator_startDrawingQuads(t);
        Tessellator_setNormal(t, 0.0f, 0.0f, 1.0f);
        RenderBlocks_renderWestFace(
            self, block, 0.0, 0.0, 0.0,
            block->vtable->getBlockTextureFromSideAndMetadata(block, 3, meta));
        Tessellator_draw(t);
        Tessellator_startDrawingQuads(t);
        Tessellator_setNormal(t, -1.0f, 0.0f, 0.0f);
        RenderBlocks_renderNorthFace(
            self, block, 0.0, 0.0, 0.0,
            block->vtable->getBlockTextureFromSideAndMetadata(block, 4, meta));
        Tessellator_draw(t);
        Tessellator_startDrawingQuads(t);
        Tessellator_setNormal(t, 1.0f, 0.0f, 0.0f);
        RenderBlocks_renderSouthFace(
            self, block, 0.0, 0.0, 0.0,
            block->vtable->getBlockTextureFromSideAndMetadata(block, 5, meta));
        Tessellator_draw(t);
        R_translatef(0.5f, 0.5f, 0.5f);
    }
}

void RenderBlocks_renderBlockFallingSand(RenderBlocks *self, Block *var1, struct World *var2,
                                         int var3, int var4, int var5) {

    IBlockAccess *savedAccess = self->blockAccess;
    self->blockAccess = (IBlockAccess *)var2;
    float var6 = 0.5F;
    float var7 = 1.0F;
    float var8 = 0.8F;
    float var9 = 0.6F;
    Tessellator *var10 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var10);
    float var11 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var3, var4, var5);
    float var12 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var3, var4 - 1, var5);
    if (var12 < var11) {
        var12 = var11;
    }
    Tessellator_setColorOpaque_F(var10, var6 * var12, var6 * var12, var6 * var12);
    RenderBlocks_renderBottomFace(self, var1, -0.5, -0.5, -0.5,
                                  var1->vtable->getBlockTextureFromSide(var1, 0));
    var12 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var3, var4 + 1, var5);
    if (var12 < var11) {
        var12 = var11;
    }
    Tessellator_setColorOpaque_F(var10, var7 * var12, var7 * var12, var7 * var12);
    RenderBlocks_renderTopFace(self, var1, -0.5, -0.5, -0.5,
                               var1->vtable->getBlockTextureFromSide(var1, 1));
    var12 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var3, var4, var5 - 1);
    if (var12 < var11) {
        var12 = var11;
    }
    Tessellator_setColorOpaque_F(var10, var8 * var12, var8 * var12, var8 * var12);
    RenderBlocks_renderEastFace(self, var1, -0.5, -0.5, -0.5,
                                var1->vtable->getBlockTextureFromSide(var1, 2));
    var12 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var3, var4, var5 + 1);
    if (var12 < var11) {
        var12 = var11;
    }
    Tessellator_setColorOpaque_F(var10, var8 * var12, var8 * var12, var8 * var12);
    RenderBlocks_renderWestFace(self, var1, -0.5, -0.5, -0.5,
                                var1->vtable->getBlockTextureFromSide(var1, 3));
    var12 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var3 - 1, var4, var5);
    if (var12 < var11) {
        var12 = var11;
    }
    Tessellator_setColorOpaque_F(var10, var9 * var12, var9 * var12, var9 * var12);
    RenderBlocks_renderNorthFace(self, var1, -0.5, -0.5, -0.5,
                                 var1->vtable->getBlockTextureFromSide(var1, 4));
    var12 = var1->vtable->getBlockBrightness(var1, self->blockAccess, var3 + 1, var4, var5);
    if (var12 < var11) {
        var12 = var11;
    }
    Tessellator_setColorOpaque_F(var10, var9 * var12, var9 * var12, var9 * var12);
    RenderBlocks_renderSouthFace(self, var1, -0.5, -0.5, -0.5,
                                 var1->vtable->getBlockTextureFromSide(var1, 5));
    Tessellator_draw(var10);
    self->blockAccess = savedAccess;
}
