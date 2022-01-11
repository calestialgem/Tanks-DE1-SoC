
#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#define STANDARD_X 320
#define STANDARD_Y 240

#define GAME_GRAVITY 9.81F

#define GAME_BULLET_CAPACITY 8
#define GAME_BULLET_DENSITY 2500.0F

#define GAME_TANK_CAPACITY 4
#define GAME_TANK_INITIAL_HEALTH 100.0F
#define GAME_TANK_NAME_CAPACITY 32

/** Vector of two floats. */
union vector {
	/** Componenets as an array. */
	float c[2];
	struct {
		/** First component. */
		float x;
		/** Second component. */
		float y;
	};
};
/** Projectiles that do damage. */
struct bullet {
	/** Position of the center of mass in m. */
	union vector position;
	/** Velocity of the center of mass in m/s. */
	union vector velocity;
	/** Diameter in m. */
	float diameter;
	/** Total mass in kg. */
	float mass;
	/** Power released when it explodes. */
	float power;
};
/** Array of bullets. */
struct bullets {
	/** Array of bullets with a set capacity. */
	struct bullet array[GAME_BULLET_CAPACITY];
	/** Amount of bullets in the array. */
	uint8_t size;
};
/** Weapon on top of a tank. */
struct gun {
	/** The angle with respect to the tank in rad. */
	float angle;
	/** The amount bullets are shot forward percent. */
	uint8_t power;
};
/** Characters controlled by players. */
struct tank {
	/** Position of the middle horizontally and down vertically in m. */
	union vector position;
	/** The angle the tank is standing with respect to the ground in rad. */
	float tilt;
	/** Remaining healt of the tank in percents. */
	uint8_t health;
	/** Whether the tank is alive or not. */
	bool alive;
	/** Weapon. */
	struct gun gun;
	/** Player's name. */
	char name[GAME_TANK_NAME_CAPACITY];
};
/** Array of tanks. */
struct tanks {
	/** Array of tanks with a set capacity. */
	struct tank array[GAME_TANK_CAPACITY];
	/** Amount of tanks in the array. */
	uint8_t size;
};
/** Terrain where tanks are on. */
struct map {
	/** Heights of the ground at different horizontal positions in m. */
	float ground[STANDARD_X];
};
/** Game state. */
struct game {
	/** Remaining tanks. */
	struct tanks tanks;
	/** Active bullets. */
	struct bullets bullets;
	/** Map. */
	struct map map;
	/** Whether the game is running. */
	bool playing;
	/** The index of the tank that can do actions. */
	uint8_t turn;
	/** Wheter the current tank is shooting. */
	bool shooting;
};

/** Adds a tank to the array. */
void game_add_tank(struct tanks *tanks, char const name[GAME_TANK_CAPACITY]);
/** Removes the tank at the given index from the array. */
void game_remove_tank(struct tanks *tanks, int index);
/** Restarts the game. */
void game_restart(struct game *game);
/** Updates the given game. */
void game_update(struct game *game);

#endif // GAME_H



#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Game.h"


#define PIXEL_BUF_CTRL_BASE   0xFF203020


//RGB to hex color converter: https://www.rapidtables.com/convert/color/rgb-to-hex.html
//Color format= 5 bit red, 6 bit green, 5 bit blue = 16 bits.
//Tank colors
#define Color_tank_red 0xC000
#define Color_tank_green 0x0260
#define Color_tank_blue 0x11B4
#define Color_tank_purple 0x8010
#define Color_tank_yellow 0xFEA0


//GUI Colors
#define Color_gui_red 0xF800
#define Color_gui_black 0x0000
#define Color_gui_grey 0x7BEF
#define Color_gui_cloud_blue 0x051D
#define Color_gui_fuel_green 0x0320
#define Color_gui_shield_blue 0x07FF
#define Color_gui_ground 0xFFFF
#define Color_gui_background 0x9E7F


// Configuration and main render file.
void graphics_build(short pixel_map[STANDARD_Y][STANDARD_X], struct game const* game_data);
void graphics_render(struct game const* game_data);


#ifdef GRAPHICS_INTERRUPT_ID
/** Handles the graphics interrupts. Called by the interrupt handler. */
void graphics_isr(void);
#endif // GRAPHICS_INTERRUPT_ID


#endif // GRAPHICS_H



void graphics_build(short pixel_map[STANDARD_Y][STANDARD_X], struct game const* game_data){
  int x,y;

  for (y=0; y<=STANDARD_Y; y++){
    for (x = 0; x <= STANDARD_X; ++x){
      if (y>=game_data.map.ground[x]) pixel_map[y][x]=Color_gui_ground; //Paint the ground
      else                            pixel_map[y][x]=Color_gui_background; //Paint the background, alternatively have a 320x240 background and take all the pixels from there for a picture.


    }
  }

}




void graphics_render(struct game const* game_data) { //game_data'yı doğru mu alıyorum?

    short pixel_map[STANDARD_Y][STANDARD_X];
    graphics_build(pixel_map,game_data);   //should return 320x240 pixel map.
    int pixel_buf_ptr = *(int *)PIXEL_BUF_CTRL_BASE;
    int pixel_ptr, x, y;

    /* assume that the box coordinates are valid */
    for (y = 0; y <= STANDARD_Y; y++)
        for (x = 0; x <= STANDARD_X; ++x) {
            pixel_ptr = pixel_buf_ptr + (y << 10) + (x << 1); //Change to correct pixel's address
            *(short *)pixel_ptr = pixel_map[y][x];            //Set pixel color
        }

}




/** Draws continuously. */
static render(struct game *const game_data) {
	while (true)
		graphics_render(&game_data);
}
/** Starts the program. */
void main() {
	struct game current = {.tanks = {.size = 0},
		.bullets = {.size = 0},
		.map = {},
		.playing = false,
		.turn = 0,
		.shooting = false};
	render(&current);
}
