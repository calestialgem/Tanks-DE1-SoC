#include "Map.h"

#include "Game.h"
#include "MathTools.h"

void map_generate(void) {
	float const start = 200.0F;
	float const stop = 150.0F;
	float const slope = (start - stop) / MAP_WIDTH;
	float height = start;
	size_t x;
	for (x = 0; x < MAP_WIDTH; x++) {
		game_instance.map.ground[x] = height;
		height += slope;
	}
}
static inline float slope(size_t const firstIndex, size_t const secondIndex) {
	float const firstHeight = game_instance.map.ground[firstIndex];
	float const secondHeight = game_instance.map.ground[secondIndex];
	return firstHeight - secondHeight;
}
float map_slope(size_t const index) {
	if (index <= 0) {
		return slope(0, 1);
	} else if (index >= MAP_WIDTH - 1) {
		return slope(MAP_WIDTH - 2, MAP_WIDTH - 1);
	}
	float const leftSlope = slope(index - 1, index);
	float const rightSlope = slope(index, index + 1);
	return math_abs(leftSlope) < math_abs(rightSlope) ? leftSlope
							  : rightSlope;
}
