#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

#define TICKS_TO_SECONDS(x) ((x) * 5 / 91)
#define TICKS_TO_MILLISECONDS(x) ((x) * 5000 / 91);

/* Gets the total amount of ticks elapsed since startup. */
uint64_t rtc_getElapsedTicks();

/* Gets the total amount of seconds elapsed since startup. */
uint64_t rtc_getElapsedSeconds();

/* Gets the total amount of milliseconds elapsed since startup. */
uint64_t rtc_getElapsedMilliseconds();

/* Gets an uint64_t whose bits 0-7 represent hours, bits 8-15 minutes and bits 16-23 seconds. */
uint64_t rtc_getCurrentTime();

/* Gets an uint64_t whose bits 0-7 represent day, bits 8-15 month and bits 16-23 year. */
uint64_t rtc_getCurrentDate();

#endif
