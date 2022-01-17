#include "Bullet.h"

#include "Audio.h"
#include "Game.h"
#include "MathTools.h"
#include "Timer.h"

#define MAX_DAMAGE_DISTANCE_SQUARED 900.0F // 30 pixels
#define GRAVITY 9.81F			   // Asuming 1 pixel = 1 meter
#define BULLET_RADIUS 0.5F		   // 1x1 pixel drawing
#define EXPLOSION_RADIUS 10.0F
#define SPEED_MULTIPLIER (20.0F * TIMER_STEP)
#define DAMAGE_MULTIPLIER 0.2F

void bullet_init(volatile Bullet *const bullet) {
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.turn];
	volatile Barrel *const barrel = &tank->gun;
	bullet->position = tank->position;
	// Go to the middle of the tank, which has 3 pixels thickness.
	bullet->position.y -= 1.5F * math_cos(tank->tilt);
	vector_init(&bullet->velocity,
		barrel->power * SPEED_MULTIPLIER,
		tank->tilt + barrel->angle);
}
void bullet_move(volatile Bullet *const bullet) {
	static Vector const acceleration = {.x = 0.0F, .y = GRAVITY};
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
	if (bullet->position.x < 0.0F || bullet->position.x >= MAP_WIDTH) {
		return true;
	}
	size_t const index = math_floor(bullet->position.x);
	return game_instance.map.ground[index] <=
	       bullet->position.y + BULLET_RADIUS;
}
static inline void apply_ground_damage(volatile Bullet const *const bullet) {
	size_t const leftEdge = math_floor(math_clamp(
		bullet->position.x - EXPLOSION_RADIUS, 0.0F, MAP_WIDTH - 1.0F));
	size_t const rightEdge = math_floor(math_clamp(
		bullet->position.x + EXPLOSION_RADIUS, 0.0F, MAP_WIDTH - 1.0F));
	size_t i;
	for (i = leftEdge; i <= rightEdge; i++) {
		float const position = i + 0.5F;
		float const potential =
			math_square(EXPLOSION_RADIUS) -
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
			EXPLOSION_RADIUS * DAMAGE_MULTIPLIER / distanceSquared;
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
