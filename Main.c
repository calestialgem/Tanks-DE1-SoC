#include "Audio.h"
#include "Error.h"
#include "Graphics.h"
#include "Interrupt.h"
#include "Keyboard.h"
#include "Timer.h"

/** Configures all the subsystems at the start. */
static config_all(void) {
	error_config();
	audio_config();
	keyboard_config();
	graphics_config();
	timer_config();
	interrupt_config();
}
/** Draws continuously. */
static render(void) {
	while (true)
		graphics_render();
}
/** Starts the program. */
int main(void) {
	config_all();
	render();
}
