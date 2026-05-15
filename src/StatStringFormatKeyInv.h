#ifndef STAT_STRING_FORMAT_KEY_INV_H
#define STAT_STRING_FORMAT_KEY_INV_H

#include "IStatStringFormat.h"

struct Minecraft;

typedef struct StatStringFormatKeyInv {
    IStatStringFormat base;

    struct Minecraft *mc;
} StatStringFormatKeyInv;

StatStringFormatKeyInv *StatStringFormatKeyInv_create(struct Minecraft *mc);

#endif
