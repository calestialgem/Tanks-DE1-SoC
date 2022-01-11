#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Game.h"

#define PIXEL_BUF_CTRL_BASE 0xFF203020

// RGB to hex color converter:
// https://www.rapidtables.com/convert/color/rgb-to-hex.html Color format= 5 bit
// red, 6 bit green, 5 bit blue = 16 bits. Tank colors
#define Color_tank_red 0xC000
#define Color_tank_green 0x0260
#define Color_tank_blue 0x11B4
#define Color_tank_purple 0x8010
#define Color_tank_yellow 0xFEA0

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

#ifdef GRAPHICS_INTERRUPT_ID
/** Handles the graphics interrupts. Called by the interrupt handler. */
void graphics_isr(void);
#endif // GRAPHICS_INTERRUPT_ID

#endif // GRAPHICS_H
