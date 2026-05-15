#ifndef UNEXPECTED_THROWABLE_H
#define UNEXPECTED_THROWABLE_H

typedef struct UnexpectedThrowable {

    const char *description;

    const char *exception;
} UnexpectedThrowable;

UnexpectedThrowable *UnexpectedThrowable_create(const char *var1, const char *var2);

void UnexpectedThrowable_destroy(UnexpectedThrowable *self);

#endif
