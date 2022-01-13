#include "Game.h"

#include "Error.h"
#include "Graphics.h"
#include "Keyboard.h"

#include <math.h>

volatile Game game_instance;

static inline void reset_tanks(void) {
	uint8_t i;
	for (i = 0; i < game_instance.tanks.size; i++) {
		volatile Tank *const tank = &game_instance.tanks.array[i];
		tank_init(tank, math_random(MAP_LEFT_BORDER, MAP_RIGHT_BORDER));
		barrel_init(&tank->gun, tank->health);
	}
}
void game_restart(void) {
	if (game_instance.playing) {
		error_show(ERROR_LOGIC_STILL_PLAYING);
		return;
	}
	game_instance.playing = true;
	game_instance.turn = 0;
	math_reseed();
	map_generate();
	game_instance.bullets.size = 0;
	reset_tanks();
}
static inline bool check_bullet_contact(volatile Bullet *const bullet) {
	size_t const index = floorf(bullet->position.x);
	return game_instance.map.ground[index] >=
	       bullet->position.y + bullet->radius;
}
static inline void explode_bullet(volatile Bullet *const bullet) {
	size_t const leftReach = floorf(bullet->position.x - bullet->power);
	size_t const rightReach = floorf(bullet->position.x + bullet->power);
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
			2 * sqrtf(potential < 0.0F ? 0.0F : potential);
		game_instance.map.ground[i] += destruction;
		if (game_instance.map.ground[i] >= MAP_HEIGHT) {
			game_instance.map.ground[i] = MAP_HEIGHT - 1;
		}
	}
}
static inline void apply_bullet_damage(volatile Bullet *const bullet) {
	size_t i;
	for (i = 0; i < game_instance.tanks.size; i++) {
		volatile Tank *const tank = &game_instance.tanks.array[i];
		if (!tank->alive) {
			continue;
		}
		float const distanceSquared = vector_square(
			vector_sub(tank->position, bullet->position));
		tank->health -= bullet->power * BULLET_DAMAGE_MULTIPLIER /
				distanceSquared;
		tank->alive = tank->health > 0.0F;
	}
}
static inline void next_turn(void) {
	if (++game_instance.turn >= game_instance.tanks.size) {
		game_instance.turn = 0;
	}
}
static inline void update_waiting_bullets(void) {
	size_t i;
	for (i = 0; i < game_instance.bullets.size; i++) {
		volatile Bullet *const bullet = &game_instance.bullets.array[i];
		bullet_move(bullet);
		if (check_bullet_contact(bullet)) {
			explode_bullet(bullet);
			apply_bullet_damage(bullet);
			*bullet = game_instance.bullets
					  .array[--game_instance.bullets.size];
			i--;
		}
	}
	game_instance.waitingBullets = game_instance.bullets.size;
}
static inline void shoot(void) {
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.turn];
	volatile Barrel *const barrel = &tank->gun;
	if (game_instance.bullets.size == BULLET_CAPACITY) {
		error_show(ERROR_LOGIC_REACHED_BULLET_CAPACITY);
		return;
	}
	volatile Bullet *const bullet =
		&game_instance.bullets.array[game_instance.bullets.size++];
	bullet->position = tank->position;
	float const angle = tank->tilt + barrel->angle;
	bullet->radius = barrel->power * BULLET_RADIUS_MULTIPLIER;
	bullet->power = barrel->power * BULLET_POWER_MULTIPLIER;
	bullet->velocity.x =
		cosf(angle) * barrel->power * BULLET_SPEED_MULTIPLIER;
	bullet->velocity.y =
		-sinf(angle) * barrel->power * BULLET_SPEED_MULTIPLIER;
	game_instance.waitingBullets = true;
}
static inline void game_update(void) {
	if (game_instance.waitingBullets) {
		update_waiting_bullets();
		return;
	}
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.turn];
	volatile Barrel *const barrel = &tank->gun;
	tank_move(tank, keyboard_tank_left() - keyboard_tank_right());
	barrel_rotate(barrel, keyboard_barrel_left() - keyboard_barrel_right());
	barrel_change_power(barrel,
		keyboard_power_up() - keyboard_power_down(),
		tank->health);
	if (keyboard_shoot()) {
		shoot();
	}
}
