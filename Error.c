#include "Error.h"

#include <stdbool.h>

#define LEDS ((volatile uint32_t *)0xFF200000)

void error_show(uint32_t const error) {
	// Show the error in the LEDs.
	*LEDS = error;
	// Stall the execution when a problem exists.
	while (error)
		;
}
