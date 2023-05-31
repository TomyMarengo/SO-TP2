#ifndef _TIME_H_
#define _TIME_H_

/* Standard library */
#include <stdint.h>

#define TICKS_TO_SECONDS(x) ((x) * 5 / 91)
#define TICKS_TO_MILLISECONDS(x) ((x) * 5000 / 91)

void rtcInterruptHandler();
unsigned long getElapsedTicks();
unsigned long getElapsedSeconds();

uint8_t getCurrentSeconds();
uint8_t getCurrentMinutes();
uint8_t getCurrentHours();
uint8_t getCurrentDay();
uint8_t getCurrentMonth();
uint8_t getCurrentYear();
void getCurrentDateString(char* buffer);
void getCurrentTimeString(char* buffer);

#endif
