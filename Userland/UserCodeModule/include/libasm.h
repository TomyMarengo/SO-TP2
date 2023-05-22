#ifndef _LIBASM_H_
#define _LIBASM_H_

#include <stdint.h>

/**
 * @brief Run an unimplemented operation.
 */
void runInvalidOpcode(void);

/**
 * @brief Try to divide by zero.
 */
void runDivideByZero(void);

#endif
