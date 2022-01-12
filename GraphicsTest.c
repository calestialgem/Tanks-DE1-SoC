#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define GAME_WIDTH 320
#define GAME_HEIGHT 240

#define GAME_GRAVITY 9.81F

#define GAME_BULLET_CAPACITY 8
#define GAME_BULLET_DENSITY 2500.0F

#define GAME_TANK_CAPACITY 4
#define GAME_TANK_INITIAL_HEALTH 100.0F
#define GAME_TANK_NAME_CAPACITY 32

/** Vector of two floats. */
typedef struct {
	/** First component. */
	float x;
	/** Second component. */
	float y;
} Vector;
/** Projectiles that do damage. */
typedef struct {
	/** Position of the center of mass in m. */
	Vector position;
	/** Velocity of the center of mass in m/s. */
	Vector velocity;
	/** Diameter in m. */
	float diameter;
	/** Total mass in kg. */
	float mass;
	/** Power released when it explodes. */
	float power;
} Bullet;
/** Array of bullets. */
typedef struct {
	/** Array of bullets with a set capacity. */
	Bullet array[GAME_BULLET_CAPACITY];
	/** Amount of bullets in the array. */
	uint8_t size;
} Bullets;
/** Weapon on top of a tank. */
typedef struct {
	/** The angle with respect to the tank in rad. */
	float angle;
	/** The amount bullets are shot forward percent. */
	uint8_t power;
} Barrel;
/** Characters controlled by players. */
typedef struct {
	/** Position of the middle horizontally and down vertically in m. */
	Vector position;
	/** The angle the tank is standing with respect to the ground in rad. */
	float tilt;
	/** Remaining healt of the tank in percents. */
	uint8_t health;
	/** Whether the tank is alive or not. */
	bool alive;
	/** Weapon. */
	Barrel gun;
	/** Player's name. */
	char name[GAME_TANK_NAME_CAPACITY];
	/** Index of the player's color. */
	uint8_t color;
} Tank;
/** Array of tanks. */
typedef struct {
	/** Array of tanks with a set capacity. */
	Tank array[GAME_TANK_CAPACITY];
	/** Amount of tanks in the array. */
	uint8_t size;
} Tanks;
/** Terrain where tanks are on. */
typedef struct {
	/** Heights of the ground at different horizontal positions in m. */
	float ground[GAME_WIDTH];
} Map;
/** Game state. */
typedef struct {
	/** Remaining tanks. */
	Tanks tanks;
	/** Active bullets. */
	Bullets bullets;
	/** Map. */
	Map map;
	/** Whether the game is running. */
	bool playing;
	/** The index of the tank that can do actions. */
	uint8_t turn;
	/** Wheter the current tank is shooting. */
	bool shooting;
} Game;

/** Adds a tank to the array. */
void game_add_tank(char const *name, uint8_t color);
/** Removes the tank at the given index from the array. */
void game_remove_tank(uint8_t index);
/** Restarts the game. */
void game_restart();
/** Updates the game. */
void game_update();
/** Copies the global game object to the given one. Does this after disabling
 * interrupts so that the global game object does not update midst the copy
 * operation. Re-enables interrupts after the copy is done. */
void game_copy_safely(Game *destination);

#endif // GAME_H


#ifndef GRAPHICS_H
#define GRAPHICS_H

#define GRAPHICS_TANK_COLOR_COUNT 5
#define Pixel_gui_border 35


// Configuration and main render file.
void graphics_build();
void graphics_render();
void graphics_draw(short originy, short originx, uint8_t pixel_count, uint8_t pixel_set[2][pixel_count], short Color);

#endif // GRAPHICS_H

























/** Tank Colors: Red, Green, Blue, Purple, Yellow */
static short Color_tank[GRAPHICS_TANK_COLOR_COUNT] = {
	0xC000, 0x0260, 0x11B4, 0x8010, 0xFEA0};
/** Copy of the game that is drawn. Before each render, the updated game is
 * copied safely by disabling interrupts. */
static Game drawn_copy;
static short Color_map_ground =(short) 0xFFFF;
static short Color_map_background =(short) 0x9E7F;

