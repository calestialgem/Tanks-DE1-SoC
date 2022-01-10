#include "Audio.h"
#include "Error.h"
#include "Graphics.h"
#include "Interrupt.h"
#include "Keyboard.h"
#include "Timer.h"

game game_data;

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
	config_all();
	render(game_data);
}
