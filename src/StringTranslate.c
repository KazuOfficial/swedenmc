#include "StringTranslate.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define TRANS_BUCKETS 4096

typedef struct TransEntry {
    char *key;
    char *value;
    struct TransEntry *next;
} TransEntry;

struct StringTranslate {
    TransEntry *buckets[TRANS_BUCKETS];
};

static unsigned int trans_hash(const char *s) {
    unsigned int h = 5381;
    while (*s)
        h = h * 33 ^ (unsigned char)*s++;
    return h % TRANS_BUCKETS;
}

static void trans_put(StringTranslate *self, const char *key, const char *value) {
    unsigned int h = trans_hash(key);
    TransEntry *e = self->buckets[h];
    while (e) {
        if (strcmp(e->key, key) == 0) {
            free(e->value);
            e->value = strdup(value);
            return;
        }
        e = e->next;
    }
    e = (TransEntry *)calloc(1, sizeof(TransEntry));
    e->key = strdup(key);
    e->value = strdup(value);
    e->next = self->buckets[h];
    self->buckets[h] = e;
}

static const char *trans_get(StringTranslate *self, const char *key) {
    unsigned int h = trans_hash(key);
    TransEntry *e = self->buckets[h];
    while (e) {
        if (strcmp(e->key, key) == 0)
            return e->value;
        e = e->next;
    }
    return NULL;
}

static void trans_load_file(StringTranslate *self, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f)
        return;
    char line[1024];
    while (fgets(line, sizeof(line), f)) {

        int len = (int)strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            line[--len] = '\0';

        if (len == 0 || line[0] == '#' || line[0] == '!')
            continue;

        char *sep = strchr(line, '=');
        if (!sep)
            sep = strchr(line, ':');
        if (!sep)
            continue;
        *sep = '\0';
        const char *key = line;
        const char *value = sep + 1;

        while (*value == ' ' || *value == '\t')
            value++;
        trans_put(self, key, value);
    }
    fclose(f);
}

static StringTranslate *s_instance = NULL;

static StringTranslate *StringTranslate_new(void) {
    StringTranslate *self = (StringTranslate *)calloc(1, sizeof(StringTranslate));

    trans_load_file(self, "lang/en_US.lang");
    trans_load_file(self, "lang/stats_US.lang");
    return self;
}

StringTranslate *StringTranslate_getInstance(void) {
    if (!s_instance)
        s_instance = StringTranslate_new();
    return s_instance;
}

const char *StringTranslate_translateKey(StringTranslate *self, const char *var1) {
    const char *v = trans_get(self, var1);
    return v ? v : var1;
}

const char *StringTranslate_vtranslateKeyFormat(StringTranslate *self, const char *var1,
                                                va_list args) {
    const char *fmt = trans_get(self, var1);
    if (!fmt)
        fmt = var1;
    static char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);
    return buf;
}

const char *StringTranslate_translateKeyFormat(StringTranslate *self, const char *var1, ...) {
    va_list args;
    va_start(args, var1);
    const char *result = StringTranslate_vtranslateKeyFormat(self, var1, args);
    va_end(args);
    return result;
}

const char *StringTranslate_translateNamedKey(StringTranslate *self, const char *var1) {
    char key[512];
    snprintf(key, sizeof(key), "%s.name", var1);
    const char *v = trans_get(self, key);
    return v ? v : "";
}
