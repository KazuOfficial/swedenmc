#ifndef SAVE_FORMAT_COMPARATOR_H
#define SAVE_FORMAT_COMPARATOR_H

#include <stdint.h>

typedef struct SaveFormatComparator {
    char fileName[256];
    char displayName[256];
    int64_t field_22169_c;
    int64_t field_22168_d;
    int field_22167_e;
} SaveFormatComparator;

void SaveFormatComparator_init(SaveFormatComparator *self, const char *fileName,
                               const char *displayName, int64_t lastTimePlayed, int64_t sizeOnDisk,
                               int needsConversion);

const char *SaveFormatComparator_getFileName(SaveFormatComparator *self);

const char *SaveFormatComparator_getDisplayName(SaveFormatComparator *self);

int64_t SaveFormatComparator_func_22159_c(SaveFormatComparator *self);

int SaveFormatComparator_func_22161_d(SaveFormatComparator *self);

int64_t SaveFormatComparator_func_22163_e(SaveFormatComparator *self);

int SaveFormatComparator_func_22160_a(const SaveFormatComparator *a, const SaveFormatComparator *b);

int SaveFormatComparator_cmp(const void *a, const void *b);

typedef struct {
    SaveFormatComparator **data;
    int size;
    int capacity;
} SaveFormatComparatorList;

void SaveFormatComparatorList_init(SaveFormatComparatorList *list);
void SaveFormatComparatorList_add(SaveFormatComparatorList *list, SaveFormatComparator *item);
void SaveFormatComparatorList_sort(SaveFormatComparatorList *list);
void SaveFormatComparatorList_free(SaveFormatComparatorList *list);

#endif
