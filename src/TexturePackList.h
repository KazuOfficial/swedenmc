#ifndef TEXTURE_PACK_LIST_H
#define TEXTURE_PACK_LIST_H

#include "TexturePackBase.h"
#include "TexturePackDefault.h"

struct Minecraft;

typedef struct {
    TexturePackBase **data;
    int size;
    int capacity;
} TexturePackArray;

typedef struct TexturePackMapEntry {
    char *key;
    TexturePackBase *value;
    struct TexturePackMapEntry *next;
} TexturePackMapEntry;
#define TEXTURE_PACK_MAP_SIZE 64
typedef struct {
    TexturePackMapEntry *buckets[TEXTURE_PACK_MAP_SIZE];
} TexturePackMap;

typedef struct TexturePackList {

    TexturePackArray availableTexturePacks;

    TexturePackDefault *defaultTexturePack;

    TexturePackBase *selectedTexturePack;

    TexturePackMap field_6538_d;

    struct Minecraft *mc;

    char texturePackDir[512];

    char currentTexturePack[256];
} TexturePackList;

TexturePackList *TexturePackList_create(struct Minecraft *var1, const char *var2);
void TexturePackList_destroy(TexturePackList *self);

int TexturePackList_setTexturePack(TexturePackList *self, TexturePackBase *var1);

void TexturePackList_updateAvaliableTexturePacks(TexturePackList *self);

TexturePackArray TexturePackList_availableTexturePacks(TexturePackList *self);

#endif
