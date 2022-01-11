#include "Graphics.h"

#include "Game.h"

/** Tank Colors: Red, Green, Blue, Purple, Yellow */
static short Color_tank[GRAPHICS_TANK_COLOR_COUNT] = {
	0xC000, 0x0260, 0x11B4, 0x8010, 0xFEA0};
/** Copy of the game that is drawn. Before each render, the updated game is
 * copied safely by disabling interrupts. */
static Game drawn_copy;
/** Pixel map that buffers the color data to the screen. */
static short pixel_map[GAME_HEIGHT][GAME_WIDTH];

static inline void graphics_build() {
	int x, y;

	for (y = 0; y < GAME_HEIGHT; y++) {
		for (x = 0; x < GAME_WIDTH; x++) {
			if (y >= drawn_copy.map.ground[x]) // Paint the ground.
				pixel_map[y][x] = Color_gui_ground;
			else // Paint the background. Alternatively take this
			     // from picture.
				pixel_map[y][x] = Color_gui_background;
		}
	}
}
void graphics_render() {
	game_copy_safely(&drawn_copy);
	graphics_build(); // Updates 320x240 pixel map.

	int pixel_buf_ptr = *(int *)0xFF203020; // PIXEL_BUF_CTRL_BASE
	int pixel_ptr, x, y;

	// Display
	for (y = 0; y < GAME_HEIGHT; y++)
		for (x = 0; x < GAME_WIDTH; x++) {
			// Change to correct pixel's address.
			pixel_ptr = pixel_buf_ptr + (y << 10) + (x << 1);
			*(short *)pixel_ptr =
				pixel_map[y][x]; // Set pixel color.
		}
}
