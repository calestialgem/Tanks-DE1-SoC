#include "Audio.h"
#include "Error.h"
#include "Graphics.h"
#include "Interrupt.h"
#include "Keyboard.h"
#include "Timer.h"

/** Initializes resources to be used at configuration of the subsystems. */
static init_all(void) {
	error_init();
	interrupt_init();
}

/** Configures all the subsystems at the start. */
static config_all(void) {
	audio_config();
	keyboard_config();
	graphics_config();
	timer_config();
	interrupt_config();
}

/** Waits for the interrupts to work. */
static wait(void) {
	while (true)
		;
}

/** Starts the program. */
int main(void) {
	init_all();
	config_all();
	wait();
}
