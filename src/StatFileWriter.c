#include "StatFileWriter.h"
#include "StatsSyncher.h"
#include "StatBase.h"
#include "Achievement.h"
#include "StatList.h"
#include "MD5String.h"
#include "J_JdomParser.h"
#include "J_InvalidSyntaxException.h"
#include "J_JsonRootNode.h"
#include "J_JsonNode.h"
#include "J_JsonStringNode.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include "platform.h"

static void writeStatToMap(StatMap *var1, StatBase *var2, int var3) {
    int found = 0;
    int var5 = StatMap_get(var1, var2, &found);
    if (!found)
        var5 = 0;
    StatMap_put(var1, var2, var5 + var3);
}

StatFileWriter *StatFileWriter_create(Session *var1, const char *var2) {
    StatFileWriter *self = (StatFileWriter *)calloc(1, sizeof(StatFileWriter));
    self->field_25102_a = StatMap_create();
    self->field_25101_b = StatMap_create();
    self->field_27189_c = 0;

    char var3[512];
    snprintf(var3, sizeof(var3), "%s/stats", var2);
    mc_mkdir(var3);

    {
        DIR *dir = opendir(var2);
        if (dir) {
            struct dirent *ent;
            while ((ent = readdir(dir)) != NULL) {
                const char *name = ent->d_name;
                size_t nlen = strlen(name);
                if (strncmp(name, "stats_", 6) == 0 && nlen > 4 &&
                    strcmp(name + nlen - 4, ".dat") == 0) {
                    char src[512], dst[512];
                    snprintf(src, sizeof(src), "%s/%s", var2, name);
                    snprintf(dst, sizeof(dst), "%s/%s", var3, name);
                    struct stat st;
                    if (stat(dst, &st) != 0) {
                        printf("Relocating %s\n", name);
                        rename(src, dst);
                    }
                }
            }
            closedir(dir);
        }
    }

    self->statsSyncher = StatsSyncher_create(var1, self, var3);
    return self;
}

void StatFileWriter_destroy(StatFileWriter *self) {
    StatMap_destroy(self->field_25102_a);
    StatMap_destroy(self->field_25101_b);

    free(self);
}

void StatFileWriter_readStat(StatFileWriter *self, StatBase *var1, int var2) {
    writeStatToMap(self->field_25101_b, var1, var2);
    writeStatToMap(self->field_25102_a, var1, var2);
    self->field_27189_c = 1;
}

StatMap *StatFileWriter_func_27176_a(StatFileWriter *self) {
    return StatMap_copy(self->field_25101_b);
}

void StatFileWriter_func_27179_a(StatFileWriter *self, StatMap *var1) {
    int i;
    if (var1 == NULL)
        return;
    self->field_27189_c = 1;
    for (i = 0; i < STAT_MAP_CAPACITY; i++) {
        StatBase *var3 = var1->entries[i].key;
        if (var3 == NULL)
            continue;
        writeStatToMap(self->field_25101_b, var3, var1->entries[i].value);
        writeStatToMap(self->field_25102_a, var3, var1->entries[i].value);
    }
}

void StatFileWriter_func_27180_b(StatFileWriter *self, StatMap *var1) {
    int i;
    if (var1 == NULL)
        return;
    for (i = 0; i < STAT_MAP_CAPACITY; i++) {
        StatBase *var3 = var1->entries[i].key;
        if (var3 == NULL)
            continue;
        {
            int found = 0;
            int var5 = StatMap_get(self->field_25101_b, var3, &found);
            if (!found)
                var5 = 0;
            StatMap_put(self->field_25102_a, var3, var1->entries[i].value + var5);
        }
    }
}

void StatFileWriter_func_27187_c(StatFileWriter *self, StatMap *var1) {
    int i;
    if (var1 == NULL)
        return;
    self->field_27189_c = 1;
    for (i = 0; i < STAT_MAP_CAPACITY; i++) {
        StatBase *var3 = var1->entries[i].key;
        if (var3 == NULL)
            continue;
        writeStatToMap(self->field_25101_b, var3, var1->entries[i].value);
    }
}

