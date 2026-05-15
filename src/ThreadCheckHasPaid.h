#ifndef THREAD_CHECK_HAS_PAID_H
#define THREAD_CHECK_HAS_PAID_H

#include <pthread.h>

struct Minecraft;

typedef struct ThreadCheckHasPaid {

    struct Minecraft *field_28146_a;
    pthread_t thread;
} ThreadCheckHasPaid;

ThreadCheckHasPaid *ThreadCheckHasPaid_create(struct Minecraft *var1);

void ThreadCheckHasPaid_destroy(ThreadCheckHasPaid *self);

#endif
