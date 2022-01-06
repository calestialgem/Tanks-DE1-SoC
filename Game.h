#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define GAME_GRAVITY 9.81
#define GAME_PIXEL_SCALE 100

#define GAME_BULLET_CAPACITY 8
#define GAME_BULLET_DENSITY 2500

#define GAME_TANK_CAPACITY 4
#define GAME_TANK_INITIAL_HEALTH 100
#define GAME_TANK_GUN_ANGLE_LIMIT 15
#define GAME_TANK_NAME_CAPACITY 32

#define GAME_MAP_SIZE 128

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
	/** Name. */
	char const name[GAME_TANK_NAME_CAPACITY];
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
} Game;

void game_create(Game *game);
void game_update(Game *game);

#endif // GAME_H
