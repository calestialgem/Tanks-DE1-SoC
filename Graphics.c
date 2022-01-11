#include "Game.h"
#include "Graphics.h"

void graphics_build(
	short pixel_map[GAME_HEIGHT][GAME_WIDTH], Game const *game_data) {
	int x, y;

	for (y=0; y < GAME_HEIGHT; y++) {
		for (x=0; x < GAME_WIDTH; x++) {
			if (y >= game_data->map.ground[x])
				// Paint the ground.
				pixel_map[y][x] = Color_gui_ground;
			else
				// Paint the background.
				/* Alternatively it can have a 320x240
				 * background and take all the pixels from there
				 * for a picture.*/
				pixel_map[y][x] = Color_gui_background;
		}
	}
}
void graphics_render(Game const *game_data) {
	short pixel_map[GAME_HEIGHT][GAME_WIDTH];
	/* Should return 320x240 pixel map. */
	graphics_build(pixel_map, game_data);
	int pixel_buf_ptr = *(int *)PIXEL_BUF_CTRL_BASE;
	int pixel_ptr, x, y;

	for (y = 0; y < GAME_HEIGHT; y++)
		for (x = 0; x < GAME_WIDTH; x++) {
			// Change to correct pixel's address.
			pixel_ptr = pixel_buf_ptr + (y << 10) + (x << 1);
			// Set pixel color.
			*(short *)pixel_ptr = pixel_map[y][x];
		}
}
