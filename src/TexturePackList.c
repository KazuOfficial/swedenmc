#include "TexturePackList.h"
#include "TexturePackCustom.h"
#include "Minecraft.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include "platform.h"
#include <ctype.h>

static void array_push(TexturePackArray *arr, TexturePackBase *item) {
    if (arr->size == arr->capacity) {
        arr->capacity = arr->capacity ? arr->capacity * 2 : 8;
        arr->data =
            (TexturePackBase **)realloc(arr->data, arr->capacity * sizeof(TexturePackBase *));
    }
    arr->data[arr->size++] = item;
}

static int array_contains(TexturePackArray *arr, TexturePackBase *item) {
    for (int i = 0; i < arr->size; i++)
        if (arr->data[i] == item)
            return 1;
    return 0;
}

static unsigned int map_hash(const char *s) {
    unsigned int h = 5381;
    while (*s)
        h = h * 33 ^ (unsigned char)*s++;
    return h % TEXTURE_PACK_MAP_SIZE;
}

static TexturePackBase *map_get(TexturePackMap *m, const char *key) {
    unsigned int h = map_hash(key);
    for (TexturePackMapEntry *e = m->buckets[h]; e; e = e->next)
        if (!strcmp(e->key, key))
            return e->value;
    return NULL;
}

static int map_contains(TexturePackMap *m, const char *key) { return map_get(m, key) != NULL; }

static void map_put(TexturePackMap *m, const char *key, TexturePackBase *value) {
    unsigned int h = map_hash(key);
    for (TexturePackMapEntry *e = m->buckets[h]; e; e = e->next) {
        if (!strcmp(e->key, key)) {
            e->value = value;
            return;
        }
    }
    TexturePackMapEntry *e = (TexturePackMapEntry *)calloc(1, sizeof(TexturePackMapEntry));
    e->key = strdup(key);
    e->value = value;
    e->next = m->buckets[h];
    m->buckets[h] = e;
}

static void map_remove(TexturePackMap *m, const char *key) {
    unsigned int h = map_hash(key);
    TexturePackMapEntry **prev = &m->buckets[h];
    for (TexturePackMapEntry *e = *prev; e; prev = &e->next, e = e->next) {
        if (!strcmp(e->key, key)) {
            *prev = e->next;
            free(e->key);
            free(e);
            return;
        }
    }
}

static int ends_with_zip(const char *name) {
    size_t len = strlen(name);
    if (len < 4)
        return 0;
    const char *ext = name + len - 4;
    return (ext[0] == '.' && tolower((unsigned char)ext[1]) == 'z' &&
            tolower((unsigned char)ext[2]) == 'i' && tolower((unsigned char)ext[3]) == 'p');
}

TexturePackList *TexturePackList_create(struct Minecraft *var1, const char *var2) {
    TexturePackList *self = (TexturePackList *)calloc(1, sizeof(TexturePackList));
    self->mc = var1;
    snprintf(self->texturePackDir, sizeof(self->texturePackDir), "%s/texturepacks", var2);

    struct stat st;
    if (stat(self->texturePackDir, &st) != 0) {
        mc_mkdir(self->texturePackDir);
    }

    strncpy(self->currentTexturePack, var1->gameSettings->skin,
            sizeof(self->currentTexturePack) - 1);

    self->defaultTexturePack = TexturePackDefault_create();
    TexturePackList_updateAvaliableTexturePacks(self);

    self->selectedTexturePack->vtable->func_6482_a(self->selectedTexturePack);
    return self;
}

void TexturePackList_destroy(TexturePackList *self) {

    free(self->availableTexturePacks.data);

    for (int i = 0; i < TEXTURE_PACK_MAP_SIZE; i++) {
        TexturePackMapEntry *e = self->field_6538_d.buckets[i];
        while (e) {
            TexturePackMapEntry *next = e->next;
            if (e->value && e->value->vtable->destroy)
                e->value->vtable->destroy(e->value);
            free(e->key);
            free(e);
            e = next;
        }
    }
    TexturePackDefault_destroy(self->defaultTexturePack);
    free(self);
}

int TexturePackList_setTexturePack(TexturePackList *self, TexturePackBase *var1) {
    if (var1 == self->selectedTexturePack) {
        return 0;
    } else {
        self->selectedTexturePack->vtable->closeTexturePackFile(self->selectedTexturePack);
        strncpy(self->currentTexturePack, var1->texturePackFileName,
                sizeof(self->currentTexturePack) - 1);
        self->selectedTexturePack = var1;
        strncpy(self->mc->gameSettings->skin, self->currentTexturePack,
                sizeof(self->mc->gameSettings->skin) - 1);
        GameSettings_saveOptions(self->mc->gameSettings);
        self->selectedTexturePack->vtable->func_6482_a(self->selectedTexturePack);
        return 1;
    }
}

void TexturePackList_updateAvaliableTexturePacks(TexturePackList *self) {
    TexturePackArray var1;
    memset(&var1, 0, sizeof(var1));
    self->selectedTexturePack = NULL;

    array_push(&var1, (TexturePackBase *)self->defaultTexturePack);

    struct stat dirSt;
    if (stat(self->texturePackDir, &dirSt) == 0 && S_ISDIR(dirSt.st_mode)) {
        DIR *dir = opendir(self->texturePackDir);
        if (dir) {
            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL) {

                if (!ends_with_zip(entry->d_name))
                    continue;
                char fullPath[768];
                snprintf(fullPath, sizeof(fullPath), "%s/%s", self->texturePackDir, entry->d_name);
                struct stat fileSt;
                if (stat(fullPath, &fileSt) != 0 || !S_ISREG(fileSt.st_mode))
                    continue;

                char var7[512];
                snprintf(var7, sizeof(var7), "%s:%lld:%lld", entry->d_name,
                         (long long)fileSt.st_size, (long long)fileSt.st_mtime);

                if (!map_contains(&self->field_6538_d, var7)) {
                    TexturePackCustom *var8 = TexturePackCustom_create(fullPath);

                    var8->base.field_6488_d = strdup(var7);
                    map_put(&self->field_6538_d, var7, (TexturePackBase *)var8);

                    var8->base.vtable->func_6485_a((TexturePackBase *)var8, self->mc);
                }

                TexturePackBase *var12 = map_get(&self->field_6538_d, var7);

                if (var12->texturePackFileName &&
                    !strcmp(var12->texturePackFileName, self->currentTexturePack)) {
                    self->selectedTexturePack = var12;
                }
                array_push(&var1, var12);
            }
            closedir(dir);
        }
    }

    if (self->selectedTexturePack == NULL) {
        self->selectedTexturePack = (TexturePackBase *)self->defaultTexturePack;
    }

    for (int i = 0; i < self->availableTexturePacks.size; i++) {
        TexturePackBase *var11 = self->availableTexturePacks.data[i];
        if (!array_contains(&var1, var11)) {

            var11->vtable->func_6484_b(var11, self->mc);

            if (var11->field_6488_d) {
                map_remove(&self->field_6538_d, var11->field_6488_d);
                var11->vtable->destroy(var11);
            }
        }
    }

    free(self->availableTexturePacks.data);
    self->availableTexturePacks = var1;
}

TexturePackArray TexturePackList_availableTexturePacks(TexturePackList *self) {
    TexturePackArray copy;
    copy.size = self->availableTexturePacks.size;
    copy.capacity = self->availableTexturePacks.size;
    copy.data = (TexturePackBase **)malloc(copy.size * sizeof(TexturePackBase *));
    memcpy(copy.data, self->availableTexturePacks.data, copy.size * sizeof(TexturePackBase *));
    return copy;
}
