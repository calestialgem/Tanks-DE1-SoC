#include "Game.h"

#include "Error.h"
#include "Graphics.h"
#include "Timer.h"

#include <string.h>

/** The game which is currently running. Volatile because it is accessed by both
 * the main loop in rendering and the timer interrupt in updating. */
static volatile Game game_instance;

static inline void copy_name(volatile char destination[GAME_TANK_NAME_CAPACITY],
	char const *const source) {
	size_t i;
	for (i = 0; i < GAME_TANK_NAME_CAPACITY; i++) {
		if (!(destination[i] = source[i])) {
			return;
		}
	}
}
void game_add_tank(char const *const name, size_t const color) {
	if (game_instance.tanks.size == GAME_TANK_CAPACITY) {
		error_show(ERROR_LOGIC);
		return;
	}
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.tanks.size++];
	copy_name(tank->name, name);
	tank->color = color;
}
void game_remove_tank(size_t const index) {
	if (game_instance.tanks.size <= index) {
		error_show(ERROR_LOGIC);
		return;
	}
	Tank const last = game_instance.tanks.array[--game_instance.tanks.size];
	volatile Tank *const removed = &game_instance.tanks.array[index];
	*removed = last;
}
static inline void generate_map() {
	float const peakCount = math_random(0.75F, 1.5F);
	float const start = math_random(0.0F, MATH_2PI);
	float const peakHeight = math_random(0.5F, 0.67F) * GAME_HEIGHT;
	float const valleyHeight = math_random(0.8F, 0.95F) * GAME_HEIGHT;
	size_t x;
	for (x = 0; x < GAME_WIDTH; x++) {
		float const angle =
			math_linearly_map(x, 0.0F, GAME_WIDTH, 0.0F, MATH_2PI);
		game_instance.map.ground[x] =
			math_linearly_map(sin(angle * peakCount + start),
				-1.0F,
				1.0F,
				peakHeight,
				valleyHeight);
	}
}
static inline void update_tank(volatile Tank *const tank) {
	size_t const index = floor(tank->position.x);
	tank->position.y = game_instance.map.ground[index];
	size_t const previousIndex = index == 0 ? index : index - 1;
	size_t const nextIndex = index == GAME_WIDTH - 1 ? index : index + 1;
	float const previousHeight = game_instance.map.ground[previousIndex];
	float const nextHeight = game_instance.map.ground[nextIndex];
	float const heightChange = previousHeight - nextHeight;
	int const steps = nextIndex - previousIndex;
	float const stepWidth = steps;
	float const slope = heightChange / stepWidth;
	tank->tilt = atan(slope);
}
static inline void place_tank(volatile Tank *const tank) {
	float const min = GAME_WIDTH * 0.1F;
	float const max = GAME_WIDTH * 0.9F;
	tank->position.x = math_random(min, max);
	update_tank(tank);
}
static inline void reset_tanks() {
	uint8_t i;
	for (i = 0; i < game_instance.tanks.size; i++) {
		volatile Tank *const tank = &game_instance.tanks.array[i];
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
	math_reseed();
	generate_map();
	game_instance.bullets.size = 0;
	reset_tanks();
}
static inline void simulate_bullet(Bullet *const bullet) {
	Vector const acceleration = {.x = 0.0F, .y = GAME_GRAVITY};
	Vector const velocityEffect = vector_mul(bullet->velocity, TIMER_STEP);
	Vector const accelerationEffect =
		vector_mul(acceleration, math_square(TIMER_STEP) / 2.0F);
	Vector const positionChange =
		vector_add(velocityEffect, accelerationEffect);
	bullet->position = vector_add(bullet->position, positionChange);
	Vector const velocityChange = vector_mul(acceleration, TIMER_STEP);
	bullet->velocity = vector_add(bullet->velocity, velocityChange);
}
static inline void explode_bullet(Bullet const bullet) {
	size_t const leftReach = floor(bullet.position.x - bullet.power);
	size_t const rightReach = floor(bullet.position.x + bullet.power);
	size_t const leftEdge = leftReach < 0 ? 0 : leftReach;
	size_t const rightEdge =
		rightReach >= GAME_WIDTH ? GAME_WIDTH - 1 : rightReach;
	size_t i;
	for (i = leftEdge; i <= rightEdge; i++) {
		float const position = i + 0.5F;
		float const potential =
			math_square(bullet.power) -
			math_square(bullet.position.x - position);
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
void game_copy_safely(Game *const destination) {
	timer_disable_interrupts();
	*destination = game_instance;
	timer_enable_interrupts();
}
