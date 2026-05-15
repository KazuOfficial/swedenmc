#ifndef NBT_TAG_COMPOUND_H
#define NBT_TAG_COMPOUND_H
#include "NBTBase.h"
#include "NBTTagList.h"
#include <stdint.h>

typedef struct {
    char *key;
    NBTBase *value;
} NBTEntry;

typedef struct {
    NBTBase base;
    NBTEntry *tagMap;
} NBTTagCompound;

NBTTagCompound *NBTTagCompound_create(void);

void NBTTagCompound_setTag(NBTTagCompound *self, const char *key, NBTBase *tag);
void NBTTagCompound_setByte(NBTTagCompound *self, const char *key, int8_t v);
void NBTTagCompound_setShort(NBTTagCompound *self, const char *key, int16_t v);
void NBTTagCompound_setInteger(NBTTagCompound *self, const char *key, int32_t v);
void NBTTagCompound_setLong(NBTTagCompound *self, const char *key, int64_t v);
void NBTTagCompound_setFloat(NBTTagCompound *self, const char *key, float v);
void NBTTagCompound_setDouble(NBTTagCompound *self, const char *key, double v);
void NBTTagCompound_setString(NBTTagCompound *self, const char *key, const char *v);
void NBTTagCompound_setByteArray(NBTTagCompound *self, const char *key, uint8_t *data, int32_t len);
void NBTTagCompound_setCompoundTag(NBTTagCompound *self, const char *key, NBTTagCompound *v);
void NBTTagCompound_setBoolean(NBTTagCompound *self, const char *key, int v);

NBTEntry *NBTTagCompound_func_28110_c(NBTTagCompound *self);

int NBTTagCompound_hasKey(NBTTagCompound *self, const char *key);
int8_t NBTTagCompound_getByte(NBTTagCompound *self, const char *key);
int16_t NBTTagCompound_getShort(NBTTagCompound *self, const char *key);
int32_t NBTTagCompound_getInteger(NBTTagCompound *self, const char *key);
int64_t NBTTagCompound_getLong(NBTTagCompound *self, const char *key);
float NBTTagCompound_getFloat(NBTTagCompound *self, const char *key);
double NBTTagCompound_getDouble(NBTTagCompound *self, const char *key);
const char *NBTTagCompound_getString(NBTTagCompound *self, const char *key);
uint8_t *NBTTagCompound_getByteArray(NBTTagCompound *self, const char *key);
NBTTagCompound *NBTTagCompound_getCompoundTag(NBTTagCompound *self, const char *key);
NBTTagList *NBTTagCompound_getTagList(NBTTagCompound *self, const char *key);
int NBTTagCompound_getBoolean(NBTTagCompound *self, const char *key);
char *NBTTagCompound_toString(NBTTagCompound *self);

#endif
