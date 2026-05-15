#include "ChatLine.h"
#include <stdlib.h>
#include <string.h>

void ChatLine_construct(ChatLine *self, const char *var1) {
    self->message = var1 ? strdup(var1) : NULL;
    self->updateCounter = 0;
}

void ChatLine_free(ChatLine *self) {
    if (!self)
        return;
    free(self->message);
    free(self);
}
