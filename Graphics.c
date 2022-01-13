#include "Graphics.h"

#include "Game.h"

// Macros
#define min(A, B) (((A) < (B)) ? (A) : (B))
#define max(A, B) (((A) > (B)) ? (A) : (B))

int fpow(int x, int y) {
	int i, res = 1;
	for (i = 0; i < y; i++)
		res = res * x;
	return res;
}

int flog(int x) {
	int log = 0;
	while (x >= 10) {
		x /= 10;
		log += 1;
	}
	return log;
}

int digit(int N) {
	if (N != 0)
		return flog(N) + 1;
}
// Colors
// Tank Colors: Red, Green, Blue, Purple, Yellow
static short Color_tank[GRAPHICS_TANK_COLOR_COUNT] = {
	0xC000, 0x0260, 0x11B4, 0x8010, 0xFEA0};
static short Color_tank_grey = (short)0xC618;
static short Color_barrel = (short)0x0000;

static short Color_gui_red = (short)0xF800;
static short Color_gui_black = (short)0x0000;
static short Color_gui_grey = (short)0x7BEF;
static short Color_gui_background = (short)0xFFFF;

static short Color_map_ground = (short)0xFFFF;
static short Color_map_background = (short)0x9E7F;

// Number Sprites
uint8_t sprite_num_0_black[2][12] = {{0, 1, 2, 3, 4, 0, 4, 0, 1, 2, 3, 4},
	{0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2}};
uint8_t sprite_num_0_white[2][3] = {{1, 2, 3}, {1, 1, 1}};
uint8_t sprite_num_1_black[2][8] = {
	{1, 4, 0, 1, 2, 3, 4, 4}, {0, 0, 1, 1, 1, 1, 1, 2}};
uint8_t sprite_num_1_white[2][7] = {
	{0, 2, 3, 0, 1, 2, 3}, {0, 0, 0, 2, 2, 2, 2}};
uint8_t sprite_num_2_black[2][11] = {
	{0, 2, 3, 4, 0, 2, 4, 0, 1, 2, 4}, {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2}};
uint8_t sprite_num_2_white[2][4] = {{1, 1, 3, 3}, {0, 1, 1, 2}};
uint8_t sprite_num_3_black[2][11] = {
	{0, 2, 4, 0, 2, 4, 0, 1, 2, 3, 4}, {0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2}};
uint8_t sprite_num_3_white[2][4] = {{1, 3, 1, 3}, {0, 0, 1, 1}};
uint8_t sprite_num_4_black[2][9] = {
	{0, 1, 2, 2, 0, 1, 2, 3, 4}, {0, 0, 0, 1, 2, 2, 2, 2, 2}};
uint8_t sprite_num_4_white[2][6] = {{3, 4, 0, 1, 3, 4}, {0, 0, 1, 1, 1, 1}};
uint8_t sprite_num_5_black[2][11] = {
	{0, 1, 2, 4, 0, 2, 4, 0, 2, 3, 4}, {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2}};
uint8_t sprite_num_5_white[2][4] = {{3, 1, 3, 1}, {0, 1, 1, 2}};
uint8_t sprite_num_6_black[2][12] = {{0, 1, 2, 3, 4, 0, 2, 4, 0, 2, 3, 4},
	{0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2}};
uint8_t sprite_num_6_white[2][3] = {{1, 3, 1}, {1, 1, 2}};
uint8_t sprite_num_7_black[2][7] = {
	{0, 0, 0, 1, 2, 3, 4}, {0, 1, 2, 2, 2, 2, 2}};
uint8_t sprite_num_7_white[2][8] = {
	{1, 2, 3, 4, 1, 2, 3, 4}, {0, 0, 0, 0, 1, 1, 1, 1}};
uint8_t sprite_num_8_black[2][13] = {{0, 1, 2, 3, 4, 0, 2, 4, 0, 1, 2, 3, 4},
	{0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2}};
uint8_t sprite_num_8_white[2][2] = {{1, 3}, {1, 1}};
uint8_t sprite_num_9_black[2][12] = {{0, 1, 2, 4, 0, 2, 4, 0, 1, 2, 3, 4},
	{0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2}};
