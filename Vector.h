#ifndef VECTOR_H
#define VECTOR_H

/** Vector of two floats. */
typedef struct {
	/** First component. */
	float x;
	/** Second component. */
	float y;
} Vector;

/** Initiales the given vector. */
void vector_init(volatile Vector *const vector,
	float const magnitude,
	float const angle);
/** Adds the given vectors, and returns the result as a new vector. */
Vector vector_add(Vector const left, Vector const right);
/** Subtracts the given vectors, and returns the result as a new vector. */
Vector vector_sub(Vector const left, Vector const right);
/** Multiplies the given vector and scalar, and returns the result as a new
 * vector. */
Vector vector_mul(Vector const left, float const right);
/** Returns the square of the given vector's length. */
float vector_square(Vector const vector);

#endif // VECTOR_H
