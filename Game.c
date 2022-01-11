#include "Game.h"

#include "Error.h"
#include "Graphics.h"
#include "Timer.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

/** The game which is currently running. Volatile because it is accessed by both
 * the main loop in rendering and the timer interrupt in updating. */
volatile Game game_instance;

void game_add_tank(char const *const name, uint8_t const color) {
	if (game_instance.tanks.size == GAME_TANK_CAPACITY) {
		error_show(ERROR_LOGIC);
		return;
	}
	Tank *const tank =
		&game_instance.tanks.array[game_instance.tanks.size++];
	strncpy(tank->name, name, GAME_TANK_NAME_CAPACITY);
	tank->color = color;
}
void game_remove_tank(uint8_t const index) {
	if (game_instance.tanks.size <= index) {
		error_show(ERROR_LOGIC);
		return;
	}
	Tank const last = game_instance.tanks.array[--game_instance.tanks.size];
	Tank *const removed = &game_instance.tanks.array[index];
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
static inline void generate_map() {
	float const peakCount = span_random(0.75F, 1.5F);
	float const start = span_random(0.0F, 2.0F * M_PI);
	float const peakHeight = span_random(0.5F, 0.67F) * GAME_HEIGHT;
	float const valleyHeight = span_random(0.8F, 0.95F) * GAME_HEIGHT;
	uint8_t x;
	for (x = 0; x < GAME_WIDTH; x++) {
		float const angle =
			change_span(x, 0.0F, GAME_WIDTH, 0.0F, 2.0F * M_PI);
		game_instance.map.ground[x] = span_sinus(
			angle * peakCount + start, peakHeight, valleyHeight);
	}
}
static inline void update_tank(Tank *const tank) {
	uint8_t const index = floor(tank->position.x);
	tank->position.y = game_instance.map.ground[index];
	uint8_t const previousIndex = index == 0 ? index : index - 1;
	uint8_t const nextIndex = index == GAME_WIDTH - 1 ? index : index + 1;
	float const previousHeight = game_instance.map.ground[previousIndex];
	float const nextHeight = game_instance.map.ground[nextIndex];
	float const heightChange = previousHeight - nextHeight;
	int const steps = nextIndex - previousIndex;
	float const stepWidth = steps;
	float const slope = heightChange / stepWidth;
	tank->tilt = atan(slope);
}
static inline void place_tank(Tank *const tank) {
	float const min = GAME_WIDTH * 0.1F;
	float const max = GAME_WIDTH * 0.9F;
	tank->position.x = get_random(min, max);
	update_tank(tank);
}
static inline void reset_tanks() {
	uint8_t i;
	for (i = 0; i < game_instance.tanks.size; i++) {
		Tank *const tank = &game_instance.tanks.array[i];
		place_tank(tank);
		tank->gun.angle = M_PI_2;
		tank->health = GAME_TANK_INITIAL_HEALTH;
		tank->alive = true;
	}
}
void game_restart() {
	if (game_instance.playing) {
		error_show(ERROR_LOGIC);
		return;
	}
	game_instance.playing = true;
	game_instance.turn = 0;
	// Change the seed.
	srand(time(0));
	generate_map();
	game_instance.bullets.size = 0;
	reset_tanks();
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
static inline void explode_bullet(Bullet const bullet) {
	uint8_t const leftReach = floor(bullet.position.x - bullet.power);
	uint8_t const rightReach = floor(bullet.position.x + bullet.power);
	uint8_t const leftEdge = leftReach < 0 ? 0 : leftReach;
	uint8_t const rightEdge =
		rightReach >= GAME_WIDTH ? GAME_WIDTH - 1 : rightReach;
	uint8_t i;
	for (i = leftEdge; i <= rightEdge; i++) {
		float const position = i + 0.5F;
		float const potential =
			powf(bullet.power, 2.0F) -
			powf(bullet.position.x - position, 2.0F);
		float const destruction =
			2 * sqrt(potential < 0.0F ? 0.0F : potential);
		game_instance.map.ground[i] += destruction;
		if (game_instance.map.ground[i] >= GAME_HEIGHT) {
			game_instance.map.ground[i] = GAME_HEIGHT - 1;
		}
	}
}
static inline void next_turn() {
	if (++game_instance.turn >= game_instance.tanks.size) {
		game_instance.turn = 0;
	}
}
void game_update() {
	if (game_instance.shooting) {
	} else {
	}
}
