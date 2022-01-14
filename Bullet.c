#include "Bullet.h"

#include "Audio.h"
#include "Game.h"
#include "MathTools.h"
#include "Timer.h"

#define MAX_DAMAGE_DISTANCE_SQUARED 900.0F // 30 pixels
#define GRAVITY 9.81F
#define RADIUS_MULTIPLIER 0.1F
#define POWER_MULTIPLIER 0.1F
#define SPEED_MULTIPLIER (20.0F * TIMER_STEP)
#define DAMAGE_MULTIPLIER 0.2F

void bullet_init(volatile Bullet *const bullet) {
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.turn];
	volatile Barrel *const barrel = &tank->gun;
	bullet->position = tank->position;
	bullet->radius = barrel->power * RADIUS_MULTIPLIER;
	bullet->power = barrel->power * POWER_MULTIPLIER;
	float const angle = tank->tilt + barrel->angle;
	float const speed = barrel->power * SPEED_MULTIPLIER;
	bullet->velocity.x = math_cos(angle) * speed;
	bullet->velocity.y = -math_sin(angle) * speed;
}
void bullet_move(volatile Bullet *const bullet) {
	Vector const acceleration = {.x = 0.0F, .y = GRAVITY};
	Vector const velocityEffect = vector_mul(bullet->velocity, TIMER_STEP);
	Vector const accelerationEffect =
		vector_mul(acceleration, math_square(TIMER_STEP) / 2.0F);
	Vector const positionChange =
		vector_add(velocityEffect, accelerationEffect);
	bullet->position = vector_add(bullet->position, positionChange);
	Vector const velocityChange = vector_mul(acceleration, TIMER_STEP);
	bullet->velocity = vector_add(bullet->velocity, velocityChange);
}
bool bullet_contact(volatile Bullet const *const bullet) {
	size_t const index = math_floor(bullet->position.x);
	return game_instance.map.ground[index] >=
	       bullet->position.y + bullet->radius;
}
static inline void apply_ground_damage(volatile Bullet const *const bullet) {
	size_t const leftReach = math_floor(bullet->position.x - bullet->power);
	size_t const rightReach =
		math_floor(bullet->position.x + bullet->power);
	size_t const leftEdge = leftReach < 0 ? 0 : leftReach;
	size_t const rightEdge =
		rightReach >= MAP_WIDTH ? MAP_WIDTH - 1 : rightReach;
	size_t i;
	for (i = leftEdge; i <= rightEdge; i++) {
		float const position = i + 0.5F;
		float const potential =
			math_square(bullet->power) -
			math_square(bullet->position.x - position);
		float const destruction =
			2 * math_sqrt(potential < 0.0F ? 0.0F : potential);
		game_instance.map.ground[i] += destruction;
		if (game_instance.map.ground[i] >= MAP_HEIGHT) {
			game_instance.map.ground[i] = MAP_HEIGHT - 1;
		}
	}
}
static inline void apply_tank_damage(volatile Bullet const *const bullet) {
	size_t i;
	for (i = 0; i < game_instance.tanks.size; i++) {
		volatile Tank *const tank = &game_instance.tanks.array[i];
		if (!tank->alive) {
			continue;
		}
		float const distanceSquared = vector_square(
			vector_sub(tank->position, bullet->position));
		tank_update_map_position(tank);
		if (distanceSquared > MAX_DAMAGE_DISTANCE_SQUARED) {
			continue;
		}
		tank->health -=
			bullet->power * DAMAGE_MULTIPLIER / distanceSquared;
		tank->alive = tank->health > 0.0F;
		if (!tank->alive) {
			audio_play_tank_death();
		}
	}
}
void bullet_explode(volatile Bullet const *const bullet) {
	apply_ground_damage(bullet);
	apply_tank_damage(bullet);
}
