#ifndef BARREL_H
#define BARREL_H

#include "MathTools.h"

#include <stdint.h>

/** Weapon on top of a tank. */
typedef struct {
	/** The counter-clockwise angle with respect to the tank in rad. */
	float angle;
	/** The amount bullets are shot forward percent. */
	uint8_t power;
} Barrel;

/** Initializes the barrel. */
void barrel_init(volatile Barrel *const barrel, uint8_t const power);
/** Rotates the barrel in counter-clockwise with the given amount of ticks. */
void barrel_rotate(volatile Barrel *const barrel, int8_t const rotation);
/** Changes the barrel's power with the given amount of percentage. */
void barrel_change_power(
	volatile Barrel *const barrel, int8_t const change, uint8_t const max);

#endif // BARREL_H
