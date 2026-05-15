#include "StepSoundStone.h"
#include "StepSound.h"

static const char *StepSoundStone_stepSoundDir(StepSound *self) {
    (void)self;
    return "random.glass";
}

static const StepSound_vtable StepSoundStone_vtable = {StepSoundStone_stepSoundDir};

StepSound *StepSoundStone_create(const char *name, float volume, float pitch) {
    StepSound *s = StepSound_create(name, volume, pitch);
    s->vtable = &StepSoundStone_vtable;
    return s;
}
