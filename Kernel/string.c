/* Standard library */
#include <string.h>

size_t
strlen(const char *str) {
    size_t l;
    for (l = 0; *str != 0; str++, l++)
        ;
    return l;
}

uint64_t
itoa(uint64_t number, char *buffer) {
    int digits = 1;
    for (int n = number / 10; n != 0; digits++, n /= 10)
        ;

    if (digits == 1) {
        buffer[0] = '0';
        buffer[1] = number + '0';
        buffer[2] = 0;
        return digits;
    }

    buffer[digits] = 0;
    for (int i = digits - 1; i >= 0; i--) {
        buffer[i] = (number % 10) + '0';
        number /= 10;
    }

    return digits;
}

int
strcmp(const char *str1, const char *str2) {
    int i;
    for (i = 0; str1[i] && str1[i] == str2[i]; i++)
        ;
    return str1[i] - str2[i];
}

char *
strcat(char *destination, const char *source) {
    char *rdest = destination;

    while (*destination)
        destination++;
    while ((*destination++ = *source++))
        ;
    return rdest;
}

char *
strcpy(char *destination, const char *source) {
    char *w;
    for (w = destination; *source != '\0'; *(w++) = *(source++))
        ;
    *w = '\0';
    return destination;
}

char *
strncpy(char *destination, const char *source, size_t size) {
    int i = 0;
    char *ret = destination;
    while (*source && i < size) {
        *(destination++) = *(source++);
        i++;
    }
    *destination = '\0';
    return ret;
}