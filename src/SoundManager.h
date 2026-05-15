#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "GameSettings.h"
#include "SoundPool.h"
#include "JavaRandom.h"
#include <AL/al.h>
#include <AL/alc.h>

struct EntityLiving;

typedef struct {
    char path[2048];
    ALuint buf;
} SoundBufEntry;

#define SOUNDMANAGER_POOL_SIZE 256

typedef struct SoundManager {

    SoundPool soundPoolSounds;

    SoundPool soundPoolStreaming;

    SoundPool soundPoolMusic;

    int field_587_e;

    GameSettings *options;

    int loaded;

    JavaRandom rand;

    int ticksBeforeMusic;

    ALCdevice *device;
    ALCcontext *context;

    ALuint soundSources[SOUNDMANAGER_POOL_SIZE];

    ALuint bgMusicSource;

    ALuint streamingSource;

    SoundBufEntry *bufCache;
    int bufCacheCount;
    int bufCacheCapacity;
} SoundManager;

SoundManager *SoundManager_create(void);
void SoundManager_destroy(SoundManager *self);

void SoundManager_loadSoundSettings(SoundManager *self, GameSettings *options);

void SoundManager_onSoundOptionsChanged(SoundManager *self);

void SoundManager_closeMinecraft(SoundManager *self);

void SoundManager_addSound(SoundManager *self, const char *name, const char *path);

void SoundManager_addStreaming(SoundManager *self, const char *name, const char *path);

void SoundManager_addMusic(SoundManager *self, const char *name, const char *path);

void SoundManager_playRandomMusicIfReady(SoundManager *self);

void SoundManager_setListener(SoundManager *self, struct EntityLiving *player, float partialTick);

void SoundManager_playStreaming(SoundManager *self, const char *sound, float x, float y, float z,
                                float volume, float pitch);

void SoundManager_playSound(SoundManager *self, const char *sound, float x, float y, float z,
                            float volume, float pitch);

void SoundManager_playSoundFX(SoundManager *self, const char *sound, float volume, float pitch);

void SoundManager_update(SoundManager *self);

#endif
