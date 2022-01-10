#include "Audio.h"
#include "Error.h"
#include "Game.h"
#include "Graphics.h"
#include "Interrupt.h"
#include "Keyboard.h"
#include "Timer.h"

/** Configures all the subsystems at the start. */
static config_all(void) {
	error_config();
	audio_config();
	keyboard_config();
	timer_config();
	interrupt_config();
}
/** Draws continuously. */
static render(struct game *const game_data) {
	while (true)
		graphics_render(game_data);
}
/** Starts the program. */
int main(void) {
	struct game current = {.tanks = {.size = 0},
		.bullets = {.size = 0},
		.map = {},
		.playing = false,
		.turn = 0,
		.shooting = false};
	config_all();
	render(&current);
}
