#ifndef MAP_H
#define MAP_H

#include "Vector.h"

#include <stdlib.h>

#define MAP_WIDTH 320
#define MAP_HEIGHT 240
#define MAP_LEFT_BORDER 5
#define MAP_RIGHT_BORDER (MAP_WIDTH - MAP_LEFT_BORDER)
#define MAP_MAX_WIND 5.0F

/** Terrain where tanks are on. */
typedef struct {
	/** Heights of the ground at different horizontal positions in pixel. */
	float ground[MAP_WIDTH];
	/** Bullet acceleration in m/s^2. */
	Vector acceleration;
	/** Position effect in m. */
	Vector positionEffect;
	/** Velocity effect in m/s. */
	Vector velocityEffect;
} Map;

/** Sets the ground to the given value after clamping. */
void map_set(size_t const index, float const height);
/** Sets the wind to a random value. */
void map_randomize_wind(void);
/** Generates the terrain. */
void map_generate(void);
/** Returns the index that corresponds to the given position. Clamps to map
 * limits. */
size_t map_index(float const position);
/** Returns the slope at the given position. */
float map_slope(size_t const index);

#endif // MAP_H
