#ifndef STRING_TRANSLATE_H
#define STRING_TRANSLATE_H

#include <stdarg.h>

typedef struct StringTranslate StringTranslate;

StringTranslate *StringTranslate_getInstance(void);

const char *StringTranslate_translateKey(StringTranslate *self, const char *var1);

const char *StringTranslate_translateKeyFormat(StringTranslate *self, const char *var1, ...);

const char *StringTranslate_vtranslateKeyFormat(StringTranslate *self, const char *var1,
                                                va_list args);

const char *StringTranslate_translateNamedKey(StringTranslate *self, const char *var1);

#endif
