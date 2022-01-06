#include "Game.h"

#include "Error.h"
#include "Graphics.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

void game_add_tank(Tanks *tanks, char const name[GAME_TANK_NAME_CAPACITY]) {
	if (tanks->size == GAME_TANK_CAPACITY) {
		error_show(ERROR_LOGIC);
		return;
	}
	Tank *const tank = &tanks->array[tanks->size++];
	strncpy(tank->name, name, GAME_TANK_NAME_CAPACITY);
}
void game_remove_tank(Tanks *tanks, int index) {
	if (tanks->size <= index) {
		error_show(ERROR_LOGIC);
		return;
	}
	Tank *const last = &tanks->array[--tanks->size];
	Tank *const removed = &tanks->array[index];
	removed->position = last->position;
	removed->tilt = last->tilt;
	removed->gunAngle = last->gunAngle;
	removed->health = last->health;
	strncpy(removed->name, last->name, GAME_TANK_NAME_CAPACITY);
}
static inline float get_random(float const lowerLimit, float const upperLimit) {
	float const randomLimit = RAND_MAX;
	float const normalized = rand() / randomLimit;
	float const span = upperLimit - lowerLimit;
	return normalized * span + lowerLimit;
}
static inline void generate_map(Map *map) {
	for (int i = 0; i < GAME_MAP_SIZE; i++) {
		float const angleMultiplier = 0.1F;
		float const angle = i * angleMultiplier;
		float const sinusoidal = sin(angle);
		float const random = get_random(0.5F, 1.0F);
		float const heightMultiplier = GAME_HEIGHT / 2.0;
		map->ground[i] = heightMultiplier * sinusoidal * random;
	}
}
static inline void move_tank(
	Map const *const map, Tank *const tank, float const x) {
	int const index = floor(x / GAME_MAP_STEP_SIZE);
	float const height = map->ground[index];
	tank->position.y = height;
	int const previousIndex = index == 0 ? index : index - 1;
	int const nextIndex = index == GAME_MAP_SIZE - 1 ? index : index + 1;
	float const previousHeight = map->ground[previousIndex];
	float const nextHeight = map->ground[nextIndex];
	float const verticalChange = previousHeight - nextHeight;
	int const steps = nextIndex - previousIndex;
	float const horizontalChange = steps * GAME_MAP_STEP_SIZE;
	float const tangent = verticalChange / horizontalChange;
	tank->tilt = atan(tangent);
}
static inline void place_tank(Map const *const map, Tank *const tank) {
	float const lowerLimit = GAME_WIDTH * 0.1F;
	float const upperLimit = GAME_WIDTH * 0.9F;
	move_tank(map, tank, get_random(lowerLimit, upperLimit));
}
void game_restart(Game *game) {
	if (game->playing) {
		error_show(ERROR_LOGIC);
		return;
	}
	game->playing = true;
	game->turn = 0;
	// Change the seed.
	srand(time(0));
	generate_map(&game->map);
	game->bullets.size = 0;
	for (int i = 0; i < game->tanks.size; i++) {
		Tank *const tank = &game->tanks.array[i];
		place_tank(&game->map, tank);
		tank->gunAngle = GAME_TANK_GUN_ANGLE_LIMIT;
		tank->health = GAME_TANK_INITIAL_HEALTH;
		tank->alive = true;
	}
}
static inline void next_turn(Game *game) {
	if (++game->turn >= game->tanks.size) {
		game->turn = 0;
	}
}
void game_update(Game *game) {}
