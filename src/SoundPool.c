#include "SoundPool.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

static unsigned int sp_hash(const char *s) {
    unsigned int h = 5381;
    while (*s)
        h = h * 33 ^ (unsigned char)*s++;
    return h % SOUNDPOOL_BUCKETS;
}

void SoundPool_init(SoundPool *self) {
    JavaRandom_init(&self->rand, 0);
    memset(self->buckets, 0, sizeof(self->buckets));
    self->allSoundPoolEntries = NULL;
    self->allCount = 0;
    self->allCapacity = 0;
    self->numberOfSoundPoolEntries = 0;
    self->field_1657_b = 1;
}

void SoundPool_free(SoundPool *self) {
    int i;
    for (i = 0; i < SOUNDPOOL_BUCKETS; i++) {
        SoundPoolMapNode *node = self->buckets[i].head;
        while (node) {
            SoundPoolMapNode *next = node->next;
            free(node->entry);
            free(node);
            node = next;
        }
    }
    free(self->allSoundPoolEntries);
}

SoundPoolEntry *SoundPool_addSound(SoundPool *self, const char *var1, const char *var2) {
    if (!var1 || !var2)
        return NULL;
    char var3[256];
    char key[512];
    int dot, len, i;

    strncpy(var3, var1, sizeof(var3) - 1);
    var3[sizeof(var3) - 1] = '\0';

    dot = -1;
    len = (int)strlen(var1);
    for (i = 0; i < len; i++) {
        if (var1[i] == '.') {
            dot = i;
            break;
        }
    }
    if (dot < 0)
        dot = len;
    strncpy(key, var1, (size_t)dot);
    key[dot] = '\0';

    if (self->field_1657_b) {
        int klen = (int)strlen(key);
        while (klen > 0 && isdigit((unsigned char)key[klen - 1])) {
            key[--klen] = '\0';
        }
    }

    for (i = 0; key[i]; i++) {
        if (key[i] == '/')
            key[i] = '.';
    }

    unsigned int h = sp_hash(key);
    SoundPoolBucket *bucket = &self->buckets[h];
    if (!bucket->used) {
        strncpy(bucket->key, key, sizeof(bucket->key) - 1);
        bucket->key[sizeof(bucket->key) - 1] = '\0';
        bucket->used = 1;
        bucket->head = NULL;
        bucket->count = 0;
    }

    int tries = 0;
    while (bucket->used && strcmp(bucket->key, key) != 0 && tries < SOUNDPOOL_BUCKETS) {
        h = (h + 1) % SOUNDPOOL_BUCKETS;
        bucket = &self->buckets[h];
        tries++;
    }
    if (!bucket->used) {
        strncpy(bucket->key, key, sizeof(bucket->key) - 1);
        bucket->key[sizeof(bucket->key) - 1] = '\0';
        bucket->used = 1;
        bucket->head = NULL;
        bucket->count = 0;
    }

    SoundPoolEntry *var4 = (SoundPoolEntry *)calloc(1, sizeof(SoundPoolEntry));
    SoundPoolEntry_init(var4, var3, var2);

    SoundPoolMapNode *node = (SoundPoolMapNode *)calloc(1, sizeof(SoundPoolMapNode));
    node->entry = var4;
    node->next = bucket->head;
    bucket->head = node;
    bucket->count++;

    if (self->allCount >= self->allCapacity) {
        int newCap = self->allCapacity == 0 ? 16 : self->allCapacity * 2;
        self->allSoundPoolEntries = (SoundPoolEntry **)realloc(
            self->allSoundPoolEntries, (size_t)newCap * sizeof(SoundPoolEntry *));
        self->allCapacity = newCap;
    }
    self->allSoundPoolEntries[self->allCount++] = var4;

    ++self->numberOfSoundPoolEntries;
    return var4;
}

SoundPoolEntry *SoundPool_getRandomSoundFromSoundPool(SoundPool *self, const char *var1) {
    if (!var1)
        return NULL;
    unsigned int h = sp_hash(var1);
    int tries = 0;
    while (tries < SOUNDPOOL_BUCKETS) {
        SoundPoolBucket *bucket = &self->buckets[h];
        if (!bucket->used)
            return NULL;
        if (strcmp(bucket->key, var1) == 0) {
            if (bucket->count == 0)
                return NULL;
            int idx = JavaRandom_nextInt(&self->rand, bucket->count);

            SoundPoolMapNode *node = bucket->head;
            for (int i = 0; i < idx && node->next; i++)
                node = node->next;
            return node->entry;
        }
        h = (h + 1) % SOUNDPOOL_BUCKETS;
        tries++;
    }
    return NULL;
}

SoundPoolEntry *SoundPool_getRandomSound(SoundPool *self) {
    if (self->allCount == 0)
        return NULL;
    int idx = JavaRandom_nextInt(&self->rand, self->allCount);
    return self->allSoundPoolEntries[idx];
}
