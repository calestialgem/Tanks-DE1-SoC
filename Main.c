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
	game_add_tank("Player 1", rand() % Color_tank_count);
	game_add_tank("Player 2", rand() % Color_tank_count);
	game_add_tank("Player 3", rand() % Color_tank_count);
	game_restart();
	config_all();

	while(1){
		graphics_render(&current);
	}

}