uint8_t sprite_num_9_white[2][3] = {{3, 1, 3}, {0, 1, 1}};
uint8_t sprite_num_P[2][10] = {
	{0, 1, 2, 3, 4, 0, 2, 0, 1, 2}, {0, 0, 0, 0, 0, 1, 1, 2, 2, 2}};

// Tank Sprites
uint8_t sprite_tank_m90_red[2][24] = {{1,
					      2,
					      3,
					      4,
					      5,
					      0,
					      1,
					      2,
					      3,
					      4,
					      5,
					      6,
					      0,
					      1,
					      2,
					      3,
					      4,
					      5,
					      6,
					      1,
					      2,
					      3,
					      4,
					      5},
	{0,
		0,
		0,
		0,
		0,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		3,
		3,
		3,
		3,
		3}};
uint8_t sprite_tank_m90_grey[2][2] = {{0, 6}, {3, 3}};
uint8_t sprite_tank_m67_red[2][25] = {{3,
					      4,
					      5,
					      1,
					      2,
					      3,
					      4,
					      5,
					      6,
					      0,
					      1,
					      2,
					      3,
					      4,
					      5,
					      6,
					      0,
					      1,
					      2,
					      3,
					      4,
					      5,
					      1,
					      2,
					      3},
	{0,
		0,
		0,
		1,
		1,
		1,
		1,
		1,
		1,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		3,
		3,
		3,
		3,
		3,
		3,
		4,
		4,
		4}};
uint8_t sprite_tank_m67_grey[2][2] = {{6, 0}, {3, 4}};
uint8_t sprite_tank_m45_red[2][22] = {
	{3, 4, 2, 3, 4, 5, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 0, 1, 2, 3, 1, 2},
	{0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5}};
uint8_t sprite_tank_m45_grey[2][2] = {{5, 0}, {0, 5}};
uint8_t sprite_tank_m22_red[2][25] = {{2,
					      3,
					      1,
					      2,
					      3,
					      4,
					      1,
					      2,
					      3,
					      4,
					      0,
					      1,
					      2,
					      3,
					      4,
					      0,
					      1,
					      2,
					      3,
					      0,
					      1,
					      2,
					      3,
					      1,
					      2},
	{0,
		0,
		1,
		1,
		1,
		1,
		2,
		2,
		2,
		2,
		3,
		3,
		3,
		3,
		3,
		4,
		4,
		4,
		4,
		5,
		5,
		5,
		5,
		6,
		6}};
uint8_t sprite_tank_m22_grey[2][2] = {{4, 3}, {0, 6}};
uint8_t sprite_tank_0_red[2][24] = {{1,
					    2,
					    0,
					    1,
					    2,
					    3,
					    0,
					    1,
					    2,
					    3,
					    0,
					    1,
					    2,
					    3,
					    0,
					    1,
					    2,
					    3,
					    0,
					    1,
					    2,
					    3,
					    1,
					    2},
	{0,
		0,
		1,
		1,
		1,
		1,
		2,
		2,
		2,
		2,
		3,
		3,
		3,
		3,
		4,
		4,
		4,
		4,
		5,
		5,
		5,
		5,
		6,
		6}};
uint8_t sprite_tank_0_grey[2][2] = {{3, 3}, {0, 6}};
uint8_t sprite_tank_22_red[2][25] = {{1,
					     2,
					     0,
					     1,
					     2,
					     3,
					     0,
					     1,
					     2,
					     3,
					     0,
					     1,
					     2,
					     3,
					     4,
					     1,
					     2,
					     3,
					     4,
					     1,
					     2,
					     3,
					     4,
					     2,
					     3},
	{0,
		0,
		1,
		1,
		1,
		1,
		2,
		2,
		2,
		2,
		3,
		3,
		3,
		3,
		3,
		4,
		4,
		4,
		4,
		5,
		5,
		5,
		5,
		6,
		6}};
uint8_t sprite_tank_22_grey[2][2] = {{3, 4}, {0, 6}};
uint8_t sprite_tank_45_red[2][22] = {
	{1, 2, 0, 1, 2, 3, 0, 1, 2, 3, 4, 1, 2, 3, 4, 5, 2, 3, 4, 5, 3, 4},
	{0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5}};
