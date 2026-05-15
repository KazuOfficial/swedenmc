#include "JavaSort.h"
#include <stdlib.h>
#include <string.h>

#define INSERTIONSORT_THRESHOLD 7

static void swap(void **x, int a, int b) {
    void *t = x[a];
    x[a] = x[b];
    x[b] = t;
}

static void mergeSort(void **src, void **dest, int low, int high, int off,
                      int (*c)(const void *, const void *)) {
    int length = high - low;

    if (length < INSERTIONSORT_THRESHOLD) {
        for (int i = low; i < high; i++)
            for (int j = i; j > low && c(&dest[j - 1], &dest[j]) > 0; j--)
                swap(dest, j, j - 1);
        return;
    }

    int destLow = low;
    int destHigh = high;
    low += off;
    high += off;
    int mid = (int)((unsigned int)(low + high) >> 1);
    mergeSort(dest, src, low, mid, -off, c);
    mergeSort(dest, src, mid, high, -off, c);

    if (c(&src[mid - 1], &src[mid]) <= 0) {
        memcpy(dest + destLow, src + low, length * sizeof(void *));
        return;
    }

    for (int i = destLow, p = low, q = mid; i < destHigh; i++) {
        if (q >= high || (p < mid && c(&src[p], &src[q]) <= 0))
            dest[i] = src[p++];
        else
            dest[i] = src[q++];
    }
}

void Arrays_sort(void **a, int len, int (*c)(const void *, const void *)) {
    void **aux = (void **)malloc(len * sizeof(void *));
    memcpy(aux, a, len * sizeof(void *));
    mergeSort(aux, a, 0, len, 0, c);
    free(aux);
}
