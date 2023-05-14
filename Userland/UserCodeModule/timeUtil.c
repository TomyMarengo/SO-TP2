#include <string.h>
#include <timeUtil.h>
#include <syscalls.h>

void getTime(char* buffer) {
	char* p = buffer;
	uint64_t time = sys_time();
	
	itoa((time & 0xFF), p);
	p[2] = ':';
	itoa((time >> 8) & 0xFF, &p[3]);
	p[5] = ':';
	itoa((time >> 16) & 0xFF, &p[6]);
    p[8] = 0;
}

void getTimeWithTenths(char* buffer, uint64_t millis) {
	char* p = buffer;
	itoa((millis/(1000*60*60)) % 24, p);
	p[2] =':';
	itoa((millis/(1000*60)) % 60, &p[3]);
	p[5] =':';
	itoa((millis/1000) % 60, &p[6]);
	p[8] =':';
	p[9] = (millis/100) % 10 + '0';
    p[10] = 0;
}

void getDate(char* buffer) {
	char* p = buffer;
	uint64_t date = sys_date();

	itoa((date & 0xFF), p);
	p[2] = '/';
	itoa((date >> 8) & 0xFF, &p[3]);
	p[5] = '/';
	itoa((date >> 16) & 0xFF, &p[6]);
    p[8] = 0;
}

void getDateAndTime(char* buffer) {
    char* p = buffer;
    getDate(p);
    p[DATE_LENGTH] = ' ';
    getTime(&p[DATE_LENGTH+1]);
}