#include "Game.h"

#include "Error.h"
#include "Graphics.h"
#include "Keyboard.h"

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
	float const start = math_random(0.0F, MATH_PI);
	float const peakHeight = math_random(0.5F, 0.67F) * GAME_HEIGHT;
	float const valleyHeight = math_random(0.8F, 0.95F) * GAME_HEIGHT;
	size_t x;
	for (x = 0; x < GAME_WIDTH; x++) {
		float const angle =
			math_linearly_map(x, 0.0F, GAME_WIDTH, 0.0F, MATH_PI);
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
static inline void reset_tanks() {
	uint8_t i;
	for (i = 0; i < game_instance.tanks.size; i++) {
		volatile Tank *const tank = &game_instance.tanks.array[i];
		tank->position.x = math_random(
			GAME_MAP_LEFT_BORDER, GAME_MAP_RIGHT_BORDER);
		update_tank(tank);
		tank->gun.angle = GAME_BARREL_INITIAL_ANGLE;
		tank->health = GAME_TANK_INITIAL_HEALTH;
		tank->alive = true;
		tank->fuel = GAME_TANK_INITIAL_FUEL;
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
static inline void simulate_bullet(volatile Bullet *const bullet) {
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
static inline bool check_bullet_contact(volatile Bullet *const bullet) {
	size_t const index = floor(bullet->position.x);
	return game_instance.map.ground[index] >=
	       bullet->position.y + bullet->radius;
}
static inline void explode_bullet(volatile Bullet *const bullet) {
	size_t const leftReach = floor(bullet->position.x - bullet->power);
	size_t const rightReach = floor(bullet->position.x + bullet->power);
	size_t const leftEdge = leftReach < 0 ? 0 : leftReach;
	size_t const rightEdge =
		rightReach >= GAME_WIDTH ? GAME_WIDTH - 1 : rightReach;
	size_t i;
	for (i = leftEdge; i <= rightEdge; i++) {
		float const position = i + 0.5F;
		float const potential =
			math_square(bullet->power) -
			math_square(bullet->position.x - position);
		float const destruction =
			2 * sqrt(potential < 0.0F ? 0.0F : potential);
		game_instance.map.ground[i] += destruction;
		if (game_instance.map.ground[i] >= GAME_HEIGHT) {
			game_instance.map.ground[i] = GAME_HEIGHT - 1;
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
		tank->health -= bullet->power * GAME_BULLET_DAMAGE_MULTIPLIER /
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
		simulate_bullet(bullet);
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
static inline void update_tank_movement(void) {
	uint32_t movement = 0;
	if (keyboard_tank_left()) {
		movement--;
	}
	if (keyboard_tank_right()) {
		movement++;
	}
	if (!movement) {
		return;
	}
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.turn];
	float const initialPosition = tank->position.x;
	tank->position.x += movement * GAME_TANK_SPEED;
	if (tank->position.x < GAME_MAP_LEFT_BORDER) {
		tank->position.x = GAME_MAP_LEFT_BORDER;
	} else if (tank->position.x > GAME_MAP_RIGHT_BORDER) {
		tank->position.x = GAME_MAP_RIGHT_BORDER;
	}
	tank->fuel -= fabsf(tank->position.x - initialPosition) *
		      GAME_TANK_FUEL_CONSUMPTION;
	update_tank(tank);
}
static inline void update_barrel_rotation(void) {
	uint32_t rotation = 0;
	if (keyboard_barrel_left()) {
		rotation++;
	}
	if (keyboard_barrel_right()) {
		rotation--;
	}
	if (!rotation) {
		return;
	}
	volatile Barrel *const barrel =
		&game_instance.tanks.array[game_instance.turn].gun;
	barrel->angle += rotation * GAME_BARREL_SPEED;
	if (barrel->angle < GAME_BARREL_LOWER_ANGLE) {
		barrel->angle = GAME_BARREL_LOWER_ANGLE;
	} else if (barrel->angle > GAME_BARREL_UPPER_ANGLE) {
		barrel->angle = GAME_BARREL_UPPER_ANGLE;
	}
}
static inline void update_power_change(void) {
	uint32_t change = 0;
	if (keyboard_barrel_left()) {
		change++;
	}
	if (keyboard_barrel_right()) {
		change--;
	}
	if (!change) {
		return;
	}
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.turn];
	volatile Barrel *const barrel = &tank->gun;
	barrel->power += change;
	if (barrel->power < 1) {
		barrel->power = 1;
	} else if (barrel->power > tank->health) {
		barrel->power = tank->health;
	}
}
static inline void shoot(void) {
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.turn];
	volatile Barrel *const barrel = &tank->gun;
	volatile Bullet *const bullet =
		&game_instance.bullets.array[game_instance.bullets.size++];
	bullet->position = tank->position;
	float const angle = tank->tilt + barrel->angle;
	bullet->radius = barrel->power * GAME_BULLET_RADIUS_MULTIPLIER;
	bullet->power = barrel->power * GAME_BULLET_POWER_MULTIPLIER;
	bullet->velocity.x =
		cosf(angle) * barrel->power * GAME_BULLET_SPEED_MULTIPLIER;
	bullet->velocity.y =
		-sinf(angle) * barrel->power * GAME_BULLET_SPEED_MULTIPLIER;
	game_instance.waitingBullets = true;
}
static inline void game_update() {
	keyboard_update();
	if (game_instance.waitingBullets) {
		update_waiting_bullets();
		return;
	}
	update_tank_movement();
	update_barrel_rotation();
	update_power_change();
	if (keyboard_shoot()) {
		shoot();
	}
}
void game_copy_safely(Game *const destination) {
	timer_disable_interrupts();
	*destination = game_instance;
	timer_enable_interrupts();
}
