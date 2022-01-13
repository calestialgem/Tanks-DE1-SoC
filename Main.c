#include "Audio.h"
#include "Error.h"
#include "Game.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Timer.h"

/** Configures all the subsystems at the start. */
static void config_all(void) {
	error_config();
	keyboard_config();
	timer_config();
	graphics_initialize();
}

/** Starts the program. */
int main() {
	error_stage(DEBUG_START);
	tank_add(math_random_index(GRAPHICS_TANK_COLOR_COUNT));
	tank_add(math_random_index(GRAPHICS_TANK_COLOR_COUNT));
	tank_add(math_random_index(GRAPHICS_TANK_COLOR_COUNT));
	game_restart();
	error_stage(DEBUG_CONFIG);
	config_all();
	error_stage(DEBUG_RUNNING);
	while (true) {
		graphics_render();
	}
	return 0;
}
