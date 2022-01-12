#include "MathTools.h"

#include <time.h>

/** Takes the given number that is currently in a range, and normalizes it. Then
 * maps it to the target range linearly. */
float math_linearly_map(float const number,
	float const currentMin,
	float const currentMax,
	float const targetMin,
	float const targetMax) {
	return (number - currentMin) / (currentMax - currentMin) *
		       (targetMax - targetMin) +
	       targetMin;
}
/** Returns a random number in the given range. */
float math_random(float const min, float const max) {
	return math_linearly_map(rand(), 0, (float)RAND_MAX, min, max);
}
/** Reseeds the random number generator with the current time. */
void math_reseed(void) {
	srand(time(NULL));
}
/** Returns the square of the given number. */
float math_square(float const number) {
	return number * number;
}
/** Returns a random index. */
size_t math_random_index(size_t const size) {
	return rand() % size;
}
