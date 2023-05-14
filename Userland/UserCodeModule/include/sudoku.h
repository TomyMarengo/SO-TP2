#ifndef _SUDOKU_H_
#define _SUDOKU_H_

#include <stdint.h>

void sdk_init();
void sdk_move(uint8_t scancode);
void sdk_update(char number);

#endif