#ifndef STAT_FILE_WRITER_H
#define STAT_FILE_WRITER_H

#include "Session.h"
#include "StatMap.h"

struct StatBase;
struct Achievement;
struct StatsSyncher;

typedef struct StatFileWriter {

    StatMap *field_25102_a;

    StatMap *field_25101_b;

    int field_27189_c;

    struct StatsSyncher *statsSyncher;
} StatFileWriter;

StatFileWriter *StatFileWriter_create(Session *var1, const char *var2);
void StatFileWriter_destroy(StatFileWriter *self);

void StatFileWriter_readStat(StatFileWriter *self, struct StatBase *var1, int var2);

StatMap *StatFileWriter_func_27176_a(StatFileWriter *self);

void StatFileWriter_func_27179_a(StatFileWriter *self, StatMap *var1);

void StatFileWriter_func_27180_b(StatFileWriter *self, StatMap *var1);

void StatFileWriter_func_27187_c(StatFileWriter *self, StatMap *var1);

StatMap *StatFileWriter_func_27177_a(const char *var0);

const char *StatFileWriter_func_27185_a(const char *var0, const char *var1, StatMap *var2);

int StatFileWriter_hasAchievementUnlocked(StatFileWriter *self, struct Achievement *var1);

int StatFileWriter_func_27181_b(StatFileWriter *self, struct Achievement *var1);

int StatFileWriter_writeStat(StatFileWriter *self, struct StatBase *var1);

void StatFileWriter_func_27175_b(StatFileWriter *self);

void StatFileWriter_syncStats(StatFileWriter *self);

void StatFileWriter_func_27178_d(StatFileWriter *self);

#endif
