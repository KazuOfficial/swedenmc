#include "AchievementMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int key;
    char *value;
} GuidEntry;

struct AchievementMap {
    GuidEntry *entries;
    int count;
    int capacity;
};

AchievementMap *AchievementMap_instance = NULL;

static void guidMap_put(AchievementMap *self, int key, const char *value) {
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->entries[i].key == key) {
            free(self->entries[i].value);
            self->entries[i].value = strdup(value);
            return;
        }
    }
    if (self->count == self->capacity) {
        int newcap = self->capacity == 0 ? 16 : self->capacity * 2;
        self->entries = (GuidEntry *)realloc(self->entries, newcap * sizeof(GuidEntry));
        self->capacity = newcap;
    }
    self->entries[self->count].key = key;
    self->entries[self->count].value = strdup(value);
    self->count++;
}

static AchievementMap *AchievementMap_new(void) {
    AchievementMap *self = (AchievementMap *)calloc(1, sizeof(AchievementMap));

    {

        FILE *var1 = fopen("achievement/map.txt", "r");
        if (var1 == NULL)
            goto catch_var5;
        char var2[512];
        while (1) {

            char *result = fgets(var2, sizeof(var2), var1);
            if (result == NULL) {

                fclose(var1);
                break;
            }

            char *tok = strtok(var2, ",");

            int var4 = atoi(tok);

            char *tok2 = strtok(NULL, ",\n\r");
            if (tok2 == NULL) {
                fclose(var1);
                goto catch_var5;
            }
            guidMap_put(self, var4, tok2);
        }
    }
    return self;
catch_var5:

    fprintf(stderr, "Exception in AchievementMap_new\n");
    return self;
}

const char *AchievementMap_getGuid(int var0) {
    AchievementMap *inst = AchievementMap_instance;
    int i;
    for (i = 0; i < inst->count; i++) {
        if (inst->entries[i].key == var0)
            return inst->entries[i].value;
    }
    return NULL;
}

void AchievementMap_init(void) {
    static int s_init = 0;
    if (s_init)
        return;
    s_init = 1;
    AchievementMap_instance = AchievementMap_new();
}
