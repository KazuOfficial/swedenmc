#ifndef STEP_SOUND_H
#define STEP_SOUND_H

typedef struct StepSound StepSound;

typedef struct StepSound_vtable {
    const char *(*stepSoundDir)(StepSound *self);
} StepSound_vtable;

struct StepSound {
    const StepSound_vtable *vtable;
    const char *soundName;
    float volume;
    float pitch;
};

extern const StepSound_vtable StepSound_defaultVtable;

StepSound *StepSound_create(const char *name, float volume, float pitch);

float StepSound_getVolume(StepSound *self);
float StepSound_getPitch(StepSound *self);
const char *StepSound_stepSoundDir(StepSound *self);
const char *StepSound_func_1145_d(StepSound *self);

#endif
