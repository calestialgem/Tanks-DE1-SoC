#include "Map.h"

#include "Game.h"
#include "MathTools.h"

void map_generate(void) {
	float const peakCount = math_random(0.75F, 1.5F);
	float const start = math_random(0.0F, MATH_2PI);
	float const peakHeight = math_random(0.5F, 0.67F) * MAP_HEIGHT;
	float const valleyHeight = math_random(0.8F, 0.95F) * MAP_HEIGHT;
	size_t x;
	for (x = 0; x < MAP_WIDTH; x++) {
		float const angle =
			math_linearly_map(x, 0.0F, MAP_WIDTH, 0.0F, MATH_2PI);
		game_instance.map.ground[x] =
			math_linearly_map(math_sin(angle * peakCount + start),
				-1.0F,
				1.0F * math_random(0.8F, 1.2F),
				peakHeight,
				valleyHeight);
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
