#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 320
#define MAP_HEIGHT 240
#define MAP_LEFT_BORDER 5
#define MAP_RIGHT_BORDER (MAP_WIDTH - MAP_LEFT_BORDER)

/** Terrain where tanks are on. */
typedef struct {
	/** Heights of the ground at different horizontal positions in pixel. */
	float ground[MAP_WIDTH];
} Map;

/** Generates the terrain. */
void map_generate(void);

#endif // MAP_H
