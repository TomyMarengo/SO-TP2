#ifndef _TIMEUTIL_H_
#define _TIMEUTIL_H_

#include <stdint.h>
#define DATE_LENGTH 8
#define TIME_LENGTH 8
#define DATE_AND_TIME_LENGTH 17
#define TIME_WITH_TENTHS_LENGTH 10

void getTime(char* buffer);
void getTimeWithTenths(char* buffer, uint64_t millis);
void getDate(char* buffer);
void getDateAndTime(char* buffer);

#endif