uint8_t sprite_tank_45_grey[2][2] = {{0, 5}, {0, 5}};
uint8_t sprite_tank_67_red[2][25] = {{1,
					     2,
					     3,
					     0,
					     1,
					     2,
					     3,
					     4,
					     5,
					     0,
					     1,
					     2,
					     3,
					     4,
					     5,
					     6,
					     1,
					     2,
					     3,
					     4,
					     5,
					     6,
					     3,
					     4,
					     5},
	{0,
		0,
		0,
		1,
		1,
		1,
		1,
		1,
		1,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		3,
		3,
		3,
		3,
		3,
		3,
		4,
		4,
		4}};
uint8_t sprite_tank_67_grey[2][2] = {{0, 6}, {0, 1}};
uint8_t sprite_tank_90_red[2][24] = {{1,
					     2,
					     3,
					     4,
					     5,
					     0,
					     1,
					     2,
					     3,
					     4,
					     5,
					     6,
					     0,
					     1,
					     2,
					     3,
					     4,
					     5,
					     6,
					     1,
					     2,
					     3,
					     4,
					     5},
	{0,
		0,
		0,
		0,
		0,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		3,
		3,
		3,
		3,
		3}};
uint8_t sprite_tank_90_grey[2][2] = {{0, 6}, {0, 0}};
// Barrel Sprites
uint8_t sprite_barrel_180[2][6] = {{0, 0, 0, 0, 0, 0}, {0, 1, 2, 3, 4, 5}};
uint8_t sprite_barrel_157[2][6] = {{0, 1, 1, 2, 2, 3}, {0, 1, 2, 3, 4, 5}};
uint8_t sprite_barrel_135[2][6] = {{0, 1, 2, 3, 4, 5}, {0, 1, 2, 3, 4, 5}};
uint8_t sprite_barrel_112[2][6] = {{0, 1, 2, 3, 4, 5}, {0, 1, 1, 2, 2, 3}};
uint8_t sprite_barrel_90[2][6] = {{0, 1, 2, 3, 4, 5}, {0, 0, 0, 0, 0, 0}};
uint8_t sprite_barrel_67[2][6] = {{5, 4, 3, 2, 1, 0}, {0, 1, 1, 2, 2, 3}};
uint8_t sprite_barrel_45[2][6] = {{5, 4, 3, 2, 1, 0}, {0, 1, 2, 3, 4, 5}};
uint8_t sprite_barrel_22[2][6] = {{3, 2, 2, 1, 1, 0}, {0, 1, 2, 3, 4, 5}};
uint8_t sprite_barrel_0[2][6] = {{0, 0, 0, 0, 0, 0}, {0, 1, 2, 3, 4, 5}};

/** Copy of the game that is drawn. Before each render, the updated game is
 * copied safely by disabling interrupts. */
static Game drawn_copy;

/** Pixel map that buffers the color data to the screen. */
short pixel_map[MAP_HEIGHT][MAP_WIDTH];

void graphics_draw_line(short originy,
	short originx,
	short finaly,
	short finalx,
	short Color) { // Input start and end points.
	int x, y;
	for (y = min(originy, finaly); y <= max(originy, finaly); y++) {
		for (x = min(originx, finalx); x <= max(originx, finalx); x++) {
			if (y - originy -
					(finaly - originy) * (x - originx) /
						(finalx - originx) ==
				0)
				pixel_map[y][x] = Color;
		}
	}
}

void graphics_draw_rectangle(short originy,
	short originx,
	short finaly,
	short finalx,
	short Color) { // Input corner points
	int x, y;
	for (y = min(originy, finaly); y <= max(originy, finaly); y++) {
		for (x = min(originx, finalx); x <= max(originx, finalx); x++) {
			pixel_map[y][x] = Color;
		}
	}
}

void graphics_draw_sprite(short originy,
	short originx,
	uint8_t pixel_count,
	uint8_t pixel_set[2][pixel_count],
	short Color) {
	int i;
	for (i = 0; i < pixel_count; i++)
		pixel_map[originy + pixel_set[0][i]]
			 [originx + pixel_set[1][i]] = Color;
}

