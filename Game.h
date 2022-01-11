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
typedef struct Vector {
	/** First component. */
	float x;
	/** Second component. */
	float y;
};
/** Projectiles that do damage. */
typedef struct Bullet {
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
};
/** Array of bullets. */
typedef struct Bullets {
	/** Array of bullets with a set capacity. */
	Bullet array[GAME_BULLET_CAPACITY];
	/** Amount of bullets in the array. */
	uint8_t size;
};
/** Weapon on top of a tank. */
typedef struct Barrel {
	/** The angle with respect to the tank in rad. */
	float angle;
	/** The amount bullets are shot forward percent. */
	uint8_t power;
};
/** Characters controlled by players. */
typedef struct Tank {
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
};
/** Array of tanks. */
typedef struct Tanks {
	/** Array of tanks with a set capacity. */
	Tank array[GAME_TANK_CAPACITY];
	/** Amount of tanks in the array. */
	uint8_t size;
};
/** Terrain where tanks are on. */
typedef struct Map {
	/** Heights of the ground at different horizontal positions in m. */
	float ground[STANDARD_X];
};
/** Game state. */
typedef struct Game {
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
};

/** Adds a tank to the array. */
void game_add_tank(Tanks *tanks, char const name[GAME_TANK_CAPACITY]);
/** Removes the tank at the given index from the array. */
void game_remove_tank(Tanks *tanks, int index);
/** Restarts the game. */
void game_restart(Game *game);
/** Updates the given game. */
void game_update(Game *game);

#endif // GAME_H
