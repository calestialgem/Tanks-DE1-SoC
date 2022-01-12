#include "Map.h"

#include "Game.h"
#include "MathTools.h"

#include <math.h>

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
			math_linearly_map(sinf(angle * peakCount + start),
				-1.0F,
				1.0F,
				peakHeight,
				valleyHeight);
	}
}
