#ifndef ISTAT_STRING_FORMAT_H
#define ISTAT_STRING_FORMAT_H

typedef struct IStatStringFormat IStatStringFormat;

typedef struct IStatStringFormatVtable {

    const char *(*formatString)(IStatStringFormat *self, const char *var1);
} IStatStringFormatVtable;

struct IStatStringFormat {
    const IStatStringFormatVtable *vtable;
};

#endif
