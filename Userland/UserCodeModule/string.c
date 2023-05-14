#include <string.h>
#include <stdint.h>

char* strcat(char* dest, const char* src) {
	char* w;
	for (w = dest; *w != '\0'; w++);
	for (; *src != '\0'; *(w++) = *(src++));
	*w = '\0';
	return dest;
}

char* strncat(char* dest, const char* src, uint64_t n) {
	char* w;
	for (w = dest; *w != '\0'; w++);
	for (; *src != '\0' && n != 0; *(w++) = *(src++), n--);
	*w = '\0';
	return dest;
}

const char* strchr(const char* str, char c) {
	for (; *str != '\0'; str++)
		if (*str == c)
			return str;
	return NULL;
}

int strcmp(const char* str1, const char* str2) {
	for (; *str1 == *str2 && *str1 != '\0'; str1++, str2++);
	return *str1 - *str2;
}

int strncmp(const char* str1, const char* str2, uint64_t n) {
	for (; *str1 == *str2 && *str1 != '\0' && n != 0; str1++, str2++, n--);
	return *str1 - *str2;
}

char* strcpy(char* dest, const char* src) {
	char* w;
	for (w = dest; *src != '\0'; *(w++) = *(src++));
	*w = '\0';
	return dest;
}

char* strncpy(char* dest, const char* src, uint64_t n) {
	char* w;
	for (w = dest; *src != '\0' && n != 0; *(w++) = *(src++), n--);
	*w = '\0';
	return dest;
}

uint64_t strlen(const char* s) {
    int l;
    for (l = 0; *s != 0; s++, l++);
    return l;
}

uint64_t itoa(uint64_t number, char* s) {
    int digits = 1;
	for (int n=number/10; n != 0; digits++, n /= 10);

    if(digits == 1) {
        s[0] = '0';
        s[1] = number + '0';
        s[2] = 0;
        return digits;
    }
	
	s[digits] = 0;    
	for (int i=digits-1; i>=0; i--) {
		s[i] = (number % 10) + '0';
		number /= 10;
	}
	
	return digits;
}

void uint64ToHex(uint64_t n, char buf[16]) {
    int i=15;
    do {
        int digit = n % 16;
        buf[i] = (digit < 10 ? '0' : ('A' - 10)) + digit;
        n /= 16;
    } while(i-- != 0);
}

int tryReadHexAddress(char* s, uint8_t** result) {
	uint64_t len = strlen(s);
	if(len<3 || len>18 || s[0]!='0' || s[1]!='x')
		return 0;

	uint64_t dir = 0;
	for(int i=2; i<len; i++){
		if(s[i]>='0' && s[i]<='9')
			dir = 16*dir + s[i]-'0';
		else if(s[i]>='a' && s[i]<='f')
			dir = 16*dir + s[i]-'a';
		else
			return 0;
	}

	*result = (uint8_t*)dir;
	return 1;
}
