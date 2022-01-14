#ifndef BULLET_H
#define BULLET_H

#include "Vector.h"

#include <stdbool.h>
#include <stdlib.h>

#define BULLET_CAPACITY 8

/** Projectiles that do damage. */
typedef struct {
	/** Position of the center of mass in pixel. */
	Vector position;
	/** Velocity of the center of mass in pixel/s. */
	Vector velocity;
} Bullet;
/** Array of bullets. */
typedef struct {
	/** Array of bullets with a set capacity. */
	Bullet array[BULLET_CAPACITY];
	/** Amount of bullets in the array. */
	size_t size;
} Bullets;

/** Initializes the bullet using the properties of the current tank. */
void bullet_init(volatile Bullet *const bullet);
/** Moves the bullet using Runge-Kutta Method. */
void bullet_move(volatile Bullet *const bullet);
/** Returns whether the bullet is under the ground or not. */
bool bullet_contact(volatile Bullet const *const bullet);
/** Applies bullet damage to the ground and the tanks. */
void bullet_explode(volatile Bullet const *const bullet);

#endif // BULLET_H
