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



void graphics_draw(short originx, short originy, uint8_t pixel_count, uint8_t pixel_set[2][pixel_count], short Color){
for(int i=0;i<pixel_count;i++)
pixel_map[ originy+pixel_set[1][i] ] [ originx+pixel_set[2][i] ]= Color;
}



void graphics_initialize(){	//Initialize the whole screen
  int x, y;
	short Color_gui_red =(short) 0xF800;
	short Color_gui_black =(short) 0x0000;
	short Color_gui_grey =(short) 0x7BEF;
	short  Color_gui_background =(short) 0xFFFF;

	short Color_gui_cloud_blue =(short) 0x051D;
	short Color_gui_fuel_green =(short) 0x0320;
	short  Color_gui_shield_blue =(short) 0x07FF;


	for (y=0; y < GAME_HEIGHT; y++) {			//Initialize GUI & screen
		for (x=0; x < GAME_WIDTH; x++) {
			if (y != Pixel_gui_border) 			  //Everything else is white.
				pixel_map[y][x] = Color_gui_background;
			else                              //Draw the border.
				pixel_map[y][x] = Color_gui_black;
		}
	}

//Define the sprites that won't change and print them on.{{y},{x}}
	uint8_t sprite_fuel_can_black[2][26]={{0,2,3,4,5,6,7,1,2,7,1,2,4,5,7,1,2,4,6,7,2,3,4,5,6,7},
																		    {0,0,0,0,0,0,0,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4,4}};
  uint8_t sprite_fuel_can_green[2][8]={{3,4,5,6,3,6,3,5},
				    													 {1,1,1,1,2,2,3,3}};

	graphics_draw(14,8,26,sprite_fuel_can_black,Color_gui_black);
  graphics_draw(14,8,8,sprite_fuel_can_green,Color_gui_fuel_green);




}




static inline void graphics_update() {
	int x, y;
	short Color_map_ground =(short) 0xFFFF;
	short Color_map_background =(short) 0x9E7F;

	for (y=0; y < GAME_HEIGHT; y++) {
		for (x=0; x < GAME_WIDTH; x++) {

			if (y >= drawn_copy.map.ground[x]) // Paint the ground.
				pixel_map[y][x] = Color_map_ground;
			else // Paint the background. Alternatively take this from picture.
				pixel_map[y][x] = Color_map_background;
		}

	}
}


void graphics_render() {
	game_copy_safely(&drawn_copy);

	int pixel_buf_ptr = *(int *) 0xFF203020; //PIXEL_BUF_CTRL_BASE
	int pixel_ptr, x, y;

	//Display
	for (y = 0; y < GAME_HEIGHT; y++)
		for (x = 0; x < GAME_WIDTH; x++) {     // Change to correct pixel's address.
			pixel_ptr = pixel_buf_ptr + (y << 10) + (x << 1);
						*(short *)pixel_ptr = pixel_map[y][x]; // Set pixel color.
		}
}
