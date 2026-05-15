#ifndef WORLD_BLOCK_POSITION_TYPE_H
#define WORLD_BLOCK_POSITION_TYPE_H

struct WorldClient;
typedef struct WorldClient WorldClient;

typedef struct WorldBlockPositionType {
    int field_1202_a;
    int field_1201_b;
    int field_1207_c;
    int field_1206_d;
    int field_1205_e;
    int field_1204_f;
    WorldClient *field_1203_g;
} WorldBlockPositionType;

void WorldBlockPositionType_init(WorldBlockPositionType *self, WorldClient *var1, int var2,
                                 int var3, int var4, int var5, int var6);

#endif