void graphics_draw_numbers(short originy,
	short originx,
	int number,
	short Color_background) { // Input origin of number. (Number is 1-3
				  // digits long)
	int i;
	graphics_draw_rectangle(originy,
		originx,
		originy + 4,
		originx + 10,
		Color_gui_background);
	int d = digit(number);
	for (i = 0; i < d; i++) {
		switch (number /
			fpow(10, d - 1 - i)) { // Number/100 gives the biggest
					       // digit for 3 digits. Cast pow
					       // result whic is double to int.
		case 0:
			graphics_draw_sprite(originy,
				originx + 4 * i,
				12,
				sprite_num_0_black,
				Color_gui_black);
			graphics_draw_sprite(originy,
				originx + 4 * i,
				3,
				sprite_num_0_white,
				Color_background);
			break;
		case 1:
			graphics_draw_sprite(originy,
				originx + 4 * i,
				8,
				sprite_num_1_black,
				Color_gui_black);
			graphics_draw_sprite(originy,
				originx + 4 * i,
				7,
				sprite_num_1_white,
				Color_background);
			break;
		case 2:
			graphics_draw_sprite(originy,
				originx + 4 * i,
				11,
				sprite_num_2_black,
				Color_gui_black);
			graphics_draw_sprite(originy,
				originx + 4 * i,
				4,
				sprite_num_2_white,
				Color_background);
			break;
		case 3:
			graphics_draw_sprite(originy,
				originx + 4 * i,
				11,
				sprite_num_3_black,
				Color_gui_black);
			graphics_draw_sprite(originy,
				originx + 4 * i,
				4,
				sprite_num_3_white,
				Color_background);
			break;
		case 4:
			graphics_draw_sprite(originy,
				originx + 4 * i,
				9,
				sprite_num_4_black,
				Color_gui_black);
			graphics_draw_sprite(originy,
				originx + 4 * i,
				6,
				sprite_num_4_white,
				Color_background);
			break;
		case 5:
			graphics_draw_sprite(originy,
				originx + 4 * i,
				11,
				sprite_num_5_black,
				Color_gui_black);
			graphics_draw_sprite(originy,
				originx + 4 * i,
				4,
				sprite_num_5_white,
				Color_background);
			break;
		case 6:
			graphics_draw_sprite(originy,
				originx + 4 * i,
				12,
				sprite_num_6_black,
				Color_gui_black);
			graphics_draw_sprite(originy,
				originx + 4 * i,
				3,
				sprite_num_6_white,
				Color_background);
			break;
		case 7:
			graphics_draw_sprite(originy,
				originx + 4 * i,
				7,
				sprite_num_7_black,
				Color_gui_black);
			graphics_draw_sprite(originy,
				originx + 4 * i,
				8,
				sprite_num_7_white,
				Color_background);
			break;
		case 8:
			graphics_draw_sprite(originy,
				originx + 4 * i,
				13,
				sprite_num_8_black,
				Color_gui_black);
			graphics_draw_sprite(originy,
				originx + 4 * i,
				2,
				sprite_num_8_white,
				Color_background);
			break;
		case 9:
			graphics_draw_sprite(originy,
				originx + 4 * i,
				12,
				sprite_num_9_black,
				Color_gui_black);
			graphics_draw_sprite(originy,
				originx + 4 * i,
				3,
				sprite_num_9_white,
				Color_background);
			break;
		}
		number = number % fpow(10, d - 1 - i);
	}
}

void graphics_draw_power(int power) {
	graphics_draw_rectangle(
		4, 72, 23, 83, Color_gui_background); // Reset its background to
						      // reset the slider.
	graphics_draw_rectangle(4, 73, 7, 82, Color_gui_red); // Draw the cone
	graphics_draw_rectangle(8, 74, 11, 81, Color_gui_red);
	graphics_draw_rectangle(12, 75, 15, 80, Color_gui_red);
	graphics_draw_rectangle(16, 76, 19, 79, Color_gui_red);
	graphics_draw_rectangle(20, 77, 23, 78, Color_gui_red);
	graphics_draw_line(24 - power / 5,
		77 - (power / 20),
		24 - power / 5,
		78 + (power / 20),
		Color_gui_black); // Draw the slider
}

