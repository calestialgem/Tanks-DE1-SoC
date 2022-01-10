#ifndef GAME_H
#define GAME_H

#include "Graphics.h"

#include <stdbool.h>
#include <stdint.h>

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
