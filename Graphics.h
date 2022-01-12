#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Game.h"

#define GRAPHICS_TANK_COLOR_COUNT 5
#define Pixel_gui_border 35

// Configuration and main render file.
void graphics_initialize(void);
void graphics_render(void);
void graphics_draw_line(short originy, short originx, short finaly, short finalx, short Color);
void graphics_draw_rectangle(short originy, short originx, short finaly, short finalx, short Color);
void graphics_draw_sprite(short originy, short originx, uint8_t pixel_count, uint8_t pixel_set[2][pixel_count], short Color);
void graphics_draw_numbers(short originy, short originx, int number, short Color_background);
void graphics_draw_power(int power);
void graphics_draw_angle(int angle);


#endif // GRAPHICS_H
