#ifndef WORLD_GEN_BIG_TREE_H
#define WORLD_GEN_BIG_TREE_H

#include "WorldGenerator.h"
#include "JavaRandom.h"

typedef struct WorldGenBigTree {
    WorldGenerator base;

    JavaRandom field_881_b;
    World *worldObj;
    int basePos[3];
    int field_878_e;
    int height;
    double field_876_g;
    double field_875_h;
    double field_874_i;
    double field_873_j;
    double field_872_k;
    int field_871_l;
    int field_870_m;
    int field_869_n;
    int (*field_868_o)[4];
    int field_868_o_count;
} WorldGenBigTree;

extern const int WorldGenBigTree_field_882_a[6];

WorldGenBigTree *WorldGenBigTree_create(void);

#endif
