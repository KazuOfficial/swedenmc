#include "StatsSyncher.h"
#include "StatFileWriter.h"
#include "ThreadStatSyncherReceive.h"
#include "ThreadStatSyncherSend.h"
#include "StatMap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

static void func_28214_a(const char *var1, const char *var2, const char *var3) {
    char var4[512];
    struct stat st;
    snprintf(var4, sizeof(var4), "%s/%s", var1, var2);

    if (stat(var4, &st) == 0 && !S_ISDIR(st.st_mode) && access(var3, F_OK) != 0) {
        rename(var4, var3);
    }
}

static StatMap *func_27408_a(StatsSyncher *self, const char *var1) {
    FILE *fp;
    char *buf;
    long len;
    StatMap *result;
    (void)self;

    fp = fopen(var1, "r");
    if (fp == NULL)
        return NULL;

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (len <= 0) {
        fclose(fp);
        return NULL;
    }

    buf = (char *)malloc((size_t)(len + 1));
    if (buf == NULL) {
        fclose(fp);
        return NULL;
    }

    {
        long pos = 0;
        char line[4096];
        buf[0] = '\0';
        while (fgets(line, sizeof(line), fp) != NULL) {
            size_t ll = strlen(line);

            if (ll > 0 && line[ll - 1] == '\n') {
                line[--ll] = '\0';
            }
            if (ll > 0 && line[ll - 1] == '\r') {
                line[--ll] = '\0';
            }

            memcpy(buf + pos, line, ll);
            pos += (long)ll;
        }
        buf[pos] = '\0';
    }
    fclose(fp);

    result = StatFileWriter_func_27177_a(buf);
    free(buf);
    return result;
}

static StatMap *func_27415_a(StatsSyncher *self, const char *var1, const char *var2,
                             const char *var3) {
    if (access(var1, F_OK) == 0)
        return func_27408_a(self, var1);
    if (access(var3, F_OK) == 0)
        return func_27408_a(self, var3);
    if (access(var2, F_OK) == 0)
        return func_27408_a(self, var2);
    return NULL;
}

static void func_27410_a(StatsSyncher *self, StatMap *var1, const char *var2, const char *var3,
                         const char *var4) {
    const char *content;
    FILE *fp;

    content = StatFileWriter_func_27185_a(self->field_27428_k->username, "local", var1);
    fp = fopen(var3, "w");
    if (fp != NULL) {
        fputs(content, fp);
        fclose(fp);
    }
    free((void *)content);

    if (access(var4, F_OK) == 0)
        remove(var4);

    if (access(var2, F_OK) == 0)
        rename(var2, var4);

    rename(var3, var2);
}

StatsSyncher *StatsSyncher_create(Session *var1, struct StatFileWriter *var2, const char *var3) {
    StatsSyncher *self = (StatsSyncher *)calloc(1, sizeof(StatsSyncher));

    char lname[64];
    {
        int ci;
        for (ci = 0; ci < 63 && var1->username[ci]; ci++)
            lname[ci] = (char)tolower((unsigned char)var1->username[ci]);
        lname[ci] = '\0';
    }

    snprintf(self->field_27434_e, sizeof(self->field_27434_e), "%s/stats_%s_unsent.dat", var3,
             lname);
    snprintf(self->field_27433_f, sizeof(self->field_27433_f), "%s/stats_%s.dat", var3, lname);
    snprintf(self->field_27430_i, sizeof(self->field_27430_i), "%s/stats_%s_unsent.old", var3,
             lname);
    snprintf(self->field_27429_j, sizeof(self->field_27429_j), "%s/stats_%s.old", var3, lname);
    snprintf(self->field_27432_g, sizeof(self->field_27432_g), "%s/stats_%s_unsent.tmp", var3,
             lname);
    snprintf(self->field_27431_h, sizeof(self->field_27431_h), "%s/stats_%s.tmp", var3, lname);

    if (strcmp(lname, var1->username) != 0) {
        char mixedUnsent[256], mixedDat[256], mixedUnsentOld[256];
        char mixedOld[256], mixedUnsentTmp[256], mixedTmp[256];
        snprintf(mixedUnsent, sizeof(mixedUnsent), "stats_%s_unsent.dat", var1->username);
        snprintf(mixedDat, sizeof(mixedDat), "stats_%s.dat", var1->username);
        snprintf(mixedUnsentOld, sizeof(mixedUnsentOld), "stats_%s_unsent.old", var1->username);
        snprintf(mixedOld, sizeof(mixedOld), "stats_%s.old", var1->username);
        snprintf(mixedUnsentTmp, sizeof(mixedUnsentTmp), "stats_%s_unsent.tmp", var1->username);
        snprintf(mixedTmp, sizeof(mixedTmp), "stats_%s.tmp", var1->username);
        func_28214_a(var3, mixedUnsent, self->field_27434_e);
        func_28214_a(var3, mixedDat, self->field_27433_f);
        func_28214_a(var3, mixedUnsentOld, self->field_27430_i);
        func_28214_a(var3, mixedOld, self->field_27429_j);
        func_28214_a(var3, mixedUnsentTmp, self->field_27432_g);
        func_28214_a(var3, mixedTmp, self->field_27431_h);
    }

    self->field_27435_d = var2;
    self->field_27428_k = var1;

    if (access(self->field_27434_e, F_OK) == 0) {
        StatMap *loaded =
            func_27415_a(self, self->field_27434_e, self->field_27432_g, self->field_27430_i);
        StatFileWriter_func_27179_a(var2, loaded);
        if (loaded)
            StatMap_destroy(loaded);
    }

    StatsSyncher_func_27418_a(self);
    return self;
}

