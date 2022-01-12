#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H

#include <stdlib.h>

#define MATH_PI 3.1415927F
#define MATH_2PI (2.0F * MATH_PI)
#define MATH_PI_2 (MATH_PI / 2.0F)
#define MATH_PI_4 (MATH_PI / 4.0F)

/** Takes the given number that is currently in a range, and normalizes it. Then
 * maps it to the target range linearly. */
float math_linearly_map(float const number,
	float const currentMin,
	float const currentMax,
	float const targetMin,
	float const targetMax);
/** Returns a random number in the given range. */
float math_random(float const min, float const max);
/** Reseeds the random number generator with the current time. */
void math_reseed(void);
/** Returns the square of the given number. */
float math_square(float const number);
/** Returns a random index. */
size_t math_random_index(size_t const size);

#endif // MATH_TOOLS_H
