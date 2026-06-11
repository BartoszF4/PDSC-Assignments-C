#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <stddef.h>

void swapElements(void *a, void *b, size_t size);
void bubbleSort(void *base, size_t num, size_t size, int (*compar)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *));

#endif