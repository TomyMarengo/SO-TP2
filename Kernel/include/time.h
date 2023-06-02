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
 * @brief Increments ticks.
 */
void interruptHandlerRTC();

/**
 * @brief Gets the total amount of ticks elapsed since startup.
 *
 * @return The total amount of ticks elapsed since startup.
 */
unsigned long getElapsedTicks();

/**
 * @brief Gets the total amount of seconds elapsed since startup.
 *
 * @return The total amount of seconds elapsed since startup.
 */
unsigned long getElapsedSeconds();

/**
 * @brief Returns current seconds.
 */
uint8_t getCurrentSeconds();

/**
 * @brief Returns current minutes.
 */
uint8_t getCurrentMinutes();

/**
 * @brief Returns current hours.
 */
uint8_t getCurrentHours();

/**
 * @brief Returns current day.
 */
uint8_t getCurrentDay();

/**
 * @brief Returns current month.
 */
uint8_t getCurrentMonth();

/**
 * @brief Returns current year.
 */
uint8_t getCurrentYear();

/**
 * @brief Returns the current date in format DD/MM/YY
 */
void getCurrentDateString(char* buffer);

/**
 * @brief Returns the current time in format HH:MM:SS
 */
void getCurrentTimeString(char* buffer);

#endif
