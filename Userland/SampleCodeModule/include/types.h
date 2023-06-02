#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

/**
 * @brief Represents a process status.
 * 
 */
typedef enum status { READY = 0, BLOCKED, KILLED } ProcessStatus;

/**
 * @brief Represents process PID
 * 
 */
typedef int Pid;

/**
 * @brief Represents process priority
 * 
 */
typedef int8_t Priority;

/**
 * @brief Represents a process' entrypoint function.
 *
 * @returns The process' exit code.
 */
typedef int (*ProcessEntryPoint)(int argc, const char* argv[]);

/**
 * @brief Represents a process' information.
 */
typedef struct {
    Pid pid;
    const char* name;
    void* stackEnd;
    void* stackStart;
    int isForeground;
    Priority priority;
    ProcessStatus status;
    void* currentRSP;
} ProcessInfo;

#endif