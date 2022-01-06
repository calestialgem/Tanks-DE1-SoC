#ifndef GAME_H
#define GAME_H

#include "Graphics.h"

#include <stdbool.h>
#include <stdint.h>

#define GAME_GRAVITY 9.81F
#define GAME_PIXEL_SCALE 100.0F
#define GAME_WIDTH (GRAPHICS_WIDTH * GAME_PIXEL_SCALE)
#define GAME_HEIGHT (GRAPHICS_HEIGHT * GAME_PIXEL_SCALE)

#define GAME_BULLET_CAPACITY 8
#define GAME_BULLET_DENSITY 2500.0F

#define GAME_TANK_CAPACITY 4
#define GAME_TANK_INITIAL_HEALTH 100.0F
#define GAME_TANK_GUN_ANGLE_LIMIT 15.0F
#define GAME_TANK_NAME_CAPACITY 32

#define GAME_MAP_SIZE 128
#define GAME_MAP_STEP_SIZE (GAME_WIDTH / GAME_MAP_SIZE)

/** Vector of two floats. */
typedef union {
	/** Componenets as an array. */
	float c[2];
	struct {
		/** First component. */
		float x;
		/** Second component. */
		float y;
	};
} Vector;
/** Projectiles that do damage. */
typedef struct {
	/** Position of the center of mass m. */
	Vector position;
	/** Velocity of the center of mass in m/s. */
	Vector velocity;
	/** Diameter in m. */
	float diameter;
	/** Mass in kg. */
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
/** Characters controlled by players. */
typedef struct {
	/** Position of the middle horizontally and down vertically in m. */
	Vector position;
	/** The angle the tank is standing with respect to the ground in rad. */
	float tilt;
	/** The angle of the tank's gun with respect to the tank in rad. */
	float gunAngle;
	/** Remaining healt of the tank. */
	float health;
	/** Whether the tank is alive or not. */
	bool alive;
	/** Name. */
	char name[GAME_TANK_NAME_CAPACITY];
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
	float ground[GAME_MAP_SIZE];
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
} Game;

/** Adds a tank to the array. */
void game_add_tank(Tanks *tanks, char const name[GAME_TANK_CAPACITY]);
/** Removes the tank at the given index from the array. */
void game_remove_tank(Tanks *tanks, int index);
/** Restarts the game. */
void game_restart(Game *game);
/** Updates the given game. */
void game_update(Game *game);

#endif // GAME_H
