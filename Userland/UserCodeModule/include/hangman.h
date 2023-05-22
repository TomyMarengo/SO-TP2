#ifndef _HANGMAN_H_
#define _HANGMAN_H_

#include <stdint.h>

/**
 * @brief Initialize the hangman game.
 */
void hang_init();

/**
 * @brief Updates the hangman game after received a character.
 * 
 * @param ascii Character to play.
 */
void hang_update(char ascii);

#endif