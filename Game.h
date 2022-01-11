#ifndef GAME_H
#define GAME_H

#include "Math.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define GAME_WIDTH 320
#define GAME_HEIGHT 240

#define GAME_GRAVITY 9.81F

#define GAME_BULLET_CAPACITY 8
#define GAME_BULLET_DENSITY 2500.0F

#define GAME_TANK_CAPACITY 4
#define GAME_TANK_INITIAL_HEALTH 100.0F
#define GAME_TANK_NAME_CAPACITY 32

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
	size_t size;
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
	/** Wheter the current tank is shooting. */
	bool shooting;
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
