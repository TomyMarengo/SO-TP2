#ifndef _DEFS_H_
#define _DEFS_H_

#include <stddef.h>
#include <stdint.h>

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
 * @brief Represents the different types of supported memory managers.
 */
typedef enum { NODE, BUDDY } TMemoryManagerType;

/**
 * @brief Represents the state of the system memory at a given point in time.
 */
typedef struct {
    size_t total;
    size_t used;
    TMemoryManagerType type;
    unsigned int chunks;
} TMemoryState;

/**
 * @brief Defines the maximum length for the name of a system resource.
 */
#define MAX_NAME_LENGTH 16

/**
 * @brief Defines the maximum amount of TPid-s that can be returned by an embedded array in a query.
 */
#define MAX_PID_ARRAY_LENGTH 8

/**
 * @brief Represents a process status.
 */
typedef enum { READY = 0, RUNNING, BLOCKED, KILLED } TProcessStatus;

/**
 * @brief Represents process' ID.
 */
typedef int TPid;
typedef int Pid;

/**
 * @brief Represents process' priority.
 */
typedef int8_t TPriority;

/**
 * @brief Defines the default priority for a newly created process.
 */
#define DEFAULT_PRIORITY 0

/**
 * @brief Defines the lowest priority for a process.
 */
#define MIN_PRIORITY 10

/**
 * @brief Defines the highest priority for a process.
 */
#define MAX_PRIORITY -10

/**
 * @brief Defines the lowest priority at which a process is considered real-time.
 */
#define PRIORITY_REALTIME -5

/**
 * @brief Represents a process' entrypoint function.
 */
typedef void (*TProcessEntryPoint)(int argc, char* argv[]);

/**
 * @brief Represents a process' information at a point in time.
 */
typedef struct {
    TPid pid;
    char name[MAX_NAME_LENGTH + 1];
    void* stackEnd;
    void* stackStart;
    int isForeground;
    TPriority priority;
    TProcessStatus status;
    void* currentRSP;
} TProcessInfo;

/**
 * @brief Represents the information needed for a create process request.
 */
typedef struct {
    const char* name;
    TProcessEntryPoint entryPoint;
    int isForeground;
    TPriority priority;
    int argc;
    const char* const* argv;
} TProcessCreateInfo;

/**
 * @brief Represents a pipe's state information at a point in time.
 */
typedef struct {
    size_t remainingBytes;
    unsigned int readerFdCount;
    unsigned int writerFdCount;
    TPid readBlockedPids[MAX_PID_ARRAY_LENGTH + 1];
    TPid writeBlockedPids[MAX_PID_ARRAY_LENGTH + 1];
    char name[MAX_NAME_LENGTH + 1];
} TPipeInfo;

/**
 * @brief Represents a semaphore.
 */
typedef int8_t TSem;

/**
 * @brief Represents a semaphore's state information at a point in time
 */
typedef struct {
    int value;
    int linkedProcesses; 
    char name[MAX_NAME_LENGTH+1];
    TPid waitingProcesses[MAX_PID_ARRAY_LENGTH+1];
} TSemaphoreInfo;


#define STDERR 2
#define KBDIN  3
#define STDIN 1
#define STDOUT 0

#endif
