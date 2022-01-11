#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Game.h"

#define GRAPHICS_TANK_COLOR_COUNT 5

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
/** Renders the game. */
void graphics_render();

#endif // GRAPHICS_H
