#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <color.h>
#include <stddef.h>
#include <stdint.h>

#define STDIN  0
#define STDOUT 1
#define STDERR 2
#define KBDIN  3

/* The width of a character in pixels. */
#define CHAR_WIDTH 9
/* The height of a character in pixels. */
#define CHAR_HEIGHT 16

/**
 * @brief Reads up to `count` bytes from a file descriptor. If no data is available to read at the moment, blocks until there is.
 *
 * @param fd File descriptor.
 * @param buf Buffer to store the read data.
 * @param count Limit to be read.
 * @return The amount of bytes read.
 */
uint64_t sys_read(uint64_t fd, char *buf, uint64_t count);

/**
 * @brief Writes up to `count` bytes to a file descriptor.
 *
 * @param fd File descriptor.
 * @param buf Buffer where the data to be written is stored.
 * @param count Limit to be written.
 * @return The amount of bytes written.
 */
uint64_t sys_write(uint64_t fd, const char *buf, uint64_t count);

/**
 * @brief Returns the current system time.
 *
 * @return The current system time in seconds, minutes, and hours. Bits 0-7 represent hours, bits 8-15 minutes and bits 16-23 seconds.
 */
uint64_t sys_time(void);

/**
 * @brief Returns the amount of milliseconds elapsed since system startup.
 *
 * @return The amount of milliseconds elapsed since system startup.
 */
uint64_t sys_millis(void);

/**
 * @brief Clears the screen, putting it all to black and resetting the pen to the top left.
 */
void sys_clearscreen(void);

/**
 * @brief Writes up to `count` characters into the screen at the specified pixel position with the specified color.
 *
 * @param fd File descriptor.
 * @param buf Buffer where the data to be written is stored.
 * @param count Limit to be written.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param Color Color to be used.
 * @return The new pen position as a 32 bit number, where the 16 lowest bits are the x and the upper 16 bits are the y.
 */
uint32_t sys_writeat(const char *buf, uint64_t count, uint16_t x, uint16_t y, Color color);

/**
 * @brief Gets the width and height of the screen in pixels.
 *
 * @return A 64 bit number, whose lower 32 bits are width and upper 32 bits are the height.
 */
uint64_t sys_screensize();

/**
 * @brief Same as `sys_read(...)` but returns prematurely if no data becomes available after the timeout expires. Passing a timeout of 0 will make `sys_pollread(...)` return immediately even if there is no available data to read. Otherwise, it only ensures at least `timeout_ms` milliseconds have passed.
 *
 * @param fd File descriptor.
 * @param buf Buffer to store the read data.
 * @param count Limit to be read.
 * @param timeout_ms Milliseconds being blocked.
 * @return The amount of bytes read.
 */
uint64_t sys_pollread(uint64_t fd, char *buf, uint64_t count, uint64_t timeout_ms);

/**
 * @brief Draws a single pixel onto the screen at the specified position.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param Color Color to be used.
 */
void sys_drawpoint(uint16_t x, uint16_t y, Color color);

/**
 * @brief Sets a specified rectangle of pixels on the screen to the specified color.
 *
 * @param x X coordinate of the top left corner.
 * @param y Y coordinate of the top left corner.
 * @param width Width from the top left corner to top right corner.
 * @param height Height from the top left corner to bottom left corner.
 * @param color Color to draw.
 */
void sys_drawrect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);

/**
 * @brief Sets a specified rectangle of pixels on the screen to the specified color.
 *
 * @param fromX X coordinate of the start of the line.
 * @param fromY Y coordinate of the start of the line
 * @param toX X coordinate of the end of the line.
 * @param toY Y coordinate of the end of the line.
 * @param color Color to draw.
 */
void sys_drawline(uint16_t fromX, uint16_t fromY, uint16_t toX, uint16_t toY, Color color);

/**
 * @brief Returns the current system date.
 *
 * @return The current date as a 64-bit number whose bits 0-7 represent day, bits 8-15 month and bits 16-23 year.
 */
uint64_t sys_date(void);

/**
 * @brief Save in reg param data about registers.
 *
 * @param reg Buffer to store register data.
 * @return If it has or not regdump stored (stored after pressing CTRL key).
 */
uint8_t sys_inforeg(uint64_t reg[17]);

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
 * @brief Close a resource by its file descriptor.
 *
 * @return 0 if the operation succeeded.
 */
int sys_close(int fd);

/**
 * @brief Create a new pipe.
 *
 * @param pipefd Array of two positions for write and read pipe fd.
 * @return 0 if the operation succeeded, 1 otherwise.
 */
int sys_pipe(int pipefd[2]);

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
int sys_process(int argc, const char* argv[]);

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


#endif
