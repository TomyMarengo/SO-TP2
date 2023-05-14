#include <steve.h>
#include <syscalls.h>

Color hairColor = {0x0A, 0x1A, 0x26};
Color eyeColor = {0x89, 0x3D, 0x52};
Color noseColor = {0x3A, 0x52, 0x80};
Color beardColor = {0x0B, 0x1E, 0x33};
Color shirtColor = {0xAF, 0xAF, 0x00};
Color darkShirtColor = {0x9E, 0x93, 0x00};
Color pantsColor = {0xA5, 0x3A, 0X46};
Color darkPantsColor = {0x89, 0x31, 0x3A};
Color skinColor = {0x72, 0x8B, 0xBD};
Color mediumSkinColor = {0x6D, 0x80, 0xAD};
Color darkSkinColor = {0x5C, 0x72, 0x9C};
Color darkShoesColor = {0x6B, 0x6B, 0x6B};
Color lightShoesColor = {0x8F, 0x8F, 0x8F};
Color shoesColor = {0x77, 0x77, 0x7A};
Color ropeColor = {0x11, 0x31, 0x47};
Color lightRopeColor = {0x4C, 0x7D, 0xC1};
Color darkRopeColor = {0x0B, 0x24, 0x36};

void drawSteveHead() {
    sys_drawrect(HEADPOSX,HEADPOSY,8*8,8*8, skinColor);
    sys_drawrect(HEADPOSX,HEADPOSY+6*8,8,2*8,darkSkinColor);
    sys_drawrect(HEADPOSX+8,HEADPOSY+7*8,8,8,darkSkinColor);
    sys_drawrect(HEADPOSX+7*8,HEADPOSY+6*8,8,2*8,darkSkinColor);
    sys_drawrect(HEADPOSX+6*8,HEADPOSY+7*8,8,8,darkSkinColor);
    sys_drawrect(HEADPOSX+2*8,HEADPOSY+3*8,8,8,mediumSkinColor);
    sys_drawrect(HEADPOSX+6*8,HEADPOSY+3*8,8,8,mediumSkinColor);
    sys_drawrect(HEADPOSX,HEADPOSY,8*8,8*2,hairColor);
    sys_drawrect(HEADPOSX,HEADPOSY+2*8,8,8,hairColor);
    sys_drawrect(HEADPOSX+7*8,HEADPOSY+2*8,8,8,hairColor);
    sys_drawrect(HEADPOSX+8,HEADPOSY+4*8,8,8,white);
    sys_drawrect(HEADPOSX+6*8,HEADPOSY+4*8,8,8,white);
    sys_drawrect(HEADPOSX+2*8,HEADPOSY+4*8,8,8,eyeColor);
    sys_drawrect(HEADPOSX+5*8,HEADPOSY+4*8,8,8,eyeColor);
    sys_drawrect(HEADPOSX+2*8,HEADPOSY+6*8,4*8, 2*8, beardColor);
    sys_drawrect(HEADPOSX+3*8,HEADPOSY+5*8,2*8, 2*8, noseColor);
}

void drawSteveTorso() {
    sys_drawrect(TORSOPOSX,TORSOPOSY,8*8,8*12,shirtColor);
    sys_drawrect(TORSOPOSX+2*8,TORSOPOSY,8,8,darkSkinColor);
    sys_drawrect(TORSOPOSX+3*8,TORSOPOSY,2*8,8,skinColor);
    sys_drawrect(TORSOPOSX+3*8,TORSOPOSY+8, 2*8, 8, darkSkinColor);
    sys_drawrect(TORSOPOSX+5*8,TORSOPOSY, 8, 8, darkSkinColor);
    sys_drawrect(TORSOPOSX+3*8,TORSOPOSY+2*8, 2*8, 8, darkShirtColor);
    sys_drawrect(TORSOPOSX+4*8,TORSOPOSY+2*8, 8, 4*8, darkShirtColor);
    sys_drawrect(TORSOPOSX+3*8,TORSOPOSY+6*8, 8, 4*8, darkShirtColor);
    sys_drawrect(TORSOPOSX,TORSOPOSY+10*8, 6*8, 8, darkPantsColor);
    sys_drawrect(TORSOPOSX+6*8,TORSOPOSY+11*8, 8, 8, darkPantsColor);
    sys_drawrect(TORSOPOSX,TORSOPOSY+11*8, 6*8, 8, pantsColor);
}

void drawSteveLeftLeg() {
    sys_drawrect(LEFTLEGPOSX,LEFTLEGPOSY,4*8,9*8,pantsColor);
    sys_drawrect(LEFTLEGPOSX,LEFTLEGPOSY+9*8,4*8,8,darkPantsColor);
    sys_drawrect(LEFTLEGPOSX+8,LEFTLEGPOSY+5*8,2*8,8,darkPantsColor);
    sys_drawrect(LEFTLEGPOSX+0,LEFTLEGPOSY,8,2*8,darkPantsColor);
    sys_drawrect(LEFTLEGPOSX,LEFTLEGPOSY+10*8,4*8,2*8,darkShoesColor);
    sys_drawrect(LEFTLEGPOSX+8,LEFTLEGPOSY+10*8,8,8,lightShoesColor);
    sys_drawrect(LEFTLEGPOSX+8,LEFTLEGPOSY+11*8,8,8,shoesColor);
    sys_drawrect(LEFTLEGPOSX+2*8,LEFTLEGPOSY+10*8,8,8,shoesColor);
}

