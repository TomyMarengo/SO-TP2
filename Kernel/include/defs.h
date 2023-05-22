#ifndef _DEFS_H_
#define _DEFS_H_

/* Flags for access rights of the segments */

/**
 * @brief Segment present in memory.
 */
#define ACS_PRESENT 0x80

/**
 * @brief Code Segment.
 */
#define ACS_CSEG    0x18

/**
 * @brief Data Segment.
 */
#define ACS_DSEG    0x10

/**
 * @brief Read Segment.
 */
#define ACS_READ    0x02

/**
 * @brief Write Segment.
 */
#define ACS_WRITE   0x02

#define ACS_IDT     ACS_DSEG

/**
 * @brief Interrupt GATE 32 bits.
 */
#define ACS_INT_386 0x0E

#define ACS_INT     (ACS_PRESENT | ACS_INT_386)

#define ACS_CODE  (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA  (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

/**
 * @brief Standard Input File Descriptor.
 */
#define STDIN  0

/**
 * @brief Standard Output File Descriptor.
 */
#define STDOUT 1

/**
 * @brief Standard Error File Descriptor.
 */
#define STDERR 2

/**
 * @brief Keyboard Input File Descriptor.
 */
#define KBDIN  3

#endif
