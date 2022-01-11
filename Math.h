#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MATH_2PI (2.0F * M_PI)

/** Vector of two floats. */
typedef struct {
	/** First component. */
	float x;
	/** Second component. */
	float y;
} Vector;

/** Adds the given vectors, and returns the result as a new vector. */
static inline Vector vector_add(Vector const left, Vector const right) {
	Vector const result = {.x = left.x + right.x, .y = left.y + right.y};
	return result;
}
/** Multiplies the given vector and scalar, and returns the result as a new
 * vector. */
static inline Vector vector_mul(Vector const left, float const right) {
	Vector const result = {.x = left.x * right, .y = left.y * right};
	return result;
}
/** Takes the given number that is currently in a range, and normalizes it. Then
 * maps it to the target range linearly. */
static inline float math_linearly_map(float const number,
	float const currentMin,
	float const currentMax,
	float const targetMin,
	float const targetMax) {
	return (number - currentMin) / (currentMax - currentMin) *
		       (targetMax - targetMin) +
	       targetMin;
}
/** Returns a random number in the given range. */
static inline float math_random(float const min, float const max) {
	return math_linearly_map(rand(), 0, (float)RAND_MAX, min, max);
}
/** Reseeds the random number generator with the current time. */
static inline void math_reseed(void) {
	srand(time(NULL));
}
/** Returns the square of the given number. */
static inline float math_square(float const number) {
	return number * number;
}
/** Returns a random index. */
static inline int math_random_index(int const size) {
	return rand() % size;
}

#endif // MATH_H
