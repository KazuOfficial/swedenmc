#ifndef REDSTONE_UPDATE_INFO_H
#define REDSTONE_UPDATE_INFO_H

#include <stdint.h>

typedef struct RedstoneUpdateInfo {
    int x;
    int y;
    int z;
    int64_t updateTime;
} RedstoneUpdateInfo;

RedstoneUpdateInfo *RedstoneUpdateInfo_create(int var1, int var2, int var3, int64_t var4);

#endif
