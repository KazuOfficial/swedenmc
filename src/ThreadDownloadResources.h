#ifndef THREAD_DOWNLOAD_RESOURCES_H
#define THREAD_DOWNLOAD_RESOURCES_H

#include <pthread.h>

struct Minecraft;

typedef struct ThreadDownloadResources {

    char *resourcesFolder;

    struct Minecraft *mc;

    int closing;
    pthread_t thread;

    char *assetsDir;
    char *assetIndex;
} ThreadDownloadResources;

ThreadDownloadResources *ThreadDownloadResources_create(const char *var1, struct Minecraft *var2,
                                                        const char *assetsDir,
                                                        const char *assetIndex);

void ThreadDownloadResources_reloadResources(ThreadDownloadResources *self);

void ThreadDownloadResources_closeMinecraft(ThreadDownloadResources *self);

void ThreadDownloadResources_destroy(ThreadDownloadResources *self);

#endif
