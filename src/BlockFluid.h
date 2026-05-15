#ifndef BLOCKFLUID_H
#define BLOCKFLUID_H

#include "Block.h"
#include "IBlockAccess.h"
#include "World.h"
#include "Material.h"

void BlockFluid_initVtable(BlockVtable *vt);

float BlockFluid_getPercentAir(int var0);

int BlockFluid_getFlowDecay(Block *self, World *var1, int var2, int var3, int var4);

int BlockFluid_getEffectiveFlowDecay(Block *self, IBlockAccess *var1, int var2, int var3, int var4);

float BlockFluid_getBlockBrightness(Block *self, IBlockAccess *var1, int var2, int var3, int var4);

double BlockFluid_func_293_a(IBlockAccess *var0, int var1, int var2, int var3, Material *var4);

int BlockFluid_colorMultiplier(Block *self, IBlockAccess *var1, int var2, int var3, int var4);

void BlockFluid_triggerLavaMixEffects(Block *self, World *var1, int var2, int var3, int var4);

void BlockFluid_onBlockAdded(Block *self, World *var1, int var2, int var3, int var4);

void BlockFluid_onNeighborBlockChange(Block *self, World *var1, int var2, int var3, int var4,
                                      int var5);

#endif
