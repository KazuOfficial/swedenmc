#include "SoundManager.h"
#include "EntityLiving.h"
#include "Entity.h"
#include "MathHelper.h"
#include "GameSettings.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "stb_vorbis.c"
#include <time.h>

static ALuint sm_load_buffer(SoundManager *self, const char *path) {
    int i;
    for (i = 0; i < self->bufCacheCount; i++) {
        if (strcmp(self->bufCache[i].path, path) == 0)
            return self->bufCache[i].buf;
    }

    int channels, rate;
    short *pcm = NULL;
    int frames = stb_vorbis_decode_filename(path, &channels, &rate, &pcm);
    if (frames <= 0 || !pcm) {
        fprintf(stderr, "SoundManager: failed to decode '%s'\n", path);
        return 0;
    }

    ALenum fmt = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    ALuint buf;
    alGenBuffers(1, &buf);
    alBufferData(buf, fmt, pcm, frames * channels * (int)sizeof(short), rate);
    free(pcm);

    if (self->bufCacheCount >= self->bufCacheCapacity) {
        int newCap = self->bufCacheCapacity == 0 ? 64 : self->bufCacheCapacity * 2;
        self->bufCache =
            (SoundBufEntry *)realloc(self->bufCache, (size_t)newCap * sizeof(SoundBufEntry));
        self->bufCacheCapacity = newCap;
    }
    strncpy(self->bufCache[self->bufCacheCount].path, path, sizeof(self->bufCache[0].path) - 1);
    self->bufCache[self->bufCacheCount].path[sizeof(self->bufCache[0].path) - 1] = '\0';
    self->bufCache[self->bufCacheCount].buf = buf;
    self->bufCacheCount++;
    return buf;
}

static void SoundManager_tryToSetLibraryAndCodecs(SoundManager *self) {
    float var1 = self->options->soundVolume;
    float var2 = self->options->musicVolume;
    self->options->soundVolume = 0.0f;
    self->options->musicVolume = 0.0f;
    GameSettings_saveOptions(self->options);

    self->device = alcOpenDevice(NULL);
    if (!self->device) {
        fprintf(stderr, "error linking with the LibraryJavaSound plug-in\n");
        goto done;
    }
    self->context = alcCreateContext(self->device, NULL);
    if (!self->context) {
        alcCloseDevice(self->device);
        self->device = NULL;
        fprintf(stderr, "error linking with the LibraryJavaSound plug-in\n");
        goto done;
    }
    alcMakeContextCurrent(self->context);
    alGenSources(SOUNDMANAGER_POOL_SIZE, self->soundSources);
    alGenSources(1, &self->bgMusicSource);
    alGenSources(1, &self->streamingSource);

    self->options->soundVolume = var1;
    self->options->musicVolume = var2;
    GameSettings_saveOptions(self->options);
done:
    self->loaded = 1;
}

SoundManager *SoundManager_create(void) {
    SoundManager *self = (SoundManager *)calloc(1, sizeof(SoundManager));
    if (!self)
        return NULL;
    SoundPool_init(&self->soundPoolSounds);
    SoundPool_init(&self->soundPoolStreaming);
    SoundPool_init(&self->soundPoolMusic);

    self->soundPoolStreaming.field_1657_b = 0;

    JavaRandom_init(&self->rand, (int64_t)time(NULL));

    self->ticksBeforeMusic = JavaRandom_nextInt(&self->rand, 12000);
    return self;
}

void SoundManager_destroy(SoundManager *self) {
    if (!self)
        return;
    SoundManager_closeMinecraft(self);
    SoundPool_free(&self->soundPoolSounds);
    SoundPool_free(&self->soundPoolStreaming);
    SoundPool_free(&self->soundPoolMusic);

    int i;
    for (i = 0; i < self->bufCacheCount; i++) {
        alDeleteBuffers(1, &self->bufCache[i].buf);
    }
    free(self->bufCache);
    free(self);
}

void SoundManager_closeMinecraft(SoundManager *self) {
    if (!self->loaded)
        return;
    alDeleteSources(SOUNDMANAGER_POOL_SIZE, self->soundSources);
    alDeleteSources(1, &self->bgMusicSource);
    alDeleteSources(1, &self->streamingSource);
    alcMakeContextCurrent(NULL);
    if (self->context) {
        alcDestroyContext(self->context);
        self->context = NULL;
    }
    if (self->device) {
        alcCloseDevice(self->device);
        self->device = NULL;
    }
}

