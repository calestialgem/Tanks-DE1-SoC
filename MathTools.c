#include "MathTools.h"

#include <stdint.h>
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
float math_power(float const base, int const exponent) {
	if (exponent == 1) {
		return base;
	}
	return base * math_power(base, exponent - 1);
}
float math_floor(float const number) {
	return (int32_t)number;
}
/* Using Newton's Method. */
float math_sqrt(float const number) {
	float result = number;
	result = 0.5F * (result + (number / result));
	result = 0.5F * (result + (number / result));
	result = 0.5F * (result + (number / result));
	result = 0.5F * (result + (number / result));
	return result;
}
float math_sin(float const angle) {
	return angle - math_pow(angle, 3) / 6.0F + math_pow(angle, 5) / 6.0F;
}
float math_cos(float const angle) {
	return 1.0F - math_pow(angle, 2) / 2.0F + math_pow(angle, 4) / 24.0F;
}
float math_atan(float const tan) {
	return -math_pow(tan, 3) / 3.0F + math_pow(tan, 5) / 5.0F -
	       math_pow(tan, 7) / 7.0F;
}
float math_abs(float const number) {
	return number < 0.0F ? -number : number;
}
