#ifndef _USERSTDLIB_H_
#define _USERSTDLIB_H_

/**
 * @brief Delay for a specified amount of milliseconds
 */
void sleep(unsigned long millis);

/**
 * @brief Reads the next character from STDIN
 */
int getChar();

/**
 * @brief Writes the character into STODUT
 */
int putChar(char c);

/**
 * @brief Reads the next character from the given output stream
 */
int fgetChar(int fd);

/**
 * @brief Writes the character to the given output stream
 */
int fputChar(int fd, char c);

/**
 * @brief Writes a string into STDOUT
 */
int print(const char* str);

/**
 * @brief Writes a string to the given output stream
 */
int fprint(int fd, const char* str);

/**
 * @brief Produces output according to a format
 */
void printf(const char* frmt, ...);

/**
 * @brief Writes output to the given output stream
 */
void fprintf(int fd, const char* frmt, ...);

/**
 * @brief Convert a string to an integer
 */
int atoi(const char* str);

/**
 * @brief Reads an entire line from STDIN. The buffer is null-terminated.
 */
int getLine(char* buffer, int maxSize);

/**
 * @brief Reads an entire line from the given output stream. The buffer is null-terminated.
 */
int fgetLine(int fd, char* buffer, int maxSize);



// thanks chatGPT!
/**
 * @brief Splits a string into tokens.
 * 
 * This function is a custom implementation of the standard C library function strtok.
 * It splits the input string into tokens, which are sequences of characters separated by the specified delimiter.
 * The function modifies the original string by replacing delimiters with null characters.
 * 
 * This function maintains static variables to keep track of its state between calls.
 * When called with a non-null string, it initializes its state and starts tokenizing the string.
 * When called with a null string, it continues tokenizing the string from the last position.
 * 
 * @param str The string to tokenize. If this parameter is null, the function continues tokenizing the last string from the last position.
 * @param delim The character used as a delimiter to separate tokens.
 * 
 * @return A pointer to the next token, or null if there are no more tokens.
 */
char* my_strtok(char* str, const char delim);


#endif
