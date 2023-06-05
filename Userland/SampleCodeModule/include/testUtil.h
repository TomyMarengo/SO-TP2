#ifndef _TEST_UTIL_H_
#define _TEST_UTIL_H_

#include <defs.h>

void testMM(int argc, char *argv[]);
void testPrio(int argc, char* argv[]);
void testProcesses(int argc, char *argv[]);
void testSync(int argc, char *argv[]);
void bussyWait(uint64_t n);
void endlessLoop(int argc, char *argv[]);
void endlessLoopPrint(int argc, char *argv[]);
uint8_t getMaxAvailableProcesses();

#endif