void SoundManager_addSound(SoundManager *self, const char *name, const char *path) {
    SoundPool_addSound(&self->soundPoolSounds, name, path);
}

void SoundManager_addStreaming(SoundManager *self, const char *name, const char *path) {
    SoundPool_addSound(&self->soundPoolStreaming, name, path);
}

void SoundManager_addMusic(SoundManager *self, const char *name, const char *path) {
    SoundPool_addSound(&self->soundPoolMusic, name, path);
}

void SoundManager_loadSoundSettings(SoundManager *self, GameSettings *options) {
    self->soundPoolStreaming.field_1657_b = 0;
    self->options = options;
    if (!self->loaded &&
        (options == NULL || options->soundVolume != 0.0f || options->musicVolume != 0.0f)) {
        SoundManager_tryToSetLibraryAndCodecs(self);
    }
}

void SoundManager_onSoundOptionsChanged(SoundManager *self) {
    if (!self->loaded && self->options &&
        (self->options->soundVolume != 0.0f || self->options->musicVolume != 0.0f)) {
        SoundManager_tryToSetLibraryAndCodecs(self);
    }
    if (self->loaded) {
        if (!self->options || self->options->musicVolume == 0.0f) {

            alSourceStop(self->bgMusicSource);
        } else {

            alSourcef(self->bgMusicSource, AL_GAIN, self->options->musicVolume);
        }
    }
}

void SoundManager_setListener(SoundManager *self, struct EntityLiving *var1, float var2) {
    if (!self->loaded || !self->options || self->options->soundVolume == 0.0f)
        return;
    if (var1 != NULL) {
        Entity *e = (Entity *)var1;
        float var3 = e->prevRotationYaw + (e->rotationYaw - e->prevRotationYaw) * var2;
        double var4 = e->prevPosX + (e->posX - e->prevPosX) * (double)var2;
        double var6 = e->prevPosY + (e->posY - e->prevPosY) * (double)var2;
        double var8 = e->prevPosZ + (e->posZ - e->prevPosZ) * (double)var2;
        float var10 =
            MathHelper_cos(-var3 * (3.14159265358979323846f / 180.0f) - 3.14159265358979323846f);
        float var11 =
            MathHelper_sin(-var3 * (3.14159265358979323846f / 180.0f) - 3.14159265358979323846f);
        float var12 = -var11;
        float var13 = 0.0f;
        float var14 = -var10;
        float var15 = 0.0f;
        float var16 = 1.0f;
        float var17 = 0.0f;

        alListener3f(AL_POSITION, (float)var4, (float)var6, (float)var8);

        ALfloat orientation[6] = {var12, var13, var14, var15, var16, var17};
        alListenerfv(AL_ORIENTATION, orientation);
    }
}

void SoundManager_playSound(SoundManager *self, const char *var1, float var2, float var3,
                            float var4, float var5, float var6) {
    if (!self->loaded || !self->options || self->options->soundVolume == 0.0f)
        return;
    SoundPoolEntry *var7 = SoundPool_getRandomSoundFromSoundPool(&self->soundPoolSounds, var1);
    if (var7 == NULL || var5 <= 0.0f)
        return;

    self->field_587_e = (self->field_587_e + 1) % 256;
    ALuint var8 = self->soundSources[self->field_587_e];
    float var9 = 16.0f;
    if (var5 > 1.0f)
        var9 *= var5;

    ALuint buf = sm_load_buffer(self, var7->soundUrl);
    if (!buf)
        return;

    alSourceStop(var8);
    alSourcei(var8, AL_BUFFER, (ALint)buf);
    alSource3f(var8, AL_POSITION, var2, var3, var4);
    alSourcei(var8, AL_LOOPING, AL_FALSE);
    alSourcei(var8, AL_SOURCE_RELATIVE, AL_FALSE);

    alSourcef(var8, AL_REFERENCE_DISTANCE, var9);
    alSourcef(var8, AL_MAX_DISTANCE, var9 * 4.0f);
    alSourcef(var8, AL_ROLLOFF_FACTOR, 1.0f);
    alSourcef(var8, AL_PITCH, var6);
    if (var5 > 1.0f)
        var5 = 1.0f;
    alSourcef(var8, AL_GAIN, var5 * self->options->soundVolume);
    alSourcePlay(var8);
}

