#ifndef THREAD_DOWNLOAD_IMAGE_DATA_H
#define THREAD_DOWNLOAD_IMAGE_DATA_H

#include "ImageBuffer.h"

struct ThreadDownloadImage;

typedef struct ThreadDownloadImageData {
    void *image;
    int imageWidth;
    int imageHeight;
    int referenceCount;
    int textureName;
    int textureSetupComplete;
    struct ThreadDownloadImage *thread_obj;
} ThreadDownloadImageData;

ThreadDownloadImageData *ThreadDownloadImageData_create(const char *var1, ImageBuffer *var2);
void ThreadDownloadImageData_destroy(ThreadDownloadImageData *self);

#endif
