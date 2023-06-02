#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

/* Standard library */
#include <stddef.h>
#include <sys/types.h>

/* Local headers */
#include <types.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

ssize_t sys_read(int fd, char* buffer, size_t size);

ssize_t sys_write(int fd, const char* buffer, size_t size);

void sys_time(char* buffer);

int sys_close(int fd);

void sys_clear();

int sys_kill(Pid pid);

int sys_block(Pid pid);

int sys_unblock(Pid pid);

int sys_printmem(void* mem_address);

int sys_create(ProcessEntryPoint entryPoint, int argc, const char* const argv[]);

void sys_date(char* buffer);

void sys_inforeg();

void* sys_malloc(size_t size);

int sys_free(void* ptr);

void* sys_realloc(void* ptr, size_t size);

void sys_yield();

Pid sys_gePid();

int sys_priority(Pid pid, Priority newPriority);

int sys_exit();

int sys_pipe(int pipefd[2]);

#endif