#ifndef _TIME_H_
#define _TIME_H_

#include <defs.h>

/**
 * @brief Converts ticks to seconds.
 */
#define TICKS_TO_SECONDS(x) ((x) *5 / 91)

/**
 * @brief Converts ticks to miliseconds.
 */
#define TICKS_TO_MILLISECONDS(x) ((x) *5000 / 91);

/**
 * @brief Invoked by the interrupt dispatcher when a timer interrupt is detected. Increments ticks.
 */
void interruptHandlerRTC();

/**
 * @brief Gets the total amount of ticks elapsed since startup.
 *
 * @returns - The total amount of ticks elapsed since startup.
 */
unsigned long getElapsedTicks();

/**
 * @brief Gets the total amount of seconds elapsed since startup.
 *
 * @returns - The total amount of seconds elapsed since startup.
 */
unsigned long getElapsedSeconds();

/**
 * @brief Gets the current seconds.
 * 
 * @returns - The current seconds.
 */
uint8_t getCurrentSeconds();

/**
 * @brief Gets the current minutes.
 * 
 * @returns - The current minutes.
 */
uint8_t getCurrentMinutes();

/**
 * @brief Gets the current hours.
 * 
 * @returns - The current hours.
 */
uint8_t getCurrentHours();

/**
 * @brief Gets the current day.
 * 
 * @returns - The current day.
 */
uint8_t getCurrentDay();

/**
 * @brief Gets the current month.
 * 
 * @returns - The current month.
 */
uint8_t getCurrentMonth();

/**
 * @brief Gets the current year.
 * 
 * @returns - The current year.
 */
uint8_t getCurrentYear();

/**
 * @brief Gets the current date in format DD/MM/YY.
 * 
 * @param buffer Buffer to archive the current date in format DD/MM/YY.
 */
void getCurrentDateString(char* buffer);

/**
 * @brief Gets the current time in format HH:MM:SS.
 * 
 * @param buffer Buffer to archive the current time in format HH:MM:SS.
 */
void getCurrentTimeString(char* buffer);

#endif