void SoundManager_playSoundFX(SoundManager *self, const char *var1, float var2, float var3) {
    if (!self->loaded || !self->options || self->options->soundVolume == 0.0f)
        return;
    SoundPoolEntry *var4 = SoundPool_getRandomSoundFromSoundPool(&self->soundPoolSounds, var1);
    if (var4 == NULL)
        return;

    self->field_587_e = (self->field_587_e + 1) % 256;
    ALuint var5 = self->soundSources[self->field_587_e];

    ALuint buf = sm_load_buffer(self, var4->soundUrl);
    if (!buf)
        return;

    alSourceStop(var5);
    alSourcei(var5, AL_BUFFER, (ALint)buf);

    alSource3f(var5, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSourcei(var5, AL_LOOPING, AL_FALSE);
    alSourcei(var5, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcef(var5, AL_ROLLOFF_FACTOR, 0.0f);
    if (var2 > 1.0f)
        var2 = 1.0f;
    var2 *= 0.25f;
    alSourcef(var5, AL_PITCH, var3);
    alSourcef(var5, AL_GAIN, var2 * self->options->soundVolume);
    alSourcePlay(var5);
}

void SoundManager_playStreaming(SoundManager *self, const char *var1, float var2, float var3,
                                float var4, float var5, float var6) {
    if (!self->loaded || !self->options || self->options->soundVolume == 0.0f)
        return;

    ALint state;
    alGetSourcei(self->streamingSource, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {
        alSourceStop(self->streamingSource);
    }

    if (var1 == NULL)
        return;
    SoundPoolEntry *var8 = SoundPool_getRandomSoundFromSoundPool(&self->soundPoolStreaming, var1);
    if (var8 == NULL || var5 <= 0.0f)
        return;

    alGetSourcei(self->bgMusicSource, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {
        alSourceStop(self->bgMusicSource);
    }

    ALuint buf = sm_load_buffer(self, var8->soundUrl);
    if (!buf)
        return;

    alSourceStop(self->streamingSource);
    alSourcei(self->streamingSource, AL_BUFFER, (ALint)buf);
    alSource3f(self->streamingSource, AL_POSITION, var2, var3, var4);
    alSourcei(self->streamingSource, AL_LOOPING, AL_FALSE);
    alSourcei(self->streamingSource, AL_SOURCE_RELATIVE, AL_FALSE);
    alSourcef(self->streamingSource, AL_REFERENCE_DISTANCE, 16.0f);
    alSourcef(self->streamingSource, AL_MAX_DISTANCE, 16.0f * 4.0f);
    alSourcef(self->streamingSource, AL_ROLLOFF_FACTOR, 1.0f);
    alSourcef(self->streamingSource, AL_PITCH, var6);
    alSourcef(self->streamingSource, AL_GAIN, 0.5f * self->options->soundVolume);
    alSourcePlay(self->streamingSource);
}

void SoundManager_playRandomMusicIfReady(SoundManager *self) {
    if (!self->loaded || !self->options || self->options->musicVolume == 0.0f)
        return;

    ALint bgState, strmState;
    alGetSourcei(self->bgMusicSource, AL_SOURCE_STATE, &bgState);
    alGetSourcei(self->streamingSource, AL_SOURCE_STATE, &strmState);
    if (bgState == AL_PLAYING || strmState == AL_PLAYING)
        return;

    if (self->ticksBeforeMusic > 0) {
        --self->ticksBeforeMusic;
        return;
    }

    SoundPoolEntry *var1 = SoundPool_getRandomSound(&self->soundPoolMusic);
    if (var1 != NULL) {
        self->ticksBeforeMusic = JavaRandom_nextInt(&self->rand, 12000) + 12000;
        ALuint buf = sm_load_buffer(self, var1->soundUrl);
        if (!buf)
            return;
        alSourceStop(self->bgMusicSource);
        alSourcei(self->bgMusicSource, AL_BUFFER, (ALint)buf);
        alSourcei(self->bgMusicSource, AL_LOOPING, AL_FALSE);
        alSourcei(self->bgMusicSource, AL_SOURCE_RELATIVE, AL_TRUE);
        alSource3f(self->bgMusicSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
        alSourcef(self->bgMusicSource, AL_ROLLOFF_FACTOR, 0.0f);
        alSourcef(self->bgMusicSource, AL_GAIN, self->options->musicVolume);
        alSourcePlay(self->bgMusicSource);
    }
}

void SoundManager_update(SoundManager *self) { (void)self; }
