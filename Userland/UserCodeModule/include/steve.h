#ifndef _STEVE_H_
#define _STEVE_H_

#include <color.h>

extern uint32_t width, height;

#define HEADPOSX (width/4-4*8)
#define HEADPOSY (height/3+10*8)
#define TORSOPOSX HEADPOSX
#define TORSOPOSY HEADPOSY+8*8
#define LEFTLEGPOSX HEADPOSX
#define LEFTLEGPOSY TORSOPOSY+12*8
#define RIGHTLEGPOSX HEADPOSX+4*8
#define RIGHTLEGPOSY TORSOPOSY+12*8
#define LEFTARMPOSX HEADPOSX-4*8
#define LEFTARMPOSY TORSOPOSY
#define RIGHTARMPOSX HEADPOSX+8*8
#define RIGHTARMPOSY TORSOPOSY
#define ROPEPOSY HEADPOSY-4*8
#define ROPEPOSX LEFTARMPOSX-5*8

void drawSteveRightLeg();
void drawSteveLeftLeg();
void drawSteveRightArm();
void drawSteveLeftArm();
void drawSteveTorso();
void drawSteveHead();
void drawSteveRope();
void drawSteveClearEyes();

#endif
