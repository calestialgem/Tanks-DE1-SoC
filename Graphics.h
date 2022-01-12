#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Game.h"

#define GRAPHICS_TANK_COLOR_COUNT 5
#define Pixel_gui_border 35

// Configuration and main render file.
void graphics_initialize(void);
void graphics_update(void);
void graphics_render(void);
void graphics_draw_sprite(short originy, short originx, uint8_t pixel_count, uint8_t pixel_set[2][pixel_count], short Color);


#endif // GRAPHICS_H
