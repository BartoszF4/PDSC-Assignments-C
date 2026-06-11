#include <stdio.h>
#include <string.h>
#include "binarySearch.h"

typedef struct {
    char name[30];
    char surname[30];
    int age;
} Person;

int compareInts(const void *a, const void *b) {
    int valA = *(const int *)a;
    int valB = *(const int *)b;
    
    if (valA < valB) {
        return -1;
    } else if (valA > valB) {
        return 1;
    }
    return 0;
}

int compareDoubles(const void *a, const void *b) {
    double valA = *(const double *)a;
    double valB = *(const double *)b;
    
    if (valA < valB) {
        return -1;
    } else if (valA > valB) {
        return 1;
    }
    return 0;
}

int compareStrings(const void *a, const void *b) {
    const char *strA = *(const char **)a;
    const char *strB = *(const char **)b;
    
    return strcmp(strA, strB);
}

int comparePersonsByName(const void *a, const void *b) {
    const Person *personA = (const Person *)a;
    const Person *personB = (const Person *)b;
    
    return strcmp(personA->name, personB->name);
}

int comparePersonsBySurname(const void *a, const void *b) {
    const Person *personA = (const Person *)a;
    const Person *personB = (const Person *)b;
    
    return strcmp(personA->surname, personB->surname);
}

int comparePersonsByAge(const void *a, const void *b) {
    const Person *personA = (const Person *)a;
    const Person *personB = (const Person *)b;
    
    if (personA->age < personB->age) {
        return -1;
    } else if (personA->age > personB->age) {
        return 1;
    }
    return 0;
}

void testInts(void) {
    printf("\n=== Test Ints ===\n");
    int arr[] = {40, 10, 50, 20, 30};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int key = 30;

    bubbleSort(arr, size, sizeof(int), compareInts);
    int *found = bsearch(&key, arr, size, sizeof(int), compareInts);
    
    if (found != NULL) {
        printf("Success: Element found %d\n", *found);
    } else {
        printf("Error: Element not found\n");
    }
}

void testDoubles(void) {
    printf("\n=== Test Doubles ===\n");
    double arr[] = {3.3, 1.1, 4.4, 2.2};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    double key = 2.5; 

    bubbleSort(arr, size, sizeof(double), compareDoubles);
    double *found = bsearch(&key, arr, size, sizeof(double), compareDoubles);
    
    if (found == NULL) {
        printf("Success: Correctly not found %f\n", key);
    } else {
        printf("Error: Found unexpected value\n");
    }
}

void testStrings(void) {
    printf("\n=== Test Strings ===\n");
    const char *arr[] = {"plum", "apple", "lemon", "banana"};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    const char *key = "lemon";

    bubbleSort(arr, size, sizeof(const char *), compareStrings);
    const char **found = bsearch(&key, arr, size, sizeof(const char *), compareStrings);
    
    if (found != NULL) {
        printf("Success: String found '%s'\n", *found);
    } else {
        printf("Error: String not found\n");
    }
}

void testStructsByName(void) {
    printf("\n=== Test Structs By Name ===\n");
    Person arr[] = {{"John", "Doe", 25}, {"Adam", "Smith", 20}, {"Eve", "Brown", 22}};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    Person key = {"Eve", "", 0}; 

    bubbleSort(arr, size, sizeof(Person), comparePersonsByName);
    Person *found = bsearch(&key, arr, size, sizeof(Person), comparePersonsByName);
    
    if (found != NULL) {
        printf("Success: Found person %s %s\n", found->name, found->surname);
    } else {
        printf("Error: Person not found\n");
    }
}

void testStructsBySurname(void) {
    printf("\n=== Test Structs By Surname ===\n");
    Person arr[] = {{"John", "Doe", 25}, {"Eve", "Brown", 22}, {"Adam", "Smith", 20}};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    Person key = {"", "Doe", 0}; 

    bubbleSort(arr, size, sizeof(Person), comparePersonsBySurname);
    Person *found = bsearch(&key, arr, size, sizeof(Person), comparePersonsBySurname);
    
    if (found != NULL) {
        printf("Success: Found person %s %s\n", found->name, found->surname);
    } else {
        printf("Error: Person not found\n");
    }
}

void testStructsByAge(void) {
    printf("\n=== Test Structs By Age ===\n");
    Person arr[] = {{"John", "Doe", 25}, {"Adam", "Smith", 20}, {"Eve", "Brown", 22}};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    Person key = {"", "", 25}; 

    bubbleSort(arr, size, sizeof(Person), comparePersonsByAge);
    Person *found = bsearch(&key, arr, size, sizeof(Person), comparePersonsByAge);
    
    if (found != NULL) {
        printf("Success: Found person %s %s (Age: %d)\n", found->name, found->surname, found->age);
    } else {
        printf("Error: Person not found\n");
    }
}

int main(void) {
    testInts();
    testDoubles();
    testStrings();
    testStructsByName();
    testStructsBySurname();
    testStructsByAge();
    
    return 0;
}