#ifndef STATS_SYNCHER_H
#define STATS_SYNCHER_H

#include "Session.h"
#include "StatMap.h"

struct StatFileWriter;

typedef struct StatsSyncher {

    volatile int field_27438_a;

    StatMap *volatile field_27437_b;

    StatMap *volatile field_27436_c;

    struct StatFileWriter *field_27435_d;

    char field_27434_e[512];

    char field_27433_f[512];

    char field_27432_g[512];

    char field_27431_h[512];

    char field_27430_i[512];

    char field_27429_j[512];

    Session *field_27428_k;

    int field_27427_l;

    int field_27426_m;
} StatsSyncher;

StatsSyncher *StatsSyncher_create(Session *var1, struct StatFileWriter *var2, const char *var3);
void StatsSyncher_destroy(StatsSyncher *self);

void StatsSyncher_func_27418_a(StatsSyncher *self);

void StatsSyncher_func_27424_a(StatsSyncher *self, StatMap *var1);

void StatsSyncher_syncStatsFileWithMap(StatsSyncher *self, StatMap *var1);

int StatsSyncher_func_27420_b(StatsSyncher *self);

void StatsSyncher_func_27425_c(StatsSyncher *self);

StatMap *StatsSyncher_func_27422_a(StatsSyncher *var0);

const char *StatsSyncher_func_27423_b(StatsSyncher *var0);

const char *StatsSyncher_func_27411_c(StatsSyncher *var0);

const char *StatsSyncher_func_27413_d(StatsSyncher *var0);

void StatsSyncher_func_27412_a(StatsSyncher *var0, StatMap *var1, const char *var2,
                               const char *var3, const char *var4);

StatMap *StatsSyncher_func_27421_a(StatsSyncher *var0, StatMap *var1);

StatMap *StatsSyncher_func_27409_a(StatsSyncher *var0, const char *var1, const char *var2,
                                   const char *var3);

int StatsSyncher_func_27416_a(StatsSyncher *var0, int var1);

const char *StatsSyncher_func_27414_e(StatsSyncher *var0);

const char *StatsSyncher_func_27417_f(StatsSyncher *var0);

const char *StatsSyncher_func_27419_g(StatsSyncher *var0);

#endif
