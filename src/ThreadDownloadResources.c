#include "ThreadDownloadResources.h"
#include "Minecraft.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "platform.h"
#include <dirent.h>
#include <errno.h>

static void installResource(ThreadDownloadResources *self, const char *name, const char *filePath) {
    Minecraft_installResource(self->mc, name, filePath);
}

static void loadResource(ThreadDownloadResources *self, const char *dir, const char *prefix) {
    DIR *d = opendir(dir);
    if (!d)
        return;
    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;
        char fullPath[4096];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dir, entry->d_name);
        struct stat st;
        if (stat(fullPath, &st) != 0)
            continue;
        char relName[4096];
        snprintf(relName, sizeof(relName), "%s%s", prefix, entry->d_name);
        if (S_ISDIR(st.st_mode)) {
            char subPrefix[4096];
            snprintf(subPrefix, sizeof(subPrefix), "%s/", relName);
            loadResource(self, fullPath, subPrefix);
        } else {
            installResource(self, relName, fullPath);
        }
    }
    closedir(d);
}

static void loadFromAssetIndex(ThreadDownloadResources *self) {
    FILE *f = fopen(self->assetIndex, "r");
    if (!f) {
        fprintf(stderr, "[resources] cannot open asset index: %s\n", self->assetIndex);
        return;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);
    char *json = (char *)malloc((size_t)len + 1);
    if (!json) {
        fclose(f);
        return;
    }
    fread(json, 1, (size_t)len, f);
    fclose(f);
    json[len] = '\0';

    const char *objects = strstr(json, "\"objects\"");
    if (!objects) {
        free(json);
        return;
    }
    const char *brace = strchr(objects, '{');
    if (!brace) {
        free(json);
        return;
    }

    const char *p = brace + 1;
    int installed = 0;

    while (*p) {
        if (self->closing)
            break;

        const char *keyStart = strchr(p, '"');
        if (!keyStart)
            break;
        keyStart++;
        const char *keyEnd = strchr(keyStart, '"');
        if (!keyEnd)
            break;

        int keyLen = (int)(keyEnd - keyStart);
        char key[1024];
        if (keyLen >= (int)sizeof(key)) {
            p = keyEnd + 1;
            continue;
        }
        memcpy(key, keyStart, (size_t)keyLen);
        key[keyLen] = '\0';

        p = keyEnd + 1;

        int isSound = (strncmp(key, "sounds/", 7) == 0 || strncmp(key, "newsound/", 9) == 0);
        int isMusic = (strncmp(key, "music/", 6) == 0 || strncmp(key, "newmusic/", 9) == 0 ||
                       strncmp(key, "streaming/", 10) == 0);
        if (!isSound && !isMusic)
            continue;

        const char *hashKey = strstr(p, "\"hash\"");
        if (!hashKey)
            break;
        const char *hashQ = strchr(hashKey + 6, '"');
        if (!hashQ)
            break;
        hashQ++;
        const char *hashEnd = strchr(hashQ, '"');
        if (!hashEnd)
            break;

        int hashLen = (int)(hashEnd - hashQ);
        char hash[64];
        if (hashLen < 2 || hashLen >= (int)sizeof(hash)) {
            p = hashEnd + 1;
            continue;
        }
        memcpy(hash, hashQ, (size_t)hashLen);
        hash[hashLen] = '\0';

        p = hashEnd + 1;

        char objPath[4096];
        snprintf(objPath, sizeof(objPath), "%s/objects/%.2s/%s", self->assetsDir, hash, hash);

        struct stat st;
        if (stat(objPath, &st) != 0)
            continue;

        char resourceKey[1024];
        if (strncmp(key, "sounds/", 7) == 0) {
            snprintf(resourceKey, sizeof(resourceKey), "newsound/%s", key + 7);
        } else {
            snprintf(resourceKey, sizeof(resourceKey), "%s", key);
        }

        installResource(self, resourceKey, objPath);
        installed++;
    }

    free(json);
    if (installed > 0)
        printf("[resources] installed %d assets from index\n", installed);
}

static void *ThreadDownloadResources_run(void *arg) {
    ThreadDownloadResources *self = (ThreadDownloadResources *)arg;

    if (self->assetIndex && self->assetsDir) {

        loadFromAssetIndex(self);
    } else {

        loadResource(self, self->resourcesFolder, "");
    }

    return NULL;
}

static void mkdirs(const char *path) {
    char tmp[4096];
    snprintf(tmp, sizeof(tmp), "%s", path);
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mc_mkdir(tmp);
            *p = '/';
        }
    }
    mc_mkdir(tmp);
}

ThreadDownloadResources *ThreadDownloadResources_create(const char *var1, struct Minecraft *var2,
                                                        const char *assetsDir,
                                                        const char *assetIndex) {
    ThreadDownloadResources *self =
        (ThreadDownloadResources *)calloc(1, sizeof(ThreadDownloadResources));
    self->mc = var2;
    self->closing = 0;

    char path[4096];
    snprintf(path, sizeof(path), "%s/resources", var1);
    self->resourcesFolder = strdup(path);

    if (assetsDir)
        self->assetsDir = strdup(assetsDir);
    if (assetIndex)
        self->assetIndex = strdup(assetIndex);

    struct stat st;
    if (stat(path, &st) != 0) {
        mkdirs(path);
    }

    pthread_create(&self->thread, NULL, ThreadDownloadResources_run, self);
    pthread_detach(self->thread);
    return self;
}

void ThreadDownloadResources_reloadResources(ThreadDownloadResources *self) {
    loadResource(self, self->resourcesFolder, "");
}

void ThreadDownloadResources_closeMinecraft(ThreadDownloadResources *self) { self->closing = 1; }

void ThreadDownloadResources_destroy(ThreadDownloadResources *self) {
    free(self->resourcesFolder);
    free(self->assetsDir);
    free(self->assetIndex);
    free(self);
}
