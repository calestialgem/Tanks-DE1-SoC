#include "Vector.h"

#include "MathTools.h"

void vector_init(volatile Vector *const vector,
	float const magnitude,
	float const angle) {
	vector->x = math_cos(angle) * magnitude;
	vector->y = -math_sin(angle) * magnitude;
}
Vector vector_add(Vector const left, Vector const right) {
	Vector const result = {.x = left.x + right.x, .y = left.y + right.y};
	return result;
}
Vector vector_sub(Vector const left, Vector const right) {
	Vector const result = {.x = left.x - right.x, .y = left.y - right.y};
	return result;
}
Vector vector_mul(Vector const left, float const right) {
	Vector const result = {.x = left.x * right, .y = left.y * right};
	return result;
}
float vector_square(Vector const vector) {
	return math_square(vector.x) + math_square(vector.y);
}
