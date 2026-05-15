#include "rand_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 16
#define CAPACITY_MULTIPLIER 2
#define OCTAL_BASE 8

typedef struct {
    char** data;
    size_t size;
    size_t capacity;
} StringList;

char* getLine(void);
bool expandBuffer(char** buffer, size_t* capacity);
char* finalizeBuffer(char* buffer, size_t length);
const char* skipSpace(const char* str);
bool isValidOctalTail(const char* start, const char** end);
char* trimAndValidateOctal(const char* line, bool* isFormatError, bool* outOfMemory);
void calculateOctalSum(const char* n1, const char* n2, char* res);
char* addOctalStrings(const char* num1, const char* num2);
bool addNumberToList(StringList* list, char* number);
void freeStringList(StringList* list);
char* duplicateString(const char* str);
void reverseString(char* str, size_t length);
void stripLeadingZeros(char* str);
void processLine(char* line, char** currentSum, StringList* nums, bool* errFmt, bool* errMem);
void printResults(const char* sum, const StringList* nums);

int main(void) {
    StringList nums = {NULL, 0, 0};
    char *sum = NULL, *line = NULL;
    bool errFmt = false, errMem = false;

    while (!errFmt && !errMem && (line = getLine()) != NULL) {
        processLine(line, &sum, &nums, &errFmt, &errMem);
        free(line);
    }

    if (line == NULL && !feof(stdin)) errMem = true;

    if (errMem) printf("Error: bad memory allocation\n");
    else if (!errFmt && sum != NULL) printResults(sum, &nums);

    free(sum);
    freeStringList(&nums);
    return errMem ? 1 : 0;
}

void processLine(char* line, char** sum, StringList* nums, bool* eFmt, bool* eMem) {
    char* clean = trimAndValidateOctal(line, eFmt, eMem);
    if (*eFmt) {
        printf("Error: Invalid input format.\n");
        return;
    }
    if (clean != NULL) {
        char* newSum = addOctalStrings(*sum, clean);
        if (newSum == NULL || !addNumberToList(nums, clean)) {
            *eMem = true;
            free(newSum);
            free(clean);
            return;
        }
        free(*sum);
        *sum = newSum;
    }
}

void printResults(const char* sum, const StringList* nums) {
    printf("\nSum:\n%s\n\nInput numbers:\n", sum);
    for (size_t i = 0; i < nums->size; ++i) {
        printf("%s\n", nums->data[i]);
    }
}

char* getLine(void) {
    size_t cap = INITIAL_CAPACITY, len = 0;
    char* buf = malloc(cap);
    int c;
    
    if (buf == NULL) return NULL;

    while ((c = getchar()) != EOF && c != '\n') {
        buf[len++] = (char)c;
        if (len >= cap && !expandBuffer(&buf, &cap)) return NULL;
    }
    
    if (c == EOF && len == 0) {
        free(buf);
        return NULL;
    }
    return finalizeBuffer(buf, len);
}

bool expandBuffer(char** buffer, size_t* capacity) {
    size_t newCap = *capacity * CAPACITY_MULTIPLIER;
    char* tmp = realloc(*buffer, newCap);
    
    if (tmp == NULL) {
        free(*buffer);
        return false;
    }
    *buffer = tmp;
    *capacity = newCap;
    return true;
}

char* finalizeBuffer(char* buffer, size_t length) {
    char* tmp = realloc(buffer, length + 1);
    
    if (tmp == NULL) {
        free(buffer);
        return NULL;
    }
    tmp[length] = '\0';
    return tmp;
}

const char* skipSpace(const char* str) {
    while (isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}

bool isValidOctalTail(const char* start, const char** end) {
    *end = start;
    while (**end != '\0' && !isspace((unsigned char)**end)) {
        if (**end < '0' || **end > '7') return false;
        (*end)++;
    }
    for (const char* t = *end; *t != '\0'; t++) {
        if (!isspace((unsigned char)*t)) return false;
    }
    return true;
}

char* trimAndValidateOctal(const char* line, bool* eFmt, bool* eMem) {
    *eFmt = *eMem = false;
    const char* start = skipSpace(line);
    
    if (*start == '\0') return NULL;

    const char* end;
    if (!isValidOctalTail(start, &end)) {
        *eFmt = true;
        return NULL;
    }

    char* num = malloc(end - start + 1);
    if (num == NULL) { *eMem = true; return NULL; }
    strncpy(num, start, end - start);
    num[end - start] = '\0';
    return num;
}

void calculateOctalSum(const char* n1, const char* n2, char* res) {
    int c = 0, i1 = (int)strlen(n1) - 1, i2 = (int)strlen(n2) - 1;
    size_t i = 0;
    
    while (i1 >= 0 || i2 >= 0 || c > 0) {
        int d1 = (i1 >= 0) ? (n1[i1--] - '0') : 0;
        int d2 = (i2 >= 0) ? (n2[i2--] - '0') : 0;
        int sum = d1 + d2 + c;
        
        res[i++] = (char)((sum % OCTAL_BASE) + '0');
        c = sum / OCTAL_BASE;
    }
    res[i] = '\0';
}

char* addOctalStrings(const char* num1, const char* num2) {
    if (num1 == NULL || num2 == NULL) {
        return duplicateString(num1 ? num1 : num2);
    }

    size_t l1 = strlen(num1), l2 = strlen(num2);
    char* res = malloc((l1 > l2 ? l1 : l2) + 2);
    if (res == NULL) return NULL;

    calculateOctalSum(num1, num2, res);
    reverseString(res, strlen(res));
    stripLeadingZeros(res);

    char* shrunk = realloc(res, strlen(res) + 1);
    return shrunk ? shrunk : res;
}

bool addNumberToList(StringList* list, char* number) {
    if (list->size >= list->capacity) {
        size_t newCap = list->capacity ? list->capacity * CAPACITY_MULTIPLIER : INITIAL_CAPACITY;
        char** newData = realloc(list->data, newCap * sizeof(char*));
        if (newData == NULL) return false;
        list->data = newData;
        list->capacity = newCap;
    }
    list->data[list->size++] = number;
    return true;
}

void freeStringList(StringList* list) {
    if (list == NULL || list->data == NULL) return;
    for (size_t i = 0; i < list->size; ++i) {
        free(list->data[i]);
    }
    free(list->data);
}

char* duplicateString(const char* str) {
    if (str == NULL) return NULL;
    char* copy = malloc(strlen(str) + 1);
    
    if (copy != NULL) {
        strcpy(copy, str);
    }
    return copy;
}

void reverseString(char* str, size_t length) {
    for (size_t j = 0; j < length / 2; ++j) {
        char temp = str[j];
        str[j] = str[length - 1 - j];
        str[length - 1 - j] = temp;
    }
}

void stripLeadingZeros(char* str) {
    if (str == NULL) return;
    size_t lead = 0;
    while (str[lead] == '0' && str[lead + 1] != '\0') {
        lead++;
    }
    if (lead > 0) {
        memmove(str, str + lead, strlen(str) - lead + 1);
    }
}