#include "Audio.h"
#include "Error.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Timer.h"

/** Configures all the sub systems at the start. */
static config_all(void) {
	audio_config();
	graphics_config();
	keyboard_config();
	timer_config();
}

/** Wait for the interrupts to work. */
static wait(void) {
	while (true)
		;
}

/** Start of the program. */
int main(void) {
	config_all();
	wait();
}
