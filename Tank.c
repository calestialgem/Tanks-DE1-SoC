#include "Tank.h"

#include "Error.h"
#include "Game.h"

#include <math.h>

#define TANK_INITIAL_HEALTH 100
#define TANK_INITIAL_FUEL 250
#define TANK_FUEL_CONSUMPTION (2.5F * TIMER_STEP)
#define TANK_SPEED (0.1F * TIMER_STEP)

static inline void copy_name(volatile char destination[TANK_NAME_CAPACITY],
	char const *const source) {
	size_t i;
	for (i = 0; i < TANK_NAME_CAPACITY; i++) {
		if (!(destination[i] = source[i])) {
			return;
		}
	}
}
void tank_add(char const *const name, size_t const color) {
	if (game_instance.tanks.size == TANK_CAPACITY) {
		error_show(ERROR_LOGIC);
		return;
	}
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.tanks.size++];
	copy_name(tank->name, name);
	tank->color = color;
}
void tank_remove(size_t const index) {
	if (game_instance.tanks.size <= index) {
		error_show(ERROR_LOGIC);
		return;
	}
	Tank const last = game_instance.tanks.array[--game_instance.tanks.size];
	volatile Tank *const removed = &game_instance.tanks.array[index];
	*removed = last;
}
void tank_place(volatile Tank *const tank, float const position) {
	tank->fuel -=
		fabsf(tank->position.x - position) * TANK_FUEL_CONSUMPTION;
	tank->position.x = position;
	size_t const index = floorf(tank->position.x);
	tank->position.y = game_instance.map.ground[index];
	size_t const previousIndex = index == 0 ? index : index - 1;
	size_t const nextIndex = index == GAME_WIDTH - 1 ? index : index + 1;
	float const previousHeight = game_instance.map.ground[previousIndex];
	float const nextHeight = game_instance.map.ground[nextIndex];
	float const heightChange = previousHeight - nextHeight;
	int const steps = nextIndex - previousIndex;
	float const stepWidth = steps;
	float const slope = heightChange / stepWidth;
	tank->tilt = atanf(slope);
}
void tank_move(volatile Tank *const tank, int8_t const movement) {
	if (!movement) {
		return;
	}
	float const position = tank->position.x + movement * TANK_SPEED;
	if (position < GAME_MAP_LEFT_BORDER) {
		tank_place(tank, GAME_MAP_LEFT_BORDER);
	} else if (position > GAME_MAP_RIGHT_BORDER) {
		tank_place(tank, GAME_MAP_RIGHT_BORDER);
	} else {
		tank_place(tank, position);
	}
}
void tank_init(volatile Tank *const tank, float const position) {
	tank_place(tank, position);
	tank->health = TANK_INITIAL_HEALTH;
	tank->alive = true;
	tank->fuel = TANK_INITIAL_FUEL;
}
