#ifndef MATHS_H
#define MATHS_H

#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MATHS_2PI (2.0F * M_PI)

/** Takes the given number that is currently in a range, and normalizes it. Then
 * maps it to the target range linearly. */
static inline float maths_linearly_map(float const number,
	float const currentMin,
	float const currentMax,
	float const targetMin,
	float const targetMax) {
	return (number - currentMin) / (currentMax - currentMin) *
		       (targetMax - targetMin) +
	       targetMin;
}
/** Returns a random number in the given range. */
static inline float maths_random(float const min, float const max) {
	return maths_linearly_map(rand(), 0, (float)RAND_MAX, min, max);
}
/** Reseeds the random number generator with the current time. */
static inline void maths_reseed(void) {
	srand(time(NULL));
}
/** Returns the square of the given number. */
static inline float maths_square(float const number) {
	return number * number;
}
/** Returns a random index. */
static inline size_t maths_random_index(size_t const size) {
	return rand() % size;
}

#endif // MATHS_H
