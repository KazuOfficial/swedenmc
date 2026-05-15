#include "ThreadDownloadImageData.h"
#include "ThreadDownloadImage.h"
#include <stdlib.h>

ThreadDownloadImageData *ThreadDownloadImageData_create(const char *var1, ImageBuffer *var2) {
    ThreadDownloadImageData *self =
        (ThreadDownloadImageData *)calloc(1, sizeof(ThreadDownloadImageData));
    self->image = NULL;
    self->referenceCount = 1;
    self->textureName = -1;
    self->textureSetupComplete = 0;
    self->thread_obj = ThreadDownloadImage_create(self, var1, var2);
    return self;
}

void ThreadDownloadImageData_destroy(ThreadDownloadImageData *self) {
    if (self->thread_obj) {
        pthread_join(self->thread_obj->thread, NULL);
        free(self->thread_obj->buffer);
        ThreadDownloadImage_destroy(self->thread_obj);
    }
    free(self);
}
