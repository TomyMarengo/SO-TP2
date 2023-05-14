#ifndef _COLOR_H_
#define _COLOR_H_

#include <stdint.h>

typedef struct {
	uint8_t b;
	uint8_t g;
	uint8_t r;
} Color;

extern Color green;
extern Color blue;
extern Color red;
extern Color yellow;
extern Color gray;
extern Color black;
extern Color white;
extern Color magenta;

#endif