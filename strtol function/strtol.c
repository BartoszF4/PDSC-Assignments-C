#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

#define BASE_AUTO 0
#define BASE_OCTAL 8
#define BASE_DECIMAL 10
#define BASE_HEX 16
#define PREFIX_HEX_LEN 2

#define VALID_BASE_MIN 2
#define VALID_BASE_MAX 36

#define INVALID_DIGIT -1
#define OVERFLOW_OCCURRED 1
#define NO_OVERFLOW 0

#define FLAG_SET 1
#define FLAG_UNSET 0

int characterDigitValue(char character) {
    if (isdigit((unsigned char)character)) {
        return character - '0';
    }
    if (isalpha((unsigned char)character)) {
        return tolower((unsigned char)character) - 'a' + BASE_DECIMAL;
    }
    return INVALID_DIGIT;
}

int hasHexPrefix(const char* str) {
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        int digit = characterDigitValue(str[PREFIX_HEX_LEN]);
        return (digit >= 0 && digit < BASE_HEX);
    }
    return FLAG_UNSET;
}

const char* getNumberBase(const char* ptr, int* base) {
    if (*base == BASE_AUTO) {
        if (hasHexPrefix(ptr)) {
            *base = BASE_HEX;
            return ptr + PREFIX_HEX_LEN;
        }
        if (*ptr == '0') {
            *base = BASE_OCTAL;
            return ptr;
        }
        *base = BASE_DECIMAL;
    } else if (*base == BASE_HEX && hasHexPrefix(ptr)) {
        return ptr + PREFIX_HEX_LEN;
    }
    return ptr;
}

const char* parseSign(const char* ptr, int* isNeg) {
    *isNeg = FLAG_UNSET;
    while (isspace((unsigned char)*ptr)) {
        ptr++;
    }
    if (*ptr == '-') {
        *isNeg = FLAG_SET;
        ptr++;
    } else if (*ptr == '+') {
        ptr++;
    }
    return ptr;
}

void calculateLimits(int isNeg, int base, long int* cutoff, int* cutlim) {
    long int limit = isNeg ? LONG_MIN : LONG_MAX;
    *cutoff = limit / base;
    *cutlim = (int)(limit % base); 
}

int checkOverflow(long int acc, long int cutoff, int cutlim, int digit, int isNeg) {
    if (isNeg) {
        if (acc < cutoff) return OVERFLOW_OCCURRED;
        if (acc == cutoff && -digit < cutlim) return OVERFLOW_OCCURRED;
    } else {
        if (acc > cutoff) return OVERFLOW_OCCURRED;
        if (acc == cutoff && digit > cutlim) return OVERFLOW_OCCURRED;
    }
    return NO_OVERFLOW;
}

long int processDigits(const char** ptr, int base, int isNeg, int* overflow, int* hasValid) {
    long int acc = 0, cutoff;
    int digit, cutlim;
    
    calculateLimits(isNeg, base, &cutoff, &cutlim);
    
    while ((digit = characterDigitValue(**ptr)) >= 0 && digit < base) {
        *hasValid = FLAG_SET;
        if (!*overflow) {
            if (checkOverflow(acc, cutoff, cutlim, digit, isNeg)) {
                *overflow = OVERFLOW_OCCURRED;
            } else {
                acc = isNeg ? (acc * base - digit) : (acc * base + digit);
            }
        }
        (*ptr)++;
    }
    return acc;
}

long int getFinalValue(int overflow, int isNeg, long int acc) {
    if (overflow) {
        errno = ERANGE;
        return isNeg ? LONG_MIN : LONG_MAX;
    }
    return acc;
}

int isBaseInvalid(int base) {
    return (base < VALID_BASE_MIN && base != BASE_AUTO) || base > VALID_BASE_MAX;
}

long int strtol(const char* nptr, char** endptr, int base) {
    int isNeg = FLAG_UNSET, hasValid = FLAG_UNSET, overflow = NO_OVERFLOW;
    const char *ptr = nptr;
    long int result = 0;

    if (isBaseInvalid(base)) {
        if (endptr) *endptr = (char*)nptr;
        errno = EINVAL;
        return 0;
    }

    ptr = parseSign(ptr, &isNeg);
    ptr = getNumberBase(ptr, &base);
    
    result = processDigits(&ptr, base, isNeg, &overflow, &hasValid);
    
    if (endptr) {
        *endptr = (char*)(hasValid ? ptr : nptr);
    }
    return getFinalValue(overflow, isNeg, result);
}