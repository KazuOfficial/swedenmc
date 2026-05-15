#include "StatCollector.h"
#include "StringTranslate.h"
#include <stdarg.h>

const char *StatCollector_translateToLocal(const char *var0) {
    return StringTranslate_translateKey(StringTranslate_getInstance(), var0);
}

const char *StatCollector_translateToLocalFormatted(const char *var0, ...) {
    va_list args;
    va_start(args, var0);
    const char *result =
        StringTranslate_vtranslateKeyFormat(StringTranslate_getInstance(), var0, args);
    va_end(args);
    return result;
}
