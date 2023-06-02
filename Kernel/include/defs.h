#ifndef _DEFS_H_
#define _DEFS_H_

#include <stddef.h>
#include <stdint.h>

/* --- File descriptors --- */

#define STDIN 0

#define STDOUT 1

#define STDERR 2

/* ---  Flags for access rights of the segments --- */

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

/* ---  File Descriptors --- */

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

/* --- Kernel types --- */
/* -------------------- */

/* --- Memory Management */

/**
 * @brief Represents the various categories of supported memory managers.
 */
typedef enum { LIST, BUDDY } MemoryManagerType;

/**
 * @brief Reflects the condition of the system memory at a specific moment.
 */
typedef struct {
    size_t total;
    size_t used;
    MemoryManagerType type;
    unsigned int chunks;
} MemoryState;

/* --- Processes --- */

/**
 * @brief Defines the maximum amount of Pids that can be returned by an embedded array in a query.
 */
#define MAX_PID_ARRAY_LENGTH 8

/**
 * @brief Maximum length for the name of a system resource, for example a process.
 */
#define MAX_NAME_LENGTH 16

/**
 * @brief Maximum amount of process living at the same time.
 */
#define MAX_PROCESSES 8

/**
 * @brief Represents a process id.
 */
typedef int Pid;

/**
 * @brief Represents a process priority.
 */
typedef int8_t Priority;

/**
 * @brief Default priority for a created process.
 */
#define PRIORITY_DEFAULT 0

/**
 * @brief Lowest priority limit reached for a process.
 */
#define PRIORITY_MIN 10

/**
 * @brief Highest priority limit reached for a process.
 */
#define PRIORITY_MAX -10

/**
 * @brief Lowest priority to be considered a process that will run next after unblock.
 */
#define PRIORITY_IMPORTANT -5

/**
 * @brief Process start function.
 */
typedef void (*ProcessStart)(int argc, char* argv[]);

/**
 * @brief Represents the various categories of supported process status.
 */
typedef enum { READY = 0, RUNNING = 1, BLOCKED = 2, KILLED = 3 } ProcessStatus;

/**
 * @brief Represents information of a process at particular time.
 */
typedef struct {
    Pid pid;
    int isForeground;
    Priority priority;
    char name[MAX_NAME_LENGTH + 1];
    void* stackEnd;
    void* stackStart;
    void* currentRSP;
    ProcessStatus status;
} ProcessInfo;

/**
 * @brief Represents the information needed for a create process request.
 */
typedef struct {
    const char* name;
    ProcessStart start;
    int isForeground;
    Priority priority;
    int argc;
    const char* const* argv;
} ProcessCreateInfo;

/* --- Pipes --- */

/**
 * @brief Represents information of a process at particular time.
 */
typedef struct {
    size_t remainingBytes;
    unsigned int readerFdCount;
    unsigned int writerFdCount;
    Pid readBlockedPids[MAX_PID_ARRAY_LENGTH + 1];
    Pid writeBlockedPids[MAX_PID_ARRAY_LENGTH + 1];
    char name[MAX_NAME_LENGTH + 1];
} PipeInfo;

/* --- Semaphores --- */

/**
 * @brief Represents a semaphore.
 */
typedef int8_t Sem;

/**
 * @brief Represents information of a semaphore at particular time.
 */
typedef struct {
    int value;
    int linkedProcesses; 
    char name[MAX_NAME_LENGTH+1];
    Pid processesWQ[MAX_PID_ARRAY_LENGTH+1];
} SemaphoreInfo;
#endif