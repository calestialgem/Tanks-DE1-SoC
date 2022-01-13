#include "Map.h"

#include "Game.h"
#include "MathTools.h"

void map_generate(void) {
	float const peakCount = math_random(1.25F, 2.0F);
	float const start = math_random(0.0F, MATH_2PI);
	float const peakHeight = math_random(0.5F, 0.67F);
	float const valleyHeight = math_random(0.75F, 0.85F);
	/* GUI at 0.15F. Give at least 0.20F playing space. */
	float const minHeight = 0.35F;
	float const maxHeight = 0.95F;
	float const sinHalf = (peakHeight + valleyHeight) / 2.0F;
	float const lineStart = math_random(peakHeight, valleyHeight) / sinHalf;
	float const lineEnd = math_random(peakHeight, valleyHeight) / sinHalf;
	float const slope = (lineEnd - lineStart) / MAP_WIDTH;
	size_t x;
	for (x = 0; x < MAP_WIDTH; x++) {
		float const angle =
			math_linearly_map(x, 0.0F, MAP_WIDTH, 0.0F, MATH_2PI);
		float const sin =
			math_linearly_map(math_sin(angle * peakCount + start),
				-1.0F,
				1.0F,
				peakHeight,
				valleyHeight);
		float const line = lineStart + x * slope;
		game_instance.map.ground[x] =
			math_clamp(sin * line, minHeight, maxHeight) *
			MAP_HEIGHT;
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
