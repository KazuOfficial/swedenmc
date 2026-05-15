#ifndef IMAGE_BUFFER_DOWNLOAD_H
#define IMAGE_BUFFER_DOWNLOAD_H

#include "ImageBuffer.h"

typedef struct ImageBufferDownload {
    ImageBuffer base;
    int *imageData;
    int imageWidth;
    int imageHeight;
} ImageBufferDownload;

void ImageBufferDownload_init(ImageBufferDownload *self);

void *ImageBufferDownload_parseUserSkin(ImageBufferDownload *self, void *var1);

#endif
