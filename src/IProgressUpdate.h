#ifndef IPROGRESS_UPDATE_H
#define IPROGRESS_UPDATE_H

typedef struct IProgressUpdate {
    void (*func_594_b)(struct IProgressUpdate *self, const char *str);
    void (*displayLoadingString)(struct IProgressUpdate *self, const char *str);
    void (*setLoadingProgress)(struct IProgressUpdate *self, int progress);
} IProgressUpdate;

#endif
