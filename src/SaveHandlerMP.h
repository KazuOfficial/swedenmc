#ifndef SAVE_HANDLER_MP_H
#define SAVE_HANDLER_MP_H

#include "ISaveHandler.h"

typedef struct SaveHandlerMP {
    ISaveHandler base;
} SaveHandlerMP;

SaveHandlerMP *SaveHandlerMP_create(void);

#endif
