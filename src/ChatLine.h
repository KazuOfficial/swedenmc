#ifndef CHAT_LINE_H
#define CHAT_LINE_H

typedef struct ChatLine {

    char *message;

    int updateCounter;
} ChatLine;

void ChatLine_construct(ChatLine *self, const char *var1);

void ChatLine_free(ChatLine *self);

#endif
