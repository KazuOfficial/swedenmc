#include "J_JsonNodeDoesNotMatchJsonNodeSelectorException.h"
#include "J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException.h"
#include <string.h>

void J_JsonNodeDoesNotMatchJsonNodeSelectorException_init(
    J_JsonNodeDoesNotMatchJsonNodeSelectorException *self, const char *msg) {
    strncpy(self->message, msg, sizeof(self->message) - 1);
    self->message[sizeof(self->message) - 1] = '\0';
}

J_JsonNodeDoesNotMatchChainedJsonNodeSelectorException *J_ChainedException_pending = NULL;
