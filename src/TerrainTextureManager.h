#ifndef TERRAIN_TEXTURE_MANAGER_H
#define TERRAIN_TEXTURE_MANAGER_H

struct IsoImageBuffer;

typedef struct TerrainTextureManager {

    float field_1181_a[768];

    int field_1180_b[5120];

    int field_1186_c[5120];

    int field_1185_d[5120];

    int field_1184_e[5120];

    int field_1183_f[34];

    int field_1182_g[768];
} TerrainTextureManager;

TerrainTextureManager *TerrainTextureManager_create(void);
void TerrainTextureManager_destroy(TerrainTextureManager *self);

void TerrainTextureManager_func_799_a(TerrainTextureManager *self, struct IsoImageBuffer *var1);

#endif
