#include "Game.h"

#include "Audio.h"
#include "Error.h"
#include "Graphics.h"
#include "Keyboard.h"

volatile Game game_instance;

static inline void reset_tanks(void) {
	game_instance.tanks.living = 0;
	uint8_t i;
	for (i = 0; i < game_instance.tanks.size; i++) {
		volatile Tank *const tank = &game_instance.tanks.array[i];
		tank_init(tank, math_random(MAP_LEFT_BORDER, MAP_RIGHT_BORDER));
		barrel_init(&tank->gun, tank->health);
	}
}
void game_restart(void) {
	game_instance.turn = 0;
	map_generate();
	reset_tanks();
	game_instance.bullets.size = 0;
}
static inline void next_turn(void) {
	do {
		game_instance.turn++;
		game_instance.turn %= game_instance.tanks.size;
	} while (!game_instance.tanks.array[game_instance.turn].alive);
	map_randomize_wind();
}
static inline void update_bullets(void) {
	size_t i;
	for (i = 0; i < game_instance.bullets.size; i++) {
		volatile Bullet *const bullet = &game_instance.bullets.array[i];
		bullet_move(bullet);
		bool remove = false;
		if (0.0F > bullet->position.x ||
			bullet->position.x >= MAP_WIDTH) {
			remove = true;
		} else if (bullet_contact(bullet)) {
			audio_play_explosion();
			bullet_explode(bullet);
			remove = true;
		}
		if (remove) {
			*bullet = game_instance.bullets
					  .array[--game_instance.bullets.size];
			i--;
		}
	}
}
static inline void update_waiting_bullets(void) {
	update_bullets();
	if (game_instance.tanks.living <= 1) {
		game_restart();
		return;
	}
	if (!game_instance.bullets.size) {
		next_turn();
	}
}
static inline void shoot(void) {
	if (game_instance.bullets.size == BULLET_CAPACITY) {
		error_show(ERROR_LOGIC_REACHED_BULLET_CAPACITY);
		return;
	}
	bullet_init(&game_instance.bullets.array[game_instance.bullets.size++]);
	audio_play_shooting();
}
void game_update(void) {
	if (keyboard_reset()) {
		game_restart();
		return;
	}
	if (game_instance.bullets.size) {
		update_waiting_bullets();
		return;
	}
	volatile Tank *const tank =
		&game_instance.tanks.array[game_instance.turn];
	volatile Barrel *const barrel = &tank->gun;
	tank_move(tank, keyboard_tank_right() - keyboard_tank_left());
	barrel_rotate(barrel, keyboard_barrel_left() - keyboard_barrel_right());
	barrel_change_power(barrel,
		keyboard_power_up() - keyboard_power_down(),
		tank->health);
	if (keyboard_shoot()) {
		shoot();
		tank->fuel = TANK_INITIAL_FUEL;
	}
}
