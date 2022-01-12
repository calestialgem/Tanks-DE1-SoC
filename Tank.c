#include "Tank.h"

#include "Error.h"
#include "Game.h"
#include "Map.h"

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
		error_show(ERROR_LOGIC_REACHED_TANK_CAPACITY);
		return;
	}
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.tanks.size++];
	copy_name(tank->name, name);
	tank->color = color;
}
void tank_remove(size_t const index) {
	if (game_instance.tanks.size <= index) {
		error_show(ERROR_LOGIC_NO_TANK_TO_REMOVE);
		return;
	}
	Tank const last = game_instance.tanks.array[--game_instance.tanks.size];
	volatile Tank *const removed = &game_instance.tanks.array[index];
	*removed = last;
}
void tank_place(volatile Tank *const tank, float const position) {
	tank->fuel -=
		fabsf(tank->position.x - position) * TANK_FUEL_CONSUMPTION;
	if (tank->fuel < 0.0F) {
		tank->fuel = 0.0F;
	}
	tank->position.x = position;
	size_t const index = floorf(tank->position.x);
	tank->position.y = game_instance.map.ground[index];
	tank->tilt = atanf(map_slope(index));
}
void tank_move(volatile Tank *const tank, int8_t const movement) {
	if (!movement || tank->fuel <= 0.0F) {
		return;
	}
	float const position = tank->position.x + movement * TANK_SPEED;
	if (position < MAP_LEFT_BORDER) {
		tank_place(tank, MAP_LEFT_BORDER);
	} else if (position > MAP_RIGHT_BORDER) {
		tank_place(tank, MAP_RIGHT_BORDER);
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
