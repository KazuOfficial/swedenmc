#ifndef SAVE_HANDLER_H
#define SAVE_HANDLER_H

#include "ISaveHandler.h"
#include "WorldInfo.h"
#include <stdint.h>

typedef struct SaveHandler {
    ISaveHandler base;

    char *saveDirectory;

    char *playersDirectory;

    char *field_28114_d;

    int64_t now;
} SaveHandler;

SaveHandler *SaveHandler_create(const char *savesDir, const char *worldName, int createLock);
void SaveHandler_destroy(SaveHandler *self);

const char *SaveHandler_getSaveDirectory(SaveHandler *self);

extern const ISaveHandlerVtable SaveHandler_vtable;

#endif
