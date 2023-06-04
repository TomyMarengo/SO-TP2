#ifndef _TEST_UTIL_H_
#define _TEST_UTIL_H_

#include <stdint.h>
#include <stddef.h>

uint32_t getUint();
uint32_t getUniform(uint32_t max);
uint8_t memcheck(void* start, uint8_t value, uint32_t size);
int64_t satoi(char* str);
void bussyWait(uint64_t n);
void endlessLoop(int argc, char* argv[]);
void endlessLoopPrint(int argc, char* argv[]);
void* memsetTest(void* destiny, int32_t c, size_t length);
uint8_t getMaxAvailableProcesses();

#endif