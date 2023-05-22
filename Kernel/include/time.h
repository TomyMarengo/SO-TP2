#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

/**
 * @brief Converts ticks to seconds.
 */
#define TICKS_TO_SECONDS(x) ((x) *5 / 91)

/**
 * @brief Converts ticks to miliseconds.
 */
#define TICKS_TO_MILLISECONDS(x) ((x) *5000 / 91);

/**
 * @brief Gets the total amount of ticks elapsed since startup.
 *
 * @return The total amount of ticks elapsed since startup.
 */
uint64_t rtc_getElapsedTicks();

/**
 * @brief Gets the total amount of seconds elapsed since startup.
 *
 * @return The total amount of seconds elapsed since startup.
 */
uint64_t rtc_getElapsedSeconds();

/**
 * @brief Gets the total amount of milliseconds elapsed since startup.
 *
 * @return The total amount of milliseconds elapsed since startup.
 */
uint64_t rtc_getElapsedMilliseconds();

/**
 * @brief Gets an uint64_t whose bits 0-7 represent hours, bits 8-15 minutes and bits 16-23 seconds.
 *
 * @return An uint64_t whose bits 0-7 represent hours, bits 8-15 minutes and bits 16-23 seconds.
 */
uint64_t rtc_getCurrentTime();

/**
 * @brief Gets an uint64_t whose bits 0-7 represent day, bits 8-15 month and bits 16-23 year.
 *
 * @return An uint64_t whose bits 0-7 represent day, bits 8-15 month and bits 16-23 year.
 */
uint64_t rtc_getCurrentDate();

#endif
