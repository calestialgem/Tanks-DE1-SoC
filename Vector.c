#include "Vector.h"

#include "Maths.h"

/** Adds the given vectors, and returns the result as a new vector. */
Vector vector_add(Vector const left, Vector const right) {
	Vector const result = {.x = left.x + right.x, .y = left.y + right.y};
	return result;
}
/** Subtracts the given vectors, and returns the result as a new vector. */
Vector vector_sub(Vector const left, Vector const right) {
	Vector const result = {.x = left.x - right.x, .y = left.y - right.y};
	return result;
}
/** Multiplies the given vector and scalar, and returns the result as a new
 * vector. */
Vector vector_mul(Vector const left, float const right) {
	Vector const result = {.x = left.x * right, .y = left.y * right};
	return result;
}
float vector_square(Vector const vector) {
	return maths_square(vector.x) + maths_square(vector.y);
}
