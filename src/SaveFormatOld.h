#ifndef SAVE_FORMAT_OLD_H
#define SAVE_FORMAT_OLD_H

#include "ISaveFormat.h"
#include "ISaveHandler.h"
#include "WorldInfo.h"
#include "SaveFormatComparator.h"

typedef struct SaveFormatOld {
    ISaveFormat base;
    char field_22180_a[4096];
} SaveFormatOld;

SaveFormatOld *SaveFormatOld_create(const char *savesDir);

const char *SaveFormatOld_func_22178_a(ISaveFormat *self);

SaveFormatComparatorList *SaveFormatOld_func_22176_b(ISaveFormat *self);

void SaveFormatOld_flushCache(ISaveFormat *self);

WorldInfo *SaveFormatOld_func_22173_b(ISaveFormat *self, const char *var1);

void SaveFormatOld_func_22172_c(ISaveFormat *self, const char *var1);

void SaveFormatOld_func_22179_a(const char *dirPath);

void SaveFormatOld_func_22170_a(ISaveFormat *self, const char *var1, const char *var2);

ISaveHandler *SaveFormatOld_getSaveLoader(ISaveFormat *self, const char *var1, int var2);

int SaveFormatOld_isOldMapFormat(ISaveFormat *self, const char *var1);

int SaveFormatOld_convertMapFormat(ISaveFormat *self, const char *var1, IProgressUpdate *var2);

#endif
