#include "ThreadStatSyncherReceive.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void *ThreadStatSyncherReceive_run(void *arg) {
    ThreadStatSyncherReceive *self = (ThreadStatSyncherReceive *)arg;
    const char *f;
    if (StatsSyncher_func_27422_a(self->field_27231_a) != NULL) {
        StatsSyncher_func_27412_a(self->field_27231_a,
                                  StatsSyncher_func_27422_a(self->field_27231_a),
                                  StatsSyncher_func_27423_b(self->field_27231_a),
                                  StatsSyncher_func_27411_c(self->field_27231_a),
                                  StatsSyncher_func_27413_d(self->field_27231_a));
    } else {

        f = StatsSyncher_func_27423_b(self->field_27231_a);
        if (access(f, F_OK) == 0) {
            StatsSyncher_func_27421_a(
                self->field_27231_a,
                StatsSyncher_func_27409_a(self->field_27231_a,
                                          StatsSyncher_func_27423_b(self->field_27231_a),
                                          StatsSyncher_func_27411_c(self->field_27231_a),
                                          StatsSyncher_func_27413_d(self->field_27231_a)));
        }
    }

    StatsSyncher_func_27416_a(self->field_27231_a, 0);
    free(self);
    return NULL;
}

ThreadStatSyncherReceive *ThreadStatSyncherReceive_create(StatsSyncher *var1) {
    ThreadStatSyncherReceive *self =
        (ThreadStatSyncherReceive *)calloc(1, sizeof(ThreadStatSyncherReceive));
    self->field_27231_a = var1;
    return self;
}

void ThreadStatSyncherReceive_start(ThreadStatSyncherReceive *self) {
    pthread_create(&self->thread, NULL, ThreadStatSyncherReceive_run, self);
    pthread_detach(self->thread);
}
