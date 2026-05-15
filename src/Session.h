#ifndef SESSION_H
#define SESSION_H

#include "Block.h"

typedef struct Session {
    char username[64];
    char sessionId[2048];
    char uuid[64];
    char mpPassParameter[256];
} Session;

extern Block *Session_registeredBlocksList[28];
#define SESSION_REGISTERED_BLOCKS_COUNT 28

Session *Session_create(const char *username, const char *sessionId);
void Session_free(Session *self);

void Session_initRegisteredBlocksList(void);

#endif
