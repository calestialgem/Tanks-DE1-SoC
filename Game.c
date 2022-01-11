#include "Game.h"

#include "Error.h"
#include "Graphics.h"
#include "Timer.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

void game_add_tank(
	Tanks *const tanks, char const name[GAME_TANK_NAME_CAPACITY]) {
	if (tanks->size == GAME_TANK_CAPACITY) {
		error_show(ERROR_LOGIC);
		return;
	}
	Tank *const tank = &tanks->array[tanks->size++];
	strncpy(tank->name, name, GAME_TANK_NAME_CAPACITY);
}
void game_remove_tank(Tanks *const tanks, size_t const index) {
	if (tanks->size <= index) {
		error_show(ERROR_LOGIC);
		return;
	}
	Tank const last = tanks->array[--tanks->size];
	Tank *const removed = &tanks->array[index];
	*removed = last;
}
static inline float change_span(float const number,
	float const currentMin,
	float const currentMax,
	float const targetMin,
	float const targetMax) {
	return (number - currentMin) / (currentMax - currentMin) *
		       (targetMax - targetMin) +
	       targetMin;
}
static inline float span_random(float const min, float const max) {
	return change_span(rand(), 0, RAND_MAX, min, max);
}
static inline float span_sinus(
	float const x, float const min, float const max) {
	return change_span(sin(x), -1.0F, 1.0F, min, max);
}
static inline void generate_map(Map *const map) {
	float const peakCount = span_random(0.75F, 1.5F);
	float const start = span_random(0.0F, 2.0F * M_PI);
	float const peakHeight = span_random(0.5F, 0.67F) * GAME_HEIGHT;
	float const valleyHeight = span_random(0.8F, 0.95F) * GAME_HEIGHT;
	size_t x;
	for (x = 0; x < GAME_WIDTH; x++) {
		float const angle =
			change_span(x, 0.0F, GAME_WIDTH, 0.0F, 2.0F * M_PI);
		map->ground[x] = span_sinus(
			angle * peakCount + start, peakHeight, valleyHeight);
	}
}
static inline void update_tank(Tank *const tank, Map const map) {
	size_t const index = floor(tank->position.x);
	tank->position.y = map.ground[index];
	size_t const previousIndex = index == 0 ? index : index - 1;
	size_t const nextIndex = index == GAME_WIDTH - 1 ? index : index + 1;
	float const previousHeight = map.ground[previousIndex];
	float const nextHeight = map.ground[nextIndex];
	float const heightChange = previousHeight - nextHeight;
	int const steps = nextIndex - previousIndex;
	float const stepWidth = steps;
	float const slope = heightChange / stepWidth;
	tank->tilt = atan(slope);
}
static inline void place_tank(Tank *const tank, Map const map) {
	float const min = GAME_WIDTH * 0.1F;
	float const max = GAME_WIDTH * 0.9F;
	tank->position.x = get_random(min, max);
	update_tank(tank, map);
}
static inline void reset_tanks(Tanks *const tanks, Map const map) {
	size_t i;
	for (i = 0; i < tanks->size; i++) {
		Tank *const tank = &tanks->array[i];
		place_tank(tank, map);
		tank->gun.angle = M_PI_2;
		tank->health = GAME_TANK_INITIAL_HEALTH;
		tank->alive = true;
	}
}
void game_restart(Game *const game) {
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
static inline Vector add_vectors(Vector const left, Vector const right) {
	Vector const result = {.x = left.x + right.x, .y = left.y + right.y};
	return result;
}
static inline Vector multiply_vector(Vector const left, float const right) {
	Vector const result = {.x = left.x * right, .y = left.y * right};
	return result;
}
static inline void simulate_bullet(Bullet *const bullet) {
	Vector const acceleration = {.x = 0.0F, .y = GAME_GRAVITY};
	Vector const velocityEffect =
		multiply_vector(bullet->velocity, TIMER_STEP);
	Vector const accelerationEffect =
		multiply_vector(acceleration, powf(TIMER_STEP, 2.0F) / 2.0F);
	Vector const positionChange =
		add_vectors(velocityEffect, accelerationEffect);
	bullet->position = add_vectors(bullet->position, positionChange);
	Vector const velocityChange = multiply_vector(acceleration, TIMER_STEP);
	bullet->velocity = add_vectors(bullet->velocity, velocityChange);
}
static inline void next_turn(Game *const game) {
	if (++game->turn >= game->tanks.size) {
		game->turn = 0;
	}
}
void game_update(Game *game) {
	if (game->shooting) {
	} else {
	}
}
