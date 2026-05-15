#ifndef SOUND_POOL_H
#define SOUND_POOL_H

#include "SoundPoolEntry.h"
#include "JavaRandom.h"

typedef struct SoundPoolMapNode {
    SoundPoolEntry *entry;
    struct SoundPoolMapNode *next;
} SoundPoolMapNode;

typedef struct SoundPoolBucket {
    char key[512];
    int used;
    SoundPoolMapNode *head;
    int count;
} SoundPoolBucket;

#define SOUNDPOOL_BUCKETS 256

typedef struct SoundPool {

    JavaRandom rand;

    SoundPoolBucket buckets[SOUNDPOOL_BUCKETS];

    SoundPoolEntry **allSoundPoolEntries;
    int allCount;
    int allCapacity;

    int numberOfSoundPoolEntries;

    int field_1657_b;
} SoundPool;

void SoundPool_init(SoundPool *self);
void SoundPool_free(SoundPool *self);

SoundPoolEntry *SoundPool_addSound(SoundPool *self, const char *var1, const char *var2);

SoundPoolEntry *SoundPool_getRandomSoundFromSoundPool(SoundPool *self, const char *var1);

SoundPoolEntry *SoundPool_getRandomSound(SoundPool *self);

#endif
