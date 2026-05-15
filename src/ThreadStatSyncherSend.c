#include "ThreadStatSyncherSend.h"
#include <stdlib.h>

static void *ThreadStatSyncherSend_run(void *arg) {
    ThreadStatSyncherSend *self = (ThreadStatSyncherSend *)arg;
    StatsSyncher_func_27412_a(self->field_27232_b, self->field_27233_a,
                              StatsSyncher_func_27414_e(self->field_27232_b),
                              StatsSyncher_func_27417_f(self->field_27232_b),
                              StatsSyncher_func_27419_g(self->field_27232_b));
    StatsSyncher_func_27416_a(self->field_27232_b, 0);
    StatMap_destroy(self->field_27233_a);
    free(self);
    return NULL;
}

ThreadStatSyncherSend *ThreadStatSyncherSend_create(StatsSyncher *var1, StatMap *var2) {
    ThreadStatSyncherSend *self = (ThreadStatSyncherSend *)calloc(1, sizeof(ThreadStatSyncherSend));
    self->field_27232_b = var1;
    self->field_27233_a = var2;
    return self;
}

void ThreadStatSyncherSend_start(ThreadStatSyncherSend *self) {
    pthread_create(&self->thread, NULL, ThreadStatSyncherSend_run, self);
    pthread_detach(self->thread);
}
