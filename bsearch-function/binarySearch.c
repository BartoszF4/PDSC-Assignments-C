#include "binarySearch.h"

void swapElements(void *a, void *b, size_t size) {
    unsigned char *p = (unsigned char *)a;
    unsigned char *q = (unsigned char *)b;
    unsigned char temp;

    for (size_t i = 0; i < size; i++) {
        temp = p[i];
        p[i] = q[i];
        q[i] = temp;
    }
}

void bubbleSort(void *base, size_t num, size_t size, int (*compar)(const void *, const void *)) {
    if (num <= 1 || base == NULL) {
        return;
    }
    
    unsigned char *ptr = (unsigned char *)base;

    for (size_t i = 0; i < num - 1; i++) {
        for (size_t j = 0; j < num - i - 1; j++) {
            void *elem1 = ptr + j * size;
            void *elem2 = ptr + (j + 1) * size;

            if (compar(elem1, elem2) > 0) {
                swapElements(elem1, elem2, size);
            }
        }
    }
}

void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *)) {
    size_t left = 0;
    size_t right = num;
    const unsigned char *ptrBase = (const unsigned char *)base;

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        const void *midElement = ptrBase + (mid * size);
        int cmpResult = compar(key, midElement);

        if (cmpResult == 0) {
            return (void *)midElement;
        } else if (cmpResult < 0) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    return NULL;
}