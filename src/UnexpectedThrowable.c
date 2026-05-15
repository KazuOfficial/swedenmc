#include "UnexpectedThrowable.h"
#include <stdlib.h>

UnexpectedThrowable *UnexpectedThrowable_create(const char *var1, const char *var2) {
    UnexpectedThrowable *self = (UnexpectedThrowable *)calloc(1, sizeof(UnexpectedThrowable));
    self->description = var1;
    self->exception = var2;
    return self;
}

void UnexpectedThrowable_destroy(UnexpectedThrowable *self) { free(self); }
