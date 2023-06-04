#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <defs.h>

const Command* getCommandByName(const char* name);

/* General commands */
int runHelp(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runClear(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runEcho(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runTime(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);

/* Physical memory management commands */
int runMem(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);

/* Processe-related commands */
int runPs(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runLoop(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runKill(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runPriority(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runBlock(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runUnblock(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);

/* Process synchronization commands */
int runSem(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);

/* Inter process communication commands */
int runCat(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runWc(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runFilter(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runPipe(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);

/* Tests */
int runTestMM(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runTestSync(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runTestProcesses(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);
int runTestPrio(int stdin, int stdout, int stderr, int isForeground, int argc, const char* const argv[], Pid* createdProcess);

#endif