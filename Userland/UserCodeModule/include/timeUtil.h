#ifndef _TIMEUTIL_H_
#define _TIMEUTIL_H_

#include <stdint.h>
#define DATE_LENGTH             8
#define TIME_LENGTH             8
#define DATE_AND_TIME_LENGTH    17
#define TIME_WITH_TENTHS_LENGTH 10

/**
 * @brief Saves the time in HH:MM:SS format in the buffer provided.
 *
 * @param buffer Char buffer with 9 positions (including null terminated).
 */
void getTime(char *buffer);

/**
 * @brief Saves the time in HH:MM:SS:mm format in the buffer provided.
 *
 * @param buffer Char buffer with 11 positions (including null terminated).
 * @param millis The amount of milliseconds elapsed since system startup.
 */
void getTimeWithTenths(char *buffer, uint64_t millis);

/**
 * @brief Saves the date in DD:MM:YY format in the buffer provided.
 *
 * @param buffer Char buffer with 9 positions (including null terminated).
 */
void getDate(char *buffer);

/**
 * @brief Saves the date and time in DD:MM:YY HH:MM:SS format in the buffer provided.
 *
 * @param buffer Char buffer with 18 positions (including null terminated).
 */
void getDateAndTime(char *buffer);

#endif
