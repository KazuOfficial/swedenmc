#ifndef SAVE_OLD_DIR_H
#define SAVE_OLD_DIR_H

#include "SaveHandler.h"

typedef struct SaveOldDir {
    SaveHandler base;
} SaveOldDir;

SaveOldDir *SaveOldDir_create(const char *savesDir, const char *worldName, int createLock);

#endif