void StatsSyncher_destroy(StatsSyncher *self) {
    if (self->field_27437_b)
        StatMap_destroy(self->field_27437_b);
    if (self->field_27436_c)
        StatMap_destroy(self->field_27436_c);
    free(self);
}

void StatsSyncher_func_27418_a(StatsSyncher *self) {
    if (self->field_27438_a) {

        fprintf(stderr,
                "IllegalStateException: Can't get stats from server while StatsSyncher is busy!\n");
        abort();
    }
    self->field_27427_l = 100;
    self->field_27438_a = 1;
    {
        ThreadStatSyncherReceive *t = ThreadStatSyncherReceive_create(self);
        ThreadStatSyncherReceive_start(t);
    }
}

void StatsSyncher_func_27424_a(StatsSyncher *self, StatMap *var1) {
    if (self->field_27438_a) {

        fprintf(stderr, "IllegalStateException: Can't save stats while StatsSyncher is busy!\n");
        abort();
    }
    self->field_27427_l = 100;
    self->field_27438_a = 1;
    {
        ThreadStatSyncherSend *t = ThreadStatSyncherSend_create(self, var1);
        ThreadStatSyncherSend_start(t);
    }
}

void StatsSyncher_syncStatsFileWithMap(StatsSyncher *self, StatMap *var1) {
    int var2 = 30;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100000000L;
    while (self->field_27438_a) {
        --var2;
        if (var2 <= 0)
            break;
        nanosleep(&ts, NULL);
    }
    self->field_27438_a = 1;
    func_27410_a(self, var1, self->field_27434_e, self->field_27432_g, self->field_27430_i);
    self->field_27438_a = 0;
}

int StatsSyncher_func_27420_b(StatsSyncher *self) {
    return self->field_27427_l <= 0 && !self->field_27438_a && self->field_27436_c == NULL;
}

void StatsSyncher_func_27425_c(StatsSyncher *self) {
    if (self->field_27427_l > 0)
        --self->field_27427_l;
    if (self->field_27426_m > 0)
        --self->field_27426_m;
    if (self->field_27436_c != NULL) {
        StatFileWriter_func_27187_c(self->field_27435_d, self->field_27436_c);
        StatMap_destroy(self->field_27436_c);
        self->field_27436_c = NULL;
    }
    if (self->field_27437_b != NULL) {
        StatFileWriter_func_27180_b(self->field_27435_d, self->field_27437_b);
        StatMap_destroy(self->field_27437_b);
        self->field_27437_b = NULL;
    }
}

StatMap *StatsSyncher_func_27422_a(StatsSyncher *var0) { return var0->field_27437_b; }

const char *StatsSyncher_func_27423_b(StatsSyncher *var0) { return var0->field_27433_f; }

const char *StatsSyncher_func_27411_c(StatsSyncher *var0) { return var0->field_27431_h; }

const char *StatsSyncher_func_27413_d(StatsSyncher *var0) { return var0->field_27429_j; }

void StatsSyncher_func_27412_a(StatsSyncher *var0, StatMap *var1, const char *var2,
                               const char *var3, const char *var4) {
    func_27410_a(var0, var1, var2, var3, var4);
}

StatMap *StatsSyncher_func_27421_a(StatsSyncher *var0, StatMap *var1) {
    return var0->field_27437_b = var1;
}

StatMap *StatsSyncher_func_27409_a(StatsSyncher *var0, const char *var1, const char *var2,
                                   const char *var3) {
    return func_27415_a(var0, var1, var2, var3);
}

int StatsSyncher_func_27416_a(StatsSyncher *var0, int var1) { return var0->field_27438_a = var1; }

const char *StatsSyncher_func_27414_e(StatsSyncher *var0) { return var0->field_27434_e; }

const char *StatsSyncher_func_27417_f(StatsSyncher *var0) { return var0->field_27432_g; }

const char *StatsSyncher_func_27419_g(StatsSyncher *var0) { return var0->field_27430_i; }
