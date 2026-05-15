#ifndef THREAD_DOWNLOAD_IMAGE_H
#define THREAD_DOWNLOAD_IMAGE_H

#include <pthread.h>
#include "ImageBuffer.h"
#include "ThreadDownloadImageData.h"

typedef struct ThreadDownloadImage {

    char *location;

    ImageBuffer *buffer;

    ThreadDownloadImageData *imageData;
    pthread_t thread;
} ThreadDownloadImage;

ThreadDownloadImage *ThreadDownloadImage_create(ThreadDownloadImageData *var1, const char *var2,
                                                ImageBuffer *var3);

void ThreadDownloadImage_destroy(ThreadDownloadImage *self);

#endif