/** Pixel map that buffers the color data to the screen. */
static short pixel_map[GAME_HEIGHT][GAME_WIDTH];



void graphics_draw_sprite(short originy, short originx, uint8_t pixel_count, uint8_t pixel_set[2][pixel_count], short Color){
for(int i=0;i<pixel_count;i++)
pixel_map[ originy+pixel_set[0][i] ] [ originx+pixel_set[1][i] ]= Color;
}



void graphics_initialize(){	//Initialize the whole screen, Draw all the pixels that doesn't need redrawing.
  int x, y;
	short Color_gui_red =(short) 0xF800;
	short Color_gui_black =(short) 0x0000;
	short Color_gui_grey =(short) 0x7BEF;
	short Color_gui_background =(short) 0xFFFF;

	short Color_gui_cloud_blue =(short) 0x051D;
	short Color_gui_fuel_green =(short) 0x0320;
	short Color_gui_shield_blue =(short) 0x07FF;
	short Color_gui_health_blue =(short) 0x001F;

	for (y=0; y < GAME_HEIGHT; y++) {			//Initialize GUI & screen
		for (x=0; x < GAME_WIDTH; x++) {
			if (y < Pixel_gui_border) 			  //Everything else is white.
				pixel_map[y][x] = Color_gui_background;
			else if (y> Pixel_gui_border)
			pixel_map[y][x]=Color_gui_cloud_blue;
			else                              //Draw the border.
				pixel_map[y][x] = Color_gui_black;
		}
	}

//Define the sprites that won't change and print them on.{{y},{x}}
	uint8_t sprite_fuel_can_black[2][26] ={{0,2,3,4,5,6,7,1,2,7,1,2,4,5,7,1,2,4,6,7,2,3,4,5,6,7},
																		     {0,0,0,0,0,0,0,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4,4}};
  uint8_t sprite_fuel_can_green[2][8]  ={{3,4,5,6,3,6,3,5},
				    													   {1,1,1,1,2,2,3,3}};
  uint8_t sprite_health_black[2][24]   ={{1,2,3,4,5,1,5,0,1,5,1,5,1,5,0,1,5,1,5,1,2,3,4,5},
                                         {0,0,0,0,0,1,1,2,2,2,3,3,4,4,5,5,5,6,6,7,7,7,7,7}};
  uint8_t sprite_health_red[2][5]      ={{3,2,3,4,3},
                                         {4,5,5,5,6}};
  uint8_t sprite_health_blue[2][2]     ={{3,3},
                                         {1,2}};
	uint8_t sprite_shield[2][16]         ={{2,3,4,1,5,0,6,0,6,0,6,1,5,2,3,4},
																	       {0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,6}};
	uint8_t sprite_repair_black[2][19]   ={{1,2,0,2,0,1,2,3,2,3,4,3,4,5,4,5,6,5,6},
																	       {0,0,1,1,2,2,2,2,3,3,3,4,4,4,5,5,5,6,6}};
	uint8_t sprite_repair_grey[2][8]     ={{5,6,4,5,6,5,2,1},
																	       {1,1,2,2,2,3,5,6}};
	uint8_t sprite_parachute_black[2][5] ={{4,5,6,5,4},
																	       {1,2,3,4,5}};
	uint8_t sprite_parachute_grey[2][17] ={{2,3,1,3,0,2,3,0,1,3,0,2,3,1,3,2,3},
																	       {0,0,1,1,2,2,2,3,3,3,4,4,4,5,5,6,6}};
	uint8_t sprite_teleport_black[2][8]  ={{1,2,0,0,1,2,3,4},
																	       {0,0,1,2,3,3,4,4}};
	uint8_t sprite_teleport_red[2][5]    ={{4,6,5,4,6},
																	       {3,3,4,5,5}};
	uint8_t sprite_cloud[2][57]          ={{2,3,4,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,2,3,4,5,2,3,4,5,1,2,3,4,5,0,1,2,3,4,5,0,1,2,3,4,5,0,1,2,3,4,5, 1, 2, 3, 4, 5, 2, 3, 4},
																	       {0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,11,11,11}};
	uint8_t sprite_big_arrow_left[2][25] ={{4,3,4,5,2,3,4,5,6,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,8},
																	       {0,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4}};
	uint8_t sprite_big_arrow_right[2][25]={{0,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,2,3,4,5,6,3,4,5,4},
																		     {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,4}};
	uint8_t sprite_key_f[2][31]          ={{0,1,2,3,4,5,6,0,6,0,6,0,6,0,6,0,6,0,1,2,3,4,5,6,2,3,4,5,2,4,2},
																	       {0,0,0,0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,6,6,2,2,2,2,3,3,4}};
	uint8_t sprite_key_r[2][33]          ={{0,1,2,3,4,5,6,0,6,0,6,0,6,0,6,0,6,0,1,2,3,4,5,6,2,3,4,5,2,4,2,3,5},
																         {0,0,0,0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,6,6,2,2,2,2,3,3,4,4,4}};
	uint8_t sprite_key_p[2][33]          ={{0,1,2,3,4,5,6,0,6,0,6,0,6,0,6,0,6,0,1,2,3,4,5,6,2,3,4,5,2,4,2,3,4},
																	       {0,0,0,0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,6,6,2,2,2,2,3,3,4,4,4}};
	uint8_t sprite_key_t[2][30]          ={{0,1,2,3,4,5,6,0,6,0,6,0,6,0,6,0,6,0,1,2,3,4,5,6,2,2,3,4,5,2},
																  	     {0,0,0,0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,6,6,2,3,3,3,3,4}};

	graphics_draw_sprite(14,8,26,sprite_fuel_can_black, Color_gui_black);
  graphics_draw_sprite(14,8,8,sprite_fuel_can_green,  Color_gui_fuel_green);
	graphics_draw_sprite(25,7,24,sprite_health_black,   Color_gui_black);
	graphics_draw_sprite(25,7,5,sprite_health_red,      Color_gui_red);
	graphics_draw_sprite(25,7,2,sprite_health_blue,     Color_gui_health_blue);
	graphics_draw_sprite(4,42,16,sprite_shield,         Color_gui_shield_blue);
	graphics_draw_sprite(15,37,19,sprite_repair_black,   Color_gui_black);
	graphics_draw_sprite(15,37,8,sprite_repair_grey,     Color_gui_grey);
	graphics_draw_sprite(15,47,5,sprite_parachute_black, Color_gui_black);
	graphics_draw_sprite(15,47,17,sprite_parachute_grey, Color_gui_grey);
	graphics_draw_sprite(15,57,8,sprite_teleport_black,  Color_gui_black);
	graphics_draw_sprite(15,57,5,sprite_teleport_red,    Color_gui_red);
	graphics_draw_sprite(4,120,57,sprite_cloud,          Color_gui_cloud_blue);
	graphics_draw_sprite(13,155,25,sprite_big_arrow_left, Color_gui_black);
	graphics_draw_sprite(13,282,25,sprite_big_arrow_right,Color_gui_black);
	graphics_draw_sprite(4,52,31,sprite_key_f,    Color_gui_black);
	graphics_draw_sprite(24,37,33,sprite_key_r,    Color_gui_black);
	graphics_draw_sprite(24,47,33,sprite_key_p,    Color_gui_black);
	graphics_draw_sprite(24,57,30,sprite_key_t,    Color_gui_black);



}




inline void graphics_update() {
	int x, y;

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
	//graphics_build(); // Updates 320x240 pixel map.

	int pixel_buf_ptr = *(int *) 0xFF203020; //PIXEL_BUF_CTRL_BASE
	int pixel_ptr, x, y;

	//Display
	for (y = 0; y < GAME_HEIGHT; y++)
		for (x = 0; x < GAME_WIDTH; x++) {     // Change to correct pixel's address.
			pixel_ptr = pixel_buf_ptr + (y << 10) + (x << 1);
						*(short *)pixel_ptr = pixel_map[y][x]; // Set pixel color.
		}
}




/** Configures all the subsystems at the start. */
static config_all(void) {
	graphics_initialize();
}

/** Starts the program. */
void main() {
	config_all();

	while (true) {
		graphics_render();
	}
}
