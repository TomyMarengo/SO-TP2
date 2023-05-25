#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <stdint.h>

/**
 * @brief Clear Interrupt Flag.
 */
void _cli(void);

/**
 * @brief Set Interrupt Flag.
 */
void _sti(void);

/**
 * @brief Halts the central processing unit (CPU) until the next external interrupt is fired.
 */
void _hlt(void);

/**
 * @brief Clear Interrupt Flag and Halt.
 */
void haltcpu(void);

/**
 * @brief Pic Master Mask.
 */
void picMasterMask(uint8_t mask);

/**
 * @brief Pic Slave Mask.
 */
void picSlaveMask(uint8_t mask);

/**
 * @brief 
 */
void awakeScheduler(void);

/**
 * @brief 
 */
void _int81(void);

#endif