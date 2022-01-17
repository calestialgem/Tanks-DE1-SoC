#include "Bullet.h"

#include "Audio.h"
#include "Error.h"
#include "Game.h"
#include "MathTools.h"
#include "Timer.h"

#define BULLET_RADIUS 0.5F // 1x1 pixel drawing
#define EXPLOSION_RADIUS 10.0F
#define SPEED_MULTIPLIER (20.0F * TIMER_STEP)
#define DAMAGE_MULTIPLIER 1000.0F // ~30 Pixel Damage Radius
#define MAX_DAMAGE 30.0F

void bullet_init(volatile Bullet *const bullet) {
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.turn];
	volatile Barrel *const barrel = &tank->gun;
	bullet->position = tank->position;
	// Go to the middle of the tank, which has 3 pixels thickness.
	bullet->position.y -= 1.5F * math_cos(tank->tilt);
	Vector unit;
	vector_init(&unit, 1.0F, tank->tilt + barrel->angle);
	bullet->position = vector_add(bullet->position, vector_mul(unit, 6.0F));
	bullet->velocity = vector_mul(unit, barrel->power * SPEED_MULTIPLIER);
}
void bullet_move(volatile Bullet *const bullet) {
	bullet->position = vector_add(
		bullet->position, vector_mul(bullet->velocity, TIMER_STEP));
	bullet->position =
		vector_add(bullet->position, game_instance.map.positionEffect);
	bullet->velocity =
		vector_add(bullet->velocity, game_instance.map.velocityEffect);
}
bool bullet_contact(volatile Bullet const *const bullet) {
	return game_instance.map.ground[map_index(bullet->position.x)] <=
	       bullet->position.y + BULLET_RADIUS;
}
static inline void apply_ground_damage(volatile Bullet const *const bullet) {
	size_t const leftEdge =
		map_index(bullet->position.x - EXPLOSION_RADIUS);
	size_t const rightEdge =
		map_index(bullet->position.x + EXPLOSION_RADIUS);
	size_t i;
	for (i = leftEdge; i <= rightEdge; i++) {
		float const position = i + 0.5F;
		float const potential =
			math_square(EXPLOSION_RADIUS) -
			math_square(bullet->position.x - position);
		if (potential < 0.0F) {
			continue;
		}
		float const destruction = 2.0F * math_sqrt(potential);
		if (math_nan(destruction)) {
			error_show(ERROR_LOGIC_NAN_DESTRUCTION);
			return;
		}
		map_set(i, game_instance.map.ground[i] + destruction);
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
		if (distanceSquared > DAMAGE_MULTIPLIER) {
			continue;
		}
		tank_damage(tank,
			math_min(DAMAGE_MULTIPLIER / distanceSquared,
				MAX_DAMAGE));
	}
}
void bullet_explode(volatile Bullet const *const bullet) {
	apply_ground_damage(bullet);
	apply_tank_damage(bullet);
}