StatMap *StatFileWriter_func_27177_a(const char *var0) {
    StatMap *var1 = StatMap_create();

    const char *var2 = "local";
    char *var3 = (char *)calloc(1, 1);
    int var3_len = 0, var3_cap = 1;

    J_JdomParser parser;
    J_JdomParser_init(&parser);
    J_InvalidSyntaxException parse_err = {0};
    J_JsonRootNode *var4 = J_JdomParser_func_27367_a(&parser, var0, &parse_err);
    if (var4 != NULL) {

        J_PathElem path1[1] = {{J_PATH_ELEM_STRING, {.str = "stats-change"}}};
        J_JsonNodeArray *var5 = J_JsonNode_func_27217_b((J_JsonNode *)var4, path1, 1);
        if (var5 != NULL) {
            for (int i6 = 0; i6 < var5->count; i6++) {
                J_JsonNode *var7 = var5->items[i6];

                J_JsonMap *var8 = var7->vtable->func_27214_c(var7);
                if (var8 != NULL && var8->count > 0) {

                    J_JsonStringNode *key9 = var8->entries[0].key;
                    J_JsonNode *val9 = var8->entries[0].value;

                    int var10 = atoi(J_JsonStringNode_func_27216_b((J_JsonNode *)key9));

                    int var11 = atoi(val9->vtable->func_27216_b(val9));
                    StatBase *var12 = StatList_func_27361_a(var10);
                    if (var12 == NULL || var12->statGuid == NULL) {
                        printf("%d is not a valid stat\n", var10);
                    } else {

                        int need = (int)(strlen(var12->statGuid) + 32);
                        if (var3_len + need + 1 > var3_cap) {
                            var3_cap = (var3_len + need + 1) * 2;
                            var3 = realloc(var3, var3_cap);
                        }
                        var3_len += snprintf(var3 + var3_len, var3_cap - var3_len, "%s,%d,",
                                             var12->statGuid, var11);
                        StatMap_put(var1, var12, var11);
                    }
                    if (var8)
                        J_JsonMap_free(var8);
                }
            }
        }

        MD5String var14;
        MD5String_init(&var14, var2);
        char md5out[33];
        const char *var15 = MD5String_func_27369_a(&var14, var3, md5out);

        J_PathElem path2[1] = {{J_PATH_ELEM_STRING, {.str = "checksum"}}};
        const char *stored_checksum = J_JsonNode_func_27213_a((J_JsonNode *)var4, path2, 1);
        if (stored_checksum == NULL || strcmp(var15, stored_checksum) != 0) {
            printf("CHECKSUM MISMATCH\n");
            StatMap_destroy(var1);
            free(var3);
            return NULL;
        }
    }
    free(var3);
    return var1;
}

static void sb_append(char **buf, int *len, int *cap, const char *src) {
    int src_len = (int)strlen(src);
    if (*len + src_len + 1 > *cap) {
        *cap = (*len + src_len + 1) * 2 + 64;
        *buf = (char *)realloc(*buf, *cap);
    }
    memcpy(*buf + *len, src, src_len + 1);
    *len += src_len;
}

