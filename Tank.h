#ifndef TANK_H
#define TANK_H

#include "Barrel.h"
#include "Vector.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define TANK_CAPACITY 4
#define TANK_NAME_CAPACITY 32

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
	char name[TANK_NAME_CAPACITY];
	/** Index of the player's color. */
	size_t color;
} Tank;
/** Array of tanks. */
typedef struct {
	/** Array of tanks with a set capacity. */
	Tank array[TANK_CAPACITY];
	/** Amount of tanks in the array. */
	size_t size;
} Tanks;

/** Adds a tank to the array. */
void tank_add(char const *const name, size_t const color);
/** Removes the tank at the given index from the array. */
void tank_remove(size_t const index);
/** Moves the tank to the given position. */
void tank_place(volatile Tank *const tank, float const position);
/** Moves the tank by the given tick amount. */
void tank_move(volatile Tank *const tank, int8_t const movement);
/** Initializes the tank. */
void tank_init(volatile Tank *const tank, float const position);

#endif // TANK_H
