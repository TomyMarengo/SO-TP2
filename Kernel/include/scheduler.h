#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <defs.h>

/**
 * @brief Initialize the Scheduler.
 */
void sch_init();

/**
 * @brief Called by process.c whenever a new process is created.
 *
 * @returns 0 if the operation is successful.
 */
int sch_onProcessCreated(Pid pid, ProcessStart start, Priority priority, void* currentRSP, int argc, const char* const argv[]);

/**
 * @brief Called by process.c when a process is being terminated. If the caller is the process itself, it is not immediately halted. 
 * To accomplish this, sch_yield() must be invoked.
 *
 * @return 0 if the operation is successful, 1 otherwise.
 */
int sch_onProcessKilled(Pid pid);

/**
 * @brief Instructs the scheduler that a specific process should remain inactive until it is unblocked.
 * If the caller is the process itself, it is not immediately blocked. To accomplish this, sch_yield() 
 * must be called subsequently.
 *
 * @return 0 if the operation is successful, 1 otherwise.
 */
int sch_blockProcess(Pid pid);

/**
 * @brief Put a process in READY state.
 *
 * @returns 0 if the operation succeeded or if it was running or ready already, 1 otherwise.
 */
int sch_unblockProcess(Pid pid);

/**
 * @brief Gets the PID of the running process.
 *
 * @returns The PID or -1 if there is no current process.
 */
Pid sch_getCurrentPID();

/**
 * @brief Sets a process priority.
 *
 * @returns 0 if the operation is successful, 1 otherwise.
 */
int sch_setPriority(Pid pid, Priority newPriority);

/**
 * @brief Determines the next process to be executed with Round Robin algorithm.
 *
 * @param currentRSP RSP (Stack Pointer) of the interrupted process.
 * @return RSP (Stack Pointer) of the next process to be executed.
 */
void* sch_switchProcess(void* currentRSP);

/**
 * @brief Populates the provided struct processInfo with the scheduler-related data of the requested process.
 * 
 * @return 0 if the operation is successful, 1 otherwise.
 */
int sch_getProcessInfo(Pid pid, ProcessInfo* processInfo);

/**
 * @brief Relinquishes CPU control to the next process on the ready list.
 * If the caller is not a process or has exited, this function does not return.
 */
void sch_yield();

#endif