#ifndef BULLET_H
#define BULLET_H

#include "Vector.h"

#include <stdlib.h>

#define BULLET_CAPACITY 8
#define BULLET_GRAVITY 9.81F
#define BULLET_RADIUS_MULTIPLIER 0.1F
#define BULLET_POWER_MULTIPLIER 0.2F
#define BULLET_SPEED_MULTIPLIER (20.0F * TIMER_STEP)
#define BULLET_DAMAGE_MULTIPLIER 0.2F

/** Projectiles that do damage. */
typedef struct {
	/** Position of the center of mass in m. */
	Vector position;
	/** Velocity of the center of mass in m/s. */
	Vector velocity;
	/** Radius in m. */
	float radius;
	/** Explosion radius in m. */
	float power;
} Bullet;
/** Array of bullets. */
typedef struct {
	/** Array of bullets with a set capacity. */
	Bullet array[BULLET_CAPACITY];
	/** Amount of bullets in the array. */
	size_t size;
} Bullets;

void bullet_move(volatile Bullet *const bullet);

#endif // BULLET_H
