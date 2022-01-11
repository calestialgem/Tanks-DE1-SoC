#include "Audio.h"
#include "Error.h"
#include "Game.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Timer.h"

#include <math.h>

/** Configures all the subsystems at the start. */
static config_all(void) {
	error_config();
	audio_config();
	keyboard_config();
	timer_config();
}

/** Starts the program. */
int main(void) {
	// Change the seed.
	srand(time(0));
	game_add_tank("Player 1", rand() % GRAPHICS_TANK_COLOR_COUNT);
	game_add_tank("Player 2", rand() % GRAPHICS_TANK_COLOR_COUNT);
	game_add_tank("Player 3", rand() % GRAPHICS_TANK_COLOR_COUNT);
	game_restart();
	config_all();

	while (true) {
		graphics_render();
	}
}
