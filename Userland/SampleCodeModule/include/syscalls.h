#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <defs.h>
#include <sys/types.h>

ssize_t sys_read(int fd, char *buffer, size_t size);
ssize_t sys_write(int fd, const char *buffer, size_t size);
int sys_close(int fd);

void sys_clearScreen();

unsigned long sys_millis();
void sys_time(char *buffer);
void sys_date(char *buffer);

void *sys_malloc(size_t size);
int sys_free(void *ptr);
void *sys_realloc(void *ptr, size_t size);
int sys_memoryState(MemoryState *memoryState);

Pid sys_getpid();
Pid sys_createProcess(int stdinMapFd, int stdoutMapFd, int stderrMapFd, const ProcessCreateInfo *createInfo);
int sys_exit();
int sys_block(Pid pid);
int sys_unblock(Pid pid);
void sys_yield();
int sys_kill(Pid pid);
int sys_priority(Pid pid, Priority newPriority);
int sys_listProcesses(ProcessInfo *array, int maxProcesses);
int sys_waitpid(Pid pid);

int sys_createPipe(int pipefd[2]);
int sys_openPipe(const char *name, int pipefd[2]);
int sys_unlinkPipe(const char *name);
int sys_listPipes(PipeInfo *array, int maxPipes);

Sem sys_openSem(const char *name, unsigned int value);
int sys_closeSem(Sem sem);
int sys_post(Sem sem);
int sys_wait(Sem sem);
int sys_listSemaphores(SemaphoreInfo *array, int maxSemaphores);

#endif