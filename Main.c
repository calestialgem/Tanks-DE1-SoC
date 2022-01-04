#include "Audio.h"
#include "Error.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Timer.h"

/** Configures all the sub systems at the start. */
static config_all(void) {
	error_config();
	audio_config();
	keyboard_config();
	graphics_config();
	timer_config();
}

/** Waits for the interrupts to work. */
static wait(void) {
	while (true)
		;
}

/** Starts the program. */
int main(void) {
	config_all();
	wait();
}
