#include "SoundPoolEntry.h"
#include <string.h>

void SoundPoolEntry_init(SoundPoolEntry *self, const char *soundName, const char *soundUrl) {
    strncpy(self->soundName, soundName, sizeof(self->soundName) - 1);
    self->soundName[sizeof(self->soundName) - 1] = '\0';
    strncpy(self->soundUrl, soundUrl, sizeof(self->soundUrl) - 1);
    self->soundUrl[sizeof(self->soundUrl) - 1] = '\0';
}