void drawSteveRightLeg() {
    sys_drawrect(RIGHTLEGPOSX,RIGHTLEGPOSY,4*8,9*8,pantsColor);
    sys_drawrect(RIGHTLEGPOSX,RIGHTLEGPOSY+9*8,4*8,8,darkPantsColor);
    sys_drawrect(RIGHTLEGPOSX+8,RIGHTLEGPOSY+5*8,2*8,8,darkPantsColor);
    sys_drawrect(RIGHTLEGPOSX+3*8,RIGHTLEGPOSY,8,2*8,darkPantsColor);
    sys_drawrect(RIGHTLEGPOSX,RIGHTLEGPOSY+10*8,4*8,2*8,darkShoesColor);
    sys_drawrect(RIGHTLEGPOSX+2*8,RIGHTLEGPOSY+10*8,8,8,lightShoesColor);
    sys_drawrect(RIGHTLEGPOSX+2*8,RIGHTLEGPOSY+11*8,8,8,shoesColor);
    sys_drawrect(RIGHTLEGPOSX+8,RIGHTLEGPOSY+10*8,8,8,shoesColor);
}

void drawSteveLeftArm() {
    sys_drawrect(LEFTARMPOSX,LEFTARMPOSY, 4*8, 4*8, shirtColor);
    sys_drawrect(LEFTARMPOSX,LEFTARMPOSY+3*8, 8, 8, darkShirtColor);
    sys_drawrect(LEFTARMPOSX+3*8,LEFTARMPOSY+3*8, 8, 8, darkShirtColor);
    sys_drawrect(LEFTARMPOSX,LEFTARMPOSY+4*8, 4*8, 8*8, skinColor);
    sys_drawrect(LEFTARMPOSX,LEFTARMPOSY+11*8, 8, 8, mediumSkinColor);
    sys_drawrect(LEFTARMPOSX+3*8,LEFTARMPOSY+11*8, 8, 8, mediumSkinColor);
    sys_drawrect(LEFTARMPOSX+2*8,LEFTARMPOSY+10*8, 8, 8, darkSkinColor);
    sys_drawrect(LEFTARMPOSX+1*8,LEFTARMPOSY+6*8, 8, 8, darkSkinColor);
    sys_drawrect(LEFTARMPOSX+3*8,LEFTARMPOSY+6*8, 8, 2*8, darkSkinColor);
}

void drawSteveRightArm() {
    sys_drawrect(RIGHTARMPOSX,RIGHTARMPOSY, 4*8, 4*8, shirtColor);
    sys_drawrect(RIGHTARMPOSX,RIGHTARMPOSY+3*8, 8, 8, darkShirtColor);
    sys_drawrect(RIGHTARMPOSX+3*8,RIGHTARMPOSY+3*8, 8, 8, darkShirtColor);
    sys_drawrect(RIGHTARMPOSX,RIGHTARMPOSY+4*8, 4*8, 8*8, skinColor);
    sys_drawrect(RIGHTARMPOSX,RIGHTARMPOSY+11*8, 8, 8, mediumSkinColor);
    sys_drawrect(RIGHTARMPOSX+3*8,RIGHTARMPOSY+11*8, 8, 8, mediumSkinColor);
    sys_drawrect(RIGHTARMPOSX+1*8,RIGHTARMPOSY+10*8, 8, 8, darkSkinColor);
    sys_drawrect(RIGHTARMPOSX+2*8,RIGHTARMPOSY+6*8, 8, 8, darkSkinColor);
    sys_drawrect(RIGHTARMPOSX,RIGHTARMPOSY+6*8, 8, 2*8, darkSkinColor);
}

void drawSteveRope() {
    sys_drawrect(ROPEPOSX,ROPEPOSY,2*8,36*8,ropeColor);
    sys_drawrect(ROPEPOSX,ROPEPOSY,12*8,2*8,ropeColor);
    sys_drawrect(ROPEPOSX+12*8,ROPEPOSY,2*8,4*8,ropeColor);
    sys_drawrect(ROPEPOSX, ROPEPOSY+2*8, 8, 8, lightRopeColor);
    sys_drawrect(ROPEPOSX, ROPEPOSY+5*8, 8, 8, lightRopeColor);
    sys_drawrect(ROPEPOSX, ROPEPOSY+2*8, 8, 8, lightRopeColor);
    sys_drawrect(ROPEPOSX+8, ROPEPOSY+20*8, 8, 8, lightRopeColor);
    sys_drawrect(ROPEPOSX+5*8, ROPEPOSY, 8, 2*8, darkRopeColor);
    sys_drawrect(ROPEPOSX+11*8, ROPEPOSY, 8, 8, darkRopeColor);
    sys_drawrect(ROPEPOSX, ROPEPOSY+24*8, 8, 2*8, darkRopeColor);
    sys_drawrect(ROPEPOSX+12*8, ROPEPOSY+2*8, 8, 8, darkRopeColor);
    sys_drawrect(ROPEPOSX, ROPEPOSY, 8, 8, darkRopeColor);
    sys_drawrect(ROPEPOSX, ROPEPOSY+36*8, 2*8, 8, darkRopeColor);
}

void drawSteveClearEyes() {
    sys_drawrect(HEADPOSX+2*8, HEADPOSY+4*8,8,8, white);
    sys_drawrect(HEADPOSX+5*8, HEADPOSY+4*8,8,8, white);
}
