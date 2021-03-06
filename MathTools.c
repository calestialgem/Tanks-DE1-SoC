#include "MathTools.h"

#include "SinusData.h"

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
float math_pow(float const base, int const exponent) {
	if (exponent == 1) {
		return base;
	}
	return base * math_pow(base, exponent - 1);
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
	return sines[((int32_t)(angle * SINES_COUNT / MATH_2PI) % SINES_COUNT +
			     SINES_COUNT) %
		     SINES_COUNT];
}
float math_cos(float const angle) {
	return math_sin(MATH_PI_2 - angle);
}
/* Taken from https://nghiaho.com/?p=997. */
float math_atan(float const tan) {
	float const abs = math_abs(tan);
	return MATH_PI_4 * tan - tan * (abs - 1.0F) * (0.2447F + 0.0663F * abs);
}
float math_abs(float const number) {
	return number < 0.0F ? -number : number;
}
float math_min(float const first, float const second) {
	return first < second ? first : second;
}
float math_max(float const first, float const second) {
	return first > second ? first : second;
}
float math_clamp(float const number, float const min, float const max) {
	return math_min(math_max(number, min), max);
}
bool math_nan(float const number) {
	return number != number;
}
