#include "Game.h"

#include "Audio.h"
#include "Error.h"
#include "Graphics.h"
#include "Keyboard.h"

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
	game_instance.turn = 0;
	map_generate();
	game_instance.bullets.size = 0;
	reset_tanks();
	game_instance.waitingBullets = false;
}
static inline void update_waiting_bullets(void) {
	size_t i;
	for (i = 0; i < game_instance.bullets.size; i++) {
		volatile Bullet *const bullet = &game_instance.bullets.array[i];
		bullet_move(bullet);
		bool remove = false;
		if (0.0F < bullet->position.x ||
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
	game_instance.waitingBullets = game_instance.bullets.size;
	for (i = 0; i < game_instance.tanks.size; i++) {
		volatile Tank *const tank = &game_instance.tanks.array[i];
		if (tank->alive) {
			return;
		}
	}
	game_restart();
}
static inline void shoot(void) {
	if (game_instance.bullets.size == BULLET_CAPACITY) {
		error_show(ERROR_LOGIC_REACHED_BULLET_CAPACITY);
		return;
	}
	bullet_init(&game_instance.bullets.array[game_instance.bullets.size++]);
	game_instance.waitingBullets = true;
	audio_play_shooting();
	if (++game_instance.turn >= game_instance.tanks.size) {
		game_instance.turn = 0;
	}
}
void game_update(void) {
	if (keyboard_reset()) {
		game_restart();
		return;
	}
	if (game_instance.waitingBullets) {
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
