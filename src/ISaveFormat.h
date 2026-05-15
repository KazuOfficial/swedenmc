#ifndef ISAVE_FORMAT_H
#define ISAVE_FORMAT_H

#include "IProgressUpdate.h"
#include "ISaveHandler.h"
#include "WorldInfo.h"
#include "SaveFormatComparator.h"

struct ISaveFormat;

typedef struct ISaveFormatVtable {

    const char *(*func_22178_a)(struct ISaveFormat *self);

    ISaveHandler *(*getSaveLoader)(struct ISaveFormat *self, const char *var1, int var2);

    SaveFormatComparatorList *(*func_22176_b)(struct ISaveFormat *self);

    void (*flushCache)(struct ISaveFormat *self);

    WorldInfo *(*func_22173_b)(struct ISaveFormat *self, const char *var1);

    void (*func_22172_c)(struct ISaveFormat *self, const char *var1);

    void (*func_22170_a)(struct ISaveFormat *self, const char *var1, const char *var2);

    int (*isOldMapFormat)(struct ISaveFormat *self, const char *var1);

    int (*convertMapFormat)(struct ISaveFormat *self, const char *var1, IProgressUpdate *var2);
} ISaveFormatVtable;

typedef struct ISaveFormat {
    const ISaveFormatVtable *vtable;
} ISaveFormat;

static inline const char *ISaveFormat_func_22178_a(struct ISaveFormat *s) {
    return s->vtable->func_22178_a(s);
}
static inline ISaveHandler *ISaveFormat_getSaveLoader(struct ISaveFormat *s, const char *var1,
                                                      int var2) {
    return s->vtable->getSaveLoader(s, var1, var2);
}
static inline SaveFormatComparatorList *ISaveFormat_func_22176_b(struct ISaveFormat *s) {
    return s->vtable->func_22176_b(s);
}
static inline void ISaveFormat_flushCache(struct ISaveFormat *s) { s->vtable->flushCache(s); }
static inline WorldInfo *ISaveFormat_func_22173_b(struct ISaveFormat *s, const char *var1) {
    return s->vtable->func_22173_b(s, var1);
}
static inline void ISaveFormat_func_22172_c(struct ISaveFormat *s, const char *var1) {
    s->vtable->func_22172_c(s, var1);
}
static inline void ISaveFormat_func_22170_a(struct ISaveFormat *s, const char *var1,
                                            const char *var2) {
    s->vtable->func_22170_a(s, var1, var2);
}
static inline int ISaveFormat_isOldMapFormat(struct ISaveFormat *s, const char *var1) {
    return s->vtable->isOldMapFormat(s, var1);
}
static inline int ISaveFormat_convertMapFormat(struct ISaveFormat *s, const char *var1,
                                               IProgressUpdate *var2) {
    return s->vtable->convertMapFormat(s, var1, var2);
}

#endif
