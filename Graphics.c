#include "Game.h"
#include "Graphics.h"

short Color_tank=[0xC000,0x0260,0x11B4,0x8010,0xFEA0] //Red, Green, Blue, Purple, Yellow



void graphics_build(
	short pixel_map[GAME_HEIGHT][GAME_WIDTH], Game const *game_data) {
	int x, y;

	for (y=0; y < GAME_HEIGHT; y++) {
		for (x=0; x < GAME_WIDTH; x++) {

			if (y >= game_data->map.ground[x]) // Paint the ground.
				pixel_map[y][x] = Color_gui_ground;
			else // Paint the background. Alternatively take this from picture.
				pixel_map[y][x] = Color_gui_background;
		}

	}
}

void graphics_render(Game const *game_data) {

	short pixel_map[GAME_HEIGHT][GAME_WIDTH];
	graphics_build(pixel_map, game_data); //Returns 320x240 pixel map.

	int pixel_buf_ptr = *(int *) 0xFF203020; //PIXEL_BUF_CTRL_BASE
	int pixel_ptr, x, y;

	//Display
	for (y = 0; y < GAME_HEIGHT; y++)
		for (x = 0; x < GAME_WIDTH; x++) {     // Change to correct pixel's address.
			pixel_ptr = pixel_buf_ptr + (y << 10) + (x << 1);
						*(short *)pixel_ptr = pixel_map[y][x]; // Set pixel color.
		}

}
