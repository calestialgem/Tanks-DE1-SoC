#include "Barrel.h"

#include "Timer.h"

#define SPEED (0.1F * TIMER_STEP)
#define INITIAL_ANGLE MATH_PI_4
#define LOWER_ANGLE 0.0F
#define UPPER_ANGLE (MATH_PI_2 - LOWER_ANGLE)
#define MIN_POWER 1

void barrel_init(volatile Barrel *const barrel, uint8_t const power) {
	barrel->angle = INITIAL_ANGLE;
	barrel->power = power;
}
void barrel_rotate(volatile Barrel *const barrel, int8_t const rotation) {
	if (!rotation) {
		return;
	}
	barrel->angle += rotation * SPEED;
	if (barrel->angle < LOWER_ANGLE) {
		barrel->angle = LOWER_ANGLE;
	} else if (barrel->angle > UPPER_ANGLE) {
		barrel->angle = UPPER_ANGLE;
	}
}
void barrel_change_power(
	volatile Barrel *const barrel, int8_t const change, uint8_t const max) {
	if (!change) {
		return;
	}
	int16_t const newPower = (int16_t)barrel->power + change;
	if (newPower < MIN_POWER) {
		barrel->power = MIN_POWER;
	} else if (newPower > max) {
		barrel->power = max;
	} else {
		barrel->power = newPower;
	}
}