void graphics_draw_angle(int angle) {
	graphics_draw_rectangle(
		3, 92, 8, 110, Color_gui_background); // Reset its background to
						      // reset the slider.
	graphics_draw_rectangle(4, 92, 7, 110, Color_gui_red); // Draw the bar
	graphics_draw_rectangle(3,
		110 - angle / 10,
		8,
		110 - angle / 10,
		Color_gui_black); // Draw the slider
}

void graphics_initialize() { // Initialize the whole screen, Draw all the pixels
			     // that doesn't need redrawing.
	int x, y;

	short Color_gui_cloud_blue = (short)0x051D;
	short Color_gui_fuel_green = (short)0x0320;
	short Color_gui_shield_blue = (short)0x07FF;
	short Color_gui_health_blue = (short)0x001F;

	for (y = 0; y < MAP_HEIGHT; y++) { // Initialize GUI & screen
		for (x = 0; x < MAP_WIDTH; x++) {
			if (y < Pixel_gui_border) // Everything else is white.
				pixel_map[y][x] = Color_gui_background;
			else if (y > Pixel_gui_border)
				pixel_map[y][x] = Color_gui_cloud_blue;
			else // Draw the border.
				pixel_map[y][x] = Color_gui_black;
		}
	}

	// Define the sprites that won't change and print them on.{{y},{x}}
	uint8_t sprite_fuel_can_black[2][26] = {{0,
							2,
							3,
							4,
							5,
							6,
							7,
							1,
							2,
							7,
							1,
							2,
							4,
							5,
							7,
							1,
							2,
							4,
							6,
							7,
							2,
							3,
							4,
							5,
							6,
							7},
		{0,
			0,
			0,
			0,
			0,
			0,
			0,
			1,
			1,
			1,
			2,
			2,
			2,
			2,
			2,
			3,
			3,
			3,
			3,
			3,
			4,
			4,
			4,
			4,
			4,
			4}};
	uint8_t sprite_fuel_can_green[2][8] = {
		{3, 4, 5, 6, 3, 6, 3, 5}, {1, 1, 1, 1, 2, 2, 3, 3}};
	uint8_t sprite_health_black[2][24] = {{1,
						      2,
						      3,
						      4,
						      5,
						      1,
						      5,
						      0,
						      1,
						      5,
						      1,
						      5,
						      1,
						      5,
						      0,
						      1,
						      5,
						      1,
						      5,
						      1,
						      2,
						      3,
						      4,
						      5},
		{0,
			0,
			0,
			0,
			0,
			1,
			1,
			2,
			2,
			2,
			3,
			3,
			4,
			4,
			5,
			5,
			5,
			6,
			6,
			7,
			7,
			7,
			7,
			7}};
	uint8_t sprite_health_red[2][5] = {{3, 2, 3, 4, 3}, {4, 5, 5, 5, 6}};
	uint8_t sprite_health_blue[2][2] = {{3, 3}, {1, 2}};
	uint8_t sprite_shield[2][16] = {
		{2, 3, 4, 1, 5, 0, 6, 0, 6, 0, 6, 1, 5, 2, 3, 4},
		{0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6}};
	uint8_t sprite_repair_black[2][19] = {
		{1, 2, 0, 2, 0, 1, 2, 3, 2, 3, 4, 3, 4, 5, 4, 5, 6, 5, 6},
		{0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6}};
	uint8_t sprite_repair_grey[2][8] = {
		{5, 6, 4, 5, 6, 5, 2, 1}, {1, 1, 2, 2, 2, 3, 5, 6}};
	uint8_t sprite_parachute_black[2][5] = {
		{4, 5, 6, 5, 4}, {1, 2, 3, 4, 5}};
	uint8_t sprite_parachute_grey[2][17] = {
		{2, 3, 1, 3, 0, 2, 3, 0, 1, 3, 0, 2, 3, 1, 3, 2, 3},
		{0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6}};
	uint8_t sprite_teleport_black[2][8] = {
		{1, 2, 0, 0, 1, 2, 3, 4}, {0, 0, 1, 2, 3, 3, 4, 4}};
	uint8_t sprite_teleport_red[2][5] = {{4, 6, 5, 4, 6}, {3, 3, 4, 5, 5}};
	uint8_t sprite_cloud[2][57] = {{2,
					       3,
					       4,
					       1,
					       2,
					       3,
					       4,
					       5,
					       1,
					       2,
					       3,
					       4,
					       5,
					       1,
					       2,
					       3,
					       4,
					       5,
					       2,
					       3,
					       4,
					       5,
					       2,
					       3,
					       4,
					       5,
					       1,
					       2,
					       3,
					       4,
					       5,
					       0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       1,
					       2,
					       3,
					       4,
					       5,
					       2,
					       3,
					       4},
		{0,
			0,
			0,
			1,
			1,
			1,
			1,
			1,
			2,
			2,
			2,
			2,
			2,
			3,
			3,
			3,
			3,
			3,
			4,
			4,
			4,
			4,
			5,
			5,
			5,
			5,
			6,
			6,
			6,
			6,
			6,
			7,
			7,
			7,
			7,
			7,
			7,
			8,
			8,
			8,
			8,
			8,
			8,
			9,
			9,
			9,
			9,
			9,
			9,
			10,
			10,
			10,
			10,
			10,
			11,
			11,
			11}};
	uint8_t sprite_big_arrow_left[2][25] = {{4,
							3,
							4,
							5,
							2,
							3,
							4,
							5,
							6,
							1,
							2,
							3,
							4,
							5,
							6,
							7,
							0,
							1,
							2,
							3,
							4,
							5,
							6,
							7,
							8},
		{0,
			1,
			1,
			1,
			2,
			2,
			2,
			2,
			2,
			3,
			3,
			3,
			3,
			3,
			3,
			3,
			4,
			4,
			4,
			4,
			4,
			4,
			4,
			4,
			4}};
	uint8_t sprite_big_arrow_right[2][25] = {{0,
							 1,
							 2,
							 3,
							 4,
							 5,
							 6,
							 7,
							 8,
							 1,
							 2,
							 3,
							 4,
							 5,
							 6,
							 7,
							 2,
							 3,
							 4,
							 5,
							 6,
							 3,
							 4,
							 5,
							 4},
		{0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			2,
			2,
			2,
			2,
			2,
			3,
			3,
			3,
			4}};
	uint8_t sprite_key_f[2][31] = {{0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       6,
					       2,
					       3,
					       4,
					       5,
					       2,
					       4,
					       2},
		{0,
			0,
			0,
			0,
			0,
			0,
			0,
			1,
			1,
			2,
			2,
			3,
			3,
			4,
			4,
			5,
			5,
			6,
			6,
			6,
			6,
			6,
			6,
			6,
			2,
			2,
			2,
			2,
			3,
			3,
			4}};
	uint8_t sprite_key_r[2][33] = {{0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       6,
					       2,
					       3,
					       4,
					       5,
					       2,
					       4,
					       2,
					       3,
					       5},
		{0,
			0,
			0,
			0,
			0,
			0,
			0,
			1,
			1,
			2,
			2,
			3,
			3,
			4,
			4,
			5,
			5,
			6,
			6,
			6,
			6,
			6,
			6,
			6,
			2,
			2,
			2,
			2,
			3,
			3,
			4,
			4,
			4}};
	uint8_t sprite_key_p[2][33] = {{0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       6,
					       2,
					       3,
					       4,
					       5,
					       2,
					       4,
					       2,
					       3,
					       4},
		{0,
			0,
			0,
			0,
			0,
			0,
			0,
			1,
			1,
			2,
			2,
			3,
			3,
			4,
			4,
			5,
			5,
			6,
			6,
			6,
			6,
			6,
			6,
			6,
			2,
			2,
			2,
			2,
			3,
			3,
			4,
			4,
			4}};
	uint8_t sprite_key_t[2][30] = {{0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       6,
					       0,
					       1,
					       2,
					       3,
					       4,
					       5,
					       6,
					       2,
					       2,
					       3,
					       4,
					       5,
					       2},
		{0,
			0,
			0,
			0,
			0,
			0,
			0,
			1,
			1,
			2,
			2,
			3,
			3,
			4,
			4,
			5,
			5,
			6,
			6,
			6,
			6,
			6,
			6,
			6,
			2,
			3,
			3,
			3,
			3,
			4}};

	graphics_draw_sprite(
		14, 8, 26, sprite_fuel_can_black, Color_gui_black); // Fuel Can
	graphics_draw_sprite(
		14, 8, 8, sprite_fuel_can_green, Color_gui_fuel_green);
	graphics_draw_sprite(
		25, 7, 24, sprite_health_black, Color_gui_black); // Health
	graphics_draw_sprite(25, 7, 5, sprite_health_red, Color_gui_red);
	graphics_draw_sprite(
		25, 7, 2, sprite_health_blue, Color_gui_health_blue);
	graphics_draw_sprite(
		4, 42, 16, sprite_shield, Color_gui_shield_blue); // Shield
	graphics_draw_sprite(15,
		37,
		19,
		sprite_repair_black,
		Color_gui_black); // Repair Pack
	graphics_draw_sprite(15, 37, 8, sprite_repair_grey, Color_gui_grey);
	graphics_draw_sprite(15,
		47,
		5,
		sprite_parachute_black,
		Color_gui_black); // Parachute
	graphics_draw_sprite(15, 47, 17, sprite_parachute_grey, Color_gui_grey);
	graphics_draw_sprite(
		15, 57, 8, sprite_teleport_black, Color_gui_black); // Teleport
	graphics_draw_sprite(15, 57, 5, sprite_teleport_red, Color_gui_red);
	graphics_draw_sprite(
		4, 120, 57, sprite_cloud, Color_gui_cloud_blue); // Cloud
	graphics_draw_sprite(13,
		155,
		25,
		sprite_big_arrow_left,
		Color_gui_black); // Big Arrows
	graphics_draw_sprite(
		13, 282, 25, sprite_big_arrow_right, Color_gui_black);
	graphics_draw_sprite(4, 52, 31, sprite_key_f, Color_gui_black); // Keys
	graphics_draw_sprite(24, 37, 33, sprite_key_r, Color_gui_black);
	graphics_draw_sprite(24, 47, 33, sprite_key_p, Color_gui_black);
	graphics_draw_sprite(24, 57, 30, sprite_key_t, Color_gui_black);
	graphics_draw_sprite(7,
		7,
		2,
		sprite_tank_0_grey,
		Color_tank_grey); // Player Indicator
	graphics_draw_sprite(3, 10, 6, sprite_barrel_45, Color_barrel);
	graphics_draw_sprite(6, 17, 10, sprite_num_P, Color_gui_black);
}

