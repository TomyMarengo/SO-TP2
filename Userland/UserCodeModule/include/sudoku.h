#ifndef _SUDOKU_H_
#define _SUDOKU_H_

#include <stdint.h>

/**
 * @brief Initializes the sudoku game.
 */
void sdk_init();

/**
 * @brief Moves sudoku pen position.
 * 
 * @param scancode The scancode of the movement (arrows).
 */
void sdk_move(uint8_t scancode);

/**
 * @brief Updates a sudoku cell.
 * 
 * @param number The number provided.
 */
void sdk_update(char number);

#endif