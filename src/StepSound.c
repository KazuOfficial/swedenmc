#include "StepSound.h"
#include <stdlib.h>
#include <stdio.h>

static char s_buf[64];

static const char *StepSound_stepSoundDir_impl(StepSound *self) {
    snprintf(s_buf, sizeof(s_buf), "step.%s", self->soundName);
    return s_buf;
}

const StepSound_vtable StepSound_defaultVtable = {StepSound_stepSoundDir_impl};

StepSound *StepSound_create(const char *name, float volume, float pitch) {
    StepSound *s = (StepSound *)calloc(1, sizeof(StepSound));
    s->vtable = &StepSound_defaultVtable;
    s->soundName = name;
    s->volume = volume;
    s->pitch = pitch;
    return s;
}

float StepSound_getVolume(StepSound *self) { return self->volume; }
float StepSound_getPitch(StepSound *self) { return self->pitch; }

const char *StepSound_stepSoundDir(StepSound *self) { return self->vtable->stepSoundDir(self); }

const char *StepSound_func_1145_d(StepSound *self) {
    snprintf(s_buf, sizeof(s_buf), "step.%s", self->soundName);
    return s_buf;
}
