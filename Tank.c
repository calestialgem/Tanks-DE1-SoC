#include "Tank.h"

#include "Audio.h"
#include "Error.h"
#include "Game.h"
#include "Map.h"

#define TANK_INITIAL_HEALTH 100
#define TANK_SPEED (5.0F * TIMER_STEP)

void tank_add(size_t const color) {
	if (game_instance.tanks.size == TANK_CAPACITY) {
		error_show(ERROR_LOGIC_REACHED_TANK_CAPACITY);
		return;
	}
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.tanks.size++];
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
void tank_update_map_position(volatile Tank *const tank) {
	size_t const index = math_floor(tank->position.x);
	tank->position.y = game_instance.map.ground[index];
	tank->tilt = math_atan(map_slope(index));
}
void tank_place(volatile Tank *const tank, float const position) {
	tank->fuel -=
		math_abs(tank->position.x - position);
	if (tank->fuel < 0.0F) {
		tank->fuel = 0.0F;
	}
	tank->position.x = position;
	tank_update_map_position(tank);
}
void tank_move(volatile Tank *const tank, int8_t const movement) {
	if (!movement || tank->fuel <= 0.0F) {
		return;
	}
	audio_play_tank_movement();
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
