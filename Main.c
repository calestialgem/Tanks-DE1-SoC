#include "Audio.h"
#include "Error.h"
#include "Game.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Timer.h"

#include <math.h>

/** Configures all the subsystems at the start. */
static void config_all(void) {
	error_config();
	audio_config();
	keyboard_config();
	timer_config();
	graphics_initialize();
}

/** Starts the program. */
int main() {
	maths_reseed();
	game_add_tank(
		"Player 1", maths_random_index(GRAPHICS_TANK_COLOR_COUNT));
	game_add_tank(
		"Player 2", maths_random_index(GRAPHICS_TANK_COLOR_COUNT));
	game_add_tank(
		"Player 3", maths_random_index(GRAPHICS_TANK_COLOR_COUNT));
	game_restart();
	config_all();

	while (true) {
		graphics_update();
		graphics_render();
	}
	return 0;
}
