#include "RedstoneUpdateInfo.h"
#include <stdlib.h>

RedstoneUpdateInfo *RedstoneUpdateInfo_create(int var1, int var2, int var3, int64_t var4) {
    RedstoneUpdateInfo *self = (RedstoneUpdateInfo *)calloc(1, sizeof(RedstoneUpdateInfo));
    self->x = var1;
    self->y = var2;
    self->z = var3;
    self->updateTime = var4;
    return self;
}
