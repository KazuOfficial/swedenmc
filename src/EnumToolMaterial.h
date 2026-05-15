#ifndef ENUM_TOOL_MATERIAL_H
#define ENUM_TOOL_MATERIAL_H

typedef enum {
    TOOL_WOOD = 0,
    TOOL_STONE = 1,
    TOOL_IRON = 2,
    TOOL_EMERALD = 3,
    TOOL_GOLD = 4
} EnumToolMaterial;

static inline int EnumToolMaterial_getHarvestLevel(EnumToolMaterial m) {
    static const int t[] = {0, 1, 2, 3, 0};
    return t[m];
}
static inline int EnumToolMaterial_getMaxUses(EnumToolMaterial m) {
    static const int t[] = {59, 131, 250, 1561, 32};
    return t[m];
}
static inline float EnumToolMaterial_getEfficiencyOnProperMaterial(EnumToolMaterial m) {
    static const float t[] = {2.0f, 4.0f, 6.0f, 8.0f, 12.0f};
    return t[m];
}
static inline int EnumToolMaterial_getDamageVsEntity(EnumToolMaterial m) {
    static const int t[] = {0, 1, 2, 3, 0};
    return t[m];
}

#endif
