#include "SaveFormatComparator.h"
#include "JavaSort.h"
#include <string.h>
#include <stdlib.h>

void SaveFormatComparator_init(SaveFormatComparator *self, const char *fileName,
                               const char *displayName, int64_t lastTimePlayed, int64_t sizeOnDisk,
                               int needsConversion) {
    strncpy(self->fileName, fileName, sizeof(self->fileName) - 1);
    self->fileName[sizeof(self->fileName) - 1] = '\0';
    strncpy(self->displayName, displayName, sizeof(self->displayName) - 1);
    self->displayName[sizeof(self->displayName) - 1] = '\0';
    self->field_22169_c = lastTimePlayed;
    self->field_22168_d = sizeOnDisk;
    self->field_22167_e = needsConversion;
}

const char *SaveFormatComparator_getFileName(SaveFormatComparator *self) { return self->fileName; }

const char *SaveFormatComparator_getDisplayName(SaveFormatComparator *self) {
    return self->displayName;
}

int64_t SaveFormatComparator_func_22159_c(SaveFormatComparator *self) {
    return self->field_22168_d;
}

int SaveFormatComparator_func_22161_d(SaveFormatComparator *self) { return self->field_22167_e; }

int64_t SaveFormatComparator_func_22163_e(SaveFormatComparator *self) {
    return self->field_22169_c;
}

int SaveFormatComparator_func_22160_a(const SaveFormatComparator *a,
                                      const SaveFormatComparator *b) {
    if (a->field_22169_c < b->field_22169_c)
        return 1;
    if (a->field_22169_c > b->field_22169_c)
        return -1;
    return strcmp(a->fileName, b->fileName);
}

int SaveFormatComparator_cmp(const void *a, const void *b) {
    const SaveFormatComparator *ca = *(const SaveFormatComparator *const *)a;
    const SaveFormatComparator *cb = *(const SaveFormatComparator *const *)b;
    return SaveFormatComparator_func_22160_a(ca, cb);
}

void SaveFormatComparatorList_init(SaveFormatComparatorList *list) {
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void SaveFormatComparatorList_add(SaveFormatComparatorList *list, SaveFormatComparator *item) {
    if (list->size >= list->capacity) {
        int newCap = list->capacity == 0 ? 8 : list->capacity * 2;
        list->data =
            (SaveFormatComparator **)realloc(list->data, newCap * sizeof(SaveFormatComparator *));
        list->capacity = newCap;
    }
    list->data[list->size++] = item;
}

void SaveFormatComparatorList_sort(SaveFormatComparatorList *list) {
    if (list->size > 1)
        Arrays_sort((void **)list->data, list->size, SaveFormatComparator_cmp);
}

void SaveFormatComparatorList_free(SaveFormatComparatorList *list) {
    for (int i = 0; i < list->size; i++)
        free(list->data[i]);
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}
