#ifndef ISTAT_TYPE_H
#define ISTAT_TYPE_H

typedef struct IStatType IStatType;

typedef struct IStatTypeVtable {

    const char *(*func_27192_a)(IStatType *self, int var1);
} IStatTypeVtable;

struct IStatType {
    const IStatTypeVtable *vtable;
};

#endif
