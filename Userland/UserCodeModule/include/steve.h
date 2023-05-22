#ifndef _STEVE_H_
#define _STEVE_H_

#include <color.h>

extern uint32_t width, height;

// Steve's body part positions
#define HEADPOSX     (width / 4 - 4 * 8)
#define HEADPOSY     (height / 3 + 10 * 8)
#define TORSOPOSX    HEADPOSX
#define TORSOPOSY    HEADPOSY + 8 * 8
#define LEFTLEGPOSX  HEADPOSX
#define LEFTLEGPOSY  TORSOPOSY + 12 * 8
#define RIGHTLEGPOSX HEADPOSX + 4 * 8
#define RIGHTLEGPOSY TORSOPOSY + 12 * 8
#define LEFTARMPOSX  HEADPOSX - 4 * 8
#define LEFTARMPOSY  TORSOPOSY
#define RIGHTARMPOSX HEADPOSX + 8 * 8
#define RIGHTARMPOSY TORSOPOSY
#define ROPEPOSY     HEADPOSY - 4 * 8
#define ROPEPOSX     LEFTARMPOSX - 5 * 8

/**
 * @brief Draws the Steve's rope, for the hangman game (Start).
 */
void drawSteveRope();

/**
 * @brief Draws the Steve's head, for the hangman game (1st life).
 */
void drawSteveHead();

/**
 * @brief Draws the Steve's torso, for the hangman game (2nd life).
 */
void drawSteveTorso();

/**
 * @brief Draws the Steve's left arm, for the hangman game (3rd life).
 */
void drawSteveLeftArm();

/**
 * @brief Draws the Steve's right arm, for the hangman game (4th life).
 */
void drawSteveRightArm();

/**
 * @brief Draws the Steve's left leg, for the hangman game (5th life).
 */
void drawSteveLeftLeg();

/**
 * @brief Draws the Steve's right leg, for the hangman game (6th life).
 */
void drawSteveRightLeg();

/**
 * @brief Draws the Steve's eyes in white color, for the hangman game (Lose).
 */
void drawSteveClearEyes();

#endif
