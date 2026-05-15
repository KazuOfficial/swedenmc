#include "StepSoundSand.h"
#include "StepSound.h"

static const char *StepSoundSand_stepSoundDir(StepSound *self) {
    (void)self;
    return "step.gravel";
}

static const StepSound_vtable StepSoundSand_vtable = {StepSoundSand_stepSoundDir};

StepSound *StepSoundSand_create(const char *name, float volume, float pitch) {
    StepSound *s = StepSound_create(name, volume, pitch);
    s->vtable = &StepSoundSand_vtable;
    return s;
}