const char *StatFileWriter_func_27185_a(const char *var0, const char *var1, StatMap *var2) {

    char *var3 = (char *)malloc(256);
    int var3_len = 0, var3_cap = 256;
    var3[0] = '\0';

    char *var4 = (char *)malloc(256);
    int var4_len = 0, var4_cap = 256;
    var4[0] = '\0';

    int var5 = 1;

    sb_append(&var3, &var3_len, &var3_cap, "{\r\n");

    if (var0 != NULL && var1 != NULL) {
        sb_append(&var3, &var3_len, &var3_cap, "  \"user\":{\r\n");
        sb_append(&var3, &var3_len, &var3_cap, "    \"name\":\"");
        sb_append(&var3, &var3_len, &var3_cap, var0);
        sb_append(&var3, &var3_len, &var3_cap, "\",\r\n");
        sb_append(&var3, &var3_len, &var3_cap, "    \"sessionid\":\"");
        sb_append(&var3, &var3_len, &var3_cap, var1);
        sb_append(&var3, &var3_len, &var3_cap, "\"\r\n");
        sb_append(&var3, &var3_len, &var3_cap, "  },\r\n");
    }

    sb_append(&var3, &var3_len, &var3_cap, "  \"stats-change\":[");

    for (int i = 0; i < STAT_MAP_CAPACITY; i++) {
        StatBase *var7 = var2->entries[i].key;
        if (var7 == NULL)
            continue;
        int val = var2->entries[i].value;

        if (!var5) {
            sb_append(&var3, &var3_len, &var3_cap, "},");
        } else {
            var5 = 0;
        }

        char tmp[64];
        char statId_str[32];
        snprintf(statId_str, sizeof(statId_str), "%d", var7->statId);
        sb_append(&var3, &var3_len, &var3_cap, "\r\n    {\"");
        sb_append(&var3, &var3_len, &var3_cap, statId_str);
        sb_append(&var3, &var3_len, &var3_cap, "\":");
        snprintf(tmp, sizeof(tmp), "%d", val);
        sb_append(&var3, &var3_len, &var3_cap, tmp);

        sb_append(&var4, &var4_len, &var4_cap, var7->statGuid ? var7->statGuid : "null");
        sb_append(&var4, &var4_len, &var4_cap, ",");

        sb_append(&var4, &var4_len, &var4_cap, tmp);
        sb_append(&var4, &var4_len, &var4_cap, ",");
    }

    if (!var5) {
        sb_append(&var3, &var3_len, &var3_cap, "}");
    }

    MD5String var8;
    MD5String_init(&var8, var1);

    sb_append(&var3, &var3_len, &var3_cap, "\r\n  ],\r\n");

    char checksum_buf[33];
    MD5String_func_27369_a(&var8, var4, checksum_buf);
    free(var4);
    sb_append(&var3, &var3_len, &var3_cap, "  \"checksum\":\"");
    sb_append(&var3, &var3_len, &var3_cap, checksum_buf);
    sb_append(&var3, &var3_len, &var3_cap, "\"\r\n");

    sb_append(&var3, &var3_len, &var3_cap, "}");

    return var3;
}

int StatFileWriter_hasAchievementUnlocked(StatFileWriter *self, struct Achievement *var1) {
    return StatMap_containsKey(self->field_25102_a, (StatBase *)var1);
}

int StatFileWriter_func_27181_b(StatFileWriter *self, struct Achievement *var1) {
    return var1->parentAchievement == NULL ||
           StatFileWriter_hasAchievementUnlocked(self, var1->parentAchievement);
}

int StatFileWriter_writeStat(StatFileWriter *self, StatBase *var1) {
    int found = 0;
    int val = StatMap_get(self->field_25102_a, var1, &found);
    return found ? val : 0;
}

void StatFileWriter_func_27175_b(StatFileWriter *self) { (void)self; }

void StatFileWriter_syncStats(StatFileWriter *self) {
    StatMap *copy = StatFileWriter_func_27176_a(self);
    StatsSyncher_syncStatsFileWithMap(self->statsSyncher, copy);
    StatMap_destroy(copy);
}

void StatFileWriter_func_27178_d(StatFileWriter *self) {
    if (self->field_27189_c && StatsSyncher_func_27420_b(self->statsSyncher)) {
        StatMap *copy = StatFileWriter_func_27176_a(self);
        StatsSyncher_func_27424_a(self->statsSyncher, copy);
    }
    StatsSyncher_func_27425_c(self->statsSyncher);
}
