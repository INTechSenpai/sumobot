#ifndef UTILS_H
#define UTILS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <stdbool.h>
#include "math.h"

#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y))
#define ABS(x) (((x) > 0) ? (x) : -(x))
#define CONSTRAIN(x,y,z) ( ((x)<(y))?(y):( ((x)>(z))?(z):(x) ) )

enum Side
{
	GREEN, PURPLE
};

#ifdef __cplusplus
extern "C" {
#endif

int modulo(int nombre, int modulo);
float fmodulo(float nombre, float modulo);
float square(float x);
void blink(uint8_t, unsigned int, unsigned int);
void blinkDelOnBoard(unsigned int, unsigned int);

#ifdef __cplusplus
}
#endif

#endif
