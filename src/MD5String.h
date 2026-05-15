#ifndef MD5STRING_H
#define MD5STRING_H

typedef struct MD5String {

    const char *field_27370_a;
} MD5String;

void MD5String_init(MD5String *self, const char *var1);

char *MD5String_func_27369_a(MD5String *self, const char *var1, char *out_buf);

#endif
