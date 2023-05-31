#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stddef.h>
#include <sys/types.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

/**
 * @brief Reads up to `count` bytes from a file descriptor. If no data is available to read at the moment, blocks until there is.
 *
 * @param fd File descriptor.
 * @param buffer Buffer to store the read data.
 * @param count Limit to be read.
 * @return The amount of bytes read.
 */
ssize_t sys_read(int fd, char* buffer, size_t count);

/**
 * @brief Writes up to `count` bytes to a file descriptor.
 *
 * @param fd File descriptor.
 * @param buf Buffer where the data to be written is stored.
 * @param count Limit to be written.
 * @return The amount of bytes written.
 */
ssize_t sys_write(int fd, const char* buffer, size_t count);

/**
 * @brief Save the current system time in the buffer parameter.
 *
 * @param buffer Buffer where time is saved.
 */
void sys_time(char* buffer);

/**
 * @brief Save the current system date in the buffer parameter.
 * @param buffer Buffer where date is saved.
 */
void sys_date(char* buffer);


/**
 * @brief Clears the screen, putting it all to black and resetting the pen to the top left.
 */
void sys_clear();

/**
 * @brief Print registers information.
 */
void sys_inforeg();

int sys_printmem(void* mem_address);

/**
 * @brief Terminate a process.
 *
 * @param pid process' PID.
 * @return 0 if the operation succeeded, 1 otherwise.
 */
int sys_kill(int pid);

/**
 * @brief Block a process.
 *
 * @param pid process' PID.
 * @return 0 if the operation succeeded, 1 otherwise.
 */
int sys_block(int pid);

/**
 * @brief Unblock a process.
 *
 * @param pid Process' PID.
 * @return 0 if the operation succeeded, 1 otherwise.
 */
int sys_unblock(int pid);

/**
 * @brief Unblock a process.
 *
 * @param argc Amount of parameters.
 * @param argv Parameters.
 * @return Process' PID.
 */
int sys_create(int argc, const char* argv[]);

/**
 * @brief Close a resource by its file descriptor.
 *
 * @return 0 if the operation succeeded.
 */
int sys_close(int fd);

/**
 * @brief Request the memory manager to reserve a chunk of memory.
 *
 * @param size Size of the chunk requested.
 * @return A pointer to the reserved memory, or NULL if the operation failed.
 */
void *sys_malloc(size_t size);

/**
 * @brief Notifies the memory manager that a previously allocated memory segment, reserved by sys_malloc(), is now available to be marked as free and used elsewhere.
 *
 * @param ptr A pointer to the reserved memory previously provided by malloc or realloc.
 * @return 0 if the operation is successful, 1 otherwise.
 */
int sys_free(void *ptr);

/**
 * @brief Instructs the memory manager to modify the size of a previously allocated block of memory.
 *
 * @param ptr A pointer to the reserved memory previously provided by malloc or realloc.
 * @param size Size of the chunk requested.
 * @return A pointer to the resized memory block, or NULL if the operation failed.
 */
void *sys_realloc(void *ptr, size_t size);

/**
 * @brief Force processor to relinquish control of the current running process.
 */
void sys_yield();

/**
 * @brief Get the pid of the calling process.
 *
 * @return The pid of the calling process
 */
int sys_getpid();

/**
 * @brief Change the priority of a process.
 *
 * @param pid Process' PID.
 * @param priority New priority.
 * @return 0 if the operation is successful, 1 otherwise.
 */
int sys_priority(int pid, int priority);

/**
 * @brief Terminate the calling process.

 * @return 0 if the operation is successful, 1 otherwise.
 */
int sys_exit();

/**
 * @brief Create a new pipe.
 *
 * @param pipefd Array of two positions for write and read pipe fd.
 * @return 0 if the operation succeeded, 1 otherwise.
 */
int sys_pipe(int pipefd[2]);

#endif