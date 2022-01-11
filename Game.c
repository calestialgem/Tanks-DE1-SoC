#include "Game.h"

#include "Error.h"
#include "Graphics.h"
#include "Timer.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

void game_add_tank(
	struct tanks *const tanks, char const name[GAME_TANK_NAME_CAPACITY]) {
	if (tanks->size == GAME_TANK_CAPACITY) {
		error_show(ERROR_LOGIC);
		return;
	}
	struct tank *const tank = &tanks->array[tanks->size++];
	strncpy(tank->name, name, GAME_TANK_NAME_CAPACITY);
}
void game_remove_tank(struct tanks *const tanks, int const index) {
	if (tanks->size <= index) {
		error_show(ERROR_LOGIC);
		return;
	}
	struct tank const last = tanks->array[--tanks->size];
	struct tank *const removed = &tanks->array[index];
	*removed = last;
}
static inline float get_random(float const lowerLimit, float const upperLimit) {
	float const randomLimit = RAND_MAX;
	float const normalized = rand() / randomLimit;
	float const span = upperLimit - lowerLimit;
	return normalized * span + lowerLimit;
}
static inline void generate_map(struct map *const map) {
	float change = get_random(-0.01F, 0.01F);
	float height = get_random(0.5F, 1.0F);
	for (int i = 0; i < STANDARD_X; i++) {
		float const scale = STANDARD_Y / 2.0F;
		map->ground[i] = scale * height;
		height += change;
		float const changeOfChange = 0.001F;
		change += get_random(-changeOfChange, changeOfChange);
	}
}
static inline void update_tank(struct tank *const tank, struct map const map) {
	int const index = floor(tank->position.x);
	tank->position.y = map.ground[index];
	int const previousIndex = index == 0 ? index : index - 1;
	int const nextIndex = index == STANDARD_X - 1 ? index : index + 1;
	float const previousHeight = map.ground[previousIndex];
	float const nextHeight = map.ground[nextIndex];
	float const heightChange = previousHeight - nextHeight;
	int const steps = nextIndex - previousIndex;
	float const stepWidth = steps;
	float const slope = heightChange / stepWidth;
	tank->tilt = atan(slope);
}
static inline void place_tank(struct tank *const tank, struct map const map) {
	float const lowerLimit = STANDARD_X * 0.1F;
	float const upperLimit = STANDARD_X * 0.9F;
	tank->position.x = get_random(lowerLimit, upperLimit);
	update_tank(tank, map);
}
static inline void reset_tanks(
	struct tanks *const tanks, struct map const map) {
	for (int i = 0; i < tanks->size; i++) {
		struct tank *const tank = &tanks->array[i];
		place_tank(tank, map);
		tank->gun.angle = M_PI_2;
		tank->health = GAME_TANK_INITIAL_HEALTH;
		tank->alive = true;
	}
}
void game_restart(struct game *const game) {
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
	reset_tanks(&game->tanks, game->map);
}
static inline union vector add_vectors(
	union vector const left, union vector const right) {
	union vector const result = {
		.x = left.x + right.x, .y = left.y + right.y};
	return result;
}
static inline union vector multiply_vector(
	union vector const left, float const right) {
	union vector const result = {.x = left.x * right, .y = left.y * right};
	return result;
}
static inline void simulate_bullet(struct bullet *const bullet) {
	union vector const acceleration = {.x = 0.0F, .y = GAME_GRAVITY};
	union vector const velocityEffect =
		multiply_vector(bullet->velocity, TIMER_STEP);
	union vector const accelerationEffect =
		multiply_vector(acceleration, powf(TIMER_STEP, 2.0F) / 2.0F);
	union vector const positionChange =
		add_vectors(velocityEffect, accelerationEffect);
	bullet->position = add_vectors(bullet->position, positionChange);
	union vector const velocityChange =
		multiply_vector(acceleration, TIMER_STEP);
	bullet->velocity = add_vectors(bullet->velocity, velocityChange);
}
static inline void next_turn(struct game *const game) {
	if (++game->turn >= game->tanks.size) {
		game->turn = 0;
	}
}
void game_update(struct game *game) {
	if (game->shooting) {
	} else {
	}
}
