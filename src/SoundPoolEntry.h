#ifndef SOUND_POOL_ENTRY_H
#define SOUND_POOL_ENTRY_H

typedef struct SoundPoolEntry {

    char soundName[256];

    char soundUrl[2048];
} SoundPoolEntry;

void SoundPoolEntry_init(SoundPoolEntry *self, const char *soundName, const char *soundUrl);

#endif
