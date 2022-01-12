#ifndef GAME_H
#define GAME_H

#include "Barrel.h"
#include "Bullet.h"
#include "MathTools.h"
#include "Timer.h"
#include "Vector.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define GAME_WIDTH 320
#define GAME_HEIGHT 240

#define GAME_TANK_CAPACITY 4
#define GAME_TANK_INITIAL_HEALTH 100
#define GAME_TANK_INITIAL_FUEL 250
#define GAME_TANK_FUEL_CONSUMPTION (2.5F * TIMER_STEP)
#define GAME_TANK_NAME_CAPACITY 32
#define GAME_TANK_SPEED (0.1F * TIMER_STEP)

#define GAME_MAP_LEFT_BORDER 5
#define GAME_MAP_RIGHT_BORDER (GAME_WIDTH - GAME_MAP_LEFT_BORDER)

/** Characters controlled by players. */
typedef struct {
	/** Position of the middle horizontally and down vertically in m. */
	Vector position;
	/** The counter-clockwise angle the tank is standing with respect to the
	 * ground in rad. */
	float tilt;
	/** Remaining healt of the tank in percents. */
	uint8_t health;
	/** Whether the tank is alive or not. */
	bool alive;
	/** Remaining fuel of the tank in m^3. */
	uint8_t fuel;
	/** Weapon. */
	Barrel gun;
	/** Player's name. */
	char name[GAME_TANK_NAME_CAPACITY];
	/** Index of the player's color. */
	size_t color;
} Tank;
/** Array of tanks. */
typedef struct {
	/** Array of tanks with a set capacity. */
	Tank array[GAME_TANK_CAPACITY];
	/** Amount of tanks in the array. */
	size_t size;
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
	size_t turn;
	/** Wheter the game is waiting for all the bullets to explode. */
	bool waitingBullets;
} Game;

/** Adds a tank to the array. */
void game_add_tank(char const *const name, size_t const color);
/** Removes the tank at the given index from the array. */
void game_remove_tank(size_t const index);
/** Restarts the game. */
void game_restart();
/** Updates the game. */
void game_update();
/** Copies the global game object to the given one. Does this after disabling
 * interrupts so that the global game object does not update midst the copy
 * operation. Re-enables interrupts after the copy is done. */
void game_copy_safely(Game *const destination);

#endif // GAME_H
