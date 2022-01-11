#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Game.h"


// GUI Colors
#define Color_gui_red 0xF800
#define Color_gui_black 0x0000
#define Color_gui_grey 0x7BEF
#define Color_gui_cloud_blue 0x051D
#define Color_gui_fuel_green 0x0320
#define Color_gui_shield_blue 0x07FF
#define Color_gui_ground 0xFFFF
#define Color_gui_background 0x9E7F

// Configuration and main render file.
void graphics_build(
	short pixel_map[GAME_HEIGHT][GAME_WIDTH], Game const *game_data);
void graphics_render(Game const *game_data);

#endif // GRAPHICS_H