void graphics_render() {
	int x, y;

	// Paint the ground & the background
	for (y = 0; y < MAP_HEIGHT; y++) {
		for (x = 0; x < MAP_WIDTH; x++) {
			// Paint the ground.
			if (y >= drawn_copy.map.ground[x])
				pixel_map[y][x] = Color_map_ground;
			else if (y > Pixel_gui_border) // Paint the background.
						       // Alternatively take
						       // this from picture.
				pixel_map[y][x] = Color_map_background;
		}
	}
	volatile Tank *const tank = &drawn_copy.tanks.array[drawn_copy.turn];
	// Update the Pixel Map
	graphics_draw_sprite(7,
		7,
		24,
		sprite_tank_0_red,
		Color_tank[tank->color]); // Player Indicator
	graphics_draw_numbers(6,
		21,
		drawn_copy.turn + 1,
		Color_gui_background); // Player number
	graphics_draw_numbers(17, 17, tank->fuel, Color_gui_background); // Fuel
	graphics_draw_numbers(
		26, 17, tank->health, Color_gui_background); // Health
	graphics_draw_numbers(
		19, 84, tank->gun.power, Color_gui_background); // Power
	graphics_draw_numbers(
		10, 96, tank->gun.angle, Color_gui_background);	 // Angle
	graphics_draw_numbers(16, 121, 0, Color_gui_background); // Wind
	graphics_draw_power(tank->gun.power);			 // Power Bar
	graphics_draw_angle(tank->gun.angle);			 // Angle Bar

	int pixel_buf_ptr = *(int *)0xFF203020; // PIXEL_BUF_CTRL_BASE
	int pixel_ptr;

	// Display
	for (y = 0; y < MAP_HEIGHT; y++)
		for (x = 0; x < MAP_WIDTH;
			x++) { // Change to correct pixel's address.
			pixel_ptr = pixel_buf_ptr + (y << 10) + (x << 1);
			*(short *)pixel_ptr =
				pixel_map[y][x]; // Set pixel color.
		}
}
