#include "Audio.h"
#include "Error.h"
#include "Game.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Timer.h"

#define TANK_COUNT 3

/** Configures all the subsystems at the start. */
static void config_all(void) {
	error_config();
	keyboard_config();
	timer_config();
	graphics_initialize();
}
static void add_tanks(void) {
	size_t colors[TANK_COUNT];
	size_t i;
	for (i = 0; i < TANK_COUNT; i++) {
		bool new;
		do {
			colors[i] =
				math_random_index(GRAPHICS_TANK_COLOR_COUNT);
			new = false;
			size_t j;
			for (j = 0; j < i; j++) {
				if ((new = colors[i] == colors[j])) {
					break;
				}
			}
		} while (new);
	}
	for (i = 0; i < TANK_COUNT; i++) {
		tank_add(colors[i]);
	}
}
/** Starts the program. */
int main() {
	math_reseed();
	add_tanks();
	game_restart();
	config_all();
	while (true) {
		graphics_render();
	}
	return 0;
}